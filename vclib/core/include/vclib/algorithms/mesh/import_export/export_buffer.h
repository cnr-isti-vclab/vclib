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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H

#include "detail.h"

#include <vclib/algorithms/mesh/utility.h>

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>
#include <vclib/space/complex.h>

/**
 * @defgroup export_buffer Export Mesh to Buffer Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Export Mesh to Buffer algorithms.
 *
 * They allow to export mesh data to pre-allocated buffers.
 */

namespace vcl {

namespace detail {

inline TriPolyIndexBiMap indexMap;

} // namespace detail

/**
 * @brief Export the vertex positions of a mesh to a buffer.
 *
 * This function exports the vertex positions of a mesh to a buffer. Vertices
 * are stored in the buffer following the order they appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * the number of positions per vertex).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexPositionsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.vertexNumber() : rowNumber;
    for (uint i = 0; const auto& p : mesh.vertices() | views::positions) {
        at(buffer, i, 0, ROW_NUM, 3, storage) = p.x();
        at(buffer, i, 1, ROW_NUM, 3, storage) = p.y();
        at(buffer, i, 2, ROW_NUM, 3, storage) = p.z();
        ++i;
    }
}

/**
 * @brief Export the indices of a quad per vertex to a buffer.
 *
 * This function exports the vertex indices of a quad per vertex to a buffer.
 * The buffer must be preallocated with the correct size (number of vertices
 * times 6).
 *
 * The indices are stored in the following order:
 *
 * ```
 * 0 1 2 1 3 2
 * ```
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexQuadIndicesToBuffer(const MeshType& mesh, auto* buffer)
{
    // creates indices for two triangles (quad) for each vertex
    //
    // 2-------3
    // | \     |
    // |  \    |
    // |   \   |
    // |    \  |
    // |     \ |
    // 0-------1
    //
    // - Triangle 1: [0, 1, 2]
    // - Triangle 2: [1, 3, 2]
    //
    for (uint i = 0; const auto& v : mesh.vertices()) {
        const uint baseIdx  = i * 6;
        const uint quadIdx  = i * 4;
        buffer[baseIdx + 0] = quadIdx + 0;
        buffer[baseIdx + 1] = quadIdx + 1;
        buffer[baseIdx + 2] = quadIdx + 2;
        buffer[baseIdx + 3] = quadIdx + 1;
        buffer[baseIdx + 4] = quadIdx + 3;
        buffer[baseIdx + 5] = quadIdx + 2;
        ++i;
    }
}

/**
 * @brief Export into a buffer the sizes of the faces of a Mesh, and return the
 * sum of the sizes.
 *
 * This function could be useful when dealing with polygonal meshes: it exports
 * the sizes of the faces of a mesh to a buffer. Sizes are stored following the
 * order the faces appear in the mesh. The buffer must be preallocated with the
 * correct size (number of faces).
 *
 * The return value is the sum of the sizes of the faces. This value is useful
 * when you need to allocate a buffer to store the vertex indices of the faces
 * (its size is the sum of the face sizes).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @return sum of the sizes of the faces
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
uint faceSizesToBuffer(const MeshType& mesh, auto* buffer)
{
    uint sum = 0;
    for (uint i = 0; const auto& f : mesh.faces()) {
        buffer[i] = f.vertexNumber();
        sum += f.vertexNumber();
        ++i;
    }
    return sum;
}

/**
 * @brief Export into a buffer the vertex indices for each face of a Mesh. Faces
 * can be polygons.
 *
 * This function exports the vertex indices of the polygonal faces of a mesh to
 * a buffer. Indices are stored consecutively in the buffer, following the order
 * the faces appear in the mesh. The buffer must be preallocated with the
 * correct size (sum of the sizes of the faces).
 *
 * You can use the function @ref vcl::faceSizesToBuffer to get the sizes of the
 * faces and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * std::vector<uint> faceSizes(myMesh.faceNumber());
 * uint sum = vcl::faceSizesToBuffer(myMesh, sizes.data());
 * std::vector<uint> faceIndices(sum);
 * vcl::faceVertexIndicesToBuffer(myMesh, faceIndices.data());
 *
 * // read indices for each face
 * uint offset = 0;
 * for (uint i = 0; i < myMesh.faceNumber(); ++i) {
 *     uint size = faceSizes[i];
 *     for (uint j = 0; j < size; ++j) {
 *         uint vIdx = faceIndices[offset + j];
 *         // do something with the vertex index
 *     }
 *     offset += size;
 * }
 * @endcode
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceVertexIndicesToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    bool            getIndicesAsIfContainerCompact = true)
{
    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    for (uint i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            buffer[i] = vIndex(f, j);
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each face of a Mesh. Faces
 * can be polygons, and the number of output columns can be set by the user with
 * the `largestFaceSize` parameter.
 *
 * This function exports the vertex indices of the polygonal faces of a mesh to
 * a buffer. Indices are stored following the order the faces appear in the
 * mesh. The buffer must be preallocated with the correct size (number of faces
 * times `largestFaceSize`). For each face that has less vertices than the
 * largest face size, the remaining indices are set to -1.
 *
 * For triangle meshes, you can set `largestFaceSize` to 3. For polygonal
 * meshes, you can use the function @ref vcl::largestFaceSize to get the largest
 * face size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lfs = vcl::largestFaceSize(myMesh);
 * Eigen::MatrixXi faceIndices(myMesh.faceNumber(), lfs);
 * vcl::faceVertexIndicesToBuffer(
 *     myMesh, faceIndices.data(), lfs, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestFaceSize: size of the largest face in the mesh
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceVertexIndicesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestFaceSize,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    bool              getIndicesAsIfContainerCompact = true,
    uint              rowNumber                      = UINT_NULL)
{
    using namespace detail;

    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    const uint ROW_NUM = rowNumber == UINT_NULL ? mesh.faceNumber() : rowNumber;

    for (uint i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < largestFaceSize; ++j) {
            at(buffer, i, j, ROW_NUM, largestFaceSize, storage) =
                j < f.vertexNumber() ? vIndex(f, j) : -1;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the vertex indices for each triangle computed
 * by triangulating the faces of a Mesh.
 *
 * This function exports the vertex indices of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Indices are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 3).
 *
 * You can use the function @ref vcl::countTriangulatedTriangles to get the
 * number of resulting triangles and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint numTris = vcl::countTriangulatedTriangles(myMesh);
 * Eigen::MatrixXi triIndices(numTris, 3);
 * vcl::TriPolyIndexBiMap indexMap;
 * vcl::triangulatedFaceVertexIndicesToBuffer(
 *     myMesh, triIndices.data(), indexMap, MatrixStorageType::COLUMN_MAJOR,
 *     numTris);
 * @endcode
 *
 * The input indexMap is used to map each triangle to the face index. If the
 * storage of the buffer is column major, the number of resulting triangles
 * (that should be known when calling this function) should be given as input.
 * If the number of resulting triangles is not given, the function will compute
 * it again.
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[out] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] numTriangles: number of resulting triangles (necessary only if
 * the storage is column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceVertexIndicesToBuffer(
    const MeshType&    mesh,
    auto*              buffer,
    TriPolyIndexBiMap& indexMap     = detail::indexMap,
    MatrixStorageType  storage      = MatrixStorageType::ROW_MAJOR,
    uint               numTriangles = UINT_NULL,
    bool               getIndicesAsIfContainerCompact = true)
{
    using namespace detail;

    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a face (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    // there will be at least a triangle for each polygon
    indexMap.clear();
    indexMap.reserve(mesh.faceNumber(), mesh.faceContainerSize());

    if constexpr (TriangleMeshConcept<MeshType>) {
        // construct the indexMap, which maps each triangle to the face index
        for (uint t = 0; const auto& f : mesh.faces()) {
            // map the ith triangle to the f face
            indexMap.insert(t, f.index());
            ++t;
        }

        return faceVertexIndicesToBuffer(
            mesh, buffer, 3, storage, getIndicesAsIfContainerCompact);
    }
    else {
        // if the user did not give the number of triangles, and the buffer
        // storage is column major, we need to compute the number of resulting
        // triangles
        if (numTriangles == UINT_NULL &&
            storage == MatrixStorageType::COLUMN_MAJOR &&
            mesh.faceNumber() > 0) {
            numTriangles = countTriangulatedTriangles(mesh);
        }
        for (uint t = 0; const auto& f : mesh.faces()) {
            std::vector<uint> vind = vcl::earCut(f);

            // for each triangle of the triangulation (t is the triangle index)
            for (uint vi = 0; vi < vind.size(); vi += 3) {
                // map the t-th triangle to the f polygonal face
                indexMap.insert(t, f.index());

                for (uint k = 0; k < 3; ++k)
                    at(buffer, t, k, numTriangles, 3, storage) =
                        vIndex(f, vind[vi + k]);

                ++t;
            }
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each edge of a Mesh.
 *
 * This function exports the vertex indices of the edges of a mesh to a
 * buffer. Indices are stored following the order the edges appear in the mesh.
 * The buffer must be preallocated with the correct size (number of edges times
 * 2).
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeVertexIndicesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    bool              getIndicesAsIfContainerCompact = true,
    uint              rowNumber                      = UINT_NULL)
{
    using namespace detail;

    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a edge (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    const uint ROW_NUM = rowNumber == UINT_NULL ? mesh.edgeNumber() : rowNumber;

    for (uint i = 0; const auto& e : mesh.edges()) {
        at(buffer, i, 0, ROW_NUM, 2, storage) = vIndex(e, 0);
        at(buffer, i, 1, ROW_NUM, 2, storage) = vIndex(e, 1);

        ++i;
    }
}

/**
 * @brief Export into a buffer the vertex indices for each edge that composes
 * the wireframe of the Mesh (i.e., the edges of the faces).
 *
 * This function exports the vertex indices of the wireframe edges of a mesh to
 * a buffer. Indices are stored following the order the edges appear in the
 * faces. The buffer must be preallocated with the correct size (number of
 * references to vertices in the mesh faces times 2 - see @ref
 * countPerFaceVertexReferences).
 *
 * @note As a default behaviour (`getIndicesAsIfContainerCompact == true`) the
 * function stores the vertex indices as if the vertex container of the mesh is
 * compact. This means that, if the mesh has deleted vertices, the vertex
 * indices stored in the buffer may not correspond to the vertex indices of the
 * mesh. If you want to store the actual vertex indices in the input mesh, set
 * `getIndicesAsIfContainerCompact` to false.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] getIndicesAsIfContainerCompact: if true, the function will
 * store the vertex indices as if the vertex container of the mesh is compact.
 * If false, the actual vertex indices in the input mesh will be stored.
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of references to vertices in the mesh faces times 2) - used only when
 * storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void wireframeVertexIndicesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    bool              getIndicesAsIfContainerCompact = true,
    uint              rowNumber                      = UINT_NULL)
{
    using namespace detail;

    const std::vector<uint> vertCompIndices =
        detail::vertCompactIndices(mesh, getIndicesAsIfContainerCompact);

    // lambda to get the vertex index of a edge (considering compact indices)
    auto vIndex = detail::vIndexLambda(mesh, vertCompIndices);

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? countPerFaceVertexReferences(mesh) : rowNumber;

    for (uint i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            uint v0 = vIndex(f, j);
            uint v1 = vIndex(f, (j + 1) % f.vertexNumber());

            at(buffer, i, 0, ROW_NUM, 2, storage) = v0;
            at(buffer, i, 1, ROW_NUM, 2, storage) = v1;

            ++i;
        }
    }
}

/**
 * @brief Export the selection status of the elements identified by
 * `ELEM_ID` of a mesh to a buffer.
 *
 * This function exports the selection status of the elements identified by
 * `ELEM_ID` of a mesh to a buffer. Values are stored in the buffer
 * following the order the elements appear in the mesh. The buffer must be
 * preallocated with the correct size (number of elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.elementNumber<ElemId::VERTEX>());
 * vcl::elementSelectionToBuffer<ElemId::VERTEX>(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this
 * function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    for (uint i = 0; const auto& e : mesh.template elements<ELEM_ID>()) {
        buffer[i] = e.selected();
        ++i;
    }
}

/**
 * @brief Export the selection status of the vertices of a mesh to a buffer.
 *
 * This function exports the selection status of the vertices of a mesh to a
 * buffer. Values are stored in the buffer following the order the vertices
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.vertexNumber());
 * vcl::vertexSelectionToBuffer(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::VERTEX>(mesh, buffer);
}

/**
 * @brief Export the selection status of the faces of a mesh to a buffer.
 *
 * This function exports the selection status of the faces of a mesh to a
 * buffer. Values are stored in the buffer following the order the faces appear
 * in the mesh. The buffer must be preallocated with the correct size (number of
 * elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.faceNumber());
 * vcl::faceSelectionToBuffer(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the selection status of the edges of a mesh to a buffer.
 *
 * This function exports the selection status of the edges of a mesh to a
 * buffer. Values are stored in the buffer following the order the edges appear
 * in the mesh. The buffer must be preallocated with the correct size (number of
 * elements).
 *
 * Usage example with std::vector<bool>:
 *
 * @code{.cpp}
 * std::vector<bool> vec(myMesh.edgeNumber());
 * vcl::edgeSelectionToBuffer(myMesh, vec.data());
 * @endif
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    elementSelectionToBuffer<ElemId::EDGE>(mesh, buffer);
}

/**
 * @brief Export the element normals identified by `ELEM_ID` of a mesh to a
 * buffer.
 *
 * This function exports the element normals identified by `ELEM_ID` of a mesh
 * to a buffer. Normals are stored in the buffer following the order the
 * elements appear in the mesh. The buffer must be preallocated with the correct
 * size (number of elements times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.template number<ELEM_ID>() : rowNumber;

    for (uint        i = 0;
         const auto& n : mesh.template elements<ELEM_ID>() | views::normals) {
        at(buffer, i, 0, ROW_NUM, 3, storage) = n.x();
        at(buffer, i, 1, ROW_NUM, 3, storage) = n.y();
        at(buffer, i, 2, ROW_NUM, 3, storage) = n.z();

        ++i;
    }
}

/**
 * @brief Export the vertex normals of a mesh to a buffer.
 *
 * This function exports the vertex normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsToBuffer<ElemId::VERTEX>(mesh, buffer, storage, rowNumber);
}

/**
 * @brief Export the face normals of a mesh to a buffer.
 *
 * This function exports the face normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsToBuffer<ElemId::FACE>(mesh, buffer, storage, rowNumber);
}

/**
 * @brief Export into a buffer the normals for each triangle computed by
 * triangulating the faces of a Mesh.
 *
 * This function exports the normals of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Normals are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 3).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map. You can
 * use the function @ref vcl::countTriangulatedTriangles to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of triangles in the map) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceNormalsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    MatrixStorageType        storage   = MatrixStorageType::ROW_MAJOR,
    uint                     rowNumber = UINT_NULL)
{
    using namespace detail;

    requirePerFaceNormal(mesh);

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? indexMap.triangleNumber() : rowNumber;

    for (const auto& f : mesh.faces()) {
        const auto& n     = f.normal();
        uint        first = indexMap.triangleBegin(f.index());
        uint        last  = first + indexMap.triangleNumber(f.index());
        for (uint t = first; t < last; ++t) {
            at(buffer, t, 0, ROW_NUM, 3, storage) = n.x();
            at(buffer, t, 1, ROW_NUM, 3, storage) = n.y();
            at(buffer, t, 2, ROW_NUM, 3, storage) = n.z();
        }
    }
}

/**
 * @brief Export the edge normals of a mesh to a buffer.
 *
 * This function exports the edge normals of a mesh to a buffer. Normals are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges times 3).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeNormalsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsToBuffer<ElemId::EDGE>(mesh, buffer, storage, rowNumber);
}

/**
 * @brief Export the element colors identified by `ELEM_ID` of a mesh to a
 * buffer having a value for each color component (RGBA).
 *
 * This function exports the element colors identified by `ELEM_ID` of a mesh
 * to a buffer. Colors are stored in the buffer following the order the elements
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    using namespace detail;

    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.template number<ELEM_ID>() : rowNumber;

    for (uint        i = 0;
         const auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
        at(buffer, i, 0, ROW_NUM, 4, storage) = R_INT ? c.red() : c.redF();
        at(buffer, i, 1, ROW_NUM, 4, storage) = R_INT ? c.green() : c.greenF();
        at(buffer, i, 2, ROW_NUM, 4, storage) = R_INT ? c.blue() : c.blueF();
        at(buffer, i, 3, ROW_NUM, 4, storage) = R_INT ? c.alpha() : c.alphaF();

        ++i;
    }
}

/**
 * @brief Export the element colors identified by `ELEM_ID` of a mesh to a
 * buffer having a value for each color (the color is packed in a single 32 bit
 * value using the provided format).
 *
 * This function exports the element colors identified by `ELEM_ID` of a mesh to
 * a buffer. Colors are stored in the buffer following the order the elements
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of elements).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible when
 * the mesh has deleted elements. To be sure to have a direct correspondence,
 * compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    for (uint        i = 0;
         const auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
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
 * @brief Export the vertex colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the vertex colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices times
 * 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsToBuffer<ElemId::VERTEX>(
        mesh, buffer, storage, representation, rowNumber);
}

/**
 * @brief Export the vertex colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the vertex colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the vertices appear in the mesh. The
 * buffer must be preallocated with the correct size (number of vertices).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible when
 * the mesh has deleted vertices. To be sure to have a direct correspondence,
 * compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::VERTEX>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the face colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the face colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsToBuffer<ElemId::FACE>(
        mesh, buffer, storage, representation, rowNumber);
}

/**
 * @brief Export into a buffer the color values (RGBA) for each triangle
 * computed by triangulating the faces of a Mesh.
 *
 * This function exports the colors of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Colors are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles* times 4).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map. You can
 * use the function @ref vcl::countTriangulatedTriangles to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of triangles in the map) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceColorsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    MatrixStorageType        storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation    representation = Color::Representation::INT_0_255,
    uint                     rowNumber      = UINT_NULL)
{
    using namespace detail;

    requirePerFaceColor(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? indexMap.triangleNumber() : rowNumber;

    for (const auto& f : mesh.faces()) {
        const auto& c     = f.color();
        uint        first = indexMap.triangleBegin(f.index());
        uint        last  = first + indexMap.triangleNumber(f.index());
        for (uint t = first; t < last; ++t) {
            at(buffer, t, 0, ROW_NUM, 4, storage) = R_INT ? c.red() : c.redF();
            at(buffer, t, 1, ROW_NUM, 4, storage) =
                R_INT ? c.green() : c.greenF();
            at(buffer, t, 2, ROW_NUM, 4, storage) =
                R_INT ? c.blue() : c.blueF();
            at(buffer, t, 3, ROW_NUM, 4, storage) =
                R_INT ? c.alpha() : c.alphaF();
        }
    }
}

/**
 * @brief Export the face colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the face colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the faces appear in the mesh. The
 * buffer must be preallocated with the correct size (number of faces).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::FACE>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the colors for each triangle computed by triangulating the
 * faces of a Mesh to a buffer having a value for each color (the color is
 * packed in a single 32 bit value using the provided format)..
 *
 * This function exports the colors of the triangles computed by
 * triangulating the faces of a mesh to a buffer. Colors are stored following
 * the order the faces appear in the mesh. The buffer must be preallocated with
 * the correct size (number of *resulting triangles*).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map. You can
 * use the function @ref vcl::countTriangulatedTriangles to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceColorsToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap,
    Color::Format            colorFormat)
{
    requirePerElementComponent<ElemId::FACE, CompId::COLOR>(mesh);

    for (const auto& f : mesh.faces()) {
        const auto& c     = f.color();
        uint        first = indexMap.triangleBegin(f.index());
        uint        last  = first + indexMap.triangleNumber(f.index());
        for (uint t = first; t < last; ++t) {
            switch (colorFormat) {
                using enum Color::Format;
            case ABGR: buffer[t] = c.abgr(); break;
            case ARGB: buffer[t] = c.argb(); break;
            case RGBA: buffer[t] = c.rgba(); break;
            case BGRA: buffer[t] = c.bgra(); break;
            }
        }
    }
}

/**
 * @brief Export the edge colors of a mesh to a buffer having a value for each
 * color component (RGBA).
 *
 * This function exports the edge colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges times 4).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] representation: representation of the color components (integer or
 * float)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsToBuffer(
    const MeshType&       mesh,
    auto*                 buffer,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsToBuffer<ElemId::EDGE>(
        mesh, buffer, storage, representation, rowNumber);
}

/**
 * @brief Export the edge colors of a mesh to a buffer having a value for each
 * color (the color is packed in a single 32 bit value using the provided
 * format).
 *
 * This function exports the edge colors of a mesh to a buffer. Colors are
 * stored in the buffer following the order the edges appear in the mesh. The
 * buffer must be preallocated with the correct size (number of edges).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] colorFormat: format of the color components
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsToBuffer(
    const MeshType& mesh,
    auto*           buffer,
    Color::Format   colorFormat)
{
    elementColorsToBuffer<ElemId::EDGE>(mesh, buffer, colorFormat);
}

/**
 * @brief Export the element quality identified by `ELEM_ID` of a mesh to a
 * buffer.
 *
 * This function exports the element quality identified by `ELEM_ID` of a mesh
 * to a buffer. Quality values are stored in the buffer following the order the
 * elements appear in the mesh. The buffer must be preallocated with the correct
 * size (number of elements).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible when
 * the mesh has deleted elements. To be sure to have a direct correspondence,
 * compact the element container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    for (uint        i = 0;
         const auto& q : mesh.template elements<ELEM_ID>() | views::quality) {
        buffer[i] = q;
        ++i;
    }
}

/**
 * @brief Export the vertex quality of a mesh to a buffer.
 *
 * This function exports the vertex quality of a mesh to a buffer. Quality
 * values are stored in the buffer following the order the vertices appear in
 * the mesh. The buffer must be preallocated with the correct size (number of
 * vertices).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible when
 * the mesh has deleted vertices. To be sure to have a direct correspondence,
 * compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::VERTEX>(mesh, buffer);
}

/**
 * @brief Export the face quality of a mesh to a buffer.
 *
 * This function exports the face quality of a mesh to a buffer. Quality values
 * are stored in the buffer following the order the faces appear in the mesh.
 * The buffer must be preallocated with the correct size (number of faces).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void faceQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the edge quality of a mesh to a buffer.
 *
 * This function exports the edge quality of a mesh to a buffer. Quality values
 * are stored in the buffer following the order the edges appear in the mesh.
 * The buffer must be preallocated with the correct size (number of edges).
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the edge indices of the mesh. This scenario is possible when
 * the mesh has deleted edges. To be sure to have a direct correspondence,
 * compact the edge container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void edgeQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Export the vertex texcoords of a mesh to a buffer.
 *
 * This function exports the vertex texcoords of a mesh to a buffer. Texcoords
 * are stored in the buffer following the order the vertices appear in the mesh.
 * The buffer must be preallocated with the correct size (number of vertices
 * times 2).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexTexCoordsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    requirePerVertexTexCoord(mesh);

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.vertexNumber() : rowNumber;

    for (uint i = 0; const auto& t : mesh.vertices() | views::texCoords) {
        at(buffer, i, 0, ROW_NUM, 2, storage) = t.u();
        at(buffer, i, 1, ROW_NUM, 2, storage) = t.v();

        ++i;
    }
}

/**
 * @brief Export the vertex material indices of a mesh to a buffer.
 *
 * This function exports the vertex material indices of a mesh to a buffer.
 * Indices are stored in the buffer following the order the vertices appear in
 * the mesh. The buffer must be preallocated with the correct size (number of
 * vertices).
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexMaterialIndicesToBuffer(const MeshType& mesh, auto* buffer)
{
    requirePerVertexMaterialIndex(mesh);

    for (uint i = 0; const auto& v : mesh.vertices()) {
        buffer[i] = v.materialIndex();
        ++i;
    }
}

/**
 * @brief Export the vertex material indices of a mesh into a buffer that has
 * a material index for each face of the mesh (as if the indices were face
 * material indices).
 *
 * This function exports the vertex material indices of a mesh to a buffer. The
 * indices are stored in the buffer following the order the faces appear in the
 * mesh. The buffer must be preallocated with the correct size (number of
 * faces).
 *
 * For each face, the function takes the material index of the first vertex of
 * the face and stores it in the buffer.
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void vertexMaterialIndicesAsFaceTexCoordIndicesToBuffer(
    const MeshType& mesh,
    auto*           buffer)
{
    requirePerVertexMaterialIndex(mesh);

    for (uint i = 0; const auto& f : mesh.faces()) {
        ushort ti = f.vertex(0)->materialIndex();
        buffer[i] = ti;
        ++i;
    }
}

/**
 * @brief Export the vertex material indices of a mesh into a buffer that has
 * a material index for each triangle of the mesh (as if the indices were face
 * material indices).
 *
 * This function exports the vertex material indices of a mesh to a buffer. The
 * indices are stored in the buffer following the order the faces appear in the
 * mesh. The buffer must be preallocated with the correct size (number of
 * triangles).
 *
 * For each triangle computed from the triangulation of a face, the function
 * takes the material index of the first vertex of the face that contains the
 * triangle and stores it in the buffer.
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void vertexMaterialIndicesAsTriangulatedFaceTexCoordIndicesToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap)
{
    requirePerVertexMaterialIndex(mesh);

    for (const auto& f : mesh.faces()) {
        ushort ti    = f.vertex(0)->materialIndex();
        uint   first = indexMap.triangleBegin(f.index());
        uint   last  = first + indexMap.triangleNumber(f.index());
        for (uint t = first; t < last; ++t) {
            buffer[t] = ti;
        }
    }
}

/**
 * @brief Export into a buffer the per face wedge texture coordinates of a mesh.
 * Faces can be polygons, and the number of output columns in the buffer can be
 * controlled by the user with the `largestFaceSize` parameter.
 *
 * This function exports the per face wedge texture coordinates of a mesh to a
 * buffer. Texture coordinates are stored in the buffer following the order the
 * faces appear in the mesh. The buffer must be preallocated with the correct
 * size (number of faces times `largestFaceSize` times 2). For each face that
 * has less wedge texcoords than `largestFaceSize`, the remaining columns are
 * filled with 0.
 *
 * For triangle meshes, you can set `largestFaceSize` to 3. For polygonal
 * meshes, you can use the function @ref vcl::largestFaceSize to get the largest
 * face size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lfs = vcl::largestFaceSize(myMesh);
 * Eigen::MatrixXi faceIndices(myMesh.faceNumber(), lfs * 2);
 * vcl::faceWedgeTexCoordsToBuffer(
 *     myMesh, faceIndices.data(), lfs, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestFaceSize: size of the largest face in the mesh
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceWedgeTexCoordsToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestFaceSize = 3,
    MatrixStorageType storage         = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber       = UINT_NULL)
{
    requirePerFaceWedgeTexCoords(mesh);

    const uint ROW_NUM = rowNumber == UINT_NULL ? mesh.faceNumber() : rowNumber;

    for (uint i = 0; const auto& f : mesh.faces()) {
        for (uint j = 0; j < largestFaceSize * 2; ++j) {
            uint fi    = j / 2;
            uint index = i * largestFaceSize * 2 + j;
            if (storage == MatrixStorageType::COLUMN_MAJOR)
                index = j * ROW_NUM + i;
            if (fi < f.vertexNumber()) {
                const auto& w = f.wedgeTexCoord(fi);
                if (j % 2 == 0)
                    buffer[index] = w.u();
                else
                    buffer[index] = w.v();
            }
            else {
                buffer[index] = 0;
            }
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the per face material indices of a mesh.
 *
 * This function exports the per face material indices of a mesh to a buffer.
 * Material indices are stored in the buffer following the order the faces
 * appear in the mesh. The buffer must be preallocated with the correct size
 * (number of faces).
 *
 * @note This function does not guarantee that the rows of the buffer
 * correspond to the face indices of the mesh. This scenario is possible when
 * the mesh has deleted faces. To be sure to have a direct correspondence,
 * compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceMaterialIndicesToBuffer(const MeshType& mesh, auto* buffer)
{
    requirePerFaceMaterialIndex(mesh);

    for (uint i = 0; const auto& f : mesh.faces()) {
        buffer[i] = f.materialIndex();
        ++i;
    }
}

/**
 * @brief Export into a buffer the per triangle material indices of a mesh.
 * Triangles are computed by triangulating the faces of the mesh.
 *
 * This function exports the per triangle material indices of a mesh to a
 * buffer. Material indices are stored in the buffer following the order the
 * faces appear in the mesh. The buffer must be preallocated with the correct
 * size (number of triangles).
 *
 * The function requires an already computed index map, which maps each triangle
 * to the face index and vice versa. You can use the @ref
 * vcl::triangulatedFaceIndicesToBuffer function to get the index map. You can
 * use the function @ref vcl::countTriangulatedTriangles to get the number of
 * resulting triangles and allocate the buffer accordingly.
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] indexMap: map from triangle index to face index
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void triangulatedFaceMaterialIndicesToBuffer(
    const MeshType&          mesh,
    auto*                    buffer,
    const TriPolyIndexBiMap& indexMap)
{
    requirePerFaceMaterialIndex(mesh);

    for (const auto& f : mesh.faces()) {
        uint first = indexMap.triangleBegin(f.index());
        uint last  = first + indexMap.triangleNumber(f.index());
        for (uint t = first; t < last; ++t) {
            buffer[t] = f.materialIndex();
        }
    }
}

/**
 * @brief Export wedge texture coordinates to a buffer of the duplicated
 * vertex texture coordinates.
 *
 * Given the list of vertices to duplicate, this function exports to the given
 * buffer the wedge texture coordinates as if they were vertex texture
 * coordinates, and appending only the texture coordinates of the vertices to
 * duplicate.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function:
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
 * wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(mesh, vertWedgeMap,
 *     facesToReassign, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices times 2).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the wedge texture coordinates.
 * @param[in] vertWedgeMap: The map from non-duplicated vertex index to face
 * index and wedge index in the face.
 * @param[in] facesToReassign: The list of lists of pairs face/vertex index in
 * the face that must be reassigned to the duplicated vertices. Each list of
 * pairs is the list of faces that must be reassigned to the corresponding
 * duplicated vertex.
 * @param[out] buffer: The buffer where to export the vertex wedge texture
 * coordinates.
 * @param[in] storage: The storage type of the matrix (row or column major).
 *
 * @ingroup append_replace_to_buffer
 */
template<FaceMeshConcept MeshType>
void wedgeTexCoordsAsDuplicatedVertexTexCoordsToBuffer(
    const MeshType&                                     mesh,
    const std::vector<std::pair<vcl::uint, vcl::uint>>& vertWedgeMap,
    const std::list<std::list<std::pair<vcl::uint, vcl::uint>>>&
                      facesToReassign,
    auto*             buffer,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR)
{
    using namespace detail;
    using TexCoordType = typename MeshType::FaceType::WedgeTexCoordType;

    requirePerFaceWedgeTexCoords(mesh);

    const uint ROW_NUM = mesh.vertexNumber() + facesToReassign.size();

    // first export the texcoords of the non-duplicated vertices, using the
    // vertWedgeMap to get the texcoord index in the face
    uint vi = 0; // current vertex (or current row in the matrix)
    for (const auto& v : mesh.vertices()) {
        TexCoordType w;
        uint         fInd = vertWedgeMap[vi].first;
        uint         wInd = vertWedgeMap[vi].second;

        // check if the vi is referenced
        if (fInd != UINT_NULL && wInd != UINT_NULL) {
            w = mesh.face(fInd).wedgeTexCoord(wInd);
        }
        at(buffer, vi, 0, ROW_NUM, 2, storage) = w.u();
        at(buffer, vi, 1, ROW_NUM, 2, storage) = w.v();

        ++vi;
    }

    // then append the texcoords of the duplicated vertices, that can be found
    // by looking into the any of the facesToReassign element lists
    for (const auto& list : facesToReassign) {
        assert(list.begin() != list.end());
        const auto& p    = list.front();
        uint        fInd = p.first;
        uint        wInd = p.second;

        const auto& w = mesh.face(fInd).wedgeTexCoord(wInd);
        at(buffer, vi, 0, ROW_NUM, 2, storage) = w.u();
        at(buffer, vi, 1, ROW_NUM, 2, storage) = w.v();

        ++vi;
    }
}

/**
 * @brief Export face material indices to a buffer of the duplicated vertex
 * material indices.
 *
 * Given the list of vertices to duplicate, this function exports to the given
 * buffer the face material indices as if they were vertex material
 * indices, and appending only the material indices of the vertices to
 * duplicate.
 *
 * Typical usage of this function is after the @ref
 * countVerticesToDuplicateByWedgeTexCoords function:
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
 * faceMaterialIndicesAsDuplicatedVertexMaterialIndicesToBuffer(mesh,
 *     vertWedgeMap, facesToReassign, buffer.data());
 * @endcode
 *
 * @note The buffer must be preallocated with the correct size (total number of
 * vertices).
 *
 * @tparam MeshType: The type of the mesh.
 *
 * @param[in] mesh: The mesh from which take the face material indices.
 * @param[in] vertWedgeMap: The map from non-duplicated vertex index to face
 * index and wedge index in the face.
 * @param[in] facesToReassign: The list of lists of pairs face/vertex index in
 * the face that must be reassigned to the duplicated vertices. Each list of
 * pairs is the list of faces that must be reassigned to the corresponding
 * duplicated vertex.
 * @param[out] buffer: The buffer where to export the vertex material indices.
 *
 * @ingroup append_replace_to_buffer
 */
template<FaceMeshConcept MeshType>
void faceMaterialIndicesAsDuplicatedVertexMaterialIndicesToBuffer(
    const MeshType&                                     mesh,
    const std::vector<std::pair<vcl::uint, vcl::uint>>& vertWedgeMap,
    const std::list<std::list<std::pair<vcl::uint, vcl::uint>>>&
          facesToReassign,
    auto* buffer)
{
    requirePerFaceMaterialIndex(mesh);

    const uint VERT_NUM = mesh.vertexNumber() + facesToReassign.size();

    // first export the material indices of the non-duplicated vertices, using
    // the vertWedgeMap to get the material index in the face
    uint vi = 0; // current vertex (or current row in the matrix)
    for (const auto& v : mesh.vertices()) {
        uint   fInd = vertWedgeMap[vi].first;
        ushort ti   = mesh.face(fInd).materialIndex();
        buffer[vi]  = ti;
        ++vi;
    }

    // then append the material indices of the duplicated vertices, that can be
    // found by looking into the any of the facesToReassign element lists
    for (const auto& list : facesToReassign) {
        assert(list.begin() != list.end());
        const auto& p    = list.front();
        uint        fInd = p.first;

        ushort ti  = mesh.face(fInd).materialIndex();
        buffer[vi] = ti;
        ++vi;
    }
}

/**
 * @brief Export into a buffer the adjacent vertex indices for each vertex of a
 * Mesh. The number of adjacent vertices for each vertex can be different, so
 * the user must provide the size of the largest adjacency list with the
 * `largestAdjacentVerticesSize` parameter.
 * For vertices that have less adjacent vertices than
 * `largestAdjacentVerticesSize`, the remaining entries are filled with
 * `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerVertexAdjacentVerticesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerVertexAdjacentVerticesNumber(myMesh);
 * Eigen::MatrixXi vertexAdj(myMesh.vertexNumber(), lva);
 * vcl::vertexAdjacentVerticesToBuffer(
 *    myMesh, vertexAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentVerticesSize: size of the largest per-vertex vertex
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<MeshConcept MeshType>
void vertexAdjacentVerticesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentVerticesSize,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    requireVertexContainerCompactness(mesh);
    requirePerVertexAdjacentVertices(mesh);

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.vertexNumber() : rowNumber;

    const uint COL_NUM = largestAdjacentVerticesSize;

    for (uint i = 0; const auto& v : mesh.vertices()) {
        uint adjIndex = 0;
        for (const auto* a : v.adjVertices()) {
            uint idx = a ? a->index() : UINT_NULL;
            at(buffer, i, adjIndex, ROW_NUM, COL_NUM, storage) = idx;
            ++adjIndex;
        }
        // fill the remaining entries with UINT_NULL
        for (; adjIndex < COL_NUM; ++adjIndex) {
            at(buffer, i, adjIndex, ROW_NUM, COL_NUM, storage) = UINT_NULL;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the adjacent faces indices for each ELEM_ID
 * element of a Mesh. The number of adjacent faces for each ELEM_ID can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentFacesSize` parameter. For elements that have less
 * adjacent faces than `largestAdjacentFacesSize`, the remaining entries
 * are filled with `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerElementAdjacentFacesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerElementAdjacentFacesNumber<ELEM_ID>(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.number<ELEM_ID>(), lva);
 * vcl::elementAdjacentFacesToBuffer<ELEM_ID>(
 *    myMesh, faceAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentFacesSize: size of the largest per-element face
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, FaceMeshConcept MeshType>
void elementAdjacentFacesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentFacesSize,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    requireFaceContainerCompactness(mesh);
    requirePerElementComponent<ELEM_ID, CompId::ADJACENT_FACES>(mesh);

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.template number<ELEM_ID>() : rowNumber;

    const uint COL_NUM = largestAdjacentFacesSize;

    for (uint i = 0; const auto& v : mesh.template elements<ELEM_ID>()) {
        uint adjIndex = 0;
        for (const auto* a : v.adjFaces()) {
            uint idx = a ? a->index() : UINT_NULL;
            at(buffer, i, adjIndex, ROW_NUM, COL_NUM, storage) = idx;
            ++adjIndex;
        }
        // fill the remaining entries with UINT_NULL
        for (; adjIndex < COL_NUM; ++adjIndex) {
            at(buffer, i, adjIndex, ROW_NUM, COL_NUM, storage) = UINT_NULL;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the adjacent faces indices for each vertex
 * of a Mesh. The number of adjacent faces for each vertex can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentFacesSize` parameter. For elements that have less
 * adjacent faces than `largestAdjacentFacesSize`, the remaining entries
 * are filled with `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerVertexAdjacentFacesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerVertexAdjacentFacesNumber(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.vertexNumber(), lva);
 * vcl::vertexAdjacentFacesToBuffer(
 *    myMesh, faceAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentFacesSize: size of the largest per-vertex face
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void vertexAdjacentFacesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentFacesSize,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementAdjacentFacesToBuffer<ElemId::VERTEX>(
        mesh, buffer, largestAdjacentFacesSize, storage, rowNumber);
}

/**
 * @brief Export into a buffer the adjacent faces indices for each face
 * of a Mesh. The number of adjacent faces for each face is expected to
 * be equal to the largestFaceSize (see @ref vcl::largestFaceSize).
 *
 * @code{.cpp}
 * uint lfs = vcl::largestFaceSize(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.faceNumber(), lfs);
 * vcl::faceAdjacentFacesToBuffer(
 *    myMesh, faceAdj.data(), lfs, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestFacesSize: size of the largest face in the mesh
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void faceAdjacentFacesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestFacesSize,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementAdjacentFacesToBuffer<ElemId::FACE>(
        mesh, buffer, largestFacesSize, storage, rowNumber);
}

/**
 * @brief Export into a buffer the adjacent faces indices for each edge
 * of a Mesh. The number of adjacent faces for each edge can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentFacesSize` parameter. For elements that have less
 * adjacent faces than `largestAdjacentFacesSize`, the remaining entries
 * are filled with `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerEdgeAdjacentFacesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerEdgeAdjacentFacesNumber(myMesh);
 * Eigen::MatrixXi faceAdj(myMesh.edgeNumber(), lva);
 * vcl::edgeAdjacentFacesToBuffer(
 *    myMesh, faceAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentFacesSize: size of the largest per-edge face
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<FaceMeshConcept MeshType>
void edgeAdjacentFacesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentFacesSize,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    uint rowNumber            = UINT_NULL) requires (EdgeMeshConcept<MeshType>)
{
    elementAdjacentFacesToBuffer<ElemId::EDGE>(
        mesh, buffer, largestAdjacentFacesSize, storage, rowNumber);
}

/**
 * @brief Export into a buffer the adjacent edges indices for each ELEM_ID
 * element of a Mesh. The number of adjacent edges for each ELEM_ID can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent faces than `largestAdjacentEdgesSize`, the remaining entries
 * are filled with `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerElementAdjacentEdgesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerElementAdjacentEdgesNumber<ELEM_ID>(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.number<ELEM_ID>(), lva);
 * vcl::elementAdjacentEdgesToBuffer<ELEM_ID>(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest per-element edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of elements in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<uint ELEM_ID, EdgeMeshConcept MeshType>
void elementAdjacentEdgesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentEdgesSize,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    requireEdgeContainerCompactness(mesh);
    requirePerElementComponent<ELEM_ID, CompId::ADJACENT_EDGES>(mesh);

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.template number<ELEM_ID>() : rowNumber;

    const uint COL_NUM = largestAdjacentEdgesSize;

    for (uint i = 0; const auto& v : mesh.template elements<ELEM_ID>()) {
        uint adjIndex = 0;
        for (const auto* a : v.adjEdges()) {
            uint idx = a ? a->index() : UINT_NULL;
            at(buffer, i, adjIndex, ROW_NUM, COL_NUM, storage) = idx;
            ++adjIndex;
        }
        // fill the remaining entries with UINT_NULL
        for (; adjIndex < COL_NUM; ++adjIndex) {
            at(buffer, i, adjIndex, ROW_NUM, COL_NUM, storage) = UINT_NULL;
        }
        ++i;
    }
}

/**
 * @brief Export into a buffer the adjacent edges indices for each vertex
 * of a Mesh. The number of adjacent edges for each vertex can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent edges than `largestAdjacentEdgesSize`, the remaining entries
 * are filled with `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerVertexAdjacentEdgesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerVertexAdjacentEdgesNumber(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.vertexNumber(), lva);
 * vcl::vertexAdjacentEdgesToBuffer(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest per-vertex edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of vertices in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void vertexAdjacentEdgesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentEdgesSize,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementAdjacentEdgesToBuffer<ElemId::VERTEX>(
        mesh, buffer, largestAdjacentEdgesSize, storage, rowNumber);
}

/**
 * @brief Export into a buffer the adjacent edges indices for each face
 * of a Mesh. The number of adjacent edges for each face can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent edges than `largestAdjacentEdgesSize`, the remaining entries
 * are filled with `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerFaceAdjacentEdgesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerFaceAdjacentEdgesNumber(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.vertexNumber(), lva);
 * vcl::faceAdjacentEdgesToBuffer(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest per-face edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of faces in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void faceAdjacentEdgesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentEdgesSize,
    MatrixStorageType storage = MatrixStorageType::ROW_MAJOR,
    uint rowNumber            = UINT_NULL) requires (FaceMeshConcept<MeshType>)
{
    elementAdjacentEdgesToBuffer<ElemId::FACE>(
        mesh, buffer, largestAdjacentEdgesSize, storage, rowNumber);
}

/**
 * @brief Export into a buffer the adjacent edges indices for each edge
 * of a Mesh. The number of adjacent edges for each edge can be
 * different, so the user must provide the size of the largest adjacency list
 * with the `largestAdjacentEdgesSize` parameter. For elements that have less
 * adjacent edges than `largestAdjacentEdgesSize`, the remaining entries
 * are filled with `UINT_NULL`.
 *
 * You can use the function @ref vcl::largestPerEdgeAdjacentEdgesNumber to
 * get the largest adjacency size and allocate the buffer accordingly:
 *
 * @code{.cpp}
 * uint lva = vcl::largestPerEdgeAdjacentEdgesNumber(myMesh);
 * Eigen::MatrixXi edgeAdj(myMesh.vertexNumber(), lva);
 * vcl::edgeAdjacentEdgesToBuffer(
 *    myMesh, edgeAdj.data(), lva, MatrixStorageType::COLUMN_MAJOR);
 * @endcode
 *
 * @param[in] mesh: input mesh
 * @param[out] buffer: preallocated buffer
 * @param[in] largestAdjacentEdgesSize: size of the largest per-edge edge
 * adjacency list
 * @param[in] storage: storage type of the matrix (row or column major)
 * @param[in] rowNumber: number of rows of the matrix (if different from the
 * number of edges in the mesh) - used only when storage is column major
 *
 * @ingroup export_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeAdjacentEdgesToBuffer(
    const MeshType&   mesh,
    auto*             buffer,
    uint              largestAdjacentEdgesSize,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementAdjacentEdgesToBuffer<ElemId::EDGE>(
        mesh, buffer, largestAdjacentEdgesSize, storage, rowNumber);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
