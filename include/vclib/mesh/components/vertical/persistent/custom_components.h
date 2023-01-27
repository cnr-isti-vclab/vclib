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

#ifndef VCL_MESH_COMPONENTS_PERSISTENT_CUSTOM_COMPONENTS_H
#define VCL_MESH_COMPONENTS_PERSISTENT_CUSTOM_COMPONENTS_H

#include "../vertical_component.h"

#include <any>
#include <string>
#include <vector>

#include "../../concepts/custom_components.h"

namespace vcl::comp {

/**
 * @brief The CustomComponents class is a container of custom and additional components associated
 * to an Element (e.g. Vertex, Face).
 *
 */
template<typename T>
class CustomComponents : public virtual VerticalComponent<T>
{
private:
	using B = VerticalComponent<T>;
	uint thisId() const { return B::index((T*)this); }

public:
	bool hasCustomComponent(const std::string& attrName) const;

	template<typename CompType>
	const CompType& customComponent(const std::string& attrName) const;

	template<typename CompType>
	CompType& customComponent(const std::string& attrName);

protected:
	template <typename Element>
	void importFrom(const Element& e);
};

} // namespace vcl::comp

#include "custom_components.cpp"

#endif // VCL_MESH_COMPONENTS_PERSISTENT_CUSTOM_COMPONENTS_H
