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

} // namespace vcl::mesh
