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

#ifndef  VCL_MESH_CONTAINERS_CONCEPTS_H
#define  VCL_MESH_CONTAINERS_CONCEPTS_H

#include <vclib/misc/types.h>

#include "../components/vertical/vertical_component.h"

namespace vcl::mesh {

class ElementContainerTriggerer
{
};

template<typename T>
concept IsElementContainer =
	std::is_base_of<ElementContainerTriggerer, T>::value;

template <typename T>
concept HasEdgeContainer = requires(T o)
{
	o.edge(uint());
};

template <typename T>
concept HasEdgeOptionalContainer =
	HasEdgeContainer<T> && comp::HasVerticalComponent<typename T::EdgeType>;

template <typename T>
concept HasHalfEdgeContainer = requires(T o)
{
	o.halfEdge(uint());
};

template <typename T>
concept HasHalfEdgeOptionalContainer =
	HasHalfEdgeContainer<T> && comp::HasVerticalComponent<typename T::HalfEdgeType>;

template <typename T>
concept HasFaceContainer = requires(T o)
{
	o.face(uint());
};

template <typename T>
concept HasFaceOptionalContainer =
	HasFaceContainer<T> && comp::HasVerticalComponent<typename T::FaceType>;

template <typename T>
concept HasVertexContainer = requires(T o)
{
	o.vertex(uint());
};

template <typename T>
concept HasVertexOptionalContainer =
	HasVertexContainer<T> && comp::HasVerticalComponent<typename T::VertexType>;

} // namespace vcl::mesh

#endif //  VCL_MESH_CONTAINERS_CONCEPTS_H
