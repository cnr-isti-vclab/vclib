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
 * @brief Get a #E Vector of booleans (or integers) containing the selection
 * status of the elements identified by `ELEM_ID` of a Mesh. The function is
 * templated on the Vector itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXi S =
 *     vcl::elementSelectionVector<VERTEX, Eigen::VectorXi>(myMesh);
 * @endif
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @tparam Vect: type of the vector to be returned.
 * @tparam MeshType: type of the input mesh, it must satisfy the MeshConcept.
 *
 * @param[in] mesh: input mesh
 * @return #E vector of booleans or integers (element selection)
 */
template<uint ELEM_ID, typename Vect, MeshConcept MeshType>
Vect elementSelectionVector(const MeshType& mesh)
{
	Vect S(mesh.template number<ELEM_ID>());

	uint i = 0;
	for (const auto& e : mesh.template elements<ELEM_ID>())
		S[i] = e.selected();
	return S;
}

/**
 * @brief Get a #V Vector of booleans (or integers) containing the selection
 * status of the vertices of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
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
	return elementSelectionVector<VERTEX, Vect>(mesh);
}

/**
 * @brief Get a #F Vector of booleans (or integers) containing the selection
 * status of the faces of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint].
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
	return elementSelectionVector<FACE, Vect>(mesh);
}

/**
 * @brief Get a #E*3 Matrix of scalars containing the normals of the elements
 * identified by `ELEM_ID` of a Mesh. The function is templated on the Matrix
 * itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-element normals.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d VN =
 *     vcl::elementNormalsMatrix<VERTEX, Eigen::MatrixX3d>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * normals available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @param[in] mesh: input mesh
 * @return #E*3 matrix of scalars (element normals)
 */
template<uint ELEM_ID, MatrixConcept Matrix, MeshConcept MeshType>
Matrix elementNormalsMatrix(const MeshType& mesh)
{
	requirePerElementComponent<ELEM_ID, NORMAL>(mesh);

	Matrix EN(mesh.template number<ELEM_ID>(), 3);

	uint i = 0;
	for (const auto& e : mesh.template elements<ELEM_ID>()) {
		for (uint j = 0; j < 3; ++j) {
			EN(i, j) = e.normal()[j];
		}
		++i;
	}
	return EN;
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
	return elementNormalsMatrix<VERTEX, Matrix>(mesh);
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
	return elementNormalsMatrix<FACE, Matrix>(mesh);
}

/**
 * @brief Get a #E*4 Matrix of integers containing the colors of the elements
 * identified by `ELEM_ID` of a Mesh. The function is templated on the Matrix
 * itself.
 *
 * This function works with every Matrix type that satisfies the MatrixConcept,
 * and requires that the mesh has per-element colors.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX4i EC =
 *     vcl::elementColorsMatrix<VERTEX, Eigen::MatrixX4i>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * colors available.
 *
 * @note This function does not guarantee that the rows of the matrix
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @param[in] mesh: input mesh
 * @return #E*4 matrix of integers (element colors)
 */
template<uint ELEM_ID, MatrixConcept Matrix, MeshConcept MeshType>
Matrix elementColorsMatrix(const MeshType& mesh)
{
	requirePerElementComponent<ELEM_ID, COLOR>(mesh);

	Matrix EC(mesh.template number<ELEM_ID>(), 4);

	uint i = 0;
	for (const auto& e : mesh.template elements<ELEM_ID>()) {
		for (uint j = 0; j < 4; ++j) {
			EC(i, j) = e.color()[j];
		}
		++i;
	}
	return EC;
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
	return elementColorsMatrix<VERTEX, Matrix>(mesh);
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
	return elementColorsMatrix<FACE, Matrix>(mesh);
}

/**
 * @brief Get a #E Vector of scalars containing the quality of the elements
 * identified by `ELEM_ID` of a Mesh. The function is templated on the Vector
 * itself.
 *
 * This function works with every Vector type that has a constructor with a
 * size_t argument and an operator[uint], and requires that the mesh has
 * per-element quality.
 *
 * Usage example with Eigen Vector:
 *
 * @code{.cpp}
 * Eigen::VectorXd EQ =
 *     vcl::elementQualityVector<VERTEX, Eigen::VectorXd>(myMesh);
 * @endcode
 *
 * @throws vcl::MissingComponentException if the mesh does not have per-element
 * quality available.
 *
 * @note This function does not guarantee that the rows of the vector
 * correspond to the element indices of the mesh. This scenario is possible
 * when the mesh has deleted elements. To be sure to have a direct
 * correspondence, compact the element container before calling this function.
 *
 * @tparam ELEM_ID: the ID of the element.
 * @param[in] mesh: input mesh
 * @return #V vector of scalars (element quality)
 */
template<uint ELEM_ID, typename Vect, MeshConcept MeshType>
Vect elementQualityVector(const MeshType& mesh)
{
	requirePerElementComponent<ELEM_ID, QUALITY>(mesh);

	Vect EQ(mesh.template number<ELEM_ID>(), 3);

	uint i = 0;
	for (const auto& e : mesh.template elements<ELEM_ID>()) {
		EQ[i] = e.quality();
		++i;
	}

	return EQ;
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
	return elementQualityVector<VERTEX, Vect>(mesh);
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
	return elementQualityVector<FACE, Vect>(mesh);
}

} // namespace vcl
