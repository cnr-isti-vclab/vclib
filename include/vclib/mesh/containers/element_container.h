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

#include <vclib/mesh/components/concepts/custom_components.h>
#include <vclib/iterators/mesh/element_container_iterator.h>
#include <vclib/iterators/mesh/element_container_range_iterator.h>
#include "../components/vertical/vertical_component.h"
#include "../components/vertical/vectors/vertical_components_vector.h"
#include "containers_concepts.h"
#include "custom_components_vector_map.h"
#include "vertical_components_vector_tuple.h"


namespace vcl::mesh {

template <typename T>
class ElementContainer : public ElementContainerTriggerer
{
	friend class comp::VerticalComponent<T>;

	template <typename U>
	friend class ElementContainer;

	using ElementContainerType = ElementContainer<T>;

	static const bool HAS_CUSTOM_COMPONENTS = comp::HasCustomComponents<T>;

public:
	using ElementType          = T;
	using ElementIterator      = ElementContainerIterator<std::vector, T>;
	using ConstElementIterator = ConstElementContainerIterator<std::vector, T>;
	using ElementRangeIterator =
		ElementContainerRangeIterator<ElementContainerType, ElementIterator>;
	using ConstElementRangeIterator =
		ConstElementContainerRangeIterator<ElementContainerType, ConstElementIterator>;

	ElementContainer();

	const T& element(uint i) const;
	T& element(uint i);

	uint elementNumber() const;
	uint elementContainerSize() const;
	uint deletedElementNumber() const;

	void deleteElement(uint i);
	void deleteElement(const T* e);

	uint elementIndexIfCompact(uint i) const;
	std::vector<int> elementCompactIndices() const;

	template<typename MeshType>
	void setParentMeshPointers(MeshType* parentMesh);

	template<typename C>
	bool isOptionalComponentEnabled() const;

	template<typename C>
	void enableOptionalComponent();

	template<typename C>
	void disableOptionalComponent();

	ElementIterator           elementBegin(bool jumpDeleted = true);
	ElementIterator           elementEnd();
	ConstElementIterator      elementBegin(bool jumpDeleted = true) const;
	ConstElementIterator      elementEnd() const;
	ElementRangeIterator      elements(bool jumpDeleted = true);
	ConstElementRangeIterator elements(bool jumpDeleted = true) const;

protected:
	uint index(const T *e) const;
	void clearElements();

	template<typename MeshType>
	uint addElement(MeshType* parentMesh);

	template<typename MeshType>
	uint addElements(uint size, MeshType* parentMesh);

	template<typename MeshType>
	void reserveElements(uint size, MeshType* parentMesh);

	std::vector<int> compactElements();

	template<typename MeshType>
	void setParentMeshPointer(T& element, MeshType* parentMesh);
	void setContainerPointer(T& element);

	template<typename MeshType>
	void updateContainerPointers(const T* oldBase, const T* newBase, MeshType* parentMesh);

	template<typename Vertex>
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	template<typename Vertex>
	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);

	template<typename Face>
	void updateFaceReferences(const Face* oldBase, const Face* newBase);

	template<typename Face>
	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

	template<typename Edge>
	void updateEdgeReferences(const Edge* oldBase, const Edge* newBase);

	template<typename Edge>
	void updateEdgeReferencesAfterCompact(const Edge* base, const std::vector<int>& newIndices);

	template<typename HalfEdge>
	void updateHalfEdgeReferences(const HalfEdge* oldBase, const HalfEdge* newBase);

	template<typename HalfEdge>
	void
	updateHalfEdgeReferencesAfterCompact(const HalfEdge* base, const std::vector<int>& newIndices);

	template<typename Container>
	void enableOptionalComponentsOf(const Container& c);

	template<typename Container>
	void importFrom(const Container& c);

	template<typename Container, typename MyBase, typename CBase>
	void importVertexReferencesFrom(const Container& c, MyBase* base, const CBase* cbase);

	template<typename Container, typename MyBase, typename CBase>
	void importFaceReferencesFrom(const Container& c, MyBase* base, const CBase* cbase);

	template<typename Container, typename MyBase, typename CBase>
	void importEdgeReferencesFrom(const Container& c, MyBase* base, const CBase* cbase);

	template<typename Container, typename MyBase, typename CBase>
	void importHalfEdgeReferencesFrom(const Container& c, MyBase* base, const CBase* cbase);
	
	// filter components of elements, taking only vertical ones
	using vComps = typename vcl::FilterTypesByCondition<comp::IsVerticalComponentPred, typename T::Components>::type;
	
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

	internal::VerticalComponentsVector<T> optionalVec;
};

} // namespace vcl::mesh

#include "element_container.cpp"

#endif // VCL_MESH_CONTAINER_ELEMENT_CONTAINER_H
