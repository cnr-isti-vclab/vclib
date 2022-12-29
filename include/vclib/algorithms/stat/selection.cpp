/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "selection.h"

namespace vcl {

template<MeshConcept MeshType>
uint vertexSelectionNumber(const MeshType &m)
{
	using VertexType = typename MeshType::VertexType;

	uint cnt = 0;
	for (const VertexType& v : m.vertices())
		if (v.isSelected())
			cnt++;

	return cnt;
}

template<FaceMeshConcept MeshType>
uint faceSelectionNumber(const MeshType &m)
{
	using FaceType = typename MeshType::FaceType;

	uint cnt = 0;
	for (const FaceType& f : m.faces())
		if (f.isSelected())
			cnt++;

	return cnt;
}

} // namespace vcl
