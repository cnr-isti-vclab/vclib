/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H
#define MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace mgp::mesh {
template <typename, typename>
class OptionalFaceReferencesVector;
}

namespace mgp::comp {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

class OptionalFaceReferencesTriggerer
{
};

template<class Face, int N, typename T>
class OptionalFaceReferences :
		public OptionalFaceReferencesTriggerer,
		public virtual OptionalInfo<T>
{
	template <typename, typename>
	friend class OptionalFaceReferencesVector;

private:
	using B = OptionalInfo<T>;

	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

public:
	// the FaceRefsContainer type will be array or vector, depending on N value
	using FaceRefsContainer = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>,
		typename std::vector<Face*>>::type;

	static const int FACE_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using FaceIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>::iterator,
		typename std::vector<Face*>::iterator>::type;

	using ConstFaceIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Face*>::const_iterator>::type;

	using FaceRangeIterator = RangeIterator<OptionalFaceReferences, FaceIterator>;
	using ConstFaceRangeIterator = ConstRangeIterator<OptionalFaceReferences, ConstFaceIterator>;

	/** Constructor **/

	OptionalFaceReferences();

	/** Member functions **/

	unsigned int faceNumber() const;

	Face*&      f(unsigned int i);
	const Face* f(unsigned int i) const;
	Face*&      fMod(int i);
	const Face* fMod(int i) const;

	void setFace(Face* f, unsigned int i);
	void setFaces(const std::vector<Face*>& list);

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeFaces(unsigned int n);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushFace(Face* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertFace(unsigned int i, Face* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseFace(unsigned int i);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearFaces();

	/** Iterator Member functions **/

	FaceIterator faceBegin();
	FaceIterator faceEnd();
	ConstFaceIterator faceBegin() const;
	ConstFaceIterator faceEnd() const;
	FaceRangeIterator faceIterator();
	ConstFaceRangeIterator faceIterator() const;

protected:
	bool faceReferencesEnabled() const;

	void updateFaceReferences(const Face* oldBase, const Face* newBase);

	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices);

private:
	unsigned int thisId() const { return ((T*) this)->id(); }
};

/**
 * Detector to check if a class has (inherits) OptionalFaceReferences
 */

template<typename T>
using hasOptionalFaceReferencesT = std::is_base_of<OptionalFaceReferencesTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasOptionalFaceReferences=
	typename std::enable_if<hasOptionalFaceReferencesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalFaceReferences()
{
	return hasOptionalFaceReferencesT<T>::value;
}

} // namespace mgp::comp

#include "optional_face_references.cpp"

#endif // MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H
