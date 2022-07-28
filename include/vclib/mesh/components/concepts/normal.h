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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_NORMAL_H
#define VCL_MESH_COMPONENTS_CONCEPTS_NORMAL_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasNormal concept
 *
 * This concept is satisfied only if a class has a member function 'normal()'.
 * No check is made on the return type.
 */
template<typename T>
concept HasNormal = requires(T o)
{
	typename T::NormalType;
	o.normal();
};

/**
 * @brief HasOptionalNormal concept
 *
 * This concept is satisfied only if a class has two member functions:
 * - 'normal()'
 * - '__optionalNormal()'
 */
template<typename T>
concept HasOptionalNormal = HasNormal<T> && requires(T o)
{
	{ o.__optionalNormal() } -> std::same_as<void>;
};

/* Detector function to check if a class has Normal enabled */

template <typename T>
bool isNormalEnabledOn(const T& element)
{
	if constexpr (HasOptionalNormal<T>) {
		return element.isNormalEnabled();
	}
	else {
		return HasNormal<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_NORMAL_H