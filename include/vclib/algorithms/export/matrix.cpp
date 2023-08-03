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

namespace vcl {

/**
 * @brief Get a #V*3 Matrix of scalars containing the coordinates of the
 * vertices of a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with an Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d V = vcl::vertexMatrix<Eigen::MatrixX3d>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return #V*3 matrix of scalars (vertex coordinates)
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexMatrix(const MeshType& mesh)
{
	Matrix V(mesh.vertexNumber(), 3);

	uint i = 0;
	for (const auto& v : mesh.vertices()) {
		for (uint j = 0; j < 3; ++j) {
			V(i, j) = v.coord()[j];
		}
		++i;
	}
	return V;
}

/**
 * @brief Get a #F*max(size(F)) Matrix of integers containing the vertex indices
 * for each face of a Mesh.
 *
 * If the mesh is polygonal, the matrix will have a number of rows equal to the
 * greatest polygon of the mesh, and unused values will be set to -1.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi F = vcl::faceMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return #F*max(size(F)) matrix of vertex indices
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceMatrix(const MeshType& mesh)
{
	vcl::requireVertexContainerCompactness(mesh);

	Matrix F(mesh.faceNumber(), 3);

	uint i = 0;
	for (const auto& f : mesh.faces()){
		// check if this face is greater than the cols of the matrix
		if (f.vertexNumber() > F.cols()) { // need to resize
			uint oldCols = F.cols(); // save old cols number
			F.conservativeResize(F.rows(), f.vertexNumber());
			// need to set to -1 all the previous rows that have been resized
			for (uint k = 0; k < i; ++k){
				for (uint j = oldCols; j < F.cols(); ++j)
					F(k, j) = -1;
			}
		}
		uint j = 0;
		for (const auto* v : f.vertices()){
			F(i, j) = mesh.index(v);
			j++;
		}
		for (; j < F.cols(); ++j) // remaining vertices set to -1
			F(i, j ) = -1;
		++i; // go to next face/row
	}
	return F;
}

/**
 * @brief Get a #F Vector of integers containing the sizes of the faces of a
 * Mesh.
 *
 * It could be useful when dealing with polygonal meshes.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator(uint).
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi F = vcl::faceSizesVector<Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return #F vector of integers (face sizes)
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceSizesVector(const MeshType& mesh)
{
	vcl::requireVertexContainerCompactness(mesh);

	Vect F(mesh.faceNumber());

	uint i = 0;
	for (const auto& f : mesh.faces()){
		F(i) = f.vertexNumber();
		++i;
	}
	return F;
}

/**
 * @brief Get a #E*2 Matrix of integers containing the indices of the vertices
 * of the edges of a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi E = vcl::edgeMatrix<Eigen::MatrixXi>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingCompactnessException if the vertex container is not
 * compact.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the edge indices of the mesh. This scenario is possible
 * when the mesh has deleted edges. To be sure to have a direct
 * correspondence, compact the edge container before calling this function.
 *
 * @tparam Matrix: type of the matrix to be returned, it must satisfy the
 * MatrixConcept.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * EdgeMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return #E*2 matrix of integers (edge indices)
 */
template<MatrixConcept Matrix, EdgeMeshConcept MeshType>
Matrix edgeMatrix(const MeshType &mesh)
{
	vcl::requireVertexContainerCompactness(mesh);

	Matrix E(mesh.edgeNumber(), 2);

	uint i = 0;
	for (const auto& e : mesh.edges()){
		E(i, 0) = mesh.index(e.vertex(0));
		E(i, 1) = mesh.index(e.vertex(1));
		++i; // go to next edge/row
	}
	return E;
}

/**
 * @brief Get a #V Vector of booleans (or integers) containing the selection
 * status of the vertices of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator(uint).
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi S = vcl::vertexSelectionVector<Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return #V vector of booleans or integers (vertex selection)
 */
template<typename Vect, MeshConcept MeshType>
Vect vertexSelectionVector(const MeshType& mesh)
{
	Vect S(mesh.vertexNumber());

	uint i = 0;
	for (const auto& v : mesh.vertices())
		S(i) = v.selected();
	return S;
}

/**
 * @brief Get a #F Vector of booleans (or integers) containing the selection
 * status of the faces of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator(uint).
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi S = vcl::faceSelectionVector<Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the
 * FaceMeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return #F vector of booleans or integers (face selection)
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceSelectionVector(const MeshType& mesh)
{
	Vect S(mesh.faceNumber());

	uint i = 0;
	for (const auto& f : mesh.faces())
		S(i) = f.selected();
	return S;
}

/**
 * @brief Get a #V*3 Matrix of scalars containing the normals of the vertices of
 * a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-vertex normals.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d VN = vcl::vertexNormalsMatrix<Eigen::MatrixX3d>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * normals available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return #V*3 matrix of scalars (vertex normals)
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexNormalsMatrix(const MeshType& mesh)
{
	requirePerVertexNormal(mesh);

	Matrix VN(mesh.vertexNumber(), 3);

	uint i = 0;
	for (const auto& v : mesh.vertices()) {
		for (uint j = 0; j < 3; ++j) {
			VN(i, j) = v.normal()[j];
		}
		++i;
	}
	return VN;
}

/**
 * @brief Get a #F*3 Matrix of scalars containing the normals of the faces of
 * a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-face normals.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d FN = vcl::faceNormalsMatrix<Eigen::MatrixX3d>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * normals available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return #F*3 matrix of scalars (face normals)
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceNormalsMatrix(const MeshType& mesh)
{
	vcl::requirePerFaceNormal(mesh);

	Matrix FN(mesh.faceNumber(), 3);

	uint i = 0;
	for (const auto& f : mesh.faces()) {
		for (uint j = 0; j < 3; ++j) {
			FN(i, j) = f.normal()[j];
		}
		++i;
	}
	return FN;
}

/**
 * @brief Get a #V*4 Matrix of integers containing the colors of the vertices of
 * a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-vertex colors.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX4i VC = vcl::vertexColorsMatrix<Eigen::MatrixX4i>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * colors available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return #V*4 matrix of integers (vertex colors)
 */
template<MatrixConcept Matrix, MeshConcept MeshType>
Matrix vertexColorsMatrix(const MeshType& mesh)
{
	requirePerVertexColor(mesh);

	Matrix VC(mesh.vertexNumber(), 4);

	uint i = 0;
	for (const auto& v : mesh.vertices()) {
		for (uint j = 0; j < 4; ++j) {
			VC(i, j) = v.color()[j];
		}
		++i;
	}
	return VC;
}

/**
 * @brief Get a #F*4 Matrix of integers containing the colors of the faces of
 * a Mesh. The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-face colors.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX4i FC = vcl::faceColorsMatrix<Eigen::MatrixX4i>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * colors available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return #F*4 matrix of integers (face colors)
 */
template<MatrixConcept Matrix, FaceMeshConcept MeshType>
Matrix faceColorsMatrix(const MeshType& mesh)
{
	vcl::requirePerFaceColor(mesh);

	Matrix FC(mesh.faceNumber(), 3);

	uint i = 0;
	for (const auto& f : mesh.faces()) {
		for (uint j = 0; j < 4; ++j) {
			FC(i, j) = f.color()[j];
		}
		++i;
	}
	return FC;
}

/**
 * @brief Get a #V Vector of scalars containing the quality of the vertices of
 * a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator(uint), and requires that the mesh has
 * per-vertex quality.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd VQ = vcl::vertexQualityVector<Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-vertex
 * quality available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the vertex indices of the mesh. This scenario is possible
 * when the mesh has deleted vertices. To be sure to have a direct
 * correspondence, compact the vertex container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return #V vector of scalars (vertex quality)
 */
template<typename Vect, MeshConcept MeshType>
Vect vertexQualityVector(const MeshType& mesh)
{
	vcl::requirePerVertexQuality(mesh);

	Vect VQ(mesh.vertexNumber());

	uint i = 0;
	for (const auto& v : mesh.vertices()) {
		VQ(i) = v.quality();
		++i;
	}

	return VQ;
}

/**
 * @brief Get a #F Vector of scalars containing the quality of the faces of
 * a Mesh. The function is templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator(uint), and requires that the mesh has
 * per-face quality.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd FQ = vcl::faceQualityVector<Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-face
 * quality available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the face indices of the mesh. This scenario is possible
 * when the mesh has deleted faces. To be sure to have a direct
 * correspondence, compact the face container before calling this function.
 *
 * @param[in] mesh: input mesh
 * @return #F vector of scalars (face quality)
 */
template<typename Vect, FaceMeshConcept MeshType>
Vect faceQualityVector(const MeshType& mesh)
{
	vcl::requirePerFaceQuality(mesh);

	Vect FQ(mesh.faceNumber());

	uint i = 0;
	for (const auto& f : mesh.faces()) {
		FQ(i) = f.quality();
		++i;
	}

	return FQ;
}

} // namespace vcl
