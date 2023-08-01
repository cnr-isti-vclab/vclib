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
	bool           clearBeforeSet)
{
	using CoordType = typename MeshType::VertexType::CoordType;

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
	bool           clearBeforeSet)
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
		// TODO - import polygon
	}
	else { // the vertex number of mesh faces is fixed
		using FaceType = typename MeshType::FaceType;

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
						"(a triangulation that does not guarantee a predefined "
						"number of faces is required).");
				}
				// TODO - manage triangulation
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
	bool           clearBeforeSet)
{
	if (edges.cols() != 2)
		throw WrongSizeException(
			"The input edge matrix must have 2 columns");

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

template<FaceMeshConcept MeshType, MatrixConcept VNMatrix>
void importVertexNormalsFromMatrix(
	MeshType&       mesh,
	const VNMatrix& vertexNormals)
{
	using NormalType = typename MeshType::VertexType::NormalType;

	if (vertexNormals.cols() != 3)
		throw WrongSizeException(
			"The input vertex normal matrix must have 3 columns");

	if (vertexNormals.rows() != mesh.vertexNumber())
		throw WrongSizeException(
			"The input vertex normal matrix must have the same number of rows "
			"as the number of vertices in the mesh");

	enableIfPerVertexNormalOptional(mesh);

	uint i = 0;
	for (auto& v : mesh.vertices()) {
		v.normal() = NormalType(
			vertexNormals(i, 0), vertexNormals(i, 1), vertexNormals(i, 2));
		i++;
	}
}

template<FaceMeshConcept MeshType, MatrixConcept FNMatrix>
void importFaceNormalsFromMatrix(MeshType& mesh, const FNMatrix& faceNormals)
{
	using NormalType = typename MeshType::FaceType::NormalType;

	if (faceNormals.cols() != 3)
		throw WrongSizeException(
			"The input face normal matrix must have 3 columns");

	if (faceNormals.rows() != mesh.faceNumber())
		throw WrongSizeException(
			"The input vertex normal matrix must have the same number of rows "
			"as the number of vertices in the mesh");

	enableIfPerFaceNormalOptional(mesh);

	uint i = 0;
	for (auto& f : mesh.faces()) {
		f.normal() =
			NormalType(faceNormals(i, 0), faceNormals(i, 1), faceNormals(i, 2));
		i++;
	}
}

} // namespace vcl
