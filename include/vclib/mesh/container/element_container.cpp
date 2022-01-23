/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include "../components/detection/vertex_references_detection.h"

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
 * @param[in] f: the pointer of the element that will be marked as deleted.
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
	if constexpr (comp::hasVerticalInfo<T>()) {
		optionalVec.clear();
	}
}

template<typename T>
uint ElementContainer<T>::addElement()
{
	T* oldB = vec.data();
	vec.push_back(T());
	T* newB = vec.data();
	en++;
	if constexpr (comp::hasVerticalInfo<T>()) {
		setContainerPointer(vec[vec.size() - 1]);
		optionalVec.resize(vec.size());
	}
	updateContainerPointers(oldB, newB);
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
uint ElementContainer<T>::addElements(uint size)
{
	uint baseId = vec.size();
	T*   oldB   = vec.data();
	vec.resize(vec.size() + size);
	T* newB = vec.data();
	en += size;
	if constexpr (comp::hasVerticalInfo<T>()) {
		optionalVec.resize(vec.size());
		for (uint i = baseId; i < vec.size(); ++i) {
			setContainerPointer(vec[i]);
		}
	}
	updateContainerPointers(oldB, newB);
	return baseId;
}

template<typename T>
void ElementContainer<T>::reserveElements(uint size)
{
	T* oldB = vec.data();
	vec.reserve(size);
	T* newB = vec.data();
	if constexpr (comp::hasVerticalInfo<T>()) {
		optionalVec.reserve(size);
	}
	updateContainerPointers(oldB, newB);
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
		if constexpr (comp::hasVerticalInfo<T>()) {
			optionalVec.compact(newIndices);
		}
	}
	return newIndices;
}

/**
 * @brief Sets this cointainer pointer to the element. Necessary to give to the element access to
 * the vertical components. This operation needs to be done after element creation in the container
 * and after every reallocation of the elements.
 * @param element
 */
template<typename T>
void ElementContainer<T>::setContainerPointer(T &element)
{
	if constexpr (comp::hasVerticalInfo<T>()) {
		element.setContainerPointer(this);
	}
}

/**
 * @brief After a reallocation, it is needed always to update the container pointers of all the
 * elements, because the assignment operator of the VerticalInfo component (which stores the pointer
 * of the container) does not copy the container pointer for security reasons.
 */
template<typename T>
void ElementContainer<T>::updateContainerPointers(const T *oldBase, const T *newBase)
{
	if constexpr (comp::hasVerticalInfo<T>()) {
		if (oldBase != newBase) {
			// all the faces must point to the right container - also the deleted ones
			for (T& f : elements(false)) {
				setContainerPointer(f);
			}
		}
	}
}

template<typename T>
template<typename Vertex>
void ElementContainer<T>::updateVertexReferences(const Vertex *oldBase, const Vertex *newBase)
{
	if constexpr(comp::hasVertexReferences<T>()) {
		for (T& e : elements()) {
			e.updateVertexReferences(oldBase, newBase);
		}
	}
	if constexpr (comp::hasOptionalAdjacentVertices<T>()) {
		if (optionalVec.isAdjacentVerticesEnabled()) {
			for (T& v : elements()) {
				v.updateVertexReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (comp::hasAdjacentVertices<T>()) {
		for (T& v : elements()) {
			v.updateVertexReferences(oldBase, newBase);
		}
	}
}

template<typename T>
template<typename Vertex>
void ElementContainer<T>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	if constexpr(comp::hasVertexReferences<T>()) {
		for (T& e: elements()) {
			e.updateVertexReferencesAfterCompact(base, newIndices);
		}
	}
	if constexpr (comp::hasOptionalAdjacentVertices<T>()) {
		if (optionalVec.isAdjacentVerticesEnabled()) {
			for (T& v : elements()) {
				v.updateVertexReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (comp::hasAdjacentVertices<T>()) {
		for (T& v : elements()) {
			v.updateVertexReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Face>
void ElementContainer<T>::updateFaceReferences(const Face *oldBase, const Face *newBase)
{
	if constexpr (comp::hasOptionalAdjacentFaces<T>()) {
		if (optionalVec.isAdjacentFacesEnabled()) {
			for (T& e : elements()) {
				e.updateFaceReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (comp::hasAdjacentFaces<T>()) {
		for (T& e : elements()) {
			e.updateFaceReferences(oldBase, newBase);
		}
	}
}

template<typename T>
template<typename Face>
void ElementContainer<T>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	if constexpr (comp::hasOptionalAdjacentFaces<T>()) {
		if (optionalVec.isAdjacentFacesEnabled()) {
			for (T& e : elements()) {
				e.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (comp::hasAdjacentFaces<T>()) {
		for (T& e : elements()) {
			e.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Edge>
void ElementContainer<T>::updateEdgeReferences(const Edge *oldBase, const Edge *newBase)
{
	if constexpr (comp::hasOptionalAdjacentEdges<T>()) {
		if (optionalVec.isAdjacentEdgesEnabled()) {
			for (T& e : elements()) {
				e.updateEdgeReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (comp::hasAdjacentEdges<T>()) {
		for (T& e : elements()) {
			e.updateEdgeReferences(oldBase, newBase);
		}
	}
}

template<typename T>
template<typename Edge>
void ElementContainer<T>::updateEdgeReferencesAfterCompact(
	const Edge*             base,
	const std::vector<int>& newIndices)
{
	if constexpr (comp::hasOptionalAdjacentEdges<T>()) {
		if (optionalVec.isAdjacentEdgesEnabled()) {
			for (T& e : elements()) {
				e.updateEdgeReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (comp::hasAdjacentEdges<T>()) {
		for (T& e : elements()) {
			e.updateEdgeReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Container>
void ElementContainer<T>::enableOptionalComponentsOf(const Container &c)
{
	// unfortunately, this function cannot be shortened in a smart way
	using CT = typename Container::ElementType;

	const uint size = elementContainerSize();

	// Adjacent Edges

	// if this Element of this container has optional adjacent edges
	if constexpr (comp::hasOptionalAdjacentEdges<T>()) {
		// if also the other Container Element type optional adjacent edges
		if constexpr (comp::hasOptionalAdjacentEdges<CT>()) {
			// if they are enabled on the other Mesh, enable also here
			if (c.optionalVec.isAdjacentEdgesEnabled()) {
				optionalVec.enableAdjacentEdges(size);
			}
		}
		else {
			// if the other Container has *non-optional* adjacent edges, I need to enable it on this
			// container
			if constexpr (comp::hasAdjacentEdges<CT>()) {
				optionalVec.enableAdjacentEdges(size);
			}
		}
	}
	// Adjacent Faces
	if constexpr (comp::hasOptionalAdjacentFaces<T>()) {
		if constexpr (comp::hasOptionalAdjacentFaces<CT>()) {
			if (c.optionalVec.isAdjacentFacesEnabled()) {
				optionalVec.enableAdjacentFaces(size);
			}
		}
		else {
			if constexpr (comp::hasAdjacentFaces<CT>()) {
				optionalVec.enableAdjacentFaces(size);
			}
		}
	}
	// Adjacent Vertices
	if constexpr (comp::hasOptionalAdjacentVertices<T>()) {
		if constexpr (comp::hasOptionalAdjacentVertices<CT>()) {
			if (c.optionalVec.isAdjacentVerticesEnabled()) {
				optionalVec.enableAdjacentVertices(size);
			}
		}
		else {
			if constexpr (comp::hasAdjacentVertices<CT>()) {
				optionalVec.enableAdjacentVertices(size);
			}
		}
	}
	// Color
	if constexpr (comp::hasOptionalColor<T>()) {
		if constexpr (comp::hasOptionalColor<CT>()) {
			if (c.optionalVec.isColorEnabled()) {
				optionalVec.enableColor(size);
			}
		}
		else {
			if constexpr (comp::hasColor<CT>()) {
				optionalVec.enableColor(size);
			}
		}
	}
	// Mark
	if constexpr (comp::hasOptionalMark<T>()) {
		if constexpr (comp::hasOptionalMark<CT>()) {
			if (c.optionalVec.isMarkEnabled()) {
				optionalVec.enableMark(size);
			}
		}
		else {
			if constexpr (comp::hasMark<CT>()) {
				optionalVec.enableMark(size);
			}
		}
	}
	// Normal
	if constexpr (comp::hasOptionalNormal<T>()) {
		if constexpr (comp::hasOptionalNormal<CT>()) {
			if (c.optionalVec.isNormalEnabled()) {
				optionalVec.enableNormal(size);
			}
		}
		else {
			if constexpr (comp::hasNormal<CT>()) {
				optionalVec.enableNormal(size);
			}
		}
	}
	// Principal Curvature
	if constexpr (comp::hasOptionalPrincipalCurvature<T>()) {
		if constexpr (comp::hasOptionalPrincipalCurvature<CT>()) {
			if (c.optionalVec.isPrincipalCurvatureEnabled()) {
				optionalVec.enablePrincipalCurvature(size);
			}
		}
		else {
			if constexpr (comp::hasPrincipalCurvature<CT>()) {
				optionalVec.enablePrincipalCurvature(size);
			}
		}
	}
	// Scalar
	if constexpr (comp::hasOptionalScalar<T>()) {
		if constexpr (comp::hasOptionalScalar<CT>()) {
			if (c.optionalVec.isScalarEnabled()) {
				optionalVec.enableScalar(size);
			}
		}
		else {
			if constexpr (comp::hasScalar<CT>()) {
				optionalVec.enableScalar(size);
			}
		}
	}
	// TexCoord
	if constexpr (comp::hasOptionalTexCoord<T>()) {
		if constexpr (comp::hasOptionalTexCoord<CT>()) {
			if (c.optionalVec.isTexCoordEnabled()) {
				optionalVec.enableTexCoord(size);
			}
		}
		else {
			if constexpr (comp::hasTexCoord<CT>()) {
				optionalVec.enableTexCoord(size);
			}
		}
	}
	// Wedge Colors
	if constexpr (comp::hasOptionalWedgeColors<T>()) {
		if constexpr (comp::hasOptionalWedgeColors<CT>()) {
			if (c.optionalVec.isWedgeColorsEnabled()) {
				optionalVec.enableWedgeColors(size);
			}
		}
		else {
			if constexpr (comp::hasWedgeColors<CT>()) {
				optionalVec.enableWedgeColors(size);
			}
		}
	}
	// Wedge TexCoords
	if constexpr (comp::hasOptionalWedgeTexCoords<T>()) {
		if constexpr (comp::hasOptionalWedgeTexCoords<CT>()) {
			if (c.optionalVec.isWedgeTexCoordsEnabled()) {
				optionalVec.enableWedgeTexCoords(size);
			}
		}
		else {
			if constexpr (comp::hasWedgeTexCoords<CT>()) {
				optionalVec.enableWedgeTexCoords(size);
			}
		}
	}
}

template<typename T>
template<typename Container>
void ElementContainer<T>::importFrom(const Container &c)
{
	clearElements();
	addElements(c.elementContainerSize());
	unsigned int eid = 0;
	for (const typename Container::ElementType& e : c.elements(false)) {
		element(eid).importFrom(e);
		++eid;
	}
}

template<typename T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importVertexReferencesFrom(const Container& c, MyBase* base, const CBase* cbase)
{

	for (uint i = 0; i < elementContainerSize(); ++i) {
		element(i).importVertexReferencesFrom(c.element(i), base, cbase);
	}
}

template<typename T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importFaceReferencesFrom(const Container& c, MyBase* base, const CBase* cbase)
{
	for (uint i = 0; i < elementContainerSize(); ++i) {
		element(i).importFaceReferencesFrom(c.element(i), base, cbase);
	}
}

template<typename T>
template<typename Container, typename MyBase, typename CBase>
void ElementContainer<T>::importEdgeReferencesFrom(const Container& c, MyBase* base, const CBase* cbase)
{
	for (uint i = 0; i < elementContainerSize(); ++i) {
		element(i).importEdgeReferencesFrom(c.element(i), base, cbase);
	}
}

} // namespace vcl::mesh
