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

namespace internal {

template<MeshConcept MeshType, MatrixConcept VMatrix>
void importVertices(MeshType& mesh, const VMatrix& vertices)
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
void importFaces(MeshType& mesh, const FMatrix& faces)
{
	if (faces.rows() > 0) { // face matrix is not empty
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
}

template<MeshConcept MeshType, MatrixConcept VNMatrix>
void importVertexNormals(MeshType& mesh, const VNMatrix& vertexNormals)
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

template<
	MeshConcept MeshType,
	MatrixConcept VMatrix,
	MatrixConcept FMatrix = Eigen::MatrixX3i,
	MatrixConcept VNMatrix = Eigen::MatrixX3d>
MeshType meshFromMatrices(
	const VMatrix& vertices,
	const FMatrix& faces = FMatrix(),
	const VNMatrix& vertexNormals = VNMatrix())
{
	MeshType mesh;

	importVertices(mesh, vertices);

	if constexpr (HasFaces<MeshType>) {
		if (faces.rows() > 0)
			importFaces(mesh, faces);
	}

	if constexpr (HasPerVertexNormal<MeshType>) {
		if (vertexNormals.rows() > 0) {
			importVertexNormals(mesh, vertexNormals);
		}
	}

	return mesh;
}

} // namespace internal

/**
 * @brief Creates and returns a new point cloud mesh from the input vertex
 * matrix and the other matrices that are given as arguments.
 *
 * The function accepts several input matrices, that are used only if their
 * number of rows is different from zero.
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
	return internal::meshFromMatrices<MeshType>(
		vertices,
		Eigen::MatrixX3i(),
		vertexNormals);
}

/**
 * @brief Creates and returns a new mesh from the input vertex and face
 * matrices, and the other matrices that are given as arguments.
 *
 * The function accepts several input matrices, that are used only if their
 * number of rows is different from zero.
 *
 * @throws vcl::WrongSizeException if the sizes of the non-empty input matrices
 * have not the expected sizes.
 *
 * @tparam MeshType: the type of the mesh to be created. It must satisfy the
 * FaceMeshConcept.
 * @tparam VMatrix: the type of the input vertex matrix. It must satisfy the
 * MatrixConcept.
 * @tparam FMatrix: the type of the input face matrix. It must satisfy the
 * MatrixConcept.
 * @tparam VNMatrix: the type of the input vertex normal matrix. It must satisfy
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
 *
 * @return a new mesh containing the data passed as argument.
 */
template<
	FaceMeshConcept MeshType,
	MatrixConcept VMatrix,
	MatrixConcept FMatrix,
	MatrixConcept VNMatrix>
MeshType meshFromMatrices(
	const VMatrix& vertices,
	const FMatrix& faces,
	const VNMatrix& vertexNormals)
{
	return internal::meshFromMatrices<MeshType>(
		vertices,
		faces,
		vertexNormals);
}

} // namespace vcl
