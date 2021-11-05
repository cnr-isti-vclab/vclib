/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_FACE_REFERENCES_H
#define MGP_MESH_PROPERTIES_FACE_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"

#include "element_references.h"

namespace mgp::prop {

class FaceReferencesTriggerer
{
};

template<class Face, int N>
class FaceReferences : protected ElementReferences<Face, N>, public FaceReferencesTriggerer
{
	using Base = ElementReferences<Face, N>;

public:
	static const int FACE_NUMBER = Base::PROPERTY_NUMBER;

	/** Iterator Types declaration **/

	using FaceIterator           = typename Base::ElementIterator;
	using ConstFaceIterator      = typename Base::ConstElementIterator;
	using FaceRangeIterator      = typename Base::ElementRangeIterator;
	using ConstFaceRangeIterator = typename Base::ConstElementRangeIterator;

	/** Constructor **/

	FaceReferences() : ElementReferences<Face, N>() {}

	/** Member functions **/

	unsigned int faceNumber() const { return Base::elementNumber(); }
	using Base::sizeMod;

	Face*&      f(unsigned int i) { return Base::element(i); }
	const Face* f(unsigned int i) const { return Base::element(i); }

	void setFace(Face* f, unsigned int i) { Base::setElement(f, i); }
	void setFaces(const std::vector<Face*>& list) { Base::setElements(list); }

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushFace(Face* f)
	{
		Base::pushElement(f);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertFace(unsigned int i, Face* f)
	{
		Base::insertElement(i, f);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseFace(unsigned int i)
	{
		Base::eraseElement(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearFaces()
	{
		Base::clearElements();
	}

	/** Iterator Member functions **/

	FaceIterator           faceBegin() { return Base::elementBegin(); }
	FaceIterator           faceEnd() { return Base::elementEnd(); }
	ConstFaceIterator      faceBegin() const { return Base::elementBegin(); }
	ConstFaceIterator      faceEnd() const { return Base::elementEnd(); }
	FaceRangeIterator      faceIterator() { return Base::elementIterator(); }
	ConstFaceRangeIterator faceIterator() const { return Base::elementIterator(); }

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

} // namespace mgp::prop

#endif // MGP_MESH_PROPERTIES_FACE_REFERENCES_H
