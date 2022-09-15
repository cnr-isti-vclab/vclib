/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#include "optional_adjacent_faces.h"

#include <algorithm>

namespace vcl::comp {

template<typename Face, int N, typename T>
OptionalAdjacentFaces<Face, N, T>::OptionalAdjacentFaces()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		if (B::contPtr)
			B::optCont().adjFaces(thisId()) = std::array<Face*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::optCont().adjFaces(thisId()) = std::vector<Face*>();
	}
}

template<typename Face, int N, typename T>
uint OptionalAdjacentFaces<Face, N, T>::adjFacesNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::optCont().adjFaces(thisId()).size();
	}
}

template<typename Face, int N, typename T>
Face*& OptionalAdjacentFaces<Face, N, T>::adjFace(uint i)
{
	assert(i < adjFacesNumber());
	return B::optCont().adjFaces(thisId())[i];
}

template<typename Face, int N, typename T>
const Face* OptionalAdjacentFaces<Face, N, T>::adjFace(uint i) const
{
	assert(i < adjFacesNumber());
	return B::optCont().adjFaces(thisId())[i];
}

template<typename Face, int N, typename T>
Face*& OptionalAdjacentFaces<Face, N, T>::adjFaceMod(int i)
{
	int n = adjFacesNumber();
	return B::optCont().adjFaces(thisId())[(i % n + n) % n];
}

template<typename Face, int N, typename T>
const Face* OptionalAdjacentFaces<Face, N, T>::adjFaceMod(int i) const
{
	int n = adjFacesNumber();
	return B::optCont().adjFaces(thisId())[(i % n + n) % n];
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::setAdjFace(Face* f, uint i)
{
	assert(i < adjFacesNumber());
	B::optCont().adjFaces(thisId())[i] = f;
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::setAdjFaces(const std::vector<Face*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		uint i = 0;
		for (const auto& f : list) {
			setFace(f, i);
			++i;
		}
	}
	else {
		B::optCont().adjFaces(thisId()) = list;
	}
}

template<typename Face, int N, typename T>
bool OptionalAdjacentFaces<Face, N, T>::containsAdjFace(const Face* f) const
{
	return std::find(B::optCont().adjFaces.begin(), B::optCont().adjFaces.end(), f) !=
		   B::optCont().adjFaces.end();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::AdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::findAdjFace(const Face* f)
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::ConstAdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::findAdjFace(const Face* f) const
{
	return std::find(adjFaceBegin(), adjFaceEnd(), f);
}

template<typename Face, int N, typename T>
int OptionalAdjacentFaces<Face, N, T>::indexOfAdjFace(const Face* f) const
{
	auto it = findAdjFace(f);
	if (it == adjFaceEnd())
		return -1;
	else
		return it - adjFaceBegin();
}

template<typename Face, int N, typename T>
bool OptionalAdjacentFaces<Face, N, T>::isAdjFacesEnabled() const
{
	if (B::contPtr != nullptr)
		return B::optCont().isAdjacentFacesEnabled();
	else
		return false;
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::resizeAdjFaces(uint n) requires (N < 0)
{
	B::optCont().adjFaces(thisId()).resize(n);
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::pushAdjFace(Face* f) requires (N < 0)
{
	B::optCont().adjFaces(thisId()).push_back(f);
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::insertAdjFace(uint i, Face* f) requires (N < 0)
{
	assert(i < adjFacesNumber());
	B::optCont().adjFaces(thisId()).insert(B::optCont().adjFaces(thisId()).begin() + i, f);
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::eraseAdjFace(uint i) requires (N < 0)
{
	assert(i < adjFacesNumber());
	B::optCont().adjFaces(thisId()).erase(B::optCont().adjFaces(thisId()).begin() + i);
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::clearAdjFaces() requires (N < 0)
{
	B::optCont().adjFaces(thisId()).clear();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::AdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceBegin()
{
	return B::optCont().adjFaces(thisId()).begin();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::AdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceEnd()
{
	return B::optCont().adjFaces(thisId()).end();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::ConstAdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceBegin() const
{
	return B::optCont().adjFaces(thisId()).begin();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::ConstAdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceEnd() const
{
	return B::optCont().adjFaces(thisId()).end();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::AdjacentFaceRangeIterator
OptionalAdjacentFaces<Face, N, T>::adjFaces()
{
	return AdjacentFaceRangeIterator(
		*this,
		&OptionalAdjacentFaces::adjFaceBegin,
		&OptionalAdjacentFaces::adjFaceEnd);
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::ConstAdjacentFaceRangeIterator
OptionalAdjacentFaces<Face, N, T>::adjFaces() const
{
	return ConstAdjacentFaceRangeIterator(
		*this,
		&OptionalAdjacentFaces::adjFaceBegin,
		&OptionalAdjacentFaces::adjFaceEnd);
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::updateFaceReferences(
	const Face* oldBase,
	const Face* newBase)
{
	for (uint j = 0; j < adjFacesNumber(); ++j) {
		if (adjFace(j) != nullptr) {
			size_t diff = adjFace(j) - oldBase;
			adjFace(j)  = (Face*) newBase + diff;
		}
	}
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	for (uint j = 0; j < adjFacesNumber(); ++j) {
		if (adjFace(j) != nullptr) {
			size_t diff = adjFace(j) - base;
			if (newIndices[diff] < 0) { // element has been removed
				adjFace(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				adjFace(j) = (Face*) base + newIndices[diff];
			}
		}
	}
}

template<typename Face, int N, typename T>
template<typename Element>
void OptionalAdjacentFaces<Face, N, T>::importFrom(const Element&)
{
}

template<typename Face, int N, typename T>
template<typename Element, typename ElFType>
void OptionalAdjacentFaces<Face, N, T>::importFaceReferencesFrom(
	const Element& e,
	Face* base,
	const ElFType* ebase)
{
	if constexpr (HasAdjacentFaces<Element>) {
		if (isAdjFacesEnabled() && isAdjacentFacesEnabledOn(e)){
			if constexpr(N > 0) {
				// same static size
				if constexpr (N == Element::ADJ_FACE_NUMBER) {
					importReferencesFrom(e, base, ebase);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::ADJ_FACE_NUMBER < 0){
					if (e.adjFacesNumber() == N){
						importReferencesFrom(e, base, ebase);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resizeAdjFaces(e.adjFacesNumber());
				importReferencesFrom(e, base, ebase);
			}
		}
	}
}

template<typename Face, int N, typename T>
template<typename Element, typename ElFType>
void OptionalAdjacentFaces<Face, N, T>::importReferencesFrom(
	const Element& e,
	Face* base,
	const ElFType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjFacesNumber(); ++i){
			if (e.adjFace(i) != nullptr){
				adjFace(i) = base + (e.adjFace(i) - ebase);
			}
		}
	}
}

} // namespace vcl::comp
