/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_adjacent_faces.h"

#include <algorithm>

namespace mgp::comp {

template<typename Face, int N, typename T>
OptionalAdjacentFaces<Face, N, T>::OptionalAdjacentFaces()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		if (B::contPtr)
			B::contPtr->adjFaces(thisId()) = std::array<Face*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::contPtr->adjFaces(thisId()) = std::vector<Face*>();
	}
}

template<typename Face, int N, typename T>
unsigned int OptionalAdjacentFaces<Face, N, T>::adjFacesNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::contPtr->adjFaces(thisId()).size();
	}
}

template<typename Face, int N, typename T>
Face*& OptionalAdjacentFaces<Face, N, T>::adjFace(unsigned int i)
{
	assert(i < adjFacesNumber());
	return B::contPtr->adjFaces(thisId())[i];
}

template<typename Face, int N, typename T>
const Face* OptionalAdjacentFaces<Face, N, T>::adjFace(unsigned int i) const
{
	assert(i < adjFacesNumber());
	return B::contPtr->adjFaces(thisId())[i];
}

template<typename Face, int N, typename T>
Face*& OptionalAdjacentFaces<Face, N, T>::adjFaceMod(int i)
{
	unsigned int n = adjFacesNumber();
	return B::contPtr->adjFaces(thisId())[(i % n + n) % n];
}

template<typename Face, int N, typename T>
const Face* OptionalAdjacentFaces<Face, N, T>::adjFaceMod(int i) const
{
	unsigned int n = adjFacesNumber();
	return B::contPtr->adjFaces(thisId())[(i % n + n) % n];
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::setAdjFace(Face* f, unsigned int i)
{
	assert(i < adjFacesNumber());
	B::contPtr->adjFaces(thisId())[i] = f;
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::setAdjFaces(const std::vector<Face*>& list)
{
	if constexpr (N >= 0) {
		assert(list.size() == N);
		unsigned int i = 0;
		for (const auto& f : list) {
			setFace(f, i);
			++i;
		}
	}
	else {
		B::contPtr->adjFaces(thisId()) = list;
	}
}

template<typename Face, int N, typename T>
bool OptionalAdjacentFaces<Face, N, T>::containsAdjFace(const Face* f) const
{
	return std::find(B::contPtr->adjFaces.begin(), B::contPtr->adjFaces.end(), f) !=
		   B::contPtr->adjFaces.end();
}

template<typename Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentFaces<Face, N, T>::resizeAdjFaces(unsigned int n)
{
	B::contPtr->adjFaces(thisId()).resize(n);
}

template<typename Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalAdjacentFaces<Face, N, T>::pushAdjFace(Face* f)
{
	B::contPtr->adjFaces(thisId()).push_back(f);
}

template<typename Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalAdjacentFaces<Face, N, T>::insertAdjFace(unsigned int i, Face* f)
{
	assert(i < adjFacesNumber());
	B::contPtr->adjFaces(thisId()).insert(B::contPtr->adjFaces(thisId()).begin() + i, f);
}

template<typename Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalAdjacentFaces<Face, N, T>::eraseAdjFace(unsigned int i)
{
	assert(i < adjFacesNumber());
	B::contPtr->adjFaces(thisId()).erase(B::contPtr->adjFaces(thisId()).begin() + i);
}

template<typename Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalAdjacentFaces<Face, N, T>::clearAdjFaces()
{
	B::contPtr->adjFaces(thisId()).clear();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::AdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceBegin()
{
	return B::contPtr->adjFaces(thisId()).begin();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::AdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceEnd()
{
	return B::contPtr->adjFaces(thisId()).end();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::ConstAdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceBegin() const
{
	return B::contPtr->adjFaces(thisId()).begin();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::ConstAdjacentFaceIterator
OptionalAdjacentFaces<Face, N, T>::adjFaceEnd() const
{
	return B::contPtr->adjFaces(thisId()).end();
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::AdjacentFaceRangeIterator
OptionalAdjacentFaces<Face, N, T>::adjFaces()
{
	return FaceRangeIterator(
		B::contPtr->adjFaces(thisId()),
		&OptionalAdjacentFaces::faceBegin,
		&OptionalAdjacentFaces::faceEnd);
}

template<typename Face, int N, typename T>
typename OptionalAdjacentFaces<Face, N, T>::ConstAdjacentFaceRangeIterator
OptionalAdjacentFaces<Face, N, T>::adjFaces() const
{
	return ConstFaceRangeIterator(
		B::contPtr->adjFaces(thisId()),
		&OptionalAdjacentFaces::faceBegin,
		&OptionalAdjacentFaces::faceEnd);
}

template<typename Face, int N, typename T>
bool OptionalAdjacentFaces<Face, N, T>::adjFacesEnabled() const
{
	return B::contPtr->isAdjacentFacesEnabled();
}

template<typename Face, int N, typename T>
void OptionalAdjacentFaces<Face, N, T>::updateFaceReferences(
	const Face* oldBase,
	const Face* newBase)
{
	for (unsigned int j = 0; j < adjFacesNumber(); ++j) {
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
	for (unsigned int j = 0; j < adjFacesNumber(); ++j) {
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

} // namespace mgp::comp
