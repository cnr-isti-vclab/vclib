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

#ifndef VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_MATRIX_H
#define VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_MATRIX_H

#include "import_buffer.h"

#include <vclib/mesh.h>
#include <vclib/space/core.h>

/**
 * @defgroup import_matrix Import Mesh from Matrix Algorithms
 *
 * @ingroup import_export
 *
 * @brief List Import Mesh from Matrix algorithms.
 *
 * They allow to import mesh data from matrices.
 */

namespace vcl {

/**
 * @brief Creates and returns a new mesh from the input matrices that are given
 * as arguments.
 *
 * The function accepts several input matrices, that are used only if
 * their number of rows is different from zero. It tries to fill all the data
 * contained in the non-empty input matrices into the Mesh:
 * - if a Mesh is not able to store same data, the relative input matrix is
 * ignored. For example, if the Mesh has no faces, the function will ignore the
 * input face matrix, also if that is not empty.
 * - if a Mesh is able to store some data and the relative input matrix is not
 * empty, the data will be stored in the Mesh. If the matrix has not the
 * expected size, an exception is thrown.
 *
 * @throws vcl::WrongSizeException if the sizes of the non-empty input matrices
 * have not the expected sizes.
 *
 * @tparam MeshType: the type of the mesh to be created. It must satisfy the
 * MeshConcept.
 * @tparam VMatrix: the type of the input vertex matrix. It must satisfy the
 * MatrixConcept.
 * @tparam FMatrix: the type of the input face matrix. It must satisfy the
 * MatrixConcept.
 * @tparam EMatrix: the type of the input edge matrix. It must satisfy the
 * MatrixConcept.
 *
 * @param[in] vertices: a \#V*3 matrix containing the positions of the
 * vertices of the mesh.
 * @param[in] faces: a \#F*K matrix containing the indices of the vertices of
 * the faces of the mesh. If the MeshType has no faces, the function will ignore
 * this matrix. If the mesh is not a polygonal mesh (e.g. a triangle mesh), K
 * must be equal to the number of vertices of each face of the mesh (e.g. 3 for
 * triangle meshes, 4 for quad meshes, etc.). If this condition is not
 * satisfied, an exception is thrown.
 * @param[in] edges: a \#E*2 matrix containing the indices of the vertices of
 * the edges of the mesh. If the MeshType has no edges, the function will ignore
 * this matrix.
 *
 * @return a new mesh containing the data passed as argument.
 *
 * @ingroup import_matrix
 */
template<
    MeshConcept   MeshType,
    MatrixConcept VMatrix,
    MatrixConcept FMatrix = Eigen::MatrixX3i,
    MatrixConcept EMatrix = Eigen::MatrixX2i>
MeshType meshFromMatrices(
    const VMatrix& vertices,
    const FMatrix& faces = FMatrix(),
    const EMatrix& edges = EMatrix())
{
    MeshType mesh;

    meshFromMatrices(mesh, vertices, faces, edges);

    return mesh;
}

/**
 * @brief Sets the given input `mesh` from the input matrices that are given as
 * arguments.
 *
 * The function accepts several input matrices, that are used only if
 * their number of rows is different from zero. It tries to fill all the data
 * contained in the non-empty input matrices into the Mesh:
 * - if a Mesh is not able to store same data, the relative input matrix is
 * ignored. For example, if the Mesh has no faces, the function will ignore the
 * input face matrix, also if that is not empty.
 * - if a Mesh is able to store some data and the relative input matrix is not
 * empty, the data will be stored in the Mesh. If the matrix has not the
 * expected size, an exception is thrown.
 *
 * The mesh is cleared and all the optional components are disabled before
 * adding the data from the input matrices.
 *
 * @note If the Mesh is not able to store some data from the matrices, that data
 * will be ignored. For example, if the Mesh has no faces, the function will
 * ignore the input face matrix, also if that is not empty.
 *
 * @throws vcl::WrongSizeException if the sizes of the non-empty input matrices
 * have not the expected sizes.
 *
 * @tparam MeshType: the type of the mesh to be created. It must satisfy the
 * MeshConcept.
 * @tparam VMatrix: the type of the input vertex matrix. It must satisfy the
 * MatrixConcept.
 * @tparam FMatrix: the type of the input face matrix. It must satisfy the
 * MatrixConcept.
 * @tparam EMatrix: the type of the input edge matrix. It must satisfy the
 * MatrixConcept.
 *
 * @param[in/out] mesh: the mesh on which import all the input data from the
 * matrices.
 * @param[in] vertices: a \#V*3 matrix containing the positions of the
 * vertices of the mesh.
 * @param[in] faces: a \#F*K matrix containing the indices of the vertices of
 * the faces of the mesh. If the MeshType has no faces, the function will ignore
 * this matrix. If the mesh is not a polygonal mesh (e.g. a triangle mesh), K
 * must be equal to the number of vertices of each face of the mesh (e.g. 3 for
 * triangle meshes, 4 for quad meshes, etc.). If this condition is not
 * satisfied, an exception is thrown.
 * @param[in] edges: a \#E*2 matrix containing the indices of the vertices of
 * the edges of the mesh. If the MeshType has no edges, the function will ignore
 * this matrix.
 *
 * @ingroup import_matrix
 */
template<
    MeshConcept   MeshType,
    MatrixConcept VMatrix,
    MatrixConcept FMatrix = Eigen::MatrixX3i,
    MatrixConcept EMatrix = Eigen::MatrixX2i>
void meshFromMatrices(
    MeshType&      mesh,
    const VMatrix& vertices,
    const FMatrix& faces = FMatrix(),
    const EMatrix& edges = EMatrix())
{
    mesh.clear();
    mesh.disableAllOptionalComponents();

    vertexPositionsFromMatrix(mesh, vertices);

    if constexpr (HasFaces<MeshType>) {
        if (faces.rows() > 0)
            faceIndicesFromMatrix(mesh, faces);
    }

    if constexpr (HasEdges<MeshType>) {
        if (edges.rows() > 0)
            edgeIndicesFromMatrix(mesh, edges);
    }
}

/**
 * @brief Sets the vertex positions of the given input `mesh` from the input
 * vertex matrix.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the vertex container of the mesh and then adds a number of vertices
 * that depends on the number of rows of the input vertex matrix. In this
 * scenario, all the old vertices with their components stored in the mesh
 * before calling this function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the number of rows of the input vertex matrix is equal to the number of
 * vertices of the mesh. If this is not the case, an exception is thrown.
 * Then, the function sets the positions of the vertices of the mesh from the
 * input vertex matrix. In this scenario, all the components (except the
 * positions) of the vertices stored in the mesh before calling this function
 * are preserved.
 *
 * All the other containers of the mesh are left as they are. Pointers to
 * vertices stored in the other containers of the mesh are still valid only if
 * the number of vertices is not changed (same allocation policy of the
 * std::vector).
 *
 * @throws vcl::WrongSizeException if the sizes of the input matrix are not
 * valid (e.g. columns != 3 or rows != mesh.vertexNumber()).
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam VMatrix: the type of the input vertex matrix. It must satisfy the
 * MatrixConcept.
 *
 * @param[in/out] mesh: the mesh on which import the input vertices.
 * @param[in] vertices: a \#V*3 matrix containing the positions of the
 * vertices of the mesh.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the vertices of the mesh before adding the vertices from the input matrix.
 * If `false`, the function sets the positions in the input matrix to the
 * vertices of the mesh, leaving all the other components untouched.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, MatrixConcept VMatrix>
void vertexPositionsFromMatrix(
    MeshType&      mesh,
    const VMatrix& vertices,
    bool           clearBeforeSet = true)
{
    if (vertices.cols() != 3)
        throw WrongSizeException("The input vertex matrix must have 3 columns");

    MatrixStorageType stg = matrixStorageType<VMatrix>();

    vertexPositionsFromBuffer(
        mesh, vertices.data(), vertices.rows(), clearBeforeSet, stg);
}

/**
 * @brief Sets the face indices of the given input `mesh` from the input face
 * matrix.
 *
 * If the MeshType is not a polygonal mesh (e.g. a triangle mesh), the number of
 * columns of the input face matrix must be equal to the number of vertices of
 * each face of the mesh (e.g. 3 for triangle meshes, 4 for quad meshes, etc.).
 * If this condition is not satisfied, an exception is thrown.
 *
 * If the MeshType is a polygonal mesh, the size of each polygonal face is
 * determined by counting the number of valid vertex indices in each row of the
 * input face matrix. Valid vertex indices are non-negative and not equal to
 * UINT_NULL.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the face container of the mesh and then adds a number of faces
 * that depends on the number of rows of the input face matrix. In this
 * scenario, all the old faces with their components stored in the mesh
 * before calling this function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the number of rows of the input face matrix is equal to the number of
 * faces of the mesh. If this is not the case, an exception is thrown.
 * Then, the function sets the indices of the faces of the mesh from the
 * input face matrix. In this scenario, all the components (except the
 * indices) of the faces stored in the mesh before calling this function
 * are preserved.
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
 * @param[in] faces: a \#F*K matrix containing the indices of the vertices of
 * the faces of the mesh. If the mesh is not a polygonal mesh (e.g. a
 * triangle mesh), K must be equal to the number of vertices of each face of
 * the mesh (e.g. 3 for triangle meshes, 4 for quad meshes, etc.). If this
 * condition is not satisfied, an exception is thrown.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the faces of the mesh before adding the faces from the input matrix.
 * If `false`, the function sets the indices from the input matrix to the
 * faces of the mesh, leaving all the other components untouched.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, MatrixConcept FMatrix>
void faceIndicesFromMatrix(
    MeshType&      mesh,
    const FMatrix& faces,
    bool           clearBeforeSet = true)
{
    MatrixStorageType stg = matrixStorageType<FMatrix>();

    faceIndicesFromBuffer(
        mesh, faces.data(), faces.rows(), faces.cols(), clearBeforeSet, stg);
}

/**
 * @brief Sets the edge indices of the given input `mesh` from the input edge
 * matrix.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the edge container of the mesh and then adds a number of edges
 * that depends on the number of rows of the input edge matrix. In this
 * scenario, all the old edges with their components stored in the mesh
 * before calling this function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the number of rows of the input edge matrix is equal to the number of
 * edges of the mesh. If this is not the case, an exception is thrown.
 * Then, the function sets the indices of the edges of the mesh from the
 * input edge matrix. In this scenario, all the components (except the
 * indices) of the edges stored in the mesh before calling this function
 * are preserved.
 *
 * All the other containers of the mesh are left as they are. Pointers to edges
 * stored in the other containers of the mesh are still valid only if the number
 * of edges is not changed (same allocation policy of the std::vector).
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @tparam EMatrix: the type of the input edge matrix. It must satisfy the
 * MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input edges.
 * @param[in] edges: a \#E*2 matrix containing the indices of the vertices of
 * the edges of the mesh.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the edges of the mesh before adding the edges from the input matrix.
 * If `false`, the function sets the indices in the input matrix to the
 * edges of the mesh, leaving all the other components untouched.
 *
 * @ingroup import_matrix
 */
template<EdgeMeshConcept MeshType, MatrixConcept EMatrix>
void edgeIndicesFromMatrix(
    MeshType&      mesh,
    const EMatrix& edges,
    bool           clearBeforeSet = true)
{
    if (edges.cols() != 2)
        throw WrongSizeException("The input edge matrix must have 2 columns");

    MatrixStorageType stg = matrixStorageType<EMatrix>();

    edgeIndicesFromBuffer(
        mesh, edges.data(), edges.rows(), clearBeforeSet, stg);
}

/**
 * @brief Sets the element identified by `ELEM_ID` selection of the given input
 * `mesh` from the input selection range (that could be anything that satisfies
 * the Range concept: e.g. std::vector<bool>, std::array<bool, N>,
 * Eigen::VectorXi, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * ELEM_ID elements of the mesh, otherwise an exception is thrown.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input element selection.
 * @param[in] selection: the input element selection range.
 *
 * @ingroup import_matrix
 */
template<uint ELEM_ID, MeshConcept MeshType, Range R>
void elementSelectionFromRange(MeshType& mesh, R&& selection)
{
    if (std::ranges::size(selection) != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input selection range must have the same number of elements "
            "as the number of " +
            elementEnumString<ELEM_ID>() + " element in the mesh");

    auto s = selection.begin();
    for (auto& e : mesh.template elements<ELEM_ID>()) {
        e.selected() = *s;
        ++s;
    }
}

/**
 * @brief Sets the vertex selection of the given input `mesh` from the input
 * selection range (that could be anything that satisfies the Range concept:
 * e.g. std::vector<bool>, std::array<bool, N>, Eigen::VectorXi, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input vertex selection.
 * @param[in] selection: the input vertex selection range.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, Range R>
void vertexSelectionFromRange(MeshType& mesh, R&& selection)
{
    elementSelectionFromRange<ElemId::VERTEX>(mesh, selection);
}

/**
 * @brief Sets the face selection of the given input `mesh` from the input
 * selection range (that could be anything that satisfies the Range concept:
 * e.g. std::vector<bool>, std::array<bool, N>, Eigen::VectorXi, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * faces of the mesh, otherwise an exception is thrown.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input face selection.
 * @param[in] selection: the input face selection range.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, Range R>
void faceSelectionFromRange(MeshType& mesh, R&& selection)
{
    elementSelectionFromRange<ElemId::FACE>(mesh, selection);
}

/**
 * @brief Sets the edge selection of the given input `mesh` from the input
 * selection range (that could be anything that satisfies the Range concept:
 * e.g. std::vector<bool>, std::array<bool, N>, Eigen::VectorXi, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * edges of the mesh, otherwise an exception is thrown.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input edge selection.
 * @param[in] selection: the input edge selection range.
 *
 * @ingroup import_matrix
 */
template<EdgeMeshConcept MeshType, Range R>
void edgeSelectionFromRange(MeshType& mesh, R&& selection)
{
    elementSelectionFromRange<ElemId::EDGE>(mesh, selection);
}

/**
 * @brief Sets the element identified by `ELEM_ID` normals of the given input
 * `mesh` from the input normals matrix.
 *
 * The number of rows of the input matrix must be equal to the number of
 * ELEM_ID elements of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-element normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam NMatrix: the type of the input normals matrix. It must satisfy the
 * MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input element normals.
 * @param[in] normals: a \#E*3 matrix containing the normals of the elements
 * of the mesh.
 *
 * @ingroup import_matrix
 */
template<uint ELEM_ID, MeshConcept MeshType, MatrixConcept NMatrix>
void elementNormalsFromMatrix(MeshType& mesh, const NMatrix& normals)
{
    MatrixStorageType stg = matrixStorageType<NMatrix>();

    if (normals.cols() != 3)
        throw WrongSizeException(
            "The input " + elementEnumString<ELEM_ID>() +
            " normal matrix must have 3 columns");

    if (normals.rows() != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input normal matrix must have the same number of rows as the "
            "number of " +
            elementEnumString<ELEM_ID>() + " elements in the mesh");

    elementNormalsFromBuffer<ELEM_ID>(mesh, normals.data(), stg);
}

/**
 * @brief Sets the vertex normals of the given input `mesh` from the input
 * vertex normals matrix.
 *
 * The number of rows of the input matrix must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-vertex normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam VNMatrix: the type of the input vertex normals matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex normals.
 * @param[in] vertexNormals: a \#V*3 matrix containing the normals of the
 * vertices of the mesh.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, MatrixConcept VNMatrix>
void vertexNormalsFromMatrix(MeshType& mesh, const VNMatrix& vertexNormals)
{
    elementNormalsFromMatrix<ElemId::VERTEX>(mesh, vertexNormals);
}

/**
 * @brief Sets the face normals of the given input `mesh` from the input
 * face normals matrix.
 *
 * The number of rows of the input matrix must be equal to the number of faces
 * of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-face normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam FNMatrix: the type of the input face normals matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input face normals.
 * @param[in] faceNormals: a \#F*3 matrix containing the normals of the
 * faces of the mesh.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, MatrixConcept FNMatrix>
void faceNormalsFromMatrix(MeshType& mesh, const FNMatrix& faceNormals)
{
    elementNormalsFromMatrix<ElemId::FACE>(mesh, faceNormals);
}

/**
 * @brief Sets the edge normals of the given input `mesh` from the input
 * edge normals matrix.
 *
 * The number of rows of the input matrix must be equal to the number of edges
 * of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-edge normal component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @tparam ENMatrix: the type of the input edge normals matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input edge normals.
 * @param[in] edgeNormals: a \#E*3 matrix containing the normals of the
 * edges of the mesh.
 *
 * @ingroup import_matrix
 */
template<EdgeMeshConcept MeshType, MatrixConcept ENMatrix>
void edgeNormalsFromMatrix(MeshType& mesh, const ENMatrix& edgeNormals)
{
    elementNormalsFromMatrix<ElemId::EDGE>(mesh, edgeNormals);
}

/**
 * @brief Sets the element identified by `ELEM_ID` colors of the given input
 * `mesh` from the input colors matrix.
 *
 * The number of rows of the input matrix must be equal to the number of
 * elements of the mesh, otherwise an exception is thrown.
 *
 * The input matrix can have 3 or 4 columns. If it has 3 columns, the alpha
 * channel is set to 255 (1.0f).
 *
 * The input matrix can have integral or floating point scalar type. If it has
 * integral scalar type, the color components are expected to be in range
 * [0, 255]. If it has floating point scalar type, the color components are
 * expected to be in range [0.0f, 1.0f].
 *
 * The function enables the per-element color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam CMatrix: the type of the input colors matrix. It must satisfy the
 * MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input colors.
 * @param[in] colors: a \#V*3 or \#V*4 matrix containing the colors of the
 * elements of the mesh.
 *
 * @ingroup import_matrix
 */
template<uint ELEM_ID, MeshConcept MeshType, MatrixConcept CMatrix>
void elementColorsFromMatrix(MeshType& mesh, const CMatrix& colors)
{
    using MatrixScalar = CMatrix::Scalar;

    Color::Representation repr = std::integral<MatrixScalar> ?
                                     Color::Representation::INT_0_255 :
                                     Color::Representation::FLOAT_0_1;

    if (colors.rows() != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input colors matrix must have the same number of rows as the "
            "number of " +
            elementEnumString<ELEM_ID>() + " elements in the mesh");

    elementColorsFromBuffer<ELEM_ID>(
        mesh, colors.data(), colors.cols(), matrixStorageType<CMatrix>(), repr);
}

/**
 * @brief Sets the element identified by `ELEM_ID` colors of the given input
 * `mesh` from the input color range (that could be anything that satisfies
 * the Range concept: e.g. std::vector<double>, std::array<double, N>,
 * Eigen::VectorXd, etc.).
 *
 * Each color of the input range is interpeted as a packed 32-bit unsigned
 * integer in the given `colorFormat`.
 *
 * The number of elements of the input range must be equal to the number of
 * ELEM_ID elements of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-element quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input element color.
 * @param[in] colors: the input element color range.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_matrix
 */
template<uint ELEM_ID, MeshConcept MeshType, Range R>
void elementColorsFromRange(
    MeshType&     mesh,
    R&&           colors,
    Color::Format colorFormat)
{
    if (std::ranges::size(colors) != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input color range must have the same number of elements "
            "as the number of " +
            elementEnumString<ELEM_ID>() + " element in the mesh");

    enableIfPerElementComponentOptional<ELEM_ID, CompId::COLOR>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    auto c = colors.begin();
    for (auto& e : mesh.template elements<ELEM_ID>()) {
        e.color() = Color(*c, colorFormat);
        ++c;
    }
}

/**
 * @brief Sets the vertex colors of the given input `mesh` from the input
 * vertex colors matrix.
 *
 * The number of rows of the input matrix must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * The input matrix can have 3 or 4 columns. If it has 3 columns, the alpha
 * channel is set to 255 (1.0f).
 *
 * The input matrix can have integral or floating point scalar type. If it has
 * integral scalar type, the color components are expected to be in range
 * [0, 255]. If it has floating point scalar type, the color components are
 * expected to be in range [0.0f, 1.0f].
 *
 * The function enables the per-vertex color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam VCMatrix: the type of the input vertex colors matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex colors.
 * @param[in] vertexColors: a \#V*3 or \#V*4 matrix containing the colors of the
 * vertices of the mesh.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, MatrixConcept VCMatrix>
void vertexColorsFromMatrix(MeshType& mesh, const VCMatrix& vertexColors)
{
    elementColorsFromMatrix<ElemId::VERTEX>(mesh, vertexColors);
}

/**
 * @brief Sets the vertex colors of the given input `mesh` from the input color
 * range (that could be anything that satisfies the Range concept: e.g.
 * std::vector<double>, std::array<double, N>, Eigen::VectorXd, etc.).
 *
 * Each color of the input range is interpeted as a packed 32-bit unsigned
 * integer in the given `colorFormat`.
 *
 * The number of elements of the input range must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-vertex quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input vertex color.
 * @param[in] colors: the input vertex color range.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, Range R>
void vertexColorsFromRange(
    MeshType&     mesh,
    R&&           colors,
    Color::Format colorFormat)
{
    elementColorsFromRange<ElemId::VERTEX>(mesh, colors, colorFormat);
}

/**
 * @brief Sets the face colors of the given input `mesh` from the input
 * face colors matrix.
 *
 * The number of rows of the input matrix must be equal to the number of faces
 * of the mesh, otherwise an exception is thrown.
 *
 * The input matrix can have 3 or 4 columns. If it has 3 columns, the alpha
 * channel is set to 255 (1.0f).
 *
 * The input matrix can have integral or floating point scalar type. If it has
 * integral scalar type, the color components are expected to be in range
 * [0, 255]. If it has floating point scalar type, the color components are
 * expected to be in range [0.0f, 1.0f].
 *
 * The function enables the per-face color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam FCMatrix: the type of the input face colors matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input face colors.
 * @param[in] faceColors: a \#F*3 or \#F*4 matrix containing the colors of the
 * faces of the mesh.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, MatrixConcept FCMatrix>
void faceColorsFromMatrix(MeshType& mesh, const FCMatrix& faceColors)
{
    elementColorsFromMatrix<ElemId::FACE>(mesh, faceColors);
}

/**
 * @brief Sets the face colors of the given input `mesh` from the input color
 * range (that could be anything that satisfies the Range concept: e.g.
 * std::vector<double>, std::array<double, N>, Eigen::VectorXd, etc.).
 *
 * Each color of the input range is interpeted as a packed 32-bit unsigned
 * integer in the given `colorFormat`.
 *
 * The number of elements of the input range must be equal to the number of
 * faces of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-face quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input face color.
 * @param[in] colors: the input face color range.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, Range R>
void faceColorsFromRange(MeshType& mesh, R&& colors, Color::Format colorFormat)
{
    elementColorsFromRange<ElemId::FACE>(mesh, colors, colorFormat);
}

/**
 * @brief Sets the edge colors of the given input `mesh` from the input
 * edge colors matrix.
 *
 * The number of rows of the input matrix must be equal to the number of edges
 * of the mesh, otherwise an exception is thrown.
 *
 * The input matrix can have 3 or 4 columns. If it has 3 columns, the alpha
 * channel is set to 255 (1.0f).
 *
 * The input matrix can have integral or floating point scalar type. If it has
 * integral scalar type, the color components are expected to be in range
 * [0, 255]. If it has floating point scalar type, the color components are
 * expected to be in range [0.0f, 1.0f].
 *
 * The function enables the per-face color component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @tparam ECMatrix: the type of the input edge colors matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input edge colors.
 * @param[in] edgeColors: a \#E*3 or \#E*4 matrix containing the colors of the
 * edges of the mesh.
 *
 * @ingroup import_matrix
 */
template<EdgeMeshConcept MeshType, MatrixConcept ECMatrix>
void edgeColorsFromMatrix(MeshType& mesh, const ECMatrix& edgeColors)
{
    elementColorsFromMatrix<ElemId::EDGE>(mesh, edgeColors);
}

/**
 * @brief Sets the edge colors of the given input `mesh` from the input color
 * range (that could be anything that satisfies the Range concept: e.g.
 * std::vector<double>, std::array<double, N>, Eigen::VectorXd, etc.).
 *
 * Each color of the input range is interpeted as a packed 32-bit unsigned
 * integer in the given `colorFormat`.
 *
 * The number of elements of the input range must be equal to the number of
 * edges of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-edge quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input edge color.
 * @param[in] colors: the input edge color range.
 * @param[in] colorFormat: the format used to pack the color in a single 32 bit
 * value.
 *
 * @ingroup import_matrix
 */
template<EdgeMeshConcept MeshType, Range R>
void edgeColorsFromRange(MeshType& mesh, R&& colors, Color::Format colorFormat)
{
    elementColorsFromRange<ElemId::EDGE>(mesh, colors, colorFormat);
}

/**
 * @brief Sets the element identified by `ELEM_ID` quality of the given input
 * `mesh` from the input quality range (that could be anything that satisfies
 * the Range concept: e.g. std::vector<double>, std::array<double, N>,
 * Eigen::VectorXd, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * ELEM_ID elements of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-element quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input element quality.
 * @param[in] quality: the input element quality range.
 *
 * @ingroup import_matrix
 */
template<uint ELEM_ID, MeshConcept MeshType, Range R>
void elementQualityFromRange(MeshType& mesh, R&& quality)
{
    if (std::ranges::size(quality) != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input quality range must have the same number of elements "
            "as the number of " +
            elementEnumString<ELEM_ID>() + " element in the mesh");

    enableIfPerElementComponentOptional<ELEM_ID, CompId::QUALITY>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::QUALITY>(mesh);

    auto q = quality.begin();
    for (auto& e : mesh.template elements<ELEM_ID>()) {
        e.quality() = *q;
        ++q;
    }
}

/**
 * @brief Sets the vertex quality of the given input `mesh` from the input
 * quality range (that could be anything that satisfies the Range concept:
 * e.g. std::vector<double>, std::array<double, N>, Eigen::VectorXd, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-vertex quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input vertex quality.
 * @param[in] quality: the input vertex quality range.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, Range R>
void vertexQualityFromRange(MeshType& mesh, R&& quality)
{
    elementQualityFromRange<ElemId::VERTEX>(mesh, quality);
}

/**
 * @brief Sets the face quality of the given input `mesh` from the input
 * quality range (that could be anything that satisfies the Range concept:
 * e.g. std::vector<double>, std::array<double, N>, Eigen::VectorXd, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * faces of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-face quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input face quality.
 * @param[in] quality: the input face quality range.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, Range R>
void faceQualityFromRange(MeshType& mesh, R&& quality)
{
    elementQualityFromRange<ElemId::FACE>(mesh, quality);
}

/**
 * @brief Sets the edge quality of the given input `mesh` from the input
 * quality range (that could be anything that satisfies the Range concept:
 * e.g. std::vector<double>, std::array<double, N>, Eigen::VectorXd, etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * edges of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-edge quality component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * EdgeMeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input edge quality.
 * @param[in] quality: the input edge quality range.
 *
 * @ingroup import_matrix
 */
template<EdgeMeshConcept MeshType, Range R>
void edgeQualityFromRange(MeshType& mesh, R&& quality)
{
    elementQualityFromRange<ElemId::EDGE>(mesh, quality);
}

/**
 * @brief Sets the vertex texcoords of the given input `mesh` from the input
 * vertex texcoords matrix.
 *
 * The number of rows of the input matrix must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-vertex texcoords component if it is not already
 * enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam VTMatrix: the type of the input vertex texcoords matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input vertex texcoords.
 * @param[in] vertexTexCoords: a \#V*2 matrix containing the texcoords of the
 * vertices of the mesh.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, MatrixConcept VTMatrix>
void vertexTexCoordsFromMatrix(MeshType& mesh, const VTMatrix& vertexTexCoords)
{
    MatrixStorageType stg = matrixStorageType<VTMatrix>();

    if (vertexTexCoords.cols() != 2)
        throw WrongSizeException(
            "The input vertex texcoords matrix must have 2 columns");

    if (vertexTexCoords.rows() != mesh.vertexNumber())
        throw WrongSizeException(
            "The input vertex texcoords must have the same number of rows as "
            "the number of vertices in the mesh");

    vertexTexCoordsFromBuffer(mesh, vertexTexCoords.data(), stg);
}

/**
 * @brief Sets the vertex material indices of the given input `mesh` from the
 * input material indices range (that could be anything that satisfies the Range
 * concept: e.g. std::vector<uint>, std::array<uint, N>, Eigen::VectorXi,
 * etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * vertices of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-vertex material indices component if it is not
 * already enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input vertex material
 * indices.
 * @param[in] materialIndices: the input vertex material indices range.
 *
 * @ingroup import_matrix
 */
template<MeshConcept MeshType, Range R>
void vertexMaterialIndicesFromRange(MeshType& mesh, R&& materialIndices)
{
    if (std::ranges::size(materialIndices) != mesh.vertexNumber())
        throw WrongSizeException(
            "The input quality range must have the same number of elements "
            "as the number of vertices in the mesh");

    enableIfPerVertexMaterialIndexOptional(mesh);
    requirePerVertexMaterialIndex(mesh);

    auto tt = materialIndices.begin();
    for (auto& v : mesh.vertices()) {
        v.materialIndex() = *tt;
        ++tt;
    }
}

/**
 * @brief Sets the face wedge texcoords of the given input `mesh` from the input
 * face wedge texcoords matrix.
 *
 * The number of rows of the input matrix must be equal to the number of
 * faces of the mesh, otherwise an exception is thrown.
 *
 * The number of columns is expected to be equal to 2 * K, where K is the
 * number of vertices of the largest face of the mesh. If the mesh has fixed
 * face size (e.g. triangle mesh, quad mesh, etc.), K is equal to the number
 * of vertices of each face of the mesh.
 *
 * The function enables the per-face wedge texcoords component if it is not
 * already enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam FTMatrix: the type of the input face wedge texcoords matrix. It must
 * satisfy the MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input face wedge texcoords.
 * @param[in] faceWedgeTexCoords: a \#F*(K*2) matrix containing the wedge
 * texcoords of the faces of the mesh.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, MatrixConcept FTMatrix>
void faceWedgeTexCoordsFromMatrix(
    MeshType&       mesh,
    const FTMatrix& faceWedgeTexCoords)
{
    MatrixStorageType stg = matrixStorageType<FTMatrix>();

    if (faceWedgeTexCoords.rows() != mesh.faceNumber())
        throw WrongSizeException(
            "The input face wedge texcoords must have the same number of rows "
            "as the number of faces in the mesh");

    faceWedgeTexCoordsFromBuffer(
        mesh, faceWedgeTexCoords.data(), faceWedgeTexCoords.cols() / 2, stg);
}

/**
 * @brief Sets the face material indices of the given input `mesh` from
 * the input material indices range (that could be anything that satisfies the
 * Range concept: e.g. std::vector<uint>, std::array<uint, N>, Eigen::VectorXi,
 * etc.).
 *
 * The number of elements of the input range must be equal to the number of
 * faces of the mesh, otherwise an exception is thrown.
 *
 * The function enables the per-face material index component if it is not
 * already enabled.
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam R: the type of the input range. It must satisfy the Range concept.
 * @param[in/out] mesh: the mesh on which import the input face material
 * indices.
 * @param[in] texCoordIndices: the input face material indices range.
 *
 * @ingroup import_matrix
 */
template<FaceMeshConcept MeshType, Range R>
void faceMaterialIndicesFromRange(MeshType& mesh, R&& texCoordIndices)
{
    if (std::ranges::size(texCoordIndices) != mesh.faceNumber())
        throw WrongSizeException(
            "The input quality range must have the same number of elements "
            "as the number of faces in the mesh");

    enableIfPerFaceMaterialIndexOptional(mesh);
    requirePerFaceMaterialIndex(mesh);

    auto tt = texCoordIndices.begin();
    for (auto& f : mesh.faces()) {
        f.materialIndex() = *tt;
        ++tt;
    }
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_MATRIX_H
