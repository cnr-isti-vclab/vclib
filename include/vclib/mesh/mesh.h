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

#ifndef VCL_MESH_MESH_H
#define VCL_MESH_MESH_H

#include <vclib/concepts/mesh.h>

#include "mesh_components.h"
#include "mesh_containers.h"

namespace vcl {

/**
 * @defgroup mesh Mesh
 *
 * @brief List of classes, aliases, concepts and functions used for the
 * creation, customization, usage and manipulation of Mesh classes.
 */

/**
 * @defgroup components Components
 * @ingroup mesh
 *
 * @brief List of all the Component classes, along with their concepts and
 * functions.
 */

/**
 * @brief The Mesh class represents a generic 3D mesh. A mesh is composed of a
 * generic number of containers of Elements (which can be vertices, faces,
 * edges...), plus some other components.
 *
 * The Mesh class will expose all the public members of its containers and is
 * components, and its role is to implement all the functionalities that allow
 * these containers to comunicate (e.g. an operation on the vertex container
 * that requires to update also some face information).
 *
 * Therefore, in this page are documented only the functions that in some way
 * need to modify the status of more than one Container of the Mesh. Other
 * functions are inherited by the Container classes of the Elements of the Mesh,
 * or from its Components, and depend on all the templates that compose a
 * specific Mesh definition.
 *
 * @ingroup mesh
 */
template<typename... Args>
class Mesh : public Args...
{
	static_assert(HasVertices<Args...>, "A Mesh must have a VertexContainer.");

	template<typename El, bool b>
	friend struct comp::internal::CustomComponentsData;

	template<typename El, bool b>
	friend struct comp::internal::ComponentData;

	template<ElementConcept T>
	friend class mesh::ElementContainer;

	template<uint ELEM_ID, typename MeshType, typename... Comps>
	friend class Element;

	// Predicate structures

	template<uint ELEM_ID>
	struct ContainerOfElement :
			public mesh::ContainerOfElement<ELEM_ID, Mesh<Args...>>
	{
	};

	/**
	 * @brief The ContainerOf struct allows to get the Container of an Element
	 * on this Mesh.
	 *
	 * Usage:
	 *
	 * ```cpp
	 * using Container = ContainerOf<ElementType>::type;
	 * ```
	 */
	template<ElementConcept El>
	struct ContainerOf : public ContainerOfElement<El::ELEMENT_ID>
	{
	};

public:
	/**
	 * @brief Containers is a vcl::TypeWrapper that wraps all the types from
	 * which the Mesh inherits (Args) that are ElementContainers (they satisfy
	 * the ElementContainerConcept).
	 */
	using Containers = vcl::FilterTypesByCondition<
		mesh::IsElementContainerPred,
		vcl::TypeWrapper<Args...>>::type;

	/**
	 * @brief Components is an alias to a vcl::TypeWrapper that wraps all the
	 * types from which the Mesh inherits (Args) that are Components (they
	 * satisfy the ComponentConcept).
	 */
	using Components = vcl::FilterTypesByCondition<
		comp::IsComponentPred,
		vcl::TypeWrapper<Args...>>::type;

	/**
	 * @brief ElementType is an alias that exposes the type of the Element
	 * identified by the template parameter ELEM_ID.
	 *
	 * To be used, the Mesh must have an ElementContainer having ID ELEM_ID.
	 *
	 * Usage:
	 * ```cpp
	 * using VertexType = MeshType::template ElementType<VERTEX>;
	 * ```
	 */
	template<uint ELEM_ID>
	using ElementType = ContainerOfElement<ELEM_ID>::type::ElementType;

	/* constructors */

	Mesh();
	Mesh(const Mesh& oth);
	Mesh(Mesh&& oth);

	/* constexpr static member functions */

	template<ElementConcept El>
	static constexpr bool hasContainerOf();

	template<uint ELEM_ID>
	static constexpr bool hasContainerOf();

	template<uint ELEM_ID, uint COMP_ID>
	static constexpr bool hasPerElementComponent();

	template<uint ELEM_ID, uint COMP_ID>
	static constexpr bool hasPerElementOptionalComponent();

	/* member functions */

	bool isCompact() const;

	void clear();

	void compact();

	void enableAllOptionalComponents();
	void disableAllOptionalComponents();

	template<typename OtherMeshType>
	void enableSameOptionalComponentsOf(const OtherMeshType& m);

	template<typename OtherMeshType>
	void importFrom(const OtherMeshType& m);

	void swap(Mesh& m2);

	Mesh& operator=(Mesh oth);

	/*** Generic Element functions ***/
	template<ElementConcept El>
	uint index(const El& e) const requires (hasContainerOf<El>());

	template<ElementConcept El>
	uint index(const El* e) const requires (hasContainerOf<El>());

	template<uint ELEM_ID>
	const auto& element(uint i) const requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	auto& element(uint i) requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	uint number() const requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	uint containerSize() const requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	uint deletedNumber() const requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	uint add() requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	uint add(uint n) requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	void clearElements() requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	void resize(uint n) requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	void reserve(uint n) requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	void deleteElement(uint i) requires (hasContainerOf<ELEM_ID>());

	template<ElementConcept El>
	void deleteElement(const El* e) const requires (hasContainerOf<El>());

	template<ElementConcept El>
	void deleteElement(const El& e) const requires (hasContainerOf<El>());

	template<uint ELEM_ID>
	std::vector<uint> conpactIndices() const
		requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	void updateIndices(const std::vector<uint>& newIndices)
		requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	auto begin(bool jumpDeleted = true) requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	auto end() requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	auto begin(bool jumpDeleted = true) const
		requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	auto end() const requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	auto elements(bool jumpDeleted = true) requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID>
	auto elements(bool jumpDeleted = true) const
		requires (hasContainerOf<ELEM_ID>());

	template<uint ELEM_ID, uint COMP_ID>
	bool isPerElementComponentEnabled() const
		requires (hasPerElementOptionalComponent<ELEM_ID, COMP_ID>());

	template<uint ELEM_ID, uint COMP_ID>
	void enablePerElementComponent()
		requires (hasPerElementOptionalComponent<ELEM_ID, COMP_ID>());

	template<uint ELEM_ID, uint COMP_ID>
	void disablePerElementComponent()
		requires (hasPerElementOptionalComponent<ELEM_ID, COMP_ID>());

protected:
	template<typename Cont>
	bool isContainerCompact() const;

	template<typename Cont>
	void compactContainer();

	template<typename Cont>
	void enableAllOptionalComponentsInContainer();

	template<typename Cont>
	void disableAllOptionalComponentsInContainer();

	template<typename Cont>
	void clearContainer();

	template<ElementConcept Element>
	void updateAllPointers(const Element* oldBase, const Element* newBase);

	template<typename Cont, typename Element>
	void updatePointers(const Element* oldBase, const Element* newBase);

	template<ElementConcept Element>
	void updateAllPointers(
		const Element*           base,
		const std::vector<uint>& newIndices);

	template<typename Cont, typename Element>
	void updatePointers(
		const Element*           base,
		const std::vector<uint>& newIndices);

private:
	// hide init and isAvailable members
	void init() {};

	bool isAvailable() const { return true; }

	// enable optional components

	template<typename Cont, typename OtherMeshType>
	void enableSameOptionalComponentsOf(const OtherMeshType& m);

	// private parent mesh pointers functions

	void updateAllParentMeshPointers();

	template<typename Cont>
	void setParentMeshPointers();

	// private import member functions

	template<typename Cont, typename OthMesh>
	void importPointers(const OthMesh& m);

	template<typename Cont, typename ElemCont, typename OthMesh>
	void importPointersOfElement(const OthMesh& m);

	// private copy and swap member functions

	template<uint i, typename Cont, typename Array, typename... A>
	static void setContainerBase(const Mesh<A...>& m, Array& bases);

	template<typename... A>
	static auto getContainerBases(const Mesh<A...>& m);

	template<typename Cont, typename Array, typename... A>
	static void updatePointersOfContainerType(
		Mesh<A...>&  m,
		const Array& bases);

	// member functions used by friends

	template<uint ELEM_ID, typename T>
	uint elementIndex(const T* el) const;

	template<typename El>
	auto& customComponents();

	template<typename El>
	const auto& customComponents() const;

	template<typename El>
	auto& verticalComponents();

	template<typename El>
	const auto& verticalComponents() const;
};

} // namespace vcl

#include "mesh.cpp"

#endif // VCL_MESH_MESH_H
