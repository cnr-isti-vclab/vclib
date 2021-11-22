/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_ADJACENT_FACES_H

#include "element_references.h"

namespace vcl::comp {

class AdjacentFacesTriggerer
{
};

/**
 * @brief The AdjacentFaces class is a container of Face references. It could be used by any
 * Element to save adjacencies information (also the Face element itself).
 *
 * It is a random access container having static or dynamic size, depending on the value of N (a
 * negative number means dynamic).
 */
template<typename Face, int N>
class AdjacentFaces : protected ElementReferences<Face, N>, public AdjacentFacesTriggerer
{
	using Base = ElementReferences<Face, N>;

public:
	static const int ADJ_FACE_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using AdjacentFaceIterator           = typename Base::GCIterator;
	using ConstAdjacentFaceIterator      = typename Base::ConstGCIterator;
	using AdjacentFaceRangeIterator      = typename Base::GCRangeIterator;
	using ConstAdjacentFaceRangeIterator = typename Base::ConstGCRangeIterator;

	/** Constructor **/

	AdjacentFaces() : Base() {}

	/** Member functions **/

	unsigned int adjFacesNumber() const { return Base::size(); }

	Face*&      adjFace(unsigned int i) { return Base::at(i); }
	const Face* adjFace(unsigned int i) const { return Base::at(i); }
	Face*&      adjFaceMod(int i) { return Base::atMod(i); }
	const Face* adjFaceMod(int i) const { return Base::atMod(i); }

	void setAdjFace(Face* f, unsigned int i) { Base::set(f, i); }
	void setAdjFaces(const std::vector<Face*>& list) { Base::set(list); }

	bool containsAdjFace(const Face* f) const { return Base::contains(f); }

	AdjacentFaceIterator findAdjFace(const Face* f) { return Base::find(f); }
	ConstAdjacentFaceIterator findAdjFace(const Face* f) const { return Base::find(f); }

	int indexOfAdjFace(const Face* f) const { return Base::indexOf(f); }

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeAdjFaces(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushAdjFace(Face* f)
	{
		Base::pushBack(f);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertAdjFace(unsigned int i, Face* f)
	{
		Base::insert(i, f);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseAdjFace(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearAdjFaces()
	{
		Base::clear();
	}

	/** Iterator Member functions **/

	AdjacentFaceIterator           adjFaceBegin() { return Base::begin(); }
	AdjacentFaceIterator           adjFaceEnd() { return Base::end(); }
	ConstAdjacentFaceIterator      adjFaceBegin() const { return Base::begin(); }
	ConstAdjacentFaceIterator      adjFaceEnd() const { return Base::end(); }
	AdjacentFaceRangeIterator      adjFaces() { return Base::rangeIterator(); }
	ConstAdjacentFaceRangeIterator adjFaces() const { return Base::rangeIterator(); }

protected:
	void updateFaceReferences(const Face* oldBase, const Face* newBase)
	{
		Base::updateElementReferences(oldBase, newBase);
	}

	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices)
	{
		Base::updateElementReferencesAfterCompact(base, newIndices);
	}
};

/**
 * Detector to check if a class has (inherits) AdjacenctFaces
 */

template<typename T>
using hasAdjacentFacesT = std::is_base_of<AdjacentFacesTriggerer, T>;

template<typename T>
bool constexpr hasAdjacentFaces()
{
	return hasAdjacentFacesT<T>::value;
}

template <typename T>
bool constexpr sanityCheckAdjacentFaces()
{
	if constexpr (hasAdjacentFaces<T>()) {
		return T::VERTEX_NUMBER == T::ADJ_FACE_NUMBER;
	}
	else {
		return true;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_FACES_H
