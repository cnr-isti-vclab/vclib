/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "matrix.h"

#include <vclib/exceptions.h>

namespace vcl {

/**
 * @brief Creates and returns a new point cloud mesh from the input vertex
 * matrix and the other matrices that are given as arguments.
 *
 * The function accepts several input matrices, that are used only if their
 * number of rows is different from zero.
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
	MeshConcept MeshType,
	MatrixConcept VMatrix,
	MatrixConcept VNMatrix>
MeshType pointCloudMeshFromMatrices(
	const VMatrix& vertices,
	const VNMatrix& vertexNormals)
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
 * The function accepts several input matrices, that are used only if their
 * number of rows is different from zero.
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
	MeshConcept MeshType,
	MatrixConcept VMatrix,
	MatrixConcept FMatrix,
	MatrixConcept VNMatrix,
	MatrixConcept FNMatrix>
MeshType meshFromMatrices(
	const VMatrix& vertices,
	const FMatrix& faces,
	const VNMatrix& vertexNormals,
	const FNMatrix& faceNormals)
{
	MeshType mesh;

	importMeshFromMatrices(
		mesh,
		vertices,
		faces,
		Eigen::MatrixX2i(),
		vertexNormals,
		faceNormals);

	return mesh;
}

/**
 * @brief Sets the given input `mesh` from the input vertex matrix, and the
 * other matrices that are given as arguments.
 *
 * @note If the Mesh is not able to store some data from the matrices, that data
 * will be ignored. For example, if the Mesh has no faces, the function will
 * ignore the input face matrix, also if that is not empty.
 *
 * @note Only the optional components having their corresponding input matrix
 * not empty will be enabled and filled. All the other optional components will
 * be disabled.
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
	MeshConcept MeshType,
	MatrixConcept VMatrix,
	MatrixConcept FMatrix,
	MatrixConcept EMatrix,
	MatrixConcept VNMatrix,
	MatrixConcept FNMatrix>
void importMeshFromMatrices(
	MeshType& mesh,
	const VMatrix& vertices,
	const FMatrix& faces,
	const EMatrix& edges,
	const VNMatrix& vertexNormals,
	const FNMatrix& faceNormals)
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

template<MeshConcept MeshType, MatrixConcept VMatrix>
void importVerticesFromMatrix(MeshType& mesh, const VMatrix& vertices)
{
	using CoordType = typename MeshType::VertexType::CoordType;

	if (vertices.cols() != 3)
		throw WrongSizeException("The input vertex matrix must have 3 columns");

	mesh.reserveVertices(vertices.rows());

	for (uint i = 0; i < vertices.rows(); ++i)
		mesh.addVertex(
			CoordType(vertices(i, 0), vertices(i, 1), vertices(i, 2)));
}

template<FaceMeshConcept MeshType, MatrixConcept FMatrix>
void importFacesFromMatrix(MeshType& mesh, const FMatrix& faces)
{
	if (faces.cols() == 3) { // faces are triangles
		mesh.reserveFaces(faces.rows());
		for (uint i = 0; i < faces.rows(); ++i)
			mesh.addFace(
				faces(i, 0), faces(i, 1), faces(i, 2));
	}
	else {
		if constexpr (HasPolygons<MeshType>) {
			// TODO - import polygon
		}
		else {
			// TODO - manage triangulation
		}
	}
}

template<EdgeMeshConcept MeshType, MatrixConcept EMatrix>
void importEdgesFromMatrix(MeshType& mesh, const EMatrix& edges)
{
	if (edges.cols() != 2)
		throw WrongSizeException(
			"The input edge matrix must have 2 columns");

	mesh.reserveFaces(edges.rows());
	for (uint i = 0; i < edges.rows(); ++i)
		mesh.addEdge(edges(i, 0), edges(i, 1));
}

template<FaceMeshConcept MeshType, MatrixConcept VNMatrix>
void importVertexNormalsFromMatrix(
	MeshType&       mesh,
	const VNMatrix& vertexNormals)
{
	using NormalType = typename MeshType::VertexType::NormalType;

	if (vertexNormals.cols() != 3)
		throw WrongSizeException(
			"The input vertex normal matrix must have 3 columns");

	if (vertexNormals.rows() != mesh.numVertices())
		throw WrongSizeException(
			"The input vertex normal matrix must have the same number of rows "
			"as the number of vertices in the mesh");

	enableIfPerVertexNormalOptional(mesh);

	for (uint i = 0; i < vertexNormals.rows(); ++i)
		mesh.vertex(i).normal() = NormalType(
			vertexNormals(i, 0), vertexNormals(i, 1), vertexNormals(i, 2));
}

template<FaceMeshConcept MeshType, MatrixConcept FNMatrix>
void importFaceNormalsFromMatrix(MeshType& mesh, const FNMatrix& faceNormals)
{
	using NormalType = typename MeshType::FaceType::NormalType;

	if (faceNormals.cols() != 3)
		throw WrongSizeException(
			"The input face normal matrix must have 3 columns");

	if (faceNormals.rows() != mesh.numFaces())
		throw WrongSizeException(
			"The input vertex normal matrix must have the same number of rows "
			"as the number of vertices in the mesh");

	enableIfPerFaceNormalOptional(mesh);

	for (uint i = 0; i < faceNormals.rows(); ++i)
		mesh.face(i).normal() = NormalType(
			faceNormals(i, 0), faceNormals(i, 1), faceNormals(i, 2));
}

} // namespace vcl
