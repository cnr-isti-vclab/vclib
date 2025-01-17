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

#include <vclib/mesh/requirements.h>
#include <vclib/views/mesh.h>

namespace vcl {

/**
 * @brief Export the vertex coordinates of a mesh to a buffer.
 *
 * This function exports the vertex coordinates of a mesh to a buffer. Vertices
 * are stored in the buffer following the order they appear in the mesh.  The
 * buffer must be preallocated with the correct size (number of vertices times
 * the number of coordinates per vertex).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param mesh
 * @param buffer
 * @param storage
 */
template<MeshConcept MeshType>
void vertexCoordsToBuffer(
    const MeshType&         mesh,
    auto*                   buffer,
    MatrixStorageType::Enum storage = MatrixStorageType::ROW_MAJOR)
{
    if (storage == MatrixStorageType::ROW_MAJOR) {
        uint i = 0;
        for (const auto& c : mesh.vertices() | views::coords) {
            buffer[i * 3 + 0] = c.x();
            buffer[i * 3 + 1] = c.y();
            buffer[i * 3 + 2] = c.z();
            ++i;
        }
    }
    else {
        uint       i        = 0;
        const uint VERT_NUM = mesh.vertexNumber();
        for (const auto& c : mesh.vertices() | views::coords) {
            buffer[0 * VERT_NUM + i] = c.x();
            buffer[1 * VERT_NUM + i] = c.y();
            buffer[2 * VERT_NUM + i] = c.z();
            ++i;
        }
    }
}

/**
 * @brief Export into a buffer the vertex indices for each triangle of a Mesh.
 *
 * This function exports the vertex indices of the triangles of a mesh to a
 * buffer. Indices are stored following the order the faces appear in the mesh.
 * The buffer must be preallocated with the correct size (number of faces times
 * 3). The function assumes that the input mesh is a triangle mesh (if there are
 * polygonal faces, only the first three vertices are considered).
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param mesh
 * @param buffer
 * @param storage
 */
template<MeshConcept MeshType>
void trianglesToBuffer(
    const MeshType&         mesh,
    auto*                   buffer,
    MatrixStorageType::Enum storage = MatrixStorageType::ROW_MAJOR)
{
    if (storage == MatrixStorageType::ROW_MAJOR) {
        uint i = 0;
        for (const auto& f : mesh.faces()) {
            buffer[i * 3 + 0] = f.vertexIndex(0);
            buffer[i * 3 + 1] = f.vertexIndex(1);
            buffer[i * 3 + 2] = f.vertexIndex(2);
            ++i;
        }
    }
    else {
        uint       i        = 0;
        const uint FACE_NUM = mesh.faceNumber();
        for (const auto& f : mesh.faces()) {
            buffer[0 * FACE_NUM + i] = f.vertexIndex(0);
            buffer[1 * FACE_NUM + i] = f.vertexIndex(1);
            buffer[2 * FACE_NUM + i] = f.vertexIndex(2);
            ++i;
        }
    }
}

/**
 * @brief Export the selection status of the elements identified by `ELEM_ID` of
 * a mesh to a buffer.
 *
 * This function exports the selection status of the elements identified by
 * `ELEM_ID` of a mesh to a buffer. Values are stored in the buffer following
 * the order the elements appear in the mesh. The buffer must be preallocated
 * with the correct size (number of elements).
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
 * correspondence, compact the element container before calling this function.
 *
 * @param mesh
 * @param buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementSelectionToBuffer(const MeshType& mesh, auto* buffer)
{
    uint i = 0;
    for (const auto& e : mesh.template elements<ELEM_ID>()) {
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
 * @param mesh
 * @param buffer
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
 * @param mesh
 * @param buffer
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
 * @param mesh
 * @param buffer
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
 * @param mesh
 * @param buffer
 * @param storage
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementNormalsToBuffer(
    const MeshType&         mesh,
    auto*                   buffer,
    MatrixStorageType::Enum storage = MatrixStorageType::ROW_MAJOR)
{
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    if (storage == MatrixStorageType::ROW_MAJOR) {
        uint i = 0;
        for (const auto& n :
             mesh.template elements<ELEM_ID>() | views::normals) {
            buffer[i * 3 + 0] = n.x();
            buffer[i * 3 + 1] = n.y();
            buffer[i * 3 + 2] = n.z();
            ++i;
        }
    }
    else {
        uint       i        = 0;
        const uint ELEM_NUM = mesh.template number<ELEM_ID>();
        for (const auto& n :
             mesh.template elements<ELEM_ID>() | views::normals) {
            buffer[0 * ELEM_NUM + i] = n.x();
            buffer[1 * ELEM_NUM + i] = n.y();
            buffer[2 * ELEM_NUM + i] = n.z();
            ++i;
        }
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
 * @param mesh
 * @param buffer
 * @param storage
 */
template<MeshConcept MeshType>
void vertexNormalsToBuffer(
    const MeshType&         mesh,
    auto*                   buffer,
    MatrixStorageType::Enum storage = MatrixStorageType::ROW_MAJOR)
{
    elementNormalsToBuffer<ElemId::VERTEX>(mesh, buffer, storage);
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
 * @param mesh
 * @param buffer
 * @param storage
 */
template<FaceMeshConcept MeshType>
void faceNormalsToBuffer(
    const MeshType&         mesh,
    auto*                   buffer,
    MatrixStorageType::Enum storage = MatrixStorageType::ROW_MAJOR)
{
    elementNormalsToBuffer<ElemId::FACE>(mesh, buffer, storage);
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
 * @param mesh
 * @param buffer
 * @param storage
 * @param representation
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType&             mesh,
    auto*                       buffer,
    MatrixStorageType::Enum     storage = MatrixStorageType::ROW_MAJOR,
    Color::Representation::Enum representation =
        Color::Representation::INT_0_255)
{
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    const bool R_INT = representation == Color::Representation::INT_0_255;

    if (storage == MatrixStorageType::ROW_MAJOR) {
        uint i = 0;
        for (const auto& c :
             mesh.template elements<ELEM_ID>() | views::colors) {
            buffer[i * 4 + 0] = R_INT ? c.red() : c.redF();
            buffer[i * 4 + 1] = R_INT ? c.green() : c.greenF();
            buffer[i * 4 + 2] = R_INT ? c.blue() : c.blueF();
            buffer[i * 4 + 3] = R_INT ? c.alpha() : c.alphaF();
            ++i;
        }
    }
    else {
        uint       i        = 0;
        const uint ELEM_NUM = mesh.template number<ELEM_ID>();
        for (const auto& c :
             mesh.template elements<ELEM_ID>() | views::colors) {
            buffer[0 * ELEM_NUM + i] = R_INT ? c.red() : c.redF();
            buffer[1 * ELEM_NUM + i] = R_INT ? c.green() : c.greenF();
            buffer[2 * ELEM_NUM + i] = R_INT ? c.blue() : c.blueF();
            buffer[3 * ELEM_NUM + i] = R_INT ? c.alpha() : c.alphaF();
            ++i;
        }
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
 * @param mesh
 * @param buffer
 * @param colorFormat
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsToBuffer(
    const MeshType&     mesh,
    auto*               buffer,
    Color::Format::Enum colorFormat)
{
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    uint i = 0;
    for (const auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
        switch (colorFormat) {
        case Color::Format::ABGR: buffer[i] = c.abgr(); break;
        case Color::Format::ARGB: buffer[i] = c.argb(); break;
        case Color::Format::RGBA: buffer[i] = c.rgba(); break;
        case Color::Format::BGRA: buffer[i] = c.bgra(); break;
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
 * @param mesh
 * @param buffer
 * @param storage
 * @param representation
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType&             mesh,
    auto*                       buffer,
    MatrixStorageType::Enum     storage = MatrixStorageType::ROW_MAJOR,
    Color::Representation::Enum representation =
        Color::Representation::INT_0_255)
{
    elementColorsToBuffer<ElemId::VERTEX>(
        mesh, buffer, storage, representation);
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
 * @param mesh
 * @param buffer
 * @param colorFormat
 */
template<MeshConcept MeshType>
void vertexColorsToBuffer(
    const MeshType&     mesh,
    auto*               buffer,
    Color::Format::Enum colorFormat)
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
 * @param mesh
 * @param buffer
 * @param storage
 * @param representation
 */
template<MeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType&             mesh,
    auto*                       buffer,
    MatrixStorageType::Enum     storage = MatrixStorageType::ROW_MAJOR,
    Color::Representation::Enum representation =
        Color::Representation::INT_0_255)
{
    elementColorsToBuffer<ElemId::FACE>(mesh, buffer, storage, representation);
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
 * @param mesh
 * @param buffer
 * @param colorFormat
 */
template<MeshConcept MeshType>
void faceColorsToBuffer(
    const MeshType&     mesh,
    auto*               buffer,
    Color::Format::Enum colorFormat)
{
    elementColorsToBuffer<ElemId::FACE>(mesh, buffer, colorFormat);
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
 * @param mesh
 * @param buffer
 * @param colorFormat
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    uint i = 0;
    for (const auto& q : mesh.template elements<ELEM_ID>() | views::quality) {
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
 * @param mesh
 * @param buffer
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
 * @param mesh
 * @param buffer
 */
template<MeshConcept MeshType>
void faceQualityToBuffer(const MeshType& mesh, auto* buffer)
{
    elementQualityToBuffer<ElemId::FACE>(mesh, buffer);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_EXPORT_BUFFER_H
