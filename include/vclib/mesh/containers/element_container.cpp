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

#include "element_container.h"

#include "../components/concepts/adjacent_edges.h"
#include "../components/concepts/adjacent_faces.h"
#include "../components/concepts/adjacent_vertices.h"
#include "../components/concepts/color.h"
#include "../components/concepts/face_half_edge_reference.h"
#include "../components/concepts/half_edge_references.h"
#include "../components/concepts/mark.h"
#include "../components/concepts/normal.h"
#include "../components/concepts/principal_curvature.h"
#include "../components/concepts/scalar.h"
#include "../components/concepts/tex_coord.h"
#include "../components/concepts/vertex_references.h"
#include "../components/concepts/vertex_half_edge_reference.h"
#include "../components/concepts/wedge_colors.h"
#include "../components/concepts/wedge_tex_coords.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Elements.
 */
template<typename T>
inline ElementContainer<T>::ElementContainer()
{
}

/**
 * @brief Returns a const reference of the element at the i-th position in the Element Container of
 * the Mesh, which will be the element having index = i.
 *
 * This function does not perform any sanity check: if i is less than elementContainerSize(), this
 * function will return a valid Element reference (note that the Element may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the element that will be returned.
 */
template<typename T>
inline const T& ElementContainer<T>::element(uint i) const
{
	return vec[i];
}

/**
 * @brief Returns a reference of the element at the i-th position in the Element Container of the
 * Mesh, which will be the element having index = i.
 *
 * This function does not perform any sanity check: if i is less than elementContainerSize(), this
 * function will return a valid Element reference (note that the Element may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the element that will be returned.
 */
template<typename T>
inline T& ElementContainer<T>::element(uint i)
{
	return vec[i];
}

/**
 * @brief Returns the number of **non-deleted** elements contained in the Element container of the
 * Mesh.
 *
 * If elementNumber() != elementContainerSize(), it means that there are some elements that are
 * flagged as deleted.
 *
 * @return The number of non-deleted elements of the Mesh.
 */
template<typename T>
inline uint ElementContainer<T>::elementNumber() const
{
	return en;
}

/**
 * @brief Returns the number of elements (also deleted) contained in the Element container of the
 * Mesh.
 *
 * If elementNumber() != elementContainerSize(), it means that there are some elements that are
 * flagged as deleted.
 *
 * @return The number of all the elements contained in the Mesh.
 */
template<typename T>
inline uint ElementContainer<T>::elementContainerSize() const
{
	return vec.size();
}

/**
 * @brief Returns the number of deleted elements in the Element container, that is
 * elementContainerSize() - elementNumber().
 *
 * @return The number of deleted elements in the container.
 */
template<typename T>
inline uint ElementContainer<T>::deletedElementNumber() const
{
	return elementContainerSize() - elementNumber();
}

/**
 * @brief Marks as deleted the element with the given id.
 *
 * This member function does not perform any reallocation of the elements: the deleted elements
 * will stay in the Element Container, but will be marked as deleted.
 *
 * Deleted elements are automatically jumped by the iterators provided by the Element Container.
 *
 * @param[in] i: the id of the element that will be marked as deleted.
 */
template<typename T>
inline void ElementContainer<T>::deleteElement(uint i)
{
	vec[i].setDeleted();
	--en;
}

/**
 * @brief Marks as deleted the given element, before asserting that the element belongs to this
 * container.
 *
 * This member function does not perform any reallocation of the elements: the deleted elements
 * will stay in the Element Container, but will be marked as deleted.
 *
 * Deleted elements are automatically jumped by the iterators provided by the Element Container.
 *
 * @param[in] e: the pointer of the element that will be marked as deleted.
 */
template<typename T>
inline void ElementContainer<T>::deleteElement(const T* e)
{
	deleteElement(index(e));
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the element with the given
 * index.
 *
 * Complexity: O(n), with n the number of elements in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a element of the container.
 * @return The index that the element with index i would have if this container would be compact.
 */
template<typename T>
uint ElementContainer<T>::elementIndexIfCompact(uint i) const
{
	if (vec.size() == en)
		return i;
	else {
		uint cnt = 0;
		for (uint ii = 0; ii < i; ii++) {
			if (!vec[ii].isDeleted())
				++cnt;
		}
		return cnt;
	}
}

/**
 * @brief Returns a vector that tells, for each actual element index, the new index that the element
 * would have in a compacted container. For each deleted element index, the value of the vector will
 * be -1.
 *
 * This is useful if you need to know the indices of the elements that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each element index, its index if the container would be compact.
 */
template<typename T>
std::vector<int> ElementContainer<T>::elementCompactIndices() const
{
	std::vector<int> newIndices(vec.size());
	uint             k = 0;
	for (uint i = 0; i < vec.size(); ++i) {
		if (!vec[i].isDeleted()) {
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = -1;
		}
	}
	return newIndices;
}

template<typename T>
template<typename MeshType>
void ElementContainer<T>::setParentMeshPointers(MeshType* parentMesh)
{
	for (auto& e : elements(false)) {
		e.setParentMesh(parentMesh);
	}
}

template<typename T>
template<typename C>
bool ElementContainer<T>::isOptionalComponentEnabled() const
{
	return vcVecTuple.template isComponentEnabled<C>();
}

template<typename T>
template<typename C>
void ElementContainer<T>::enableOptionalComponent()
{
	vcVecTuple.template enableComponent<C>();
	if constexpr (comp::HasInitMemberFunction<C>) {
		for (auto& e : elements()) {
			e.C::init();
		}
	}
}

template<typename T>
template<typename C>
void ElementContainer<T>::disableOptionalComponent()
{
	vcVecTuple.template disableComponent<C>();
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted elements of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return An iterator the the first element of the container.
 */
template<typename T>
typename ElementContainer<T>::ElementIterator ElementContainer<T>::elementBegin(bool jumpDeleted)
{
	auto it = vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted elements, and the first element is deleted, we need
		// to move forward until we find the first non-deleted element
		while (it != vec.end() && it->isDeleted()) {
			++it;
		}
	}
	return ElementIterator(it, vec, jumpDeleted && vec.size() != en);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<typename T>
typename ElementContainer<T>::ElementIterator ElementContainer<T>::elementEnd()
{
	return ElementIterator(vec.end(), vec);
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted elements of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return A const iterator the the first element of the container.
 */
template<typename T>
typename ElementContainer<T>::ConstElementIterator ElementContainer<T>::elementBegin(bool jumpDeleted) const
{
	auto it = vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted elements, and the first element is deleted, we need
		// to move forward until we find the first non-deleted element
		while (it != vec.end() && it->isDeleted()) {
			++it;
		}
	}
	return ConstElementIterator(it, vec, jumpDeleted && vec.size() != en);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<typename T>
typename ElementContainer<T>::ConstElementIterator ElementContainer<T>::elementEnd() const
{
	return ConstElementIterator(vec.end(), vec);
}

/**
 * @brief Returns a small utility object that allows to iterate over the elements of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the elements using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (Element& f : m.elements()){
 *     // do something with this element
 * }
 * @endcode
 *
 * The iterator used to iterate over elements is automatically initialized to jump deleted elements of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<typename T>
typename ElementContainer<T>::ElementRangeIterator ElementContainer<T>::elements(bool jumpDeleted)
{
	return ElementRangeIterator(
		*this,
		jumpDeleted && vec.size() != en,
		&ElementContainer::elementBegin,
		&ElementContainer::elementEnd);
}

/**
 * @brief Returns a small utility object that allows to iterate over the elements of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the elements using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (const Element& f : m.elements()){
 *     // do something with this element
 * }
 * @endcode
 *
 * The iterator used to iterate over elements is automatically initialized to jump deleted elements of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted elements.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<typename T>
typename ElementContainer<T>::ConstElementRangeIterator ElementContainer<T>::elements(bool jumpDeleted) const
{
	return ConstElementRangeIterator(
		*this,
		jumpDeleted && vec.size() != en,
		&ElementContainer::elementBegin,
		&ElementContainer::elementEnd);
}

template<typename T>
inline uint ElementContainer<T>::index(const T* e) const
{
	assert(!vec.empty() && e >= vec.data() && e <= &vec.back());
	return e - vec.data();
}

template<typename T>
void ElementContainer<T>::clearElements()
{
	vec.clear();
	en = 0;

	vcVecTuple.clear();
	ccVecMap.clear();
}

template<typename T>
template<typename MeshType>
uint ElementContainer<T>::addElement(MeshType* parentMesh)
{
	vcVecTuple.resize(vec.size() + 1);
	ccVecMap.resize(vec.size() + 1);

	T* oldB = vec.data();
	vec.push_back(T());
	T* newB = vec.data();
	en++;

	vec.back().setParentMesh(parentMesh);
	vec.back().initVerticalComponents();

	if (oldB != newB) {
		setParentMeshPointers(parentMesh);
	}

	return vec.size() - 1;
}

/**
 * @brief Adds size elements to the Element Container.
 *
 * Returns the id of the first added element.
 *
 * @param size
 * @return the id of the first added element.
 */
template<typename T>
template<typename MeshType>
uint ElementContainer<T>::addElements(uint size, MeshType* parentMesh)
{
	ccVecMap.resize(vec.size() + size);
	vcVecTuple.resize(vec.size() + size);

	uint baseId = vec.size();
	T*   oldB   = vec.data();
	vec.resize(vec.size() + size);
	T* newB = vec.data();
	en += size;

	for (uint i = baseId; i < vec.size(); ++i) {
		vec[i].setParentMesh(parentMesh);
		vec[i].initVerticalComponents();
	}

	if (oldB != newB) {
		setParentMeshPointers(parentMesh);
	}

	return baseId;
}

template<typename T>
template<typename MeshType>
void ElementContainer<T>::reserveElements(uint size, MeshType* parentMesh)
{
	T* oldB = vec.data();
	vec.reserve(size);
	T* newB = vec.data();

	ccVecMap.reserve(size);
	vcVecTuple.reserve(size);

	if (oldB != newB) {
		setParentMeshPointers(parentMesh);
	}
}

template<typename T>
template<typename MeshType>
void ElementContainer<T>::resizeElements(uint size, MeshType* parentMesh)
{
	T* oldB = vec.data();
	vec.resize(size);
	T* newB = vec.data();

	ccVecMap.resize(size);
	vcVecTuple.resize(size);

	if (oldB != newB) {
		setParentMeshPointers(parentMesh);
	}
}

/**
 * @brief Compacts the element container, keeping only the non-deleted elements.
 *
 * @return a vector that tells, for each old element index, the new index of the element. Will
 * contain -1 if the element has been deleted.
 */
template<typename T>
std::vector<int> ElementContainer<T>::compactElements()
{
	std::vector<int> newIndices = elementCompactIndices();
	if (elementNumber() != elementContainerSize()) {
		// k will indicate the position of the ith non-deleted vertices after compacting
		uint k = 0;
		for (uint i = 0; i < newIndices.size(); ++i) {
			if (newIndices[i] >= 0) {
				k = newIndices[i];
				if (i != k)
					vec[k] = vec[i];
			}
		}
		k++;
		vec.resize(k);

		ccVecMap.compact(newIndices);
		vcVecTuple.compact(newIndices);
	}
	return newIndices;
}

template<typename T>
template<typename Element>
void ElementContainer<T>::updateReferences(const Element* oldBase, const Element* newBase)
{
	using Comps = typename T::Components;

	updateReferencesOnComponents(oldBase, newBase, Comps());
}

template<typename T>
template<typename Vertex>
void ElementContainer<T>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	using Comps = typename T::Components;

	updateReferencesAfterCompactOnComponents(base, newIndices, Comps());
}

template<typename T>
template<typename Face>
void ElementContainer<T>::updateFaceReferences(const Face *oldBase, const Face *newBase)
{
	using Comps = typename T::Components;

	updateReferencesOnComponents(oldBase, newBase, Comps());
}

template<typename T>
template<typename Face>
void ElementContainer<T>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	using Comps = typename T::Components;

	updateReferencesAfterCompactOnComponents(base, newIndices, Comps());
}

template<typename T>
template<typename Edge>
void ElementContainer<T>::updateEdgeReferences(const Edge *oldBase, const Edge *newBase)
{
	using Comps = typename T::Components;

	updateReferencesOnComponents(oldBase, newBase, Comps());
}

template<typename T>
template<typename Edge>
void ElementContainer<T>::updateEdgeReferencesAfterCompact(
	const Edge*             base,
	const std::vector<int>& newIndices)
{
	using Comps = typename T::Components;

	updateReferencesAfterCompactOnComponents(base, newIndices, Comps());
}

template<typename T>
template<typename HalfEdge>
void ElementContainer<T>::updateHalfEdgeReferences(const HalfEdge *oldBase, const HalfEdge *newBase)
{
	using Comps = typename T::Components;

	updateReferencesOnComponents(oldBase, newBase, Comps());
}

template<typename T>
template<typename HalfEdge>
void ElementContainer<T>::updateHalfEdgeReferencesAfterCompact(
	const HalfEdge*             base,
	const std::vector<int>& newIndices)
{
	using Comps = typename T::Components;

	updateReferencesAfterCompactOnComponents(base, newIndices, Comps());
}

template<typename T>
template<typename Container>
void ElementContainer<T>::enableOptionalComponentsOf(const Container &c)
{
	// unfortunately, this function cannot be shortened in a smart way
	using CT = typename Container::ElementType;

	// Adjacent Edges
	// if this Element of this container has optional adjacent edges
	if constexpr (comp::HasOptionalAdjacentEdges<T>) {
		// if the other Container Element type has adjacent edges
		if constexpr (comp::HasAdjacentEdges<CT>) {

			// short circuited or: if optional, then I check if enabled; if not optional, then true
			if (!comp::HasOptionalAdjacentEdges<CT> ||
				c.template isOptionalComponentEnabled<typename CT::AdjacentEdgesComponent>()) {
				enableOptionalComponent<typename T::AdjacentEdgesComponent>();
			}
		}
	}
	// Adjacent Faces
	if constexpr (comp::HasOptionalAdjacentFaces<T>) {
		if constexpr (comp::HasAdjacentFaces<CT>) {
			if (!comp::HasOptionalAdjacentFaces<CT> ||
				c.template isOptionalComponentEnabled<typename CT::AdjacentFacesComponent>()) {
				enableOptionalComponent<typename T::AdjacentFacesComponent>();
			}
		}
	}
	// Adjacent Vertices
	if constexpr (comp::HasOptionalAdjacentVertices<T>) {
		if constexpr (comp::HasAdjacentVertices<CT>) {
			if (!comp::HasOptionalAdjacentVertices<CT> ||
				c.template isOptionalComponentEnabled<typename CT::AdjacentVerticesComponent>()) {
				enableOptionalComponent<typename T::AdjacentVerticesComponent>();
			}
		}
	}
	// Color
	if constexpr (comp::HasOptionalColor<T>) {
		if constexpr (comp::HasColor<CT>) {
			if (!comp::HasOptionalColor<CT> ||
				c.template isOptionalComponentEnabled<typename CT::ColorComponent>()) {
				enableOptionalComponent<typename T::ColorComponent>();
			}
		}
	}
	// Mark
	if constexpr (comp::HasOptionalMark<T>) {
		if constexpr (comp::HasMark<CT>) {
			if (!comp::HasOptionalMark<CT> ||
				c.template isOptionalComponentEnabled<typename CT::MarkComponent>()) {
				enableOptionalComponent<typename T::MarkComponent>();
			}
		}
	}
	// Normal
	if constexpr (comp::HasOptionalNormal<T>) {
		if constexpr (comp::HasNormal<CT>) {
			if (!comp::HasOptionalNormal<CT> ||
				c.template isOptionalComponentEnabled<typename CT::NormalComponent>()) {
				enableOptionalComponent<typename T::NormalComponent>();
			}
		}
	}
	// Principal Curvature
	if constexpr (comp::HasOptionalPrincipalCurvature<T>) {
		if constexpr (comp::HasPrincipalCurvature<CT>) {
			if (!comp::HasOptionalPrincipalCurvature<CT> ||
				c.template isOptionalComponentEnabled<typename CT::PrincipalCurvatureComponent>()) {
				enableOptionalComponent<typename T::PrincipalCurvatureComponent>();
			}
		}
	}
	// Scalar
	if constexpr (comp::HasOptionalScalar<T>) {
		if constexpr (comp::HasScalar<CT>) {
			if (!comp::HasOptionalScalar<CT> ||
				c.template isOptionalComponentEnabled<typename CT::ScalarComponent>()) {
				enableOptionalComponent<typename T::ScalarComponent>();
			}
		}
	}
	// TexCoord
	if constexpr (comp::HasOptionalTexCoord<T>) {
		if constexpr (comp::HasTexCoord<CT>) {
			if (!comp::HasOptionalTexCoord<CT> ||
				c.template isOptionalComponentEnabled<typename CT::TexCoordComponent>()) {
				enableOptionalComponent<typename T::TexCoordComponent>();
			}
		}
	}
	// Wedge Colors
	if constexpr (comp::HasOptionalWedgeColors<T>) {
		if constexpr (comp::HasWedgeColors<CT>) {
			if (!comp::HasOptionalWedgeColors<CT> ||
				c.template isOptionalComponentEnabled<typename CT::WedgeColorsComponent>()) {
				enableOptionalComponent<typename T::WedgeColorsComponent>();
			}
		}
	}
	// Wedge TexCoords
	if constexpr (comp::HasOptionalWedgeTexCoords<T>) {
		if constexpr (comp::HasWedgeTexCoords<CT>) {
			if (!comp::HasOptionalWedgeTexCoords<CT> ||
				c.template isOptionalComponentEnabled<typename CT::WedgeTexCoordsComponent>()) {
				enableOptionalComponent<typename T::WedgeTexCoordsComponent>();
			}
		}
	}
}

template<typename T>
template<typename Container, typename ParentMeshType>
void ElementContainer<T>::importFrom(const Container &c, ParentMeshType* parent)
{
	clearElements();
	// pointer to parent mesh needs to be updated later by the mesh
	addElements(c.elementContainerSize(), parent);
	unsigned int eid = 0;
	for (const typename Container::ElementType& e : c.elements(false)) {
		element(eid).importFrom(e);
		++eid;
	}
}

/**
 * This member function is called when this Element container needs to import vertex references from
 * another Container c.
 *
 * It is called when an import from another mesh type is performed. The import first creates all
 * the elements in the newly created mesh using the importFrom function, and then imports references
 * (pointers) from each element of the other mesh.
 *
 * For each element of this and the other container, we compute the offset between the any vertex
 * reference contained in the other element and its base (the pointer of the vertex 0) in the other
 * mesh, and then add this offset to the base (the pointer of the vertex 0) in this mesh.
 *
 * Takes the following inputs:
 * - c: another container of the same element of this container (but with different components)
 * - base: the pointer of the first Vertex reference of this mesh. We will use it to compute the
 *         new vertex pointers in this container
 * - cbase: the pointer of the first Vertex reference of the other mesh. We will use it to compute
 *          the offset between any vertex reference contained in the other element and the base,
 *          and the offset is then used to compute the new vertex pointer for this container
 */
template<typename T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importVertexReferencesFrom(const Container& c, MyBase* base, const CBase* cbase)
{
	using Comps = typename T::Components;

	importReferencesOnComponentsFrom(c, base, cbase, Comps());
}

/**
 * This member function is called when this Element container needs to import face references from
 * another Container c.
 *
 * It is called when an import from another mesh type is performed. The import first creates all
 * the elements in the newly created mesh using the importFrom function, and then imports references
 * (pointers) from each element of the other mesh.
 *
 * For each element of this and the other container, we compute the offset between the any face
 * reference contained in the other element and its base (the pointer of the face 0) in the other
 * mesh, and then add this offset to the base (the pointer of the face 0) in this mesh.
 *
 * Takes the following inputs:
 * - c: another container of the same element of this container (but with different components)
 * - base: the pointer of the first Face reference of this mesh. We will use it to compute the
 *         new face pointers in this container
 * - cbase: the pointer of the first Face reference of the other mesh. We will use it to compute
 *          the offset between any face reference contained in the other element and the base,
 *          and the offset is then used to compute the new face pointer for this container
 */
template<typename T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importFaceReferencesFrom(const Container& c, MyBase* base, const CBase* cbase)
{
	using Comps = typename T::Components;

	importReferencesOnComponentsFrom(c, base, cbase, Comps());
}

/**
 * This member function is called when this Element container needs to import edge references from
 * another Container c.
 *
 * It is called when an import from another mesh type is performed. The import first creates all
 * the elements in the newly created mesh using the importFrom function, and then imports references
 * (pointers) from each element of the other mesh.
 *
 * For each element of this and the other container, we compute the offset between the any edge
 * reference contained in the other element and its base (the pointer of the edge 0) in the other
 * mesh, and then add this offset to the base (the pointer of the edge 0) in this mesh.
 *
 * Takes the following inputs:
 * - c: another container of the same element of this container (but with different components)
 * - base: the pointer of the first Edge reference of this mesh. We will use it to compute the
 *         new edge pointers in this container
 * - cbase: the pointer of the first Edge reference of the other mesh. We will use it to compute
 *          the offset between any edge reference contained in the other element and the base,
 *          and the offset is then used to compute the new edge pointer for this container
 */
template<typename T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importEdgeReferencesFrom(const Container& c, MyBase* base, const CBase* cbase)
{
	using Comps = typename T::Components;

	importReferencesOnComponentsFrom(c, base, cbase, Comps());
}

/**
 * This member function is called when this Element container needs to import half edge references
 * from another Container c.
 *
 * It is called when an import from another mesh type is performed. The import first creates all
 * the elements in the newly created mesh using the importFrom function, and then imports references
 * (pointers) from each element of the other mesh.
 *
 * For each element of this and the other container, we compute the offset between the any half edge
 * reference contained in the other element and its base (the pointer of the half edge 0) in the
 * other mesh, and then add this offset to the base (the pointer of the half edge 0) in this mesh.
 *
 * Takes the following inputs:
 * - c: another container of the same element of this container (but with different components)
 * - base: the pointer of the first HalfEdge reference of this mesh. We will use it to compute the
 *         new half edge pointers in this container
 * - cbase: the pointer of the first HalfEdge reference of the other mesh. We will use it to compute
 *          the offset between any half edge reference contained in the other element and the base,
 *          and the offset is then used to compute the new half edge pointer for this container
 */
template<typename T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importHalfEdgeReferencesFrom(const Container& c, MyBase* base, const CBase* cbase)
{
	using Comps = typename T::Components;

	importReferencesOnComponentsFrom(c, base, cbase, Comps());
}

template<typename T>
template<typename ElRef, typename... Comps>
void ElementContainer<T>::updateReferencesOnComponents(
	const ElRef* oldBase,
	const ElRef* newBase,
	TypeWrapper<Comps...>)
{
	(updateReferencesOnComponent<Comps>(oldBase, newBase), ...);
}

template<typename T>
template<typename ElRef, typename... Comps>
void ElementContainer<T>::updateReferencesAfterCompactOnComponents(
	const ElRef* base,
	const std::vector<int>& newIndices,
	TypeWrapper<Comps...>)
{
	(updateReferencesAfterCompactOnComponent<Comps>(base, newIndices), ...);
}

template<typename T>
template<typename Container, typename ElRef, typename CBase, typename... Comps>
void ElementContainer<T>::importReferencesOnComponentsFrom(
	const Container& c,
	ElRef*           base,
	const CBase*     cbase,
	TypeWrapper<Comps...>)
{
	(importReferencesOnComponentFrom<Comps>(c, base, cbase), ...);
}

/*
 * This function is called for each component of the element.
 *
 * Only if a component has references of the type ElRef, then the updateReferences on each element
 * will be executed
 */
template<typename T>
template<typename Comp, typename ElRef>
void ElementContainer<T>::updateReferencesOnComponent(const ElRef* oldBase, const ElRef* newBase)
{
	if constexpr (comp::HasReferencesOfType<Comp, ElRef>) {
		if constexpr (comp::HasOptionalReferencesOfType<Comp, ElRef>) {
			if(isOptionalComponentEnabled<Comp>()) {
				for (T& e : elements()) {
					e.Comp::updateReferences(oldBase, newBase);
				}
			}
		}
		else {
			for (T& e : elements()) {
				e.Comp::updateReferences(oldBase, newBase);
			}
		}
	}
}

template<typename T>
template<typename Comp, typename ElRef>
void ElementContainer<T>::updateReferencesAfterCompactOnComponent(
	const ElRef* base,
	const std::vector<int>& newIndices)
{
	if constexpr (comp::HasReferencesOfType<Comp, ElRef>) {
		if constexpr (comp::HasOptionalReferencesOfType<Comp, ElRef>) {
			if(isOptionalComponentEnabled<Comp>()) {
				for (T& e : elements()) {
					e.Comp::updateReferencesAfterCompact(base, newIndices);
				}
			}
		}
		else {
			for (T& e : elements()) {
				e.Comp::updateReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

template<typename T>
template<typename Comp, typename Container, typename ElRef, typename CBase>
void ElementContainer<T>::importReferencesOnComponentFrom(
	const Container& c,
	ElRef*           base,
	const CBase*     cbase)
{
	if constexpr (comp::HasReferencesOfType<Comp, ElRef>) {
		if constexpr (comp::HasOptionalReferencesOfType<Comp, ElRef>) {
			if(isOptionalComponentEnabled<Comp>()) {
				for (uint i = 0; i < elementContainerSize(); ++i) {
					element(i).Comp::importReferencesFrom(c.element(i), base, cbase);
				}
			}
		}
		else {
			for (uint i = 0; i < elementContainerSize(); ++i) {
				element(i).Comp::importReferencesFrom(c.element(i), base, cbase);
			}
		}
	}
}

} // namespace vcl::mesh
