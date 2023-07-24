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

template<
	MeshConcept MeshType,
	MatrixConcept VMatrix,
	MatrixConcept FMatrix = Eigen::MatrixX3i>
MeshType meshFromMatrices(
	const VMatrix& vertices,
	const FMatrix& faces = FMatrix())
{
	MeshType mesh;

	importVertices(mesh, vertices);

	if constexpr (HasFaces<MeshType>){
		importFaces(mesh, faces);
	}

	return mesh;
}

} // namespace internal

template<MeshConcept MeshType, MatrixConcept VMatrix>
MeshType pointCloudMeshFromMatrices(const VMatrix& vertices)
{
	return internal::meshFromMatrices<MeshType>(vertices);
}

template<FaceMeshConcept MeshType, MatrixConcept VMatrix, MatrixConcept FMatrix>
MeshType faceMeshFromMatrices(const VMatrix& vertices, const FMatrix& faces)
{
	return internal::meshFromMatrices<MeshType>(vertices, faces);
}

} // namespace vcl
