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

template <typename MeshType, typename... Args>
template<typename ElType>
void Element<MeshType, Args...>::importFrom(const ElType& v)
{
	(Args::importFrom(v), ...);
}

template <typename MeshType, typename... Args>
template<typename ElType>
uint Element<MeshType, Args...>::index() const
{
	assert(comp::ParentMeshPointer<MeshType>::parentMesh());
	return comp::ParentMeshPointer<MeshType>::parentMesh()->index(
		static_cast<const ElType*>(this));
}

template<typename MeshType, typename... Args>
void Element<MeshType, Args...>::initVerticalComponents()
{
	(construct<Args>(), ...);
}

template<typename MeshType, typename... Args>
template<typename Comp>
void Element<MeshType, Args...>::construct()
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
