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

#ifndef VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H
#define VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H

#include <vclib/types.h>

namespace vcl::comp {

template<typename T>
concept HasInitMemberFunction = requires(T o)
{
	{ o.init() } -> std::same_as<void>;
};

template<typename T>
concept HasIsEnabledMemberFunction = requires(T o)
{
	{ o.isEnabled() } -> std::same_as<bool>;
};

template<typename T>
concept IsVerticalComponent = T::IS_VERTICAL == true && requires (T o)
{
	typename T::DataValueType;
	{ o.IS_VERTICAL } -> std::same_as<const bool&>;
};

template<typename T>
struct IsVerticalComponentPred
{
	static const bool value = IsVerticalComponent<T>;
};

template<typename T>
concept IsOptionalComponent = IsVerticalComponent<T> && T::IS_OPTIONAL == true && requires(T o)
{
	{ o.IS_OPTIONAL } -> std::same_as<const bool&>;
};

template<typename T>
class PointersComponentTriggerer
{
};

/**
 * @brief HasReferencesOfType concept
 *
 * Each component that store reference of a type R, must:
 *
 * - inherit from ReferencesComponentTriggerer<R>
 * - provide the following **protected** member functions:
 *   - void updateReferences(const R* oldBase, const R* newBase);
 *
 *     the function updates the stored R references having the old base of the container and the
 *     new base of the container.
 *
 *   - void updateReferencesAfterCompact(const R* base, const std::vector<int>& newIndices);
 *
 *     the function updates the stored R references having the base of the container, and, for each
 *     old element index, its new index in the container.
 *
 *   - template<typename Element, typename ElRType>
 *     void importReferencesFrom(const Element& e, const R* base, const ElRType* ebase);
 *
 *     the function imports the references from the references of another element.
 *     - e is the another element;
 *     - base is the base of container that stores this element
 *     - ebase is the base of the container that stores the another elements
 */
template<typename T, typename R>
concept HasReferencesOfType =
	std::is_base_of<PointersComponentTriggerer<R>, T>::value;

template<typename T, typename R>
concept HasOptionalReferencesOfType = HasReferencesOfType<T, R> && IsOptionalComponent<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H
