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

#ifndef VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_OPTIONAL_H
#define VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_OPTIONAL_H

#include "../components/vertical/persistent/custom_components.h"
#include "../components/vertical/optional/optional_color.h"
#include "../components/vertical/optional/optional_mark.h"
#include "../components/vertical/optional/optional_scalar.h"

namespace vcl::hedge {

/* Port OptionalColor class into hedge namespace */
template<typename EdgeType>
using OptionalColor = comp::OptionalColor<EdgeType>;

/* Port OptionalMark class into hedge namespace */
template<typename EdgeType>
using OptionalMark = comp::OptionalMark<EdgeType>;

template<typename EdgeType>
bool constexpr hasOptionalMark()
{
	return comp::hasOptionalMark<EdgeType>();
}

/* Port OptionalScalar class into hedge namespace */
template<typename ScalarType, typename EdgeType>
using OptionalScalar = comp::OptionalScalar<ScalarType, EdgeType>;

template<typename EdgeType>
using OptionalScalarf = comp::OptionalScalar<float, EdgeType>;

template<typename EdgeType>
using OptionalScalard = comp::OptionalScalar<double, EdgeType>;

template<typename EdgeType>
bool constexpr hasOptionalScalar()
{
	return comp::hasOptionalScalar<EdgeType>();
}

/* Port CustomComponents class into hedge namespace */
template<typename EdgeType>
using CustomComponents = comp::CustomComponents<EdgeType>;

/* Port VerticalComponent class into hedge namespace */
template<typename EdgeType>
using VerticalComponent = comp::VerticalComponent<EdgeType>;

} // namespace vcl::hedge

#endif // VCL_MESH_ELEMENTS_HALF_EDGE_COMPONENTS_OPTIONAL_H
