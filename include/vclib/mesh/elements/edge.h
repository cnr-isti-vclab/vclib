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

#ifndef VCL_MESH_ELEMENTS_EDGE_H
#define VCL_MESH_ELEMENTS_EDGE_H

#include <vclib/concepts/mesh/elements/edge.h>

#include "element.h"

namespace vcl {

/**
 * @brief The Edge class
 *
 * @ingroup elements
 */
template<typename MeshType, typename... Comps>
class Edge : public Element<EDGE, MeshType, Comps...>
{
	// VertexPointers component of the Edge
	using VPtrs = typename Edge::VertexPointers;

public:
	using VertexType = typename VPtrs::VertexType;
};

template<typename MeshType, typename... Comps>
class Edge<MeshType, TypeWrapper<Comps...>> : public Edge<MeshType, Comps...>
{
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_H
