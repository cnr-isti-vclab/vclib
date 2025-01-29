/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_APPEND_REPLACE_TO_BUFFER_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_APPEND_REPLACE_TO_BUFFER_H

#include <vclib/mesh/requirements.h>
#include <vclib/space/complex/tri_poly_index_bimap.h>
#include <vclib/types.h>
#include <vclib/views/mesh.h>

/**
 * @defgroup append_replace_to_buffer Append/Replace Mesh Data to Existing
 * Buffer Algorithms
 *
 * @ingroup import_export
 *
 * @brief Append missing data or Replace existing data to already allocated and
 * partially filled buffers.
 *
 * These algorithms are useful when you already have allocated and filled
 * buffers with mesh data, and you want to append missing data to them or
 * replace existing data with new data.
 *
 * An example of missing data are the duplicate vertices that are necessary to
 * render a mesh with wedge texture coordinates. In this scenario, an example of
 * workflow is:
 *
 * 0. Compute all the required data necessary to know the number of duplicated
 * vertices and which vertices are duplicated;
 * 1. Allocate the buffers having the correct size for the vertices (including
 * the duplicates);
 * 2. Fill the non-duplicated vertices using the algorithms defined in @ref
 * export_buffer;
 * 3. Use the algorithms defined in this module to append the duplicated
 * vertices and the texture coordinates.
 *
 * You can access these algorithms by including `#include
 * <vclib/algorithms/mesh/import_export.h>`
 */

namespace vcl {

/**
 * @brief Append the coordinates of the duplicated vertices to the given buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the coordinates of the vertices listed in the input list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexCoordsToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<double> buffer((mesh.vertexNumber() + nV) * 3);
 * vertexCoordsToBuffer(mesh, buffer.data());
 * appendDuplicateVertexCoordsToBuffer(mesh, vertsToDuplicate, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices times 3).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the coordinates.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[in/out] buffer: The buffer where to append the duplicated vertices.
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexCoordsToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    const uint VERT_NUM = mesh.vertexNumber() + vertsToDuplicate.size();

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        const auto& coord = mesh.vertex(v).coord();
        if (storage == MatrixStorageType::ROW_MAJOR) {
            buffer[i * 3 + 0] = coord.x();
            buffer[i * 3 + 1] = coord.y();
            buffer[i * 3 + 2] = coord.z();
        }
        else {
            buffer[0 * VERT_NUM + i] = coord.x();
            buffer[1 * VERT_NUM + i] = coord.y();
            buffer[2 * VERT_NUM + i] = coord.z();
        }

        ++i;
    }
}

/**
 * @brief Replace the face vertex indices in the given buffer with the new
 * indices of the duplicated vertices.
 *
 * Given a buffer containing the vertex indices of the faces of a mesh (stored
 * in row-major order, and with a fixed face size), this function updates the
 * indices of the vertices that have been duplicated, according to the data
 * stored in the input lists.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with any of the
 * functions defined in @ref export_buffer that export face indices. e.g.:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 * uint lfs = vcl::largestFaceSize(mesh);
 *
 * std::vector<uint> buffer(mesh.faceNumber() * lfs);
 * faceIndicesToBuffer(mesh, buffer.data(), lfs);
 * replaceFaceIndicesByVertexDuplicationToBuffer(mesh, vertsToDuplicate,
 *    faceToReassign, buffer.data(), lfs);
 * @endcode
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The input mesh.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be replaced in the buffer.
 * @param[in] facesToReassign: The list of lists of pairs face/vertex index in
 * the face that must be reassigned to the duplicated vertices. Each list of
 * pairs is the list of faces that must be reassigned to the corresponding
 * duplicated vertex.
 * @param[out] buffer: The buffer where to replace the vertex indices.
 * @param[in] largestFaceSize: The size of the largest face in the mesh (that
 * corresponds to the number of columns in the buffer)
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<FaceMeshConcept MeshType>
void replaceFaceIndicesByVertexDuplicationToBuffer(
    const MeshType&                                    mesh,
    const std::list<uint>&                             vertsToDuplicate,
    const std::list<std::list<std::pair<uint, uint>>>& facesToReassign,
    auto*                                              buffer,
    uint                                               largestFaceSize = 3,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR)
{
    // no vertices have been duplicated, do nothing
    if (vertsToDuplicate.empty())
        return;

    assert(vertsToDuplicate.size() == facesToReassign.size());

    const uint FACE_NUM = mesh.faceNumber();

    uint vFirst = mesh.vertexNumber();
    uint vLast  = mesh.vertexNumber() + vertsToDuplicate.size();
    for (uint vi = vFirst; const auto& faces : facesToReassign) {
        for (const auto& f : faces) {
            if (storage == MatrixStorageType::ROW_MAJOR)
                buffer[f.first * largestFaceSize + f.second] = vi;
            else
                buffer[f.second * FACE_NUM + f.first] = vi;
        }
        ++vi;
    }
}

/**
 * @brief Replace the triangulated face vertex indices in the given buffer with
 * the new indices of the duplicated vertices.
 *
 * Given a buffer containing the vertex indices of the triangles of a mesh that
 * has been triangulated (stored in row-major order), this function updates the
 * indices of the vertices that have been duplicated, according to the data
 * stored in the input lists.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with @ref
 * triangulatedFaceIndicesToBuffer function that export triangulated face
 * indices. e.g.:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 * TriPolyIndexBiMap indexMap;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * uint numTris = vcl::countTriangulatedTriangles(myMesh);
 *
 * std::vector<uint> buffer(mesh.faceNumber() * numTris);
 * triangulatedFaceIndicesToBuffer(mesh, buffer.data(), indexMap);
 * replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(mesh,
 *    vertsToDuplicate, faceToReassign, indexMap, buffer.data());
 * @endcode
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The input mesh.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be replaced in the buffer.
 * @param[in] facesToReassign: The list of lists of pairs face/vertex index in
 * the face that must be reassigned to the duplicated vertices. Each list of
 * pairs is the list of faces that must be reassigned to the corresponding
 * duplicated vertex.
 * @param[in] indexMap: The map from triangle index to face index.
 * @param[out] buffer: The buffer where to replace the vertex indices.
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<FaceMeshConcept MeshType>
void replaceTriangulatedFaceIndicesByVertexDuplicationToBuffer(
    const MeshType&                                    mesh,
    const std::list<uint>&                             vertsToDuplicate,
    const std::list<std::list<std::pair<uint, uint>>>& facesToReassign,
    const TriPolyIndexBiMap&                           indexMap,
    auto*                                              buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR)
{
    // no vertices have been duplicated, do nothing
    if (vertsToDuplicate.empty())
        return;

    assert(vertsToDuplicate.size() == facesToReassign.size());

    uint vFirst = mesh.vertexNumber();
    uint vLast  = mesh.vertexNumber() + vertsToDuplicate.size();

    const uint FACE_NUM = indexMap.triangleNumber();

    // the facesToReassign lists for each vertex contain pairs that in the
    // second element store the index of the vertex in the face. However, the
    // face has been triangulated, and this info is not useful anymore. We need
    // to look into the triangles generated for the face (first elem of the
    // pair) and look for the vertex index to set (that is the vertex index
    // stored in the vertsToDuplicate list).
    for (uint vi = vFirst; const auto& [vert, faces] :
                           std::views::zip(vertsToDuplicate, facesToReassign)) {
        // vert is the vertex index in the mesh that we need to reassign with
        // the index vi in the buffer

        // for each face f that has at least a vertex to reassign
        for (const auto& f : faces) {
            // get the triangle indices of the face using the index map
            uint tBegin = indexMap.triangleBegin(f.first);
            uint tEnd   = tBegin + indexMap.triangleNumber(f.first);
            for (uint t = tBegin; t < tEnd; ++t) { // look into the triangles
                for (uint j = 0; j < 3; ++j) { // for each vertex of triangle
                    if (storage == MatrixStorageType::ROW_MAJOR) {
                        if (buffer[t * 3 + j] == vert) {
                            buffer[t * 3 + j] = vi;
                        }
                    }
                    else {
                        if (buffer[j * FACE_NUM + t] == vert) {
                            buffer[j * FACE_NUM + t] = vi;
                        }
                    }
                }
            }
        }
        ++vi;
    }
}

/**
 * @brief Append the selection of the duplicated vertices to the given buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the selection of the vertices listed in the input list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexSelectionToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<uint> buffer(mesh.vertexNumber() + nV);
 * vertexSelectionToBuffer(mesh, buffer.data());
 * appendDuplicateVertexCoordsToBuffer(mesh, vertsToDuplicate, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the selection.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[out] buffer: The buffer where to append the duplicated vertex
 * selection.
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexSelectionToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        buffer[i] = mesh.vertex(v).selected();
        ++i;
    }
}

/**
 * @brief Append the normals of the duplicated vertices to the given buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the normals of the vertices listed in the input list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexNormalsToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<double> buffer((mesh.vertexNumber() + nV) * 3);
 * vertexNormalsToBuffer(mesh, buffer.data());
 * appendDuplicateVertexNormalsToBuffer(mesh, vertsToDuplicate, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices times 3).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the normals.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[out] buffer: The buffer where to append the duplicated vertex normals.
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexNormalsToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer,
    MatrixStorageType      storage = MatrixStorageType::ROW_MAJOR)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    requirePerVertexNormal(mesh);

    const uint VERT_NUM = mesh.vertexNumber() + vertsToDuplicate.size();

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        const auto& normal = mesh.vertex(v).normal();
        if (storage == MatrixStorageType::ROW_MAJOR) {
            buffer[i * 3 + 0] = normal.x();
            buffer[i * 3 + 1] = normal.y();
            buffer[i * 3 + 2] = normal.z();
        }
        else {
            buffer[0 * VERT_NUM + i] = normal.x();
            buffer[1 * VERT_NUM + i] = normal.y();
            buffer[2 * VERT_NUM + i] = normal.z();
        }
        ++i;
    }
}

/**
 * @brief Append the colors of the duplicated vertices to the given buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the colors of the vertices listed in the input list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexColorsToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<uint> buffer((mesh.vertexNumber() + nV) * 4);
 * vertexColorsToBuffer(mesh, buffer.data());
 * appendDuplicateVertexColorsToBuffer(mesh, vertsToDuplicate, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices times 4).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the colors.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[out] buffer: The buffer where to append the duplicated vertex colors.
 * @param[in] representation: The representation of the color in the buffer.
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexColorsToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer,
    Color::Representation  representation = Color::Representation::INT_0_255,
    MatrixStorageType      storage = MatrixStorageType::ROW_MAJOR)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    requirePerVertexColor(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;
    const uint VERT_NUM = mesh.vertexNumber() + vertsToDuplicate.size();

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        const auto& c = mesh.vertex(v).color();
        if (storage == MatrixStorageType::ROW_MAJOR) {
            buffer[i * 4 + 0] = R_INT ? c.red() : c.redF();
            buffer[i * 4 + 1] = R_INT ? c.green() : c.greenF();
            buffer[i * 4 + 2] = R_INT ? c.blue() : c.blueF();
            buffer[i * 4 + 3] = R_INT ? c.alpha() : c.alphaF();
        }
        else {
            buffer[0 * VERT_NUM + i] = R_INT ? c.red() : c.redF();
            buffer[1 * VERT_NUM + i] = R_INT ? c.green() : c.greenF();
            buffer[2 * VERT_NUM + i] = R_INT ? c.blue() : c.blueF();
            buffer[3 * VERT_NUM + i] = R_INT ? c.alpha() : c.alphaF();
        }
        ++i;
    }
}

/**
 * @brief Append the color (packed in a single 32 bit value using the provided
 * format) of the duplicated vertices to the given buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the color (packed in a single 32 bit value using the provided format)
 * of the vertices listed in the input list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexColorToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<double> buffer(mesh.vertexNumber() + nV);
 * vertexColorToBuffer(mesh, buffer.data(), Color::Format::RGBA);
 * appendDuplicateVertexColorToBuffer(mesh, vertsToDuplicate, buffer.data(),
 *     Color::Format::RGBA);
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the colors.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[out] buffer: The buffer where to append the duplicated vertex color.
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexColorsToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer,
    Color::Format          colorFormat)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    requirePerVertexColor(mesh);

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        const auto& c = mesh.vertex(v).color();
        switch (colorFormat) {
            using enum Color::Format;
        case ABGR: buffer[i] = c.abgr(); break;
        case ARGB: buffer[i] = c.argb(); break;
        case RGBA: buffer[i] = c.rgba(); break;
        case BGRA: buffer[i] = c.bgra(); break;
        }
        ++i;
    }
}

/**
 * @brief Append the quality of the duplicated vertices to the given buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the quality of the vertices listed in the input list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexQualityToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<double> buffer(mesh.vertexNumber() + nV);
 * vertexQualityToBuffer(mesh, buffer.data());
 * appendDuplicateVertexQualityToBuffer(mesh, vertsToDuplicate, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the quality.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[out] buffer: The buffer where to append the duplicated vertex quality.
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexQualityToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    requirePerVertexQuality(mesh);

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        buffer[i] = mesh.vertex(v).quality();
        ++i;
    }
}

/**
 * @brief Append the texture coordinates of the duplicated vertices to the given
 * buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the vertex texture coordinates of the vertices listed in the input
 * list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexTexCoordsToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<double> buffer((mesh.vertexNumber() + nV) * 2);
 * vertexTexCoordsToBuffer(mesh, buffer.data());
 * appendDuplicateVertexTexCoordsToBuffer(mesh, vertsToDuplicate,
 *     buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices times 2).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the vertex texture coordinates.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[out] buffer: The buffer where to append the duplicated vertex texture
 * coordinates.
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexTexCoordsToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer,
    MatrixStorageType      storage = MatrixStorageType::ROW_MAJOR)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    requirePerVertexTexCoord(mesh);

    const uint VERT_NUM = mesh.vertexNumber() + vertsToDuplicate.size();

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        const auto& t = mesh.vertex(v).texCoord();
        if (storage == MatrixStorageType::ROW_MAJOR) {
            buffer[i * 2 + 0] = t.u();
            buffer[i * 2 + 1] = t.v();
        }
        else {
            buffer[0 * VERT_NUM + i] = t.u();
            buffer[1 * VERT_NUM + i] = t.v();
        }
        ++i;
    }
}

/**
 * @brief Append the texture coordinate indices of the duplicated vertices to
 * the given buffer.
 *
 * Given the list of vertices to duplicate, this function appends to the given
 * buffer the vertex texture coordinate indices of the vertices listed in the
 * input list.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function and along with the @ref
 * vertexTexCoordIndicesToBuffer function:
 *
 * @code{.cpp}
 *
 * std::vector<std::pair<uint, uint>> vertWedgeMap;
 * std::list<uint> vertsToDuplicate;
 * std::list<std::list<std::pair<uint, uint>>> facesToReassign;
 *
 * uint nV = countVerticesToDuplicateByWedgeTexCoords(mesh, vertWedgeMap,
 *     vertsToDuplicate, facesToReassign);
 *
 * std::vector<ushort> buffer(mesh.vertexNumber() + nV);
 * vertexTexCoordIndicesToBuffer(mesh, buffer.data());
 * appendDuplicateVertexTexCoordIndicesToBuffer(mesh, vertsToDuplicate,
 *     buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the vertex texture coordinate
 * indices.
 * @param[in] vertsToDuplicate: The list of vertices to duplicate: each element
 * is the index of a vertex in the mesh, that must be appended to the buffer.
 * @param[out] buffer: The buffer where to append the duplicated vertex texture
 * coordinate indices.
 *
 * @ingroup append_replace_to_buffer
 */
template<MeshConcept MeshType>
void appendDuplicateVertexTexCoordIndicesToBuffer(
    const MeshType&        mesh,
    const std::list<uint>& vertsToDuplicate,
    auto*                  buffer)
{
    // no vertices to duplicate, do nothing
    if (vertsToDuplicate.empty())
        return;

    requirePerVertexTexCoord(mesh);

    for (uint i = mesh.vertexNumber(); const auto& v : vertsToDuplicate) {
        buffer[i] = mesh.vertex(v).texCoord().index();
        ++i;
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_APPEND_REPLACE_TO_BUFFER_H
