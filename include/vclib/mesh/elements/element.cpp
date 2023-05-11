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

#include "element.h"

namespace vcl {

template <uint ELEM_TYPE, typename MeshType, typename... Comps>
uint Element<ELEM_TYPE, MeshType, Comps...>::index() const
{
	assert(comp::ParentMeshPointer<MeshType>::parentMesh());
	return comp::ParentMeshPointer<MeshType>::parentMesh()
		->template elementIndex<ELEM_TYPE>(this);
}

template <uint ELEM_TYPE, typename MeshType, typename... Comps>
template<typename ElType>
void Element<ELEM_TYPE, MeshType, Comps...>::importFrom(const ElType& v)
{
	(Comps::importFrom(v), ...);
}

template <uint ELEM_TYPE, typename MeshType, typename... Comps>
template<uint COMPONENT_ID>
auto& Element<ELEM_TYPE, MeshType, Comps...>::component()
{
	using Comp = typename GetComponentFromID<COMPONENT_ID>::type;
	return *static_cast<Comp*>(this);
}

template <uint ELEM_TYPE, typename MeshType, typename... Comps>
template<uint COMPONENT_ID>
const auto& Element<ELEM_TYPE, MeshType, Comps...>::component() const
{
	using Comp = typename GetComponentFromID<COMPONENT_ID>::type;
	return *static_cast<const Comp*>(this);
}

template<uint ELEM_TYPE, typename MeshType, typename... Comps>
void Element<ELEM_TYPE, MeshType, Comps...>::initVerticalComponents()
{
	(construct<Comps>(), ...);
}

template<uint ELEM_TYPE, typename MeshType, typename... Comps>
template<typename Comp>
void Element<ELEM_TYPE, MeshType, Comps...>::construct()
{
	if constexpr (comp::IsVerticalComponent<Comp> && comp::HasInitMemberFunction<Comp>) {
		if constexpr (comp::HasIsEnabledMemberFunction<Comp>) {
			if (Comp::isEnabled()) {
				Comp::init();
			}
		}
		else { // no possibility to check if is enabled
			Comp::init();
		}
	}
}

} // namespace vcl
