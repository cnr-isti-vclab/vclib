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
	BIT_FLAGS = 0,
	COORDINATE,
	NORMAL,
	COLOR,
	QUALITY,
	MARK,
	PRINCIPAL_CURVATURE,
	TEX_COORD,
	VERTEX_PTRS,
	ADJ_EDGES,
	ADJ_FACES,
	ADJ_VERTICES,
	WEDGE_COLORS,
	WEDGE_TEX_COORDS,
	FACE_HALF_EDGE_PTRS,
	HALF_EDGE_PTRS,
	VERTEX_HALF_EDGE_PTRS,
	BOUNDING_BOX,
	NAME,
	TEXTURE_PATHS,
	TRANSFORM_MATRIX,
	CUSTOM_COMPONENTS
};

inline static constexpr uint COMPONENTS_NUMBER = 22;

template<typename T>
concept ComponentConcept = requires {
	{ T::COMPONENT_TYPE } -> std::same_as<const uint&>;
};

/**
 * @brief The predicate IsComponentPred sets its bool `value` to `true` when the type T
 * satisfies the ComponentConcept concept
 */
template<typename T>
struct IsComponentPred
{
	static const bool value = ComponentConcept<T>;
};

/**
 * @brief Given the ComponentEnumType of a Component and a list of Components, this predicate
 * sets its bool `value` to `true` if there exists a Component in the list having the ID COMP_TYPE,
 * and sets `type` to the TypeWrapper of the found component.
 *
 * If no Component was found, value will be set to `false` and type will contain an empty
 * TypeWrapper.
 */
template<uint COMP_TYPE, typename ... Components>
struct ComponentOfTypePred
{
private:
	template <typename Comp>
	struct SameCompPred
	{
		static constexpr bool value = Comp::COMPONENT_TYPE == COMP_TYPE;
	};

public:
	// TypeWrapper of the found container, if any
	using type = typename vcl::FilterTypesByCondition<SameCompPred, Components...>::type;
	static constexpr bool value = NumberOfTypes<type>::value == 1;
};

// TypeWrapper specialization
template<uint COMP_TYPE, typename ... Containers>
struct ComponentOfTypePred<COMP_TYPE, TypeWrapper<Containers...>> :
		public ComponentOfTypePred<COMP_TYPE, Containers...>
{
};

template<typename T, uint COMP_TYPE>
concept HasComponentOfType = ComponentOfTypePred<COMP_TYPE, typename T::Components>::value;

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

template<typename T, uint COMP_TYPE>
concept HasVerticalComponentOfType
	= HasComponentOfType<T, COMP_TYPE>
	  && IsVerticalComponent<typename FirstType<
		  typename ComponentOfTypePred<COMP_TYPE, typename T::Components>::type>::type>;

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

template<typename T, uint COMP_TYPE>
concept HasOptionalComponentOfType
	= HasComponentOfType<T, COMP_TYPE>
	  && IsOptionalComponent<typename FirstType<
		  typename ComponentOfTypePred<COMP_TYPE, typename T::Components>::type>::type>;

template<typename T>
class PointersComponentTriggerer
{
};

/**
 * @brief HasPointersOfType concept
 *
 * Each component that store pointers of a type R, must:
 *
 * - inherit from PointersComponentTriggerer<R> (automatic from Component class)
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
