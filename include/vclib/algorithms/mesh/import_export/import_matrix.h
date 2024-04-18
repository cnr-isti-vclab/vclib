/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/concepts/space/matrix.h>
#include <vclib/exceptions.h>
#include <vclib/mesh/requirements.h>
#include <vclib/space/polygon.h>

namespace vcl {

namespace detail {

template<MatrixConcept FMatrix>
std::vector<uint> faceVertIndices(const FMatrix& faces, uint f)
{
    std::vector<uint> fVerts;

    uint j = 0;
    while (j < faces.cols() && faces(f, j) != -1 && faces(f, j) != UINT_NULL)
        fVerts.push_back(faces(f, j));

    return fVerts;
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
                e.color() =
                    vcl::Color(colors(i, 0), colors(i, 1), colors(i, 2));
            }
            else {
                e.color() = vcl::Color(
                    colors(i, 0), colors(i, 1), colors(i, 2), colors(i, 3));
            }
        }
        else { // Matrix has colors in range 0-1
            if (colors.cols() == 3) {
                e.color() = vcl::Color(
                    colors(i, 0) * 255, colors(i, 1) * 255, colors(i, 2) * 255);
            }
            else {
                e.color() = vcl::Color(
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
 * @brief Creates and returns a new point cloud mesh from the input vertex
 * matrix and the other matrices that are given as arguments.
 *
 * The function accepts several input matrices/vectors, that are used only if
 * their number of rows is different from zero. It tries to fill all the data
 * contained in the non-empty input matrices into the Mesh:
 * - if a Mesh is not able to store same data, the relative input matrix is
 * ignored. For example, if the Mesh has no per Vertex Normals, the function
 * will ignore the input vertex normals matrix, also if that is not empty.
 * - if a Mesh is able to store some data and the relative input matrix is not
 * empty, the data will be stored in the Mesh. If the matrix has not the
 * expected size, an exception is thrown.
 *
 * @note All the disabled optional components will be enabled if their
 * corresponding input matrix is not empty.
 *
 * @throws vcl::WrongSizeException if the sizes of the non-empty input matrices
 * have not the expected sizes.
 *
 * @tparam MeshType: the type of the mesh to be created. It must satisfy the
 * MeshConcept.
 * @tparam VMatrix: the type of the input vertex matrix. It must satisfy the
 * MatrixConcept.
 * @tparam VNMatrix: the type of the input vertex normal matrix. It must satisfy
 * the MatrixConcept.
 *
 * @param[in] vertices: a #V*3 matrix containing the coordinates of the vertices
 * of the mesh.
 * @param[in] vertexNormals: a #V*3 matrix containing the normals of the
 * vertices of the mesh. If the number of rows of this matrix is zero, the
 * function will not add vertex normals to the mesh.
 *
 * @return a new point cloud mesh containing the data passed as argument.
 */
template<
    MeshConcept   MeshType,
    MatrixConcept VMatrix,
    MatrixConcept VNMatrix = Eigen::MatrixX3d>
MeshType pointCloudMeshFromMatrices(
    const VMatrix&  vertices,
    const VNMatrix& vertexNormals = VNMatrix())
{
    MeshType mesh;

    importMeshFromMatrices(
        mesh,
        vertices,
        Eigen::MatrixX3i(),
        Eigen::MatrixX2i(),
        vertexNormals,
        Eigen::MatrixX3d());

    return mesh;
}

/**
 * @brief Creates and returns a new mesh from the input vertex and face (if any)
 * matrices, and the other matrices that are given as arguments.
 *
 * The function accepts several input matrices/vectors, that are used only if
 * their number of rows is different from zero. It tries to fill all the data
 * contained in the non-empty input matrices into the Mesh:
 * - if a Mesh is not able to store same data, the relative input matrix is
 * ignored. For example, if the Mesh has no faces, the function will ignore the
 * input face matrix, also if that is not empty.
 * - if a Mesh is able to store some data and the relative input matrix is not
 * empty, the data will be stored in the Mesh. If the matrix has not the
 * expected size, an exception is thrown.
 *
 * @note All the disabled optional components will be enabled if their
 * corresponding input matrix is not empty.
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
 * @tparam VNMatrix: the type of the input vertex normal matrix. It must satisfy
 * the MatrixConcept.
 * @tparam FNMatrix: the type of the input face normal matrix. It must satisfy
 * the MatrixConcept.
 *
 * @param[in] vertices: a #V*3 matrix containing the coordinates of the vertices
 * of the mesh.
 * @param[in] faces: a #F*3 matrix containing the indices of the vertices of the
 * faces of the mesh. If the number of rows of this matrix is zero, the
 * function will not add faces to the mesh.
 * @param[in] vertexNormals: a #V*3 matrix containing the normals of the
 * vertices of the mesh. If the number of rows of this matrix is zero, the
 * function will not add vertex normals to the mesh.
 * @param[in] faceNormals: a #F*3 matrix containing the normals of the faces of
 * the mesh. If the number of rows of this matrix is zero, the function will not
 * add face normals to the mesh.
 *
 * @return a new mesh containing the data passed as argument.
 */
template<
    MeshConcept   MeshType,
    MatrixConcept VMatrix,
    MatrixConcept FMatrix  = Eigen::MatrixX3i,
    MatrixConcept VNMatrix = Eigen::MatrixX3d,
    MatrixConcept FNMatrix = Eigen::MatrixX3d>
MeshType meshFromMatrices(
    const VMatrix&  vertices,
    const FMatrix&  faces         = FMatrix(),
    const VNMatrix& vertexNormals = VNMatrix(),
    const FNMatrix& faceNormals   = FNMatrix())
{
    MeshType mesh;

    importMeshFromMatrices(
        mesh, vertices, faces, Eigen::MatrixX2i(), vertexNormals, faceNormals);

    return mesh;
}

/**
 * @brief Sets the given input `mesh` from the input vertex matrix, and the
 * other matrices that are given as arguments.
 *
 * The function accepts several input matrices/vectors, that are used only if
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
 * @note Only the optional components having their corresponding input matrix
 * not empty will be enabled and filled. All the other optional components will
 * be left disabled.
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
 * @tparam VNMatrix: the type of the input vertex normal matrix. It must satisfy
 * the MatrixConcept.
 * @tparam FNMatrix: the type of the input face normal matrix. It must satisfy
 * the MatrixConcept.
 *
 * @param[in] mesh: the mesh on which import all the input data from the
 * matrices.
 * @param[in] vertices: a #V*3 matrix containing the coordinates of the vertices
 * of the mesh.
 * @param[in] faces: a #F*3 matrix containing the indices of the vertices of the
 * faces of the mesh. If the number of rows of this matrix is zero, the
 * function will not add faces to the mesh.
 * @param[in] edges: a #E*2 matrix containing the indices of the vertices of the
 * edges of the mesh. If the number of rows of this matrix is zero, the
 * function will not add edges to the mesh.
 * @param[in] vertexNormals: a #V*3 matrix containing the normals of the
 * vertices of the mesh. If the number of rows of this matrix is zero, the
 * function will not add vertex normals to the mesh.
 * @param[in] faceNormals: a #F*3 matrix containing the normals of the faces of
 * the mesh. If the number of rows of this matrix is zero, the function will not
 * add face normals to the mesh.
 */
template<
    MeshConcept   MeshType,
    MatrixConcept VMatrix,
    MatrixConcept FMatrix  = Eigen::MatrixX3i,
    MatrixConcept EMatrix  = Eigen::MatrixX2i,
    MatrixConcept VNMatrix = Eigen::MatrixX3d,
    MatrixConcept FNMatrix = Eigen::MatrixX3d>
void importMeshFromMatrices(
    MeshType&       mesh,
    const VMatrix&  vertices,
    const FMatrix&  faces         = FMatrix(),
    const EMatrix&  edges         = EMatrix(),
    const VNMatrix& vertexNormals = VNMatrix(),
    const FNMatrix& faceNormals   = FNMatrix())
{
    mesh.clear();
    mesh.disableAllOptionalComponents();

    importVerticesFromMatrix(mesh, vertices);

    if constexpr (HasPerVertexNormal<MeshType>) {
        if (vertexNormals.rows() > 0) {
            importVertexNormalsFromMatrix(mesh, vertexNormals);
        }
    }

    if constexpr (HasFaces<MeshType>) {
        if (faces.rows() > 0)
            importFacesFromMatrix(mesh, faces);

        if constexpr (HasPerFaceNormal<MeshType>) {
            if (faceNormals.rows() > 0) {
                importFaceNormalsFromMatrix(mesh, faceNormals);
            }
        }
    }

    if constexpr (HasEdges<MeshType>) {
        if (edges.rows() > 0)
            importEdgesFromMatrix(mesh, edges);
    }
}

/**
 * @brief Sets the vertices of the given input `mesh` from the input vertex
 * matrix.
 *
 * If the argument `clearBeforeSet` is set to `true` (default), the function
 * container of the mesh and then adds a number of vertices that depends on the
 * number of rows of the input vertex matrix. In this scenario, all the other
 * components of the vertices stored in the mesh before calling this function
 * are lost.
 *
 * If the argument `clearBeforeSet` is set to `false`, the function checks that
 * the number of rows of the input vertex matrix is equal to the number of
 * vertices of the mesh. If this is not the case, an exception is thrown.
 * Then, the function sets the coordinates of the vertices of the mesh from the
 * input vertex matrix. In this scenario, all the other components of the
 * vertices stored in the mesh before calling this function are preserved.
 *
 * All the other containers of the mesh and the optional components of vertices
 * are left as they are. Pointers to vertices stored in the mesh are still
 * valid (if the number of vertices and their index in the container is not
 * changed).
 *
 * @throws vcl::WrongSizeException if the sizes of the input matrix are not
 * valid (e.g. columns != 3 or rows != mesh.vertexNumber()).
 *
 * @tparam MeshType: the type of the mesh to be filled. It must satisfy the
 * MeshConcept.
 * @tparam VMatrix: the type of the input vertex matrix. It must satisfy the
 * MatrixConcept.
 *
 * @param[in] mesh: the mesh on which import the input vertices.
 * @param[in] vertices: a #V*3 matrix containing the coordinates of the vertices
 * of the mesh.
 * @param[in] clearBeforeSet: if `true`, the function clears the container of
 * the vertices of the mesh before adding the vertices from the input matrix.
 * If `false`, the function sets the coordinates in the input matrix to the
 * vertices of the mesh, leaving all the other components untouched.
 */
template<MeshConcept MeshType, MatrixConcept VMatrix>
void importVerticesFromMatrix(
    MeshType&      mesh,
    const VMatrix& vertices,
    bool           clearBeforeSet = true)
{
    using CoordType = MeshType::VertexType::CoordType;

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
        v.coord() = CoordType(vertices(i, 0), vertices(i, 1), vertices(i, 2));
        i++;
    }
}

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
                "than the number of faces of the mesh.");
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
                f.vertex(j) = &mesh.vertex(faces(i, j));
        }
    }
    else { // the vertex number of mesh faces is fixed
        using FaceType = MeshType::FaceType;

        constexpr int VN = FaceType::VERTEX_NUMBER;
        if (faces.cols() == VN) { // faces of matrix and mesh have same size

            uint i = 0;
            for (auto& f : mesh.faces()) {
                for (uint j = 0; j < VN; ++j)
                    f.vertex(j) = &mesh.vertex(faces(i, j));
            }
        }
        else { // faces of matrix and mesh have different sizes
            // matrix cols is higher than 3 (polygons), but we have a triangle
            // mesh and we can triangulate the faces
            if constexpr (VN == 3) {
                if (!clearBeforeSet) {
                    throw WrongSizeException(
                        "Cannot import the input face matrix into the mesh "
                        "without clearing the face container first "
                        "(need to perform a triangulation to import polygons "
                        "in a triangle mesh, and this operation that does not "
                        "guarantee a predefined number of faces is required).");
                }
                mesh.reserveFaces(faces.rows());
                for (uint i = 0; i < faces.rows(); ++i) {
                    std::vector<uint> fVertIndices =
                        detail::faceVertIndices(faces, i);

                    addTriangleFacesFromPolygon(mesh, fVertIndices);
                }
            }
            else {
                // no triangulation available because VN != 3, we don't
                // know how to import the faces and we throw an exception
                throw WrongSizeException(
                    "The input face matrix has a different number of columns "
                    "than the number of vertices of the mesh faces.");
            }
        }
    }
}

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
        e.vertex(0) = &mesh.vertex(edges(i, 0));
        e.vertex(1) = &mesh.vertex(edges(i, 1));
        i++;
    }
}

template<MeshConcept MeshType, MatrixConcept VNMatrix>
void importVertexNormalsFromMatrix(
    MeshType&       mesh,
    const VNMatrix& vertexNormals)
{
    detail::importElementNormalsFromMatrix<ElemId::VERTEX>(mesh, vertexNormals);
}

template<FaceMeshConcept MeshType, MatrixConcept FNMatrix>
void importFaceNormalsFromMatrix(MeshType& mesh, const FNMatrix& faceNormals)
{
    detail::importElementNormalsFromMatrix<ElemId::FACE>(mesh, faceNormals);
}

template<MeshConcept MeshType, MatrixConcept VCMatrix>
void importVertexColorsFromMatrix(MeshType& mesh, const VCMatrix& vertexColors)
{
    detail::importElementColorsFromMatrix<ElemId::VERTEX>(mesh, vertexColors);
}

template<FaceMeshConcept MeshType, MatrixConcept FCMatrix>
void importFaceColorsFromMatrix(MeshType& mesh, const FCMatrix& faceColors)
{
    detail::importElementColorsFromMatrix<ElemId::FACE>(mesh, faceColors);
}

template<EdgeMeshConcept MeshType, MatrixConcept ECMatrix>
void importEdgeColorsFromMatrix(MeshType& mesh, const ECMatrix& edgeColors)
{
    detail::importElementColorsFromMatrix<ElemId::EDGE>(mesh, edgeColors);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_IMPORT_EXPORT_IMPORT_MATRIX_H
