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

#include "transform.h"

#include "normal.h"

#include <vclib/mesh/requirements.h>

namespace vcl {

template<typename MeshType, typename ScalarM>
void applyTransformMatrix(MeshType& mesh, const Matrix44<ScalarM>& matrix, bool updateNormals)
{
	requireVertices<MeshType>();

	using VertexType = typename MeshType::VertexType;
	for (VertexType& v : mesh.vertices()) {
		v.coord() = v.coord() * matrix;
	}
	if (updateNormals) {
		if constexpr (hasPerVertexNormal<MeshType>()) {
			if (isPerVertexNormalEnabled(mesh)) {
				multiplyPerVertexNormalsByMatrix(mesh, matrix);
			}
		}
		if constexpr (hasPerFaceNormal<MeshType>()) {
			if (isPerVertexNormalEnabled(mesh)) {
				multiplyPerFaceNormalsByMatrix(mesh, matrix);
			}
		}
	}
}

} // namespace vcl
