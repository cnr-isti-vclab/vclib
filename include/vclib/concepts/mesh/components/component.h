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

enum ComponentEnumType {
	ADJ_EDGES = 0,
	ADJ_FACES,
	ADJ_VERTICES,
	BIT_FLAGS,
	BOUNDING_BOX,
	COLOR,
	COORDINATE,
	CUSTOM_COMPONENTS,
	FACE_HALF_EDGE_PTRS,
	HALF_EDGE_PTRS,
	MARK,
	NAME,
	NORMAL,
	PRINCIPAL_CURVATURE,
	QUALITY,
	TEX_COORD,
	TEXTURE_PATHS,
	TRANSFORM_MATRIX,
	VERTEX_HALF_EDGE_PTRS,
	VERTEX_PTRS,
	WEDGE_COLORS,
	WEDGE_TEX_COORDS
};

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
concept IsTiedToVertexNumber = T::TIED_TO_VERTEX_NUMBER;

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
 * @brief HasPointersOfType concept
 *
 * Each component that store pointers of a type R, must:
 *
 * - inherit from PointersComponentTriggerer<R>
 * - provide the following **protected** member functions:
 *   - void updatePointers(const R* oldBase, const R* newBase);
 *
 *     the function updates the stored R pointers having the old base of the container and the
 *     new base of the container.
 *
 *   - void updatePointersAfterCompact(const R* base, const std::vector<int>& newIndices);
 *
 *     the function updates the stored R pointers having the base of the container, and, for each
 *     old element index, its new index in the container.
 *
 *   - template<typename Element, typename ElRType>
 *     void importPointersFrom(const Element& e, const R* base, const ElRType* ebase);
 *
 *     the function imports the pointers from the pointers of another element.
 *     - e is the another element;
 *     - base is the base of container that stores this element
 *     - ebase is the base of the container that stores the another elements
 */
template<typename T, typename R>
concept HasPointersOfType =
	std::is_base_of<PointersComponentTriggerer<R>, T>::value;

template<typename T, typename R>
concept HasOptionalPointersOfType = HasPointersOfType<T, R> && IsOptionalComponent<T>;

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_COMPONENTS_COMPONENT_H
