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

#ifndef VCL_MESH_ELEMENTS_ELEMENT_H
#define VCL_MESH_ELEMENTS_ELEMENT_H

#include <vclib/concepts/mesh/elements/element.h>

#include <vclib/concepts/mesh/components/component.h>
#include <vclib/mesh/components/parent_mesh_pointer.h>
#include <vclib/types.h>

/**
 * @defgroup elements Elements
 * @ingroup mesh
 *
 * @brief List of all the Element classes, along with their concepts and
 * functions.
 */

namespace vcl::mesh {

template<ElementConcept>
class ElementContainer;

} // namespace vcl::mesh

namespace vcl {

/**
 * @brief The Element class
 *
 * @ingroup elements
 */
template <uint ELEM_TYPE, typename MeshType, typename... Comps>
class Element : public comp::ParentMeshPointer<MeshType>, public Comps...
{
	template<ElementConcept>
	friend class mesh::ElementContainer;

public:
	using ParentMeshType = MeshType;

	/**
	 * @brief Components is an alias to a vcl::TypeWrapper that wraps all the
	 * types from which the Element inherits (Comps) that are Components (they
	 * satisfy the ComponentConcept).
	 */
	using Components = typename vcl::FilterTypesByCondition<
		comp::IsComponentPred,
		vcl::TypeWrapper<Comps...>>::type;

	static const uint ELEMENT_TYPE = ELEM_TYPE;

	uint index() const;

	template<uint COMPONENT_ID>
	auto& component();

	template<uint COMPONENT_ID>
	const auto& component() const;

	template<typename ElType>
	void importFrom(const ElType& v);

private:
	// hide init and isAvailable members
	void init() {}
	bool isAvailable() const { return true; }

	// init to call after set parent mesh
	void initVerticalComponents();

	template<typename Comp>
	void construct();

	// Predicate structures

	// Components can be individuated with their ID, which is an unsigned int.
	// This struct sets its bool `value` to true if this Element has a Component
	// with the given unsigned integer COMP_ID. Sets also `type` with a
	// TypeWrapper contianing the Component that satisfied the condition. The
	// TypeWrapper will be empty if no Components were found.
	template<uint COMP_ID>
	struct ComponentIDPred
	{
	private:
		template <typename Cont>
		struct SameCmpPred
		{
			static constexpr bool value = Cont::COMPONENT_ID == COMP_ID;
		};

	public:
		// TypeWrapper of the found component, if any
		using type =
			typename vcl::FilterTypesByCondition<SameCmpPred, Components>::type;
		static constexpr bool value = NumberOfTypes<type>::value == 1;
	};

	template<uint COMP_ID>
	struct GetComponentFromID
	{
	private:
		template<typename>
		struct TypeUnwrapper {};

		template<typename C>
		struct TypeUnwrapper<TypeWrapper<C>>
		{
			using type = C;
		};
	public:
		using type = typename TypeUnwrapper<
			typename ComponentIDPred<COMP_ID>::type>::type;
	};
};

} // namespace vcl

#include "element.cpp"

#endif // VCL_MESH_ELEMENTS_ELEMENT_H
