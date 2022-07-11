/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "export_to_matrix.h"

namespace vcl {

/**
 * @brief Get a #V*3 Matrix of scalars containing the coordinates of the vertices of a Mesh.
 * The function is templated on the Matrix itself.
 *
 * This function works with every Matrix type that:
 * - has a constructor with rows and columns numbers;
 * - has the `()` operator to acces to the (i,j) element (e.g.: `M(i,j) = 0;`).
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixX3d V = vcl::vertexMatrix<Eigen::MatrixX3d>(myMesh);
 * @endif
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param[in] mesh: input mesh
 * @return #V*3 matrix of scalars (vertex coordinates)
 */
template<typename Matrix, MeshConcept MeshType>
Matrix vertexMatrix(const MeshType& mesh)
{
	using VertexType = typename MeshType::VertexType;

	Matrix V(mesh.vertexNumber(), 3);

	uint i = 0;
	for (const VertexType& v : mesh.vertices()) {
		for (uint j = 0; j < 3; ++j) {
			V(i, j) = v.coord()[j];
		}
		++i;
	}
	return V;
}

/**
 * @brief Get a #F*max(size(F)) Matrix of integers containing the vertex indices for each face
 * of a Mesh.
 *
 * If the mesh is polygonal, the matrix will have a number of rows equal to the greatest polygon of
 * the mesh, and unused values will be set to -1.
 *
 * This function works with every Matrix type that:
 * - has a constructor with rows and columns numbers;
 * - has the `()` operator to acces to the (i,j) element (e.g.: `M(i,j) = 0;`);
 * - has the rows() and cols() member functions;
 * - has the conservativeResize() member function.
 *
 * Usage example with Eigen Matrix:
 *
 * @code{.cpp}
 * Eigen::MatrixXi F = vcl::faceMatrix<Eigen::MatrixXi>(myMesh);
 * @endif
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *     - Compactness
 *   - Faces
 *
 * @param[in] mesh: input mesh
 * @return #F*max(size(F))3 matrix of vertex indices
 */
template<typename Matrix, FaceMeshConcept MeshType>
Matrix faceMatrix(const MeshType& mesh)
{
	vcl::requireVertexContainerCompactness(mesh);

	using VertexType = typename MeshType::VertexType;
	using FaceType = typename MeshType::FaceType;

	Matrix F(mesh.faceNumber(), 3);

	uint i = 0;
	for (const FaceType& f : mesh.faces()){
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
		for (const VertexType* v : f.vertices()){
			F(i, j) = mesh.index(v);
			j++;
		}
		for (; j < F.cols(); ++j) // remaining vertices set to -1
			F(i, j ) = -1;
		++i; // go to next face/row
	}
	return F;
}

} // namespace vcl
