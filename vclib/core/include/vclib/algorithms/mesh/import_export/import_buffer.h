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
 * The number of elements of the input buffer must be equal to the number of
 * ELEM_ID elements of the mesh, otherwise an exception is thrown.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element selection.
 * @param[in] buffer: the input element selection buffer.
 * @param[in] elementNumber: the number of elements contained in the input
 * buffer.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementSelectionFromBuffer(
    MeshType&   mesh,
    const auto* buffer,
    uint        elementNumber)
{
    if (elementNumber != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input selection buffer must have the same number of elements "
            "as the number of " +
            elementEnumString<ELEM_ID>() + " element in the mesh");

    for (uint i = 0; auto& e : mesh.template elements<ELEM_ID>()) {
        e.selected() = buffer[i];
        ++i;
    }
}

/**
 * @brief Sets the vertex selection of the given input `mesh` from the input
 * selection buffer.
 *
 * The number of elements of the input buffer must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex selection.
 * @param[in] buffer: the input vertex selection buffer.
 * @param[in] elementNumber: the number of elements contained in the input
 * buffer.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexSelectionFromBuffer(
    MeshType&   mesh,
    const auto* buffer,
    uint        elementNumber)
{
    elementSelectionFromBuffer<ElemId::VERTEX>(mesh, buffer, elementNumber);
}

/**
 * @brief Sets the face selection of the given input `mesh` from the input
 * selection buffer.
 *
 * The number of elements of the input buffer must be equal to the number of
 * faces of the mesh, otherwise an exception is thrown.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face selection.
 * @param[in] buffer: the input face selection buffer.
 * @param[in] elementNumber: the number of elements contained in the input
 * buffer.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceSelectionFromBuffer(
    MeshType&   mesh,
    const auto* buffer,
    uint        elementNumber)
{
    elementSelectionFromBuffer<ElemId::FACE>(mesh, buffer, elementNumber);
}

/**
 * @brief Sets the edge selection of the given input `mesh` from the input
 * selection buffer.
 *
 * The number of elements of the input buffer must be equal to the number of
 * edges of the mesh, otherwise an exception is thrown.
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
void edgeSelectionFromBuffer(
    MeshType&   mesh,
    const auto* buffer,
    uint        elementNumber)
{
    elementSelectionFromBuffer<ElemId::EDGE>(mesh, buffer, elementNumber);
}

/**
 * @brief Sets the element identified by `ELEM_ID` normals of the given input
 * `mesh` from the input buffer, that is expected to be a contiguous array of
 * scalars, where each row contains the 3 components of the normal of a
 * element.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The given elementNumber must be equal to the number of ELEM_ID elements of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-element normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * elements of the mesh.
 * @param[in] elementNumber: the number of elements contained in the input
 * buffer.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `elementNumber`.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              elementNumber,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM = rowNumber == UINT_NULL ? elementNumber : rowNumber;

    // elementNumber must be equal to the number of elements of the given type
    if (elementNumber != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input element number does not match the number of " +
            elementEnumString<ELEM_ID>() +
            " elements of the mesh\n"
            "Number of " +
            elementEnumString<ELEM_ID>() + " elements in the mesh: " +
            std::to_string(mesh.template number<ELEM_ID>()) +
            "\nNumber of input element number: " +
            std::to_string(elementNumber));

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
 * buffer, that is expected to be a contiguous array of scalars, where each row
 * contains the 3 components of the normal of a vertex.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The given vertexNumber must be equal to the number of vertices of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-vertex normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * vertices of the mesh.
 * @param[in] vertexNumber: the number of vertices contained in the input
 * buffer.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `vertexNumber`.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              vertexNumber,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsFromBuffer<ElemId::VERTEX, MeshType>(
        mesh, buffer, vertexNumber, storage, rowNumber);
}

/**
 * @brief Sets the face normals of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of scalars, where each row
 * contains the 3 components of the normal of a face.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The given faceNumber must be equal to the number of faces of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-face normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * faces of the mesh.
 * @param[in] faceNumber: the number of faces contained in the input
 * buffer.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `faceNumber`.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              faceNumber,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsFromBuffer<ElemId::FACE, MeshType>(
        mesh, buffer, faceNumber, storage, rowNumber);
}

/**
 * @brief Sets the edge normals of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of scalars, where each row
 * contains the 3 components of the normal of an edge.
 *
 * The layout of the buffer can be either row-major or column-major, as
 * specified by the `storage` argument. The default is row-major.
 *
 * The given edgeNumber must be equal to the number of edges of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-edge normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edge normals.
 * @param[in] buffer: a contiguous array containing the normals of the
 * edges of the mesh.
 * @param[in] edgeNumber: the number of edges contained in the input
 * buffer.
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `edgeNumber`.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeNormalsFromBuffer(
    MeshType&         mesh,
    const auto*       buffer,
    uint              edgeNumber,
    MatrixStorageType storage   = MatrixStorageType::ROW_MAJOR,
    uint              rowNumber = UINT_NULL)
{
    elementNormalsFromBuffer<ElemId::EDGE, MeshType>(
        mesh, buffer, edgeNumber, storage, rowNumber);
}

/**
 * @brief Sets the element identified by `ELEM_ID` colors of the given input
 * `mesh` from the input buffer, that is expected to be a contiguous array of
 * scalars, where each row (number of channels) contains the 3 or 4 components
 * of the color of a element.
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
 * The given elementNumber must be equal to the number of ELEM_ID elements of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-element color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input element colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] elementNumber: the number of elements contained in the input
 * buffer.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `elementNumber`.
 *
 * @ingroup import_buffer
 */
template<uint ELEM_ID, MeshConcept MeshType>
void elementColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  elementNumber,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    using namespace detail;

    const uint ROW_NUM = rowNumber == UINT_NULL ? elementNumber : rowNumber;

    if (channelsNumber!= 3 && channelsNumber != 4)
        throw WrongSizeException(
            "The input " + elementEnumString<ELEM_ID>() +
            " colors must have 3 or 4 channels.");

    // elementNumber must be equal to the number of elements of the given type
    if (elementNumber != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input element number does not match the number of " +
            elementEnumString<ELEM_ID>() +
            " elements of the mesh\n"
            "Number of " +
            elementEnumString<ELEM_ID>() + " elements in the mesh: " +
            std::to_string(mesh.template number<ELEM_ID>()) +
            "\nNumber of input element number: " +
            std::to_string(elementNumber));

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
 * @brief Sets the vertex colors of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of scalars, where each row
 * (number of channels) contains the 3 or 4 components of the color of a
 * vertex.
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
 * The given vertexNumber must be equal to the number of vertices of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-vertex color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] vertexNumber: the number of vertices contained in the input
 * buffer.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `vertexNumber`.
 *
 * @ingroup import_buffer
 */
template<MeshConcept MeshType>
void vertexColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  vertexNumber,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsFromBuffer<ElemId::VERTEX, MeshType>(
        mesh,
        buffer,
        vertexNumber,
        channelsNumber,
        storage,
        representation,
        rowNumber);
}

/**
 * @brief Sets the face colors of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of scalars, where each row
 * (number of channels) contains the 3 or 4 components of the color of a
 * face.
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
 * The given faceNumber must be equal to the number of faces of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-face color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input face colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] faceNumber: the number of faces contained in the input
 * buffer.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `faceNumber`.
 *
 * @ingroup import_buffer
 */
template<FaceMeshConcept MeshType>
void faceColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  faceNumber,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsFromBuffer<ElemId::FACE, MeshType>(
        mesh,
        buffer,
        faceNumber,
        channelsNumber,
        storage,
        representation,
        rowNumber);
}

/**
 * @brief Sets the edge colors of the given input `mesh` from the input
 * buffer, that is expected to be a contiguous array of scalars, where each row
 * (number of channels) contains the 3 or 4 components of the color of an
 * edge.
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
 * The given edgeNumber must be equal to the number of edges of
 * the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-edge color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @param[in/out] mesh: the mesh on which import the input edge colors.
 * @param[in] buffer: a contiguous array containing the colors of the
 * elements of the mesh.
 * @param[in] edgeNumber: the number of edges contained in the input
 * buffer.
 * @param[in] channelsNumber: the number of channels per color in the input
 * buffer. It can be either 3 (RGB) or 4 (RGBA).
 * @param[in] storage: the storage type of the input buffer. It can be either
 * row-major or column-major.
 * @param[in] representation: the representation of the color scalars in the
 * input buffer. It can be either in the range [0,255] or in the range [0,1].
 * @param[in] rowNumber: if the storage type is column-major, this parameter
 * specifies the number of rows in the input buffer. If it is not specified
 * (default), it is assumed to be equal to `edgeNumber`.
 *
 * @ingroup import_buffer
 */
template<EdgeMeshConcept MeshType>
void edgeColorsFromBuffer(
    MeshType&             mesh,
    const auto*           buffer,
    uint                  edgeNumber,
    uint                  channelsNumber = 4,
    MatrixStorageType     storage        = MatrixStorageType::ROW_MAJOR,
    Color::Representation representation = Color::Representation::INT_0_255,
    uint                  rowNumber      = UINT_NULL)
{
    elementColorsFromBuffer<ElemId::EDGE, MeshType>(
        mesh,
        buffer,
        edgeNumber,
        channelsNumber,
        storage,
        representation,
        rowNumber);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_BUFFER_H
