/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_ELEMENT_REFERENCES_H
#define VCL_MESH_COMPONENTS_ELEMENT_REFERENCES_H

#include "generic_container.h"

namespace vcl::comp {

/**
 * @brief The ElementReferences class is a generic container of references to another Element
 * (that could be Vertex, Face...). This class is meant to be inherited and used by Components
 * like Vertex References, Face References: in general, a class that need to store a static or
 * dynamic number of references of another element.
 *
 * Its major use is for adjacencies.
 */
template<typename Elem, int N>
class ElementReferences : protected GenericContainer<Elem*, N>
{
private:
	using Base = GenericContainer<Elem*, N>;

public:
	/** Constructor **/

	ElementReferences();

protected:
	void updateElementReferences(const Elem* oldBase, const Elem* newBase);

	void updateElementReferencesAfterCompact(const Elem* base, const std::vector<int>& newIndices);
};

} // namespace vcl::comp

#include "element_references.cpp"

#endif // VCL_MESH_COMPONENTS_ELEMENT_REFERENCES_H
