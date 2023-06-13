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

#include "pointers.h"

namespace vcl {

namespace internal {

template<typename Component, typename MeshElement, typename ElementType>
void updateElementPointersOnComponent(MeshElement& el, const std::map<ElementType*, ElementType*>& mapElements)
{
	//todo
}

template<typename MeshElement, typename ElementType, typename... Components>
void updateElementPointersOnComponents(MeshElement& el, const std::map<ElementType*, ElementType*>& mapElements, TypeWrapper<Components...>)
{
	// Update pointers on components
	(updateElementPointersOnComponent<Components>(el, mapElements), ...);
}

template<typename Container, MeshConcept MeshType, typename ElementType>
void updateElementPointersOnContainer(MeshType& m, const std::map<ElementType*, ElementType*>& mapElements)
{
	using MeshElement = typename Container::ElementType;
	constexpr uint ELEMENT_ID = MeshElement::ELEMENT_TYPE;
	for (MeshElement& e : m.template elements<ELEMENT_ID>()) {
		updateElementPointersOnComponents(e, mapElements, MeshElement::Components());
	}
}


template<MeshConcept MeshType, typename ElementType, typename... Containers>
void updateElementPointersOnContainers(MeshType& m, const std::map<ElementType*, ElementType*>& mapElements, TypeWrapper<Containers...>)
{
	// Update pointers on containers
	(updateElementPointersOnContainer<Containers>(m, mapElements), ...);
}

} // namespace vcl::internal

template<MeshConcept MeshType, typename ElementType>
void updateElementPointers(MeshType& m, const std::map<ElementType*, ElementType*>& mapElements)
{
	internal::updateElementPointersOnContainers(m, mapElements, MeshType::Containers());
}

} // namespace vcl
