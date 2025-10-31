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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_BUFFER_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_BUFFER_H

#include "detail.h"

#include <vclib/mesh.h>
#include <vclib/space/core.h>

/**
 * @defgroup import_buffer Import Mesh from Buffer Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Import Mesh from Buffer algorithms.
 *
 * They allow to import mesh data from buffers.
 */

namespace vcl {

/**
 * @brief Sets the vertex positions of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of scalars, where each row
 * contains the 3 components of the position of a vertex.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the vertex container of the mesh and then adds a number of vertices
 * that depends on the given `vertexNumber`. In this scenario, all the old
 * vertices with their components stored in the mesh before calling this
 * function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the given `vertexNumber` is equal to the number of vertices of the mesh. If
 * this is not the case, an exception is thrown. Then, the function sets the
 * positions of the vertices of the mesh from the input buffer. In this
 * scenario, all the components (except the positions) of the vertices stored in
 * the mesh before calling this function are preserved.
 *
 * All the other containers of the mesh are left as they are. Pointers to
 * vertices stored in the other containers of the mesh are still valid only if
 * the number of vertices is not changed (same allocation policy of the
 * std::vector).
 *
 * @throws vcl::WrongSizeException if `clearBeforeSet` is false and
 * `vertexNumber` != mesh.vertexNumber().
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 *
 * @param[in/out] mesh: the mesh on which import the input vertices.
 * @param[in] buffer: a contiguous array containing the positions of the
 * vertices of the mesh.
 * @param[in] vertexNumber: the number of vertices contained in the input
 * buffer.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the vertices of the mesh before adding the vertices from the input buffer.
 * If `false`, the function sets the positions in the input buffer to the
 * vertices of the mesh, leaving all the other components untouched.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `vertexNumber`.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexPositionsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              vertexNumber,
    bool              clearBeforeSet = true,
    MatrixStorageType storage        = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber      = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM = rowNumber == UINT_NULL ? vertexNumber : rowNumber;

    if (clearBeforeSet) {
        mesh.clearVertices();
        mesh.resizeVertices(vertexNumber);
    }
    else {
        if (vertexNumber != mesh.vertexNumber()) {
            throw WrongSizeException(
                "The input vertex number does not match the number of vertices "
                "of the mesh\n"
                "Number of vertices in the mesh: " +
                std::to_string(mesh.vertexNumber()) +
                "\nNumber of input vertex number: " +
                std::to_string(vertexNumber));
        }
    }

    for (uint i = 0; auto& p : mesh.vertices() | views::positions) {
        p.x() = at(buffer, i, 0, ROW_NUM, 3, storage);
        p.y() = at(buffer, i, 1, ROW_NUM, 3, storage);
        p.z() = at(buffer, i, 2, ROW_NUM, 3, storage);

        ++i;
    }
}

/**
 * @brief Sets the face indices of the given input `mesh` from the input face
 * buffer, that is expected to be a contiguous array of integers, where each row
 * contains the indices of the vertices of a face.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * If the MeshType is not a polygonal mesh (e.g. a triangle mesh), the given
 * `faceSize` must be equal to the number of vertices of each face of the mesh
 * (e.g. 3 for triangle meshes, 4 for quad meshes, etc.). If this condition is
 * not satisfied, an exception is thrown.
 *
 * If the MeshType is a polygonal mesh, the size of each polygonal face is
 * determined by counting the number of valid vertex indices in each row of the
 * input buffer. Valid vertex indices are non-negative and not equal to
 * UINT_NULL.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the face container of the mesh and then adds a number of faces
 * that depends on the given `faceNumber`. In this scenario, all the old faces
 * with their components stored in the mesh before calling this function are
 * lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the given `faceNumber` is equal to the number of faces of the mesh. If this
 * is not the case, an exception is thrown. Then, the function sets the indices
 * of the faces of the mesh from the input buffer. In this scenario, all
 * the components (except the indices) of the faces stored in the mesh before
 * calling this function are preserved.
 *
 * All the other containers of the mesh are left as they are. Pointers to faces
 * stored in the other containers of the mesh are still valid only if the number
 * of faces is not changed (same allocation policy of the std::vector).
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam FMatrix: the type of the input face matrix. It must satisfy the
 * MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input faces.
 * @param[in] buffer: a contiguous array of \#F*faceSize values containing the
 * indices of the vertices of the faces of the mesh.
 * @param[in] faceNumber: the number of faces contained in the input buffer.
 * @param[in] faceSize: the number of vertex indices per face contained in the
 * input buffer. If the MeshType is not a polygonal mesh (e.g. a triangle
 * mesh), this parameter must be equal to the number of vertices of each face of
 * the mesh (e.g. 3 for triangle meshes, 4 for quad meshes, etc.).
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the faces of the mesh before adding the faces from the input buffer.
 * If `false`, the function sets the indices from the input buffer to the
 * faces of the mesh, leaving all the other components untouched.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `faceNumber`.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceIndicesFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              faceNumber,
    uint              faceSize       = 3,
    bool              clearBeforeSet = true,
    MatrixStorageType storage        = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber      = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM = rowNumber == UINT_NULL ? faceNumber : rowNumber;

    if (clearBeforeSet) {
        mesh.clearFaces();
        mesh.resizeFaces(faceNumber);
    }
    else {
        if (faceNumber != mesh.faceNumber()) {
            throw WrongSizeException(
                "The input face number does not match the number of faces "
                "of the mesh\n"
                "Number of faces in the mesh: " +
                std::to_string(mesh.faceNumber()) +
                "\nNumber of input face number: " + std::to_string(faceNumber));
        }
    }

    if constexpr (HasPolygons<MeshType>) {
        uint i = 0;
        for (auto& f : mesh.faces()) {
            uint vertexNumber = 0;

            // count the number of vertices of the face
            while (vertexNumber < faceSize &&
                   at(buffer, i, vertexNumber, ROW_NUM, faceSize, storage) !=
                       -1 &&
                   at(buffer, i, vertexNumber, ROW_NUM, faceSize, storage) !=
                       UINT_NULL)
                vertexNumber++;

            f.resizeVertices(vertexNumber);

            for (uint j = 0; j < vertexNumber; ++j)
                f.setVertex(j, at(buffer, i, j, ROW_NUM, faceSize, storage));
            ++i;
        }
    }
    else { // the vertex number of mesh faces is fixed
        using FaceType = MeshType::FaceType;

        constexpr int VN = FaceType::VERTEX_NUMBER;
        if (faceSize == VN) { // faces of matrix and mesh have same size
            uint i = 0;
            for (auto& f : mesh.faces()) {
                for (uint j = 0; j < VN; ++j)
                    f.setVertex(
                        j, at(buffer, i, j, ROW_NUM, faceSize, storage));
                ++i;
            }
        }
        else {
            // cannot import the faces if the number of face indices in the
            // buffer is different w.r.t. face size of the mesh
            throw WrongSizeException(
                "The input face buffer has a different face size "
                "than the vertex number of the faces of the mesh.\n"
                "Vertex number of faces in the mesh: " +
                std::to_string(VN) +
                "\nNumber of columns in the input face buffer: " +
                std::to_string(faceSize));
        }
    }
}

/**
 * @brief Sets the edge indices of the given input `mesh` from the input edge
 * buffer, that is expected to be a contiguous array of integers, where each row
 * contains the indices of the vertices of an edge.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the edge container of the mesh and then adds a number of edges
 * that depends on the given `edgeNumber`. In this scenario, all the old edges
 * with their components stored in the mesh before calling this function are
 * lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the given `edgeNumber` is equal to the number of edges of the mesh. If this
 * is not the case, an exception is thrown. Then, the function sets the indices
 * of the edges of the mesh from the input edge buffer. In this scenario, all
 * the components (except the indices) of the edges stored in the mesh before
 * calling this function are preserved.
 *
 * All the other containers of the mesh are left as they are. Pointers to edges
 * stored in the other containers of the mesh are still valid only if the number
 * of edges is not changed (same allocation policy of the std::vector).
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edges.
 * @param[in] buffer: a contiguous array of \#E*2 values containing the indices
 * of the vertices of the edges of the mesh.
 * @param[in] edgeNumber: the number of edges contained in the input buffer.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the edges of the mesh before adding the edges from the input buffer.
 * If `false`, the function sets the indices from the input buffer to the
 * edges of the mesh, leaving all the other components untouched.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `edgeNumber`.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeIndicesFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              edgeNumber,
    bool              clearBeforeSet = true,
    MatrixStorageType storage        = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber      = UINT_NULL)
{
    using namespace detail;

    if (clearBeforeSet) {
        mesh.clearEdges();
        mesh.resizeEdges(edgeNumber);
    }
    else {
        if (edgeNumber != mesh.edgeNumber()) {
            throw WrongSizeException(
                "The input edge number does not match the number of edges "
                "of the mesh\n"
                "Number of edges in the mesh: " +
                std::to_string(mesh.edgeNumber()) +
                "\nNumber of input edge number: " + std::to_string(edgeNumber));
        }
    }

    uint i = 0;
    for (auto& e : mesh.edges()) {
        e.setVertex(0, at(buffer, i, 0, rowNumber, 2, storage));
        e.setVertex(1, at(buffer, i, 1, rowNumber, 2, storage));
        i++;
    }
}

/**
 * @brief Sets the element identified by `ELEM_ID` selection of the given input
 * `mesh` from the input selection buffer.
 *
 * The number of elements of the input buffer is expected to be at least the
 * number of ELEM_ID elements of the mesh. The extra elements are ignored.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element selection.
 * @param[in] buffer: the input element selection buffer.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementSelectionFromBuffer(MeshType& mesh, const auto* buffer)
{
    for (uint i = 0; auto& e : mesh.template elements<ELEM_ID>()) {
        e.selected() = buffer[i];
        ++i;
    }
}

/**
 * @brief Sets the vertex selection of the given input `mesh` from the input
 * selection buffer.
 *
 * The number of elements of the input buffer is expected to be at least the
 * number vertices of the mesh. The extra elements are ignored.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex selection.
 * @param[in] buffer: the input vertex selection buffer.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexSelectionFromBuffer(MeshType& mesh, const auto* buffer)
{
    elementSelectionFromBuffer<ElemId::VERTEX>(mesh, buffer);
}

/**
 * @brief Sets the face selection of the given input `mesh` from the input
 * selection buffer.
 *
 * The number of elements of the input buffer is expected to be at least the
 * number faces of the mesh. The extra elements are ignored.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face selection.
 * @param[in] buffer: the input face selection buffer.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceSelectionFromBuffer(MeshType& mesh, const auto* buffer)
{
    elementSelectionFromBuffer<ElemId::FACE>(mesh, buffer);
}

/**
 * @brief Sets the edge selection of the given input `mesh` from the input
 * selection buffer.
 *
 * The number of elements of the input buffer is expected to be at least the
 * number edges of the mesh. The extra elements are ignored.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edge selection.
 * @param[in] buffer: the input edge selection buffer.
 * @param[in] elementNumber: the number of elements contained in the input
 * buffer.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeSelectionFromBuffer(MeshType& mesh, const auto* buffer)
{
    elementSelectionFromBuffer<ElemId::EDGE>(mesh, buffer);
}

/**
 * @brief Sets the element identified by `ELEM_ID` normals of the given input
 * `mesh` from the input buffer, that is expected to be a contiguous array of
 * #E*3 scalars, where each row contains the 3 components of the normal of a
 * element.
 *
 * If not specified, the number of normals in the input buffer is assumed to be
 * at least the number of ELEM_ID elements of the mesh. The extra normals are
 * ignored.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-element normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * elements of the mesh.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of ELEM_ID elements of the
 * mesh.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.template number<ELEM_ID>() : rowNumber;

    enableIfPerElementComponentOptional<ELEM_ID, CompId::NORMAL>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    for (uint  i = 0;
         auto& n : mesh.template elements<ELEM_ID>() | views::normals) {
        n.x() = at(buffer, i, 0, ROW_NUM, 3, storage);
        n.y() = at(buffer, i, 1, ROW_NUM, 3, storage);
        n.z() = at(buffer, i, 2, ROW_NUM, 3, storage);

        ++i;
    }
}

/**
 * @brief Sets the vertex normals of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #V*3 scalars, where each
 * row contains the 3 components of the normal of a vertex.
 *
 * If not specified, the number of normals in the input buffer is assumed to be
 * at least the number of vertices of the mesh. The extra normals are ignored.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-vertex normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * vertices of the mesh.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of vertices of the mesh.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsFromBuffer<ElemId::VERTEX, MeshType>(
        mesh, buffer, storage, rowNumber);
}

/**
 * @brief Sets the face normals of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #F*3 scalars, where each
 * row contains the 3 components of the normal of a face.
 *
 * If not specified, the number of normals in the input buffer is assumed to be
 * at least the number of faces of the mesh. The extra normals are ignored.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-face normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * faces of the mesh.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of faces of the mesh.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsFromBuffer<ElemId::FACE, MeshType>(
        mesh, buffer, storage, rowNumber);
}

/**
 * @brief Sets the edge normals of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #F*3 scalars, where each
 * row contains the 3 components of the normal of a edge.
 *
 * If not specified, the number of normals in the input buffer is assumed to be
 * at least the number of edges of the mesh. The extra normals are ignored.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-edge normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edge normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * edges of the mesh.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of edges of the mesh.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsFromBuffer<ElemId::EDGE, MeshType>(
        mesh, buffer, storage, rowNumber);
}

/**
 * @brief Sets the element identified by `ELEM_ID` colors of the given input
 * `mesh` from the input buffer, that is expected to be a contiguous array of
 * #E*3 or #E*4 scalars, where each row contains the 3 or 4 components of the
 * color of a element.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of ELEM_ID elements of the mesh. The extra colors are
 * ignored.
 *
 * Scalars can be either in the range [0,255] or in the range [0,1], as
 * specified by the `representation` argument. The default is [0,255].
 *
 * The number of channels can be either 3 (RGB) or 4 (RGBA), as specified by
 * the `channelsNumber` argument. The default is 4.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-element color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of ELEM_ID elements of the
 * mesh.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.template number<ELEM_ID>() : rowNumber;

    if (channelsNumber != 3 && channelsNumber != 4)
        throw WrongSizeException(
            "The input " + elementEnumString<ELEM_ID>() +
            " colors must have 3 or 4 channels.");

    enableIfPerElementComponentOptional<ELEM_ID, CompId::COLOR>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    for (uint  i = 0;
         auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
        if (representation == Color::Representation::INT_0_255) {
            c.x() = at(buffer, i, 0, ROW_NUM, channelsNumber, storage);
            c.y() = at(buffer, i, 1, ROW_NUM, channelsNumber, storage);
            c.z() = at(buffer, i, 2, ROW_NUM, channelsNumber, storage);

            if (channelsNumber == 4)
                c.w() = at(buffer, i, 3, ROW_NUM, channelsNumber, storage);
            else
                c.w() = 255;
        }
        else {
            c.x() = at(buffer, i, 0, ROW_NUM, channelsNumber, storage) * 255;
            c.y() = at(buffer, i, 1, ROW_NUM, channelsNumber, storage) * 255;
            c.z() = at(buffer, i, 2, ROW_NUM, channelsNumber, storage) * 255;

            if (channelsNumber == 4)
                c.w() =
                    at(buffer, i, 3, ROW_NUM, channelsNumber, storage) * 255;
            else
                c.w() = 255;
        }
        ++i;
    }
}

/**
 * @brief Sets the element identified by `ELEM_ID` color of the given input
 * `mesh` from the input buffer, that is expected to be a contiguous array of #E
 * 32 bit values, on each of them the color is packed using the provided
 * `colorFormat`.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of ELEM_ID elements of the mesh. The extra colors are
 * ignored.
 *
 * The function enables the per-element color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element color.
 * @param[in] buffer: the input element color buffer.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsFromBuffer(
    MeshType&     mesh,
    const auto*   buffer,
    Color::Format colorFormat)
{
    enableIfPerElementComponentOptional<ELEM_ID, CompId::COLOR>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    for (uint  i = 0;
         auto& c : mesh.template elements<ELEM_ID>() | views::colors) {
        c = Color(buffer[i++], colorFormat);
    }
}

/**
 * @brief Sets the vertex colors of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #V*3 or #V*4 scalars,
 * where each row contains the 3 or 4 components of the color of a vertex.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of vertices of the mesh. The extra colors are ignored.
 *
 * Scalars can be either in the range [0,255] or in the range [0,1], as
 * specified by the `representation` argument. The default is [0,255].
 *
 * The number of channels can be either 3 (RGB) or 4 (RGBA), as specified by
 * the `channelsNumber` argument. The default is 4.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-vertex color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of vertices of the mesh.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsFromBuffer<ElemId::VERTEX, MeshType>(
        mesh, buffer, channelsNumber, storage, representation, rowNumber);
}

/**
 * @brief Sets the vertex color of the given input `mesh` from the input buffer,
 * that is expected to be a contiguous array of #V 32 bit values, on each of
 * them the color is packed using the provided `colorFormat`.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of vertices of the mesh. The extra colors are ignored.
 *
 * The function enables the per-vertex color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex color.
 * @param[in] buffer: the input vertex color buffer.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexColorsFromBuffer(
    MeshType&     mesh,
    const auto*   buffer,
    Color::Format colorFormat)
{
    elementColorsFromBuffer<ElemId::VERTEX, MeshType>(
        mesh, buffer, colorFormat);
}

/**
 * @brief Sets the face colors of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #F*3 or #F*4 scalars,
 * where each row contains the 3 or 4 components of the color of a face.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of faces of the mesh. The extra colors are ignored.
 *
 * Scalars can be either in the range [0,255] or in the range [0,1], as
 * specified by the `representation` argument. The default is [0,255].
 *
 * The number of channels can be either 3 (RGB) or 4 (RGBA), as specified by
 * the `channelsNumber` argument. The default is 4.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-face color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of faces of the mesh.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsFromBuffer<ElemId::FACE, MeshType>(
        mesh, buffer, channelsNumber, storage, representation, rowNumber);
}

/**
 * @brief Sets the face color of the given input `mesh` from the input buffer,
 * that is expected to be a contiguous array of #V 32 bit values, on each of
 * them the color is packed using the provided `colorFormat`.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of faces of the mesh. The extra colors are ignored.
 *
 * The function enables the per-face color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face color.
 * @param[in] buffer: the input face color buffer.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsFromBuffer(
    MeshType&     mesh,
    const auto*   buffer,
    Color::Format colorFormat)
{
    elementColorsFromBuffer<ElemId::FACE, MeshType>(mesh, buffer, colorFormat);
}

/**
 * @brief Sets the edge colors of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #E*3 or #E*4 scalars,
 * where each row contains the 3 or 4 components of the color of a edge.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of edges of the mesh. The extra colors are ignored.
 *
 * Scalars can be either in the range [0,255] or in the range [0,1], as
 * specified by the `representation` argument. The default is [0,255].
 *
 * The number of channels can be either 3 (RGB) or 4 (RGBA), as specified by
 * the `channelsNumber` argument. The default is 4.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The function enables the per-edge color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edge colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of edges of the mesh.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsFromBuffer<ElemId::EDGE, MeshType>(
        mesh, buffer, channelsNumber, storage, representation, rowNumber);
}

/**
 * @brief Sets the edge color of the given input `mesh` from the input buffer,
 * that is expected to be a contiguous array of #V 32 bit values, on each of
 * them the color is packed using the provided `colorFormat`.
 *
 * If not specified, the number of colors in the input buffer is assumed to be
 * at least the number of edges of the mesh. The extra colors are ignored.
 *
 * The function enables the per-edge color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edge color.
 * @param[in] buffer: the input edge color buffer.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsFromBuffer(
    MeshType&     mesh,
    const auto*   buffer,
    Color::Format colorFormat)
{
    elementColorsFromBuffer<ElemId::EDGE, MeshType>(mesh, buffer, colorFormat);
}

/**
 * @brief Sets the element identified by `ELEM_ID` quality of the given input
 * `mesh` from the input quality buffer, that is expected to be a contiguous
 * array of #E scalars, where each value contains the quality of a element.
 *
 * If not specified, the number of quality values in the input buffer is assumed
 * to be at least the number of ELEM_ID elements of the mesh. The extra quality
 * values are ignored.
 *
 * The function enables the per-element quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element quality.
 * @param[in] buffer: the input element quality buffer.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementQualityFromBuffer(MeshType& mesh, const auto* buffer)
{
    enableIfPerElementComponentOptional<ELEM_ID, CompId::QUALITY>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    for (uint i = 0; auto& e : mesh.template elements<ELEM_ID>()) {
        e.quality() = buffer[i];
        ++i;
    }
}

/**
 * @brief Sets the vertex quality of the given input `mesh` from the input
 * quality buffer, that is expected to be a contiguous array of #V scalars,
 * where each value contains the quality of a vertex.
 *
 * If not specified, the number of quality values in the input buffer is assumed
 * to be at least the number of vertices of the mesh. The extra quality values
 * are ignored.
 *
 * The function enables the per-vertex quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex quality.
 * @param[in] buffer: the input vertex quality buffer.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexQualityFromBuffer(MeshType& mesh, const auto* buffer)
{
    elementQualityFromBuffer<ElemId::VERTEX, MeshType>(mesh, buffer);
}

/**
 * @brief Sets the face quality of the given input `mesh` from the input
 * quality buffer, that is expected to be a contiguous array of #F scalars,
 * where each value contains the quality of a face.
 *
 * If not specified, the number of quality values in the input buffer is assumed
 * to be at least the number of faces of the mesh. The extra quality values
 * are ignored.
 *
 * The function enables the per-face quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face quality.
 * @param[in] buffer: the input face quality buffer.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceQualityFromBuffer(MeshType& mesh, const auto* buffer)
{
    elementQualityFromBuffer<ElemId::FACE, MeshType>(mesh, buffer);
}

/**
 * @brief Sets the edge quality of the given input `mesh` from the input
 * quality buffer, that is expected to be a contiguous array of #F scalars,
 * where each value contains the quality of a edge.
 *
 * If not specified, the number of quality values in the input buffer is assumed
 * to be at least the number of edges of the mesh. The extra quality values
 * are ignored.
 *
 * The function enables the per-edge quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edge quality.
 * @param[in] buffer: the input edge quality buffer.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeQualityFromBuffer(MeshType& mesh, const auto* buffer)
{
    elementQualityFromBuffer<ElemId::EDGE, MeshType>(mesh, buffer);
}

/**
 * @brief Sets the vertex texcoords of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #V*2 scalars, where each
 * row contains the 2 components of the texture coordinates of a vertex.
 *
 * If not specified, the number of texcoords in the input buffer is assumed to
 * be at least the number of vertices of the mesh. The extra texcoords are
 * ignored.
 *
 * The function enables the per-vertex texcoords component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 *
 * @param[in/out] mesh: the mesh on which import the input texcoords.
 * @param[in] buffer: a contiguous array containing the texcoords of the
 * vertices of the mesh.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of vertices of the mesh.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexTexCoordsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM =
        rowNumber == UINT_NULL ? mesh.vertexNumber() : rowNumber;

    enableIfPerVertexTexCoordOptional(mesh);
    requirePerVertexTexCoord(mesh);

    for (uint i = 0; auto& t : mesh.vertices() | views::texCoords) {
        t.u() = at(buffer, i, 0, ROW_NUM, 2, storage);
        t.v() = at(buffer, i, 1, ROW_NUM, 2, storage);

        ++i;
    }
}

/**
 * @brief Sets the vertex material indices of the given input `mesh` from the
 * input material indices buffer, that is expected to be a contiguous array of
 * #V scalars, where each value contains the material index of a vertex.
 *
 * If not specified, the number of material indices values in the input buffer
 * is assumed to be at least the number of vertices of the mesh. The extra
 * material indices values are ignored.
 *
 * The function enables the per-vertex material index component if it is not
 * already enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex material
 * indices.
 * @param[in] buffer: the input vertex material indices buffer.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexMaterialIndicesFromBuffer(MeshType& mesh, const auto* buffer)
{
    enableIfPerVertexMaterialIndexOptional(mesh);
    requirePerVertexMaterialIndex(mesh);

    for (uint i = 0; auto& v : mesh.vertices()) {
        v.materialIndex() = buffer[i];
        ++i;
    }
}

/**
 * @brief Sets the face wedge texcoords of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of #F*(LFS*2) of scalars,
 * where each row contains the 2*largetFaceSize components of the wedge texture
 * coordinates of a face.
 *
 * If not specified, the number of per face wedge texcoords in the input buffer
 * is assumed to be at least the number of faces of the mesh.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The number of columns of the input buffer must be equal to 2*largestFaceSize,
 * where largestFaceSize is the size of the largest face of the mesh. If a face
 * has a size smaller than largestFaceSize, only the first 2*faceSize columns of
 * the corresponding row are used.
 *
 * The function enables the per-face wedge texcoords component if it is not
 * already enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 *
 * @param[in/out] mesh: the mesh on which import the input wedge texcoords.
 * @param[in] buffer: a contiguous array containing the wedge texcoords of the
 * faces of the mesh.
 * @param[in] largestFaceSize: the largest size of the faces, that corresponds
 * to the number of columns of the input buffer divided by 2 (u and v).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to the number of faces of the mesh.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceWedgeTexCoordsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              largestFaceSize = 3,
    MatrixStorageType storage         = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber       = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM = rowNumber == UINT_NULL ? mesh.faceNumber() : rowNumber;
    const uint COL_NUM = largestFaceSize * 2;

    enableIfPerFaceWedgeTexCoordsOptional(mesh);
    requirePerFaceWedgeTexCoords(mesh);

    for (uint i = 0; auto& f : mesh.faces()) {
        for (uint j = 0; auto& w : f.wedgeTexCoords()) {
            if (j < largestFaceSize) {
                w.u() = at(buffer, i, 2 * j + 0, ROW_NUM, COL_NUM, storage);
                w.v() = at(buffer, i, 2 * j + 1, ROW_NUM, COL_NUM, storage);
            }
            ++j;
        }
        ++i;
    }
};

/**
 * @brief Sets the face wedge texcoord indices of the given input `mesh` from
 * the input texcoord indices buffer, that is expected to be a contiguous array
 * of #F scalars, where each value contains the wedge texcoord index of a face.
 *
 * If not specified, the number of wedge texcoord indices values in the input
 * buffer is assumed to be at least the number of faces of the mesh. The
 * extra wedge texcoord indices values are ignored.
 *
 * The function enables the per-face texcoords component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face texcoord
 * indices.
 * @param[in] buffer: the input face texcoord indices buffer.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceWedgeTexCoordIndicesFromBuffer(MeshType& mesh, const auto* buffer)
{
    enableIfPerFaceWedgeTexCoordsOptional(mesh);
    requirePerFaceWedgeTexCoords(mesh);

    for (uint i = 0; auto& f : mesh.faces()) {
        f.textureIndex() = buffer[i];
        ++i;
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_BUFFER_H
