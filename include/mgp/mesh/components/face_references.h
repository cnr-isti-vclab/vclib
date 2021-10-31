/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_FACE_REFERENCES_H
#define MGP_MESH_COMPONENTS_FACE_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"

namespace mgp::components {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

class FaceReferencesTriggerer
{
};

template<class Face, int N>
class FaceReferences : public FaceReferencesTriggerer
{
private:
	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

	// the Container type will be array or vector, depending on N value
	using Container = typename std::conditional<
		(N >= 0),
		typename std::array<Face*, ARRAY_SIZE>,
		typename std::vector<Face*>>::type;

public:
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

	using FaceRangeIterator = RangeIterator<Container, FaceIterator>;
	using ConstFaceRangeIterator = ConstRangeIterator<Container, ConstFaceIterator>;

	/** Constructor **/

	FaceReferences();

	/** Member functions **/

	unsigned int faceNumber() const;
	unsigned int sizeMod(unsigned int i) const;

	Face*&      f(unsigned int i);
	const Face* f(unsigned int i) const;

	void setFace(Face* f, unsigned int i);
	void setFaces(const std::vector<Face*>& list);

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushFace(Face* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertFace(unsigned int i, Face* f);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseFace(unsigned int i);

	/** Iterator Member functions **/

	FaceIterator faceBegin();
	FaceIterator faceEnd();
	ConstFaceIterator faceBegin() const;
	ConstFaceIterator faceEnd() const;
	FaceRangeIterator faceIterator();
	ConstFaceRangeIterator faceIterator() const;

private:
	Container faceRefs;

	void updateFaceReferences(const Face* oldBase, const Face* newBase);
};

/**
 * Detector to check if a class has (inherits) AdjacenctFaces
 */

template<typename T>
using hasFaceReferencesT = std::is_base_of<FaceReferencesTriggerer, T>;

template<typename T>
bool constexpr hasFaceReferences()
{
	return hasFaceReferencesT<T>::value;
}

} // namespace mgp::components

#include "face_references.cpp"

#endif // MGP_MESH_COMPONENTS_FACE_REFERENCES_H
