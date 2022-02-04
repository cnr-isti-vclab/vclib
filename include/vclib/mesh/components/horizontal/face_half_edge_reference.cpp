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

#include "face_half_edge_reference.h"

namespace vcl::comp {

template<typename HalfEdge>
FaceHalfEdgeReference<HalfEdge>::FaceHalfEdgeReference()
{
}

template<typename HalfEdge>
const HalfEdge *FaceHalfEdgeReference<HalfEdge>::outerHalfEdge() const
{
	return ohe;
}

template<typename HalfEdge>
HalfEdge*& FaceHalfEdgeReference<HalfEdge>::outerHalfEdge()
{
	return ohe;
}

template<typename HalfEdge>
uint FaceHalfEdgeReference<HalfEdge>::numberHoles() const
{
	return ihe.size();
}

template<typename HalfEdge>
const HalfEdge* FaceHalfEdgeReference<HalfEdge>::innerHalfEdge(uint i) const
{
	return ihe[i];
}

template<typename HalfEdge>
HalfEdge*& FaceHalfEdgeReference<HalfEdge>::innerHalfEdge(uint i)
{
	return ihe[i];
}

} // namespace vcl::comp
