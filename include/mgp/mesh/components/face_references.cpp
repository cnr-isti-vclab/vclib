/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_references.h"

namespace mgp::components {

template<class Face, int N>
FaceReferences<Face, N>::FaceReferences()
{
	if constexpr (N >= 0) {
		// I'll use the array, N is >= 0.
		// There will be a static number of references.
		faceRefs = std::array<Face*, N> {nullptr};
	}
	else {
		// I'll use the vector, because N is < 0.
		// There will be a dynamic number of references.
		faceRefs = std::vector<Face*>();
	}
}

template<class Face, int N>
unsigned int FaceReferences<Face, N>::faceNumber() const
{
	if constexpr (N >= 0) {
		return N;
	}
	else {
		return faceRefs.size();
	}
}

template<class Face, int N>
unsigned int FaceReferences<Face, N>::sizeMod(unsigned int i) const
{
	return i % faceNumber();
}

template<class Face, int N>
Face*& FaceReferences<Face, N>::f(unsigned int i)
{
	assert(i < faceNumber());
	return faceRefs[i];
}

template<class Face, int N>
const Face* FaceReferences<Face, N>::f(unsigned int i) const
{
	assert(i < faceNumber());
	return faceRefs[i];
}

template<class Face, int N>
void FaceReferences<Face, N>::setFace(Face* f, unsigned int i)
{
	assert(i < faceNumber());
	faceRefs[i] = f;
}

template<class Face, int N>
void FaceReferences<Face, N>::setFaces(const std::vector<Face*>& list)
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
		faceRefs = list;
	}
}

template<class Face, int N>
template<int U>
internal::ReturnIfIsVector<U, void> FaceReferences<Face, N>::pushFace(Face* f)
{
	faceRefs.push_back(f);
}

template<class Face, int N>
template<int U>
internal::ReturnIfIsVector<U, void>
FaceReferences<Face, N>::insertFace(unsigned int i, Face* f)
{
	assert(i < faceNumber());
	faceRefs.insert(faceRefs.begin() + i, f);
}

template<class Face, int N>
template<int U>
internal::ReturnIfIsVector<U, void> FaceReferences<Face, N>::eraseFace(unsigned int i)
{
	assert(i < faceNumber());
	faceRefs.erase(faceRefs.begin() + i);
}

template<class Face, int N>
typename FaceReferences<Face, N>::FaceIterator FaceReferences<Face, N>::faceBegin()
{
	return faceRefs.begin();
}

template<class Face, int N>
typename FaceReferences<Face, N>::FaceIterator FaceReferences<Face, N>::faceEnd()
{
	return faceRefs.end();
}

template<class Face, int N>
typename FaceReferences<Face, N>::ConstFaceIterator FaceReferences<Face, N>::faceBegin() const
{
	return faceRefs.begin();
}

template<class Face, int N>
typename FaceReferences<Face, N>::ConstFaceIterator FaceReferences<Face, N>::faceEnd() const
{
	return faceRefs.end();
}

template<class Face, int N>
typename FaceReferences<Face, N>::FaceRangeIterator FaceReferences<Face, N>::faceIterator()
{
	return FaceRangeIterator(
		faceRefs, &faceBegin, &faceEnd);
}

template<class Face, int N>
typename FaceReferences<Face, N>::ConstFaceRangeIterator FaceReferences<Face, N>::faceIterator() const
{
	return ConstFaceRangeIterator(
		faceRefs, &faceBegin, &faceEnd);
}

template<class Face, int N>
void FaceReferences<Face, N>::updateFaceReferences(
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

} // namespace mgp::components
