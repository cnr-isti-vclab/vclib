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

#ifndef VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_H
#define VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_H

#include "../components/horizontal/bit_flags.h"
#include "../components/horizontal/color.h"
#include "../components/horizontal/half_edge_references.h"
#include "../components/horizontal/mark.h"
#include "../components/horizontal/normal.h"
#include "../components/horizontal/scalar.h"

namespace vcl::hedge {

/* Port BitFlags class into hedge namespace */
using BitFlags = comp::BitFlags;

/* Port Color class into hedge namespace */
using Color = comp::Color;

/* Port HalfEdgeReferences class into hedge namespace */
template<typename HalfEdgeType, typename VertexType, typename FaceType>
using HalfEdgeReferences = comp::HalfEdgeReferences<HalfEdgeType, VertexType, FaceType>;

/* Port Mark class into hedge namespace */
using Mark = comp::Mark;

template<typename HalfEdgeType>
bool constexpr hasMark()
{
	return comp::hasMark<HalfEdgeType>();
}

/* Port Scalar class into hedge namespace */
template<typename ScalarType>
using Scalar = comp::Scalar<ScalarType>;

using Scalarf = comp::Scalarf;
using Scalard = comp::Scalard;

template<typename HalfEdgeType>
bool constexpr hasScalar()
{
	return comp::hasScalar<HalfEdgeType>();
}

} // namespace vcl::hedge

#endif // VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_H
