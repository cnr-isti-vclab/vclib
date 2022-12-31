/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "transform.h"

#include "normal.h"

namespace vcl {

template<MeshConcept MeshType, typename ScalarM>
void applyTransformMatrix(MeshType& mesh, const Matrix44<ScalarM>& matrix, bool updateNormals)
{
	using VertexType = typename MeshType::VertexType;
	for (VertexType& v : mesh.vertices()) {
		v.coord() *= matrix;
	}
	if (updateNormals) {
		if constexpr (HasPerVertexNormal<MeshType>) {
			if (isPerVertexNormalEnabled(mesh)) {
				multiplyPerVertexNormalsByMatrix(mesh, matrix);
			}
		}
		if constexpr (HasPerFaceNormal<MeshType>) {
			if (isPerFaceNormalEnabled(mesh)) {
				multiplyPerFaceNormalsByMatrix(mesh, matrix);
			}
		}
	}
}

template<MeshConcept MeshType, PointConcept PointType>
void translate(MeshType& mesh, const PointType& t)
{
	using VertexType = typename MeshType::VertexType;
	for (VertexType& v : mesh.vertices()) {
		v.coord() += t;
	}
}

template<MeshConcept MeshType, PointConcept PointType>
void scale(MeshType& mesh, const PointType& s)
{
	using VertexType = typename MeshType::VertexType;
	for (VertexType& v : mesh.vertices()) {
		v.coord()(0) *= s(0);
		v.coord()(1) *= s(1);
		v.coord()(2) *= s(2);
	}
}

} // namespace vcl
