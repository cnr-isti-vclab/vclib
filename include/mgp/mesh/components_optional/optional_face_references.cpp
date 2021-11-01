/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "optional_face_references.h"

namespace mgp::components {

template<class Face, int N, typename T>
OptionalFaceReferences<Face, N, T>::OptionalFaceReferences()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		if (B::contPtr)
			B::contPtr->faceRefs(thisId()) = std::array<Face*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		if (B::contPtr)
			B::contPtr->faceRefs(thisId()) = std::vector<Face*>();
	}
}

template<class Face, int N, typename T>
unsigned int OptionalFaceReferences<Face, N, T>::faceNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return B::contPtr->faceRefs(thisId()).size();
	}
}

template<class Face, int N, typename T>
unsigned int OptionalFaceReferences<Face, N, T>::sizeMod(unsigned int i) const
{
	return i % faceNumber();
}

template<class Face, int N, typename T>
Face*& OptionalFaceReferences<Face, N, T>::f(unsigned int i)
{
	assert(i < faceNumber());
	return B::contPtr->faceRefs(thisId())[i];
}

template<class Face, int N, typename T>
const Face* OptionalFaceReferences<Face, N, T>::f(unsigned int i) const
{
	assert(i < faceNumber());
	return B::contPtr->faceRefs(thisId())[i];
}

template<class Face, int N, typename T>
void OptionalFaceReferences<Face, N, T>::setFace(Face* f, unsigned int i)
{
	assert(i < faceNumber());
	B::contPtr->faceRefs(thisId())[i] = f;
}

template<class Face, int N, typename T>
void OptionalFaceReferences<Face, N, T>::setFaces(const std::vector<Face*>& list)
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
		B::contPtr->faceRefs(thisId()) = list;
	}
}

template<class Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalFaceReferences<Face, N, T>::pushFace(Face* f)
{
	B::contPtr->faceRefs(thisId()).push_back(f);
}

template<class Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void>
OptionalFaceReferences<Face, N, T>::insertFace(unsigned int i, Face* f)
{
	assert(i < faceNumber());
	B::contPtr->faceRefs(thisId()).insert(B::contPtr->faceRefs(thisId()).begin() + i, f);
}

template<class Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalFaceReferences<Face, N, T>::eraseFace(unsigned int i)
{
	assert(i < faceNumber());
	B::contPtr->faceRefs(thisId()).erase(B::contPtr->faceRefs(thisId()).begin() + i);
}

template<class Face, int N, typename T>
template<int U>
internal::ReturnIfIsVector<U, void> OptionalFaceReferences<Face, N, T>::clearFaces()
{
	B::contPtr->faceRefs(thisId()).clear();
}

template<class Face, int N, typename T>
typename OptionalFaceReferences<Face, N, T>::FaceIterator
OptionalFaceReferences<Face, N, T>::faceBegin()
{
	return B::contPtr->faceRefs(thisId()).begin();
}

template<class Face, int N, typename T>
typename OptionalFaceReferences<Face, N, T>::FaceIterator
OptionalFaceReferences<Face, N, T>::faceEnd()
{
	return B::contPtr->faceRefs(thisId()).end();
}

template<class Face, int N, typename T>
typename OptionalFaceReferences<Face, N, T>::ConstFaceIterator
OptionalFaceReferences<Face, N, T>::faceBegin() const
{
	return B::contPtr->faceRefs(thisId()).begin();
}

template<class Face, int N, typename T>
typename OptionalFaceReferences<Face, N, T>::ConstFaceIterator
OptionalFaceReferences<Face, N, T>::faceEnd() const
{
	return B::contPtr->faceRefs(thisId()).end();
}

template<class Face, int N, typename T>
typename OptionalFaceReferences<Face, N, T>::FaceRangeIterator
OptionalFaceReferences<Face, N, T>::faceIterator()
{
	return FaceRangeIterator(
		B::contPtr->faceRefs(thisId()),
		&OptionalFaceReferences::faceBegin,
		&OptionalFaceReferences::faceEnd);
}

template<class Face, int N, typename T>
typename OptionalFaceReferences<Face, N, T>::ConstFaceRangeIterator
OptionalFaceReferences<Face, N, T>::faceIterator() const
{
	return ConstFaceRangeIterator(
		B::contPtr->faceRefs(thisId()),
		&OptionalFaceReferences::faceBegin,
		&OptionalFaceReferences::faceEnd);
}

template<class Face, int N, typename T>
void OptionalFaceReferences<Face, N, T>::updateFaceReferences(
	const Face* oldBase,
	const Face* newBase)
{
	for (unsigned int j = 0; j < faceNumber(); ++j) {
		if (f(j) != nullptr) {
			size_t diff = f(j) - oldBase;
			f(j)        = (Face*) newBase + diff;
		}
	}
}

template<class Face, int N, typename T>
void OptionalFaceReferences<Face, N, T>::updateFaceReferencesAfterCompact(
	const Face* base,
	const std::vector<int>& newIndices)
{
	for (unsigned int j = 0; j < faceNumber(); ++j) {
		if (f(j) != nullptr) {
			size_t diff = f(j) - base;
			if (newIndices[diff] < 0){ // element has been removed
				f(j) = nullptr;
			}
			else { // the new pointer will be base + newIndices[diff]
				f(j) = base + newIndices[diff];
			}
		}
	}
}

} // namespace mgp::components
