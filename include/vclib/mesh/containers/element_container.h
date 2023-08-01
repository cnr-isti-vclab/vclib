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

#ifndef VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
#define VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H

#include <vector>

#include <vclib/concepts/mesh/components/custom_components.h>
#include <vclib/concepts/mesh/containers.h>
#include <vclib/concepts/mesh/elements/element.h>
#include <vclib/iterators/mesh/element_container_iterator.h>
#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/views/view.h>

#include "custom_components_vector_map.h"
#include "vertical_components_vector_tuple.h"

namespace vcl::mesh {

template<ElementConcept T>
class ElementContainer : public ElementContainerTriggerer
{
	template<ElementConcept U>
	friend class ElementContainer;

	using ElementContainerType = ElementContainer<T>;

public:
	using ElementType    = T;
	using ParentMeshType = typename T::ParentMeshType;

	static const uint ELEMENT_TYPE = T::ELEMENT_TYPE;

	ElementContainer();

protected:
	/* Members that are directly inherited by Containers (just renaming them) */
	using ElementIterator      = ElementContainerIterator<std::vector, T>;
	using ConstElementIterator = ConstElementContainerIterator<std::vector, T>;

	const T& element(uint i) const;
	T&       element(uint i);

	uint elementNumber() const;
	uint elementContainerSize() const;
	uint deletedElementNumber() const;

	uint addElement();
	uint addElements(uint size);

	void clearElements();
	void resizeElements(uint size);
	void reserveElements(uint size);

	std::vector<uint> compactElements();

	void deleteElement(uint i);
	void deleteElement(const T* e);

	uint              elementIndexIfCompact(uint i) const;
	std::vector<uint> elementCompactIndices() const;

	void updateElementIndices(const std::vector<uint>& newIndices);

	ElementIterator      elementBegin(bool jumpDeleted = true);
	ElementIterator      elementEnd();
	ConstElementIterator elementBegin(bool jumpDeleted = true) const;
	ConstElementIterator elementEnd() const;
	auto                 elements(bool jumpDeleted = true);
	auto                 elements(bool jumpDeleted = true) const;

	void enableAllOptionalComponents();
	void disableAllOptionalComponents();

	template<typename C>
	bool isOptionalComponentEnabled() const;

	template<uint COMP_TYPE>
	bool isOptionalComponentEnabled() const;

	template<typename C>
	void enableOptionalComponent();

	template<uint COMP_TYPE>
	void enableOptionalComponent();

	template<typename C>
	void disableOptionalComponent();

	template<uint COMP_TYPE>
	void disableOptionalComponent();

	// Custom Components

	bool hasElemCustomComponent(const std::string& name) const
		requires comp::HasCustomComponents<T>;

	std::vector<std::string> elemCustomComponentNames() const
		requires comp::HasCustomComponents<T>;

	template<typename K>
	bool isElemCustomComponentOfType(const std::string& name) const
		requires comp::HasCustomComponents<T>;

	std::type_index elemComponentType(const std::string& name) const
		requires comp::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> elemCustomComponentNamesOfType() const
		requires comp::HasCustomComponents<T>;

	template<typename K>
	void addElemCustomComponent(const std::string& name)
		requires comp::HasCustomComponents<T>;

	void deleteElemCustomComponent(const std::string& name)
		requires comp::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> customComponentVectorHandle(
		const std::string& name) requires comp::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K> customComponentVectorHandle(
		const std::string& name) const requires comp::HasCustomComponents<T>;

	uint index(const T* e) const;

	void setParentMeshPointers(void* pm);

	template<typename Element>
	void updatePointers(const Element* oldBase, const Element* newBase);

	template<typename Element>
	void updatePointers(
		const Element*           base,
		const std::vector<uint>& newIndices);

	template<typename OtherMesh>
	void enableOptionalComponentsOf(const OtherMesh& m);

	template<typename OtherMesh>
	void importFrom(const OtherMesh& m);

	template<typename OtherMesh, typename ElPtrBase>
	void importPointersFrom(const OtherMesh& othMesh, ElPtrBase* base);

	// filter components of elements, taking only vertical ones
	using vComps = typename vcl::FilterTypesByCondition<
		comp::IsVerticalComponentPred,
		typename T::Components>::type;

	ParentMeshType* parentMesh = nullptr;

	/**
	 * @brief en: the number of elements in the container. Could be different
	 * from elements.size() due to elements marked as deleted into the
	 * container.
	 */
	uint en = 0;

	/**
	 * @brief vec: the vector of elements: will contain the set of elements,
	 * each one of these will contain the data of the horizontal components and
	 * a pointer to the parent mesh
	 */
	std::vector<T> vec;

	/**
	 * @brief vcVecTuple the tuple of vectors of all the vertical components of
	 * the element. Contains both the optional and the persistent vertical
	 * components
	 */
	VerticalComponentsVectorTuple<vComps> vcVecTuple;

	/**
	 * @brief ccVecMap the map that associates a string to a vector of custom
	 * components
	 */
	CustomComponentsVectorMap<comp::HasCustomComponents<T>> ccVecMap;

private:
	template<typename ElPtr, typename... Comps>
	void updatePointersOnComponents(
		const ElPtr* oldBase,
		const ElPtr* newBase,
		TypeWrapper<Comps...>);

	template<typename ElPtr, typename... Comps>
	void updatePointersOnComponents(
		const ElPtr*             base,
		const std::vector<uint>& newIndices,
		TypeWrapper<Comps...>);

	template<typename Container, typename MyBase, typename CBase>
	void importPointersFromContainer(
		const Container& c,
		MyBase*          base,
		const CBase*     cbase);

	template<
		typename Container,
		typename ElPtr,
		typename CBase,
		typename... Comps>
	void importPointersOnComponentsFrom(
		const Container& c,
		ElPtr*           base,
		const CBase*     cbase,
		TypeWrapper<Comps...>);

	template<typename Comp, typename ElPtr>
	void updatePointersOnComponent(const ElPtr* oldBase, const ElPtr* newBase);

	template<typename Comp, typename ElPtr>
	void updatePointersOnComponent(
		const ElPtr*             base,
		const std::vector<uint>& newIndices);

	template<typename Comp, typename Container, typename ElPtr, typename CBase>
	void importPointersOnComponentFrom(
		const Container& c,
		ElPtr*           base,
		const CBase*     cbase);

	template<typename Cont2, typename... Comps>
	void enableSameOptionalComponents(TypeWrapper<Comps...>, const Cont2& c2);

	template<typename Comp, typename Cont2>
	void enableSameOptionalComponent(const Cont2& c2);
};

} // namespace vcl::mesh

#include "element_container.cpp"

#endif // VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
