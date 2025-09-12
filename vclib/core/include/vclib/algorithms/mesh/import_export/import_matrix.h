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
 *
 * You can access these algorithms by including `#include
 * <vclib/algorithms/mesh/import_export.h>`
 */

namespace vcl {

namespace detail {

template<uint ELEM_ID, MeshConcept MeshType, Range R>
void importElementSelectionFromRange(MeshType& mesh, R&& selection)
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

template<uint ELEM_ID, MeshConcept MeshType, MatrixConcept NMatrix>
void importElementNormalsFromMatrix(MeshType& mesh, const NMatrix& normals)
{
    // The type of the normal of the element
    using NormalType = MeshType::template ElementType<ELEM_ID>::NormalType;

    if (normals.cols() != 3)
        throw WrongSizeException(
            "The input " + elementEnumString<ELEM_ID>() +
            " normal matrix must have 3 columns");

    // matrix rows must be equal to the number of elements of the given type
    if (normals.rows() != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input normal matrix must have the same number of rows "
            "as the number of " +
            elementEnumString<ELEM_ID>() + " element in the mesh");

    enableIfPerElementComponentOptional<ELEM_ID, CompId::NORMAL>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::NORMAL>(mesh);

    uint i = 0;
    for (auto& e : mesh.template elements<ELEM_ID>()) {
        e.normal() = NormalType(normals(i, 0), normals(i, 1), normals(i, 2));
        i++;
    }
}

template<uint ELEM_ID, MeshConcept MeshType, MatrixConcept CMatrix>
void importElementColorsFromMatrix(MeshType& mesh, const CMatrix& colors)
{
    using MatrixScalar = CMatrix::Scalar;

    if (colors.cols() != 3 && colors.cols() != 4)
        throw WrongSizeException(
            "The input " + elementEnumString<ELEM_ID>() +
            " color matrix must have 3 or 4 columns");

    // matrix rows must be equal to the number of elements of the given type
    if (colors.rows() != mesh.template number<ELEM_ID>())
        throw WrongSizeException(
            "The input color matrix must have the same number of rows "
            "as the number of " +
            elementEnumString<ELEM_ID>() + " element in the mesh");

    enableIfPerElementComponentOptional<ELEM_ID, CompId::COLOR>(mesh);
    requirePerElementComponent<ELEM_ID, CompId::COLOR>(mesh);

    uint i = 0;
    for (auto& e : mesh.template elements<ELEM_ID>()) {
        // Matrix has colors in range 0-255
        if constexpr (std::integral<MatrixScalar>) {
            if (colors.cols() == 3) {
                e.color() = Color(colors(i, 0), colors(i, 1), colors(i, 2));
            }
            else {
                e.color() = Color(
                    colors(i, 0), colors(i, 1), colors(i, 2), colors(i, 3));
            }
        }
        else { // Matrix has colors in range 0-1
            if (colors.cols() == 3) {
                e.color() = Color(
                    colors(i, 0) * 255, colors(i, 1) * 255, colors(i, 2) * 255);
            }
            else {
                e.color() = Color(
                    colors(i, 0) * 255,
                    colors(i, 1) * 255,
                    colors(i, 2) * 255,
                    colors(i, 3) * 255);
            }
        }

        i++;
    }
}

} // namespace detail

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

    importMeshFromMatrices(mesh, vertices, faces, edges);

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
 */
template<
    MeshConcept   MeshType,
    MatrixConcept VMatrix,
    MatrixConcept FMatrix = Eigen::MatrixX3i,
    MatrixConcept EMatrix = Eigen::MatrixX2i>
void importMeshFromMatrices(
    MeshType&      mesh,
    const VMatrix& vertices,
    const FMatrix& faces = FMatrix(),
    const EMatrix& edges = EMatrix())
{
    mesh.clear();
    mesh.disableAllOptionalComponents();

    importVerticesFromMatrix(mesh, vertices);

    if constexpr (HasFaces<MeshType>) {
        if (faces.rows() > 0)
            importFacesFromMatrix(mesh, faces);
    }

    if constexpr (HasEdges<MeshType>) {
        if (edges.rows() > 0)
            importEdgesFromMatrix(mesh, edges);
    }
}

/**
 * @brief Sets the vertex positions of the given input `mesh` from the input
 * vertex matrix.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the vertex container of the mesh and then adds a number of vertices
 * that depends on the number of rows of the input vertex matrix. In this
 * scenario, all the other components of the vertices stored in the mesh before
 * calling this function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the number of rows of the input vertex matrix is equal to the number of
 * vertices of the mesh. If this is not the case, an exception is thrown.
 * Then, the function sets the positions of the vertices of the mesh from the
 * input vertex matrix. In this scenario, all the other components of the
 * vertices stored in the mesh before calling this function are preserved.
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
 */
template<MeshConcept MeshType, MatrixConcept VMatrix>
void importVerticesFromMatrix(
    MeshType&      mesh,
    const VMatrix& vertices,
    bool           clearBeforeSet = true)
{
    using PositionType = MeshType::VertexType::PositionType;

    if (vertices.cols() != 3)
        throw WrongSizeException("The input vertex matrix must have 3 columns");

    if (clearBeforeSet) {
        mesh.clearVertices();
        mesh.resizeVertices(vertices.rows());
    }
    else {
        if (vertices.rows() != mesh.vertexNumber()) {
            throw WrongSizeException(
                "The input vertex matrix has a different number of rows than "
                "the number of vertices of the mesh");
        }
    }

    uint i = 0;
    for (auto& v : mesh.vertices()) {
        v.position() =
            PositionType(vertices(i, 0), vertices(i, 1), vertices(i, 2));
        i++;
    }
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
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the face container of the mesh and then adds a number of faces that
 * depends on the number of rows of the input face matrix. In this
 * scenario, all the other components of the faces stored in the mesh before
 * calling this function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the number of rows of the input face matrix is equal to the number of
 * faces of the mesh. If this is not the case, an exception is thrown.
 * Then, the function sets the vertex indices of the faces of the mesh from the
 * input face matrix. In this scenario, all the other components of the
 * faces stored in the mesh before calling this function are preserved.
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
 * If `false`, the function sets the indices in the input matrix to the
 * faces of the mesh, leaving all the other components untouched.
 */
template<FaceMeshConcept MeshType, MatrixConcept FMatrix>
void importFacesFromMatrix(
    MeshType&      mesh,
    const FMatrix& faces,
    bool           clearBeforeSet = true)
{
    if (clearBeforeSet) {
        mesh.clearFaces();
        mesh.resizeFaces(faces.rows());
    }
    else {
        if (faces.rows() != mesh.faceNumber()) {
            throw WrongSizeException(
                "The input face matrix has a different number of rows "
                "than the number of faces of the mesh.\n"
                "Number of faces in the mesh: " +
                std::to_string(mesh.faceNumber()) +
                "\nNumber of rows in the input face matrix: " +
                std::to_string(faces.rows()));
        }
    }

    if constexpr (HasPolygons<MeshType>) {
        uint i = 0;
        for (auto& f : mesh.faces()) {
            uint vertexNumber = 0;

            // count the number of vertices of the face
            while (vertexNumber < faces.cols() &&
                   faces(i, vertexNumber) != -1 &&
                   faces(i, vertexNumber) != UINT_NULL)
                vertexNumber++;

            f.resizeVertices(vertexNumber);

            for (uint j = 0; j < vertexNumber; ++j)
                f.setVertex(j, faces(i, j));
            ++i;
        }
    }
    else { // the vertex number of mesh faces is fixed
        using FaceType = MeshType::FaceType;

        constexpr int VN = FaceType::VERTEX_NUMBER;
        if (faces.cols() == VN) { // faces of matrix and mesh have same size
            uint i = 0;
            for (auto& f : mesh.faces()) {
                for (uint j = 0; j < VN; ++j)
                    f.setVertex(j, faces(i, j));
                ++i;
            }
        }
        else {
            // cannot import the faces if the number of vertices is different
            // between the matrix and the mesh
            throw WrongSizeException(
                "The input face matrix has a different number of columns "
                "than the vertex number of the faces of the mesh.\n"
                "Vertex number of faces in the mesh: " +
                std::to_string(VN) +
                "\nNumber of columns in the input face matrix: " +
                std::to_string(faces.cols()));
        }
    }
}

/**
 * @brief Sets the edge indices of the given input `mesh` from the input edge
 * matrix.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * clears the edge container of the mesh and then adds a number of edges that
 * depends on the number of rows of the input edge matrix. In this
 * scenario, all the other components of the edges stored in the mesh before
 * calling this function are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the number of rows of the input edge matrix is equal to the number of
 * edges of the mesh. If this is not the case, an exception is thrown.
 * Then, the function sets the vertex indices of the edges of the mesh from the
 * input edge matrix. In this scenario, all the other components of the
 * edges stored in the mesh before calling this function are preserved.
 *
 * All the other containers of the mesh are left as they are. Pointers to edges
 * stored in the other containers of the mesh are still valid only if the number
 * of edges is not changed (same allocation policy of the std::vector).
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * FaceMeshConcept.
 * @tparam FMatrix: the type of the input face matrix. It must satisfy the
 * MatrixConcept.
 * @param[in/out] mesh: the mesh on which import the input faces.
 * @param[in] edges: a \#E*2 matrix containing the indices of the vertices of
 * the edges of the mesh.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the edges of the mesh before adding the edges from the input matrix.
 * If `false`, the function sets the indices in the input matrix to the
 * edges of the mesh, leaving all the other components untouched.
 */
template<EdgeMeshConcept MeshType, MatrixConcept EMatrix>
void importEdgesFromMatrix(
    MeshType&      mesh,
    const EMatrix& edges,
    bool           clearBeforeSet = true)
{
    if (edges.cols() != 2)
        throw WrongSizeException("The input edge matrix must have 2 columns");

    if (clearBeforeSet) {
        mesh.clearEdges();
        mesh.resizeEdges(edges.rows());
    }
    else {
        if (edges.rows() != mesh.edgeNumber()) {
            throw WrongSizeException(
                "The input edge matrix has a different number of rows than "
                "the number of edges of the mesh");
        }
    }

    uint i = 0;
    for (auto& e : mesh.edges()) {
        e.setVertex(0, edges(i, 0));
        e.setVertex(1, edges(i, 1));
        i++;
    }
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
 */
template<MeshConcept MeshType, MatrixConcept VNMatrix>
void importVertexNormalsFromMatrix(
    MeshType&       mesh,
    const VNMatrix& vertexNormals)
{
    detail::importElementNormalsFromMatrix<ElemId::VERTEX>(mesh, vertexNormals);
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
 */
template<FaceMeshConcept MeshType, MatrixConcept FNMatrix>
void importFaceNormalsFromMatrix(MeshType& mesh, const FNMatrix& faceNormals)
{
    detail::importElementNormalsFromMatrix<ElemId::FACE>(mesh, faceNormals);
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
 */
template<EdgeMeshConcept MeshType, MatrixConcept ENMatrix>
void importEdgeNormalsFromMatrix(MeshType& mesh, const ENMatrix& edgeNormals)
{
    detail::importElementNormalsFromMatrix<ElemId::EDGE>(mesh, edgeNormals);
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
 */
template<MeshConcept MeshType, MatrixConcept VCMatrix>
void importVertexColorsFromMatrix(MeshType& mesh, const VCMatrix& vertexColors)
{
    detail::importElementColorsFromMatrix<ElemId::VERTEX>(mesh, vertexColors);
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
 */
template<FaceMeshConcept MeshType, MatrixConcept FCMatrix>
void importFaceColorsFromMatrix(MeshType& mesh, const FCMatrix& faceColors)
{
    detail::importElementColorsFromMatrix<ElemId::FACE>(mesh, faceColors);
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
 */
template<EdgeMeshConcept MeshType, MatrixConcept ECMatrix>
void importEdgeColorsFromMatrix(MeshType& mesh, const ECMatrix& edgeColors)
{
    detail::importElementColorsFromMatrix<ElemId::EDGE>(mesh, edgeColors);
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
 */
template<MeshConcept MeshType, Range R>
void importVertexSelectionFromRange(MeshType& mesh, R&& selection)
{
    detail::importElementSelectionFromRange<ElemId::VERTEX>(mesh, selection);
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
 */
template<FaceMeshConcept MeshType, Range R>
void importFaceSelectionFromRange(MeshType& mesh, R&& selection)
{
    detail::importElementSelectionFromRange<ElemId::FACE>(mesh, selection);
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
 */
template<EdgeMeshConcept MeshType, Range R>
void importEdgeSelectionFromRange(MeshType& mesh, R&& selection)
{
    detail::importElementSelectionFromRange<ElemId::EDGE>(mesh, selection);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_MATRIX_H
