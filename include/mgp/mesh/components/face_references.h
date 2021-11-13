/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_FACE_REFERENCES_H
#define MGP_MESH_COMPONENTS_FACE_REFERENCES_H

#include "element_references.h"

namespace mgp::comp {

class FaceReferencesTriggerer
{
};

template<class Face, int N>
/**
 * @brief The FaceReferences class is a container of Face references. It could be used by any
 * Element to save adjacencies information (also Face).
 *
 * It is a random access container having static or dynamic size, depending on the value of N (
 * a negative number means dynamic).
 */
class FaceReferences : protected ElementReferences<Face, N>, public FaceReferencesTriggerer
{
	using Base = ElementReferences<Face, N>;

public:
	static const int FACE_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using FaceIterator           = typename Base::GCIterator;
	using ConstFaceIterator      = typename Base::ConstGCIterator;
	using FaceRangeIterator      = typename Base::GCRangeIterator;
	using ConstFaceRangeIterator = typename Base::ConstGCRangeIterator;

	/** Constructor **/

	FaceReferences() : Base() {}

	/** Member functions **/

	unsigned int faceNumber() const { return Base::size(); }

	Face*&      f(unsigned int i) { return Base::at(i); }
	const Face* f(unsigned int i) const { return Base::at(i); }
	Face*&      fMod(int i) { return Base::atMod(i); }
	const Face* fMod(int i) const { return Base::atMod(i); }

	void setFace(Face* f, unsigned int i) { Base::set(f, i); }
	void setFaces(const std::vector<Face*>& list) { Base::set(list); }

	using Base::contains;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeFaces(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushFace(Face* f)
	{
		Base::pushBack(f);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertFace(unsigned int i, Face* f)
	{
		Base::insert(i, f);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseFace(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearFaces()
	{
		Base::clear();
	}

	/** Iterator Member functions **/

	FaceIterator           faceBegin() { return Base::begin(); }
	FaceIterator           faceEnd() { return Base::end(); }
	ConstFaceIterator      faceBegin() const { return Base::begin(); }
	ConstFaceIterator      faceEnd() const { return Base::end(); }
	FaceRangeIterator      faceIterator() { return Base::rangeIterator(); }
	ConstFaceRangeIterator faceIterator() const { return Base::rangeIterator(); }

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
using hasFaceReferencesT = std::is_base_of<FaceReferencesTriggerer, T>;

template<typename T>
bool constexpr hasFaceReferences()
{
	return hasFaceReferencesT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_FACE_REFERENCES_H
