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

template <ElementConcept T>
class ElementContainer : public ElementContainerTriggerer
{
	template <ElementConcept U>
	friend class ElementContainer;

	using ElementContainerType = ElementContainer<T>;

public:
	using ElementType    = T;
	using ParentMeshType = typename T::ParentMeshType;

	static const uint ELEMENT_TYPE = T::ELEMENT_TYPE;

	ElementContainer();

protected:
	using ElementIterator      = ElementContainerIterator<std::vector, T>;
	using ConstElementIterator = ConstElementContainerIterator<std::vector, T>;

	const T& element(uint i) const;
	T& element(uint i);

	uint elementNumber() const;
	uint elementContainerSize() const;
	uint deletedElementNumber() const;

	void deleteElement(uint i);
	void deleteElement(const T* e);

	uint elementIndexIfCompact(uint i) const;
	std::vector<int> elementCompactIndices() const;

	void setParentMeshPointers(void* pm);

	template<typename C>
	bool isOptionalComponentEnabled() const;

	template<typename C>
	void enableOptionalComponent();

	template<typename C>
	void disableOptionalComponent();

	// Custom Components

	bool hasElemCustomComponent(const std::string& name) const
		requires comp::HasCustomComponents<T>;

	std::vector<std::string> elemCustomComponentNames() const
		requires comp::HasCustomComponents<T>;

	template<typename K>
	bool isElemCustomComponentOfType(const std::string& name) const
		requires comp::HasCustomComponents<T>;
	
	std::type_index elemComponentType(const std::string& name) const;

	template<typename K>
	std::vector<std::string> elemCustomComponentNamesOfType() const
		requires comp::HasCustomComponents<T>;

	template<typename K>
	void addElemCustomComponent(const std::string& name)
		requires comp::HasCustomComponents<T>;

	void deleteElemCustomComponent(const std::string& name)
		requires comp::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> customComponentVectorHandle(const std::string& name)
		requires comp::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K> customComponentVectorHandle(
		const std::string& name) const requires comp::HasCustomComponents<T>;

	ElementIterator      elementBegin(bool jumpDeleted = true);
	ElementIterator      elementEnd();
	ConstElementIterator elementBegin(bool jumpDeleted = true) const;
	ConstElementIterator elementEnd() const;
	auto                 elements(bool jumpDeleted = true);
	auto                 elements(bool jumpDeleted = true) const;

	uint index(const T *e) const;
	void clearElements();

	template<typename MeshType>
	uint addElement(MeshType* parentMesh);

	template<typename MeshType>
	uint addElements(uint size, MeshType* parentMesh);

	template<typename MeshType>
	void reserveElements(uint size, MeshType* parentMesh);

	template<typename MeshType>
	void resizeElements(uint size, MeshType* parentMesh);

	std::vector<int> compactElements();

	template<typename Element>
	void updatePointers(const Element* oldBase, const Element* newBase);

	template<typename Element>
	void updatePointersAfterCompact(const Element* base, const std::vector<int>& newIndices);

	template<typename OtherMesh>
	void enableOptionalComponentsOf(const OtherMesh& m);

	template<typename OtherMesh, typename MeshType>
	void importFrom(const OtherMesh& m, MeshType* parent);

	template<typename OtherMesh, typename ElPtrBase>
	void importPointersFrom(const OtherMesh& othMesh, ElPtrBase* base);
	
	// filter components of elements, taking only vertical ones
	using vComps = typename vcl::FilterTypesByCondition<comp::IsVerticalComponentPred, typename T::Components>::type;

    ParentMeshType* parentMesh = nullptr;

	/**
	 * @brief en: the number of elements in the container. Could be different from elements.size()
	 * due to elements marked as deleted into the container.
	 */
	uint en = 0;

	/**
	 * @brief vec: the vector of elements: will contain the set of elements, each one of these will
	 * contain the data of the horizontal components and a pointer to the parent mesh
	 */
	std::vector<T> vec;

	/**
	 * @brief vcVecTuple the tuple of vectors of all the vertical components of the element. Contains
	 * both the optional and the persistent vertical components
	 */
	VerticalComponentsVectorTuple<vComps> vcVecTuple;

	/**
	 * @brief ccVecMap the map that associates a string to a vector of custom components
	 */
	CustomComponentsVectorMap<T, comp::HasCustomComponents<T>> ccVecMap;

private:
	template<typename ElPtr, typename... Comps>
	void updatePointersOnComponents(
		const ElPtr* oldBase,
		const ElPtr* newBase,
		TypeWrapper<Comps...>);

	template<typename ElPtr, typename... Comps>
	void updatePointersAfterCompactOnComponents(
		const ElPtr*            base,
		const std::vector<int>& newIndices,
		TypeWrapper<Comps...>);

	template<typename Container, typename MyBase, typename CBase>
	void importPointersFromContainer(const Container& c, MyBase* base, const CBase* cbase);

	template<typename Container, typename ElPtr, typename CBase, typename... Comps>
	void importPointersOnComponentsFrom(
		const Container& c,
		ElPtr*           base,
		const CBase*     cbase,
		TypeWrapper<Comps...>);

	template<typename Comp, typename ElPtr>
	void updatePointersOnComponent(const ElPtr* oldBase, const ElPtr* newBase);

	template<typename Comp, typename ElPtr>
	void updatePointersAfterCompactOnComponent(
		const ElPtr* base,
		const std::vector<int>& newIndices);

	template<typename Comp, typename Container, typename ElPtr, typename CBase>
	void importPointersOnComponentFrom(
		const Container& c,
		ElPtr*           base,
		const CBase*     cbase);
};

} // namespace vcl::mesh

#include "element_container.cpp"

#endif // VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
