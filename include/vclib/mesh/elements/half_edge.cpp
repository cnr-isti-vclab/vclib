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

#include "half_edge.h"

namespace vcl {

template<typename MeshType, typename... Args>
HalfEdge<MeshType, Args...>::HalfEdge()
{
	(construct<Args>(), ...);
}

template<typename MeshType, typename... Args>
template<typename Element>
void HalfEdge<MeshType, Args...>::importFrom(const Element& e)
{
	(Args::importFrom(e), ...);
}

template<typename MeshType, typename... Args>
template<typename Comp>
void HalfEdge<MeshType, Args...>::construct()
{
	if constexpr (vcl::comp::HasInitMemberFunction<Comp>) {
		// todo - check here if component is optional and, in case, if enabled
		Comp::init();
	}
}

} // namespace vcl
