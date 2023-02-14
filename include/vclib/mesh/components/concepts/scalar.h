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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_SCALAR_H
#define VCL_MESH_COMPONENTS_CONCEPTS_SCALAR_H

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasScalar concept is satisfied only if a Element class provides the types and member
 * functions specified in this concept. These types and member functions allow to access to a
 * Scalar component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal Scalar component
 * and the vertical OptionalScalar component, therefore it does not guarantee that a
 * template Element type that satisfies this concept provides Scalar component at runtime
 * (it is guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that Scalar is available at runtime, you need to call the member
 * function `isScalarEnabled()`.
 */
template<typename T>
concept HasScalar = requires(
	T o,
	const T& co)
{
	typename T::ScalarType;
	typename T::ScalarComponent;
	{ o.scalar() } -> std::same_as<typename T::ScalarType&>;
	{ co.scalar() } -> std::same_as<const typename T::ScalarType&>;
	{ co.isScalarEnabled() } -> std::same_as<bool>;
};

/**
 * @brief HasOptionalScalar concept is satisfied only if a class satisfis the HasScalar concept and
 * the static boolean constant IS_OPTIONAL is set to true.
 */
template<typename T>
concept HasOptionalScalar = HasScalar<T> && IsOptionalComponent<typename T::ScalarComponent>;


/* Detector function to check if a class has Scalar enabled */

template <typename T>
bool isScalarEnabledOn(const T& element)
{
	if constexpr (HasOptionalScalar<T>) {
		return element.isScalarEnabled();
	}
	else {
		return HasScalar<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_SCALAR_H
