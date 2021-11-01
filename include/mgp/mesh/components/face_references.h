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

#include "component_references.h"

namespace mgp::components {

class FaceReferencesTriggerer
{
};

template<class Face, int N>
class FaceReferences : protected ComponentReferences<Face, N>, public FaceReferencesTriggerer
{
	using Base = ComponentReferences<Face, N>;

public:
	static const int FACE_NUMBER = Base::COMPONENT_NUMBER;

	/** Iterator Types declaration **/

	using FaceIterator = typename Base::ComponentIterator;
	using ConstFaceIterator = typename Base::ConstComponentIterator;
	using FaceRangeIterator = typename Base::ComponentRangeIterator;
	using ConstFaceRangeIterator = typename Base::ConstComponentRangeIterator;

	/** Constructor **/

	FaceReferences() : ComponentReferences<Face, N>() {}

	/** Member functions **/

	unsigned int faceNumber() const { return Base::componentNumber(); }
	using Base::sizeMod;

	Face*&      f(unsigned int i) { return Base::c(i); }
	const Face* f(unsigned int i) const {return Base::c(i);}

	void setFace(Face* f, unsigned int i) {Base::setComponent(f, i);}
	void setFaces(const std::vector<Face*>& list) {Base::setComponents(list);}

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushFace(Face* f) { Base::pushComponent(f); }

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertFace(unsigned int i, Face* f)
	{
		Base::insertComponent(i, f);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseFace(unsigned int i)
	{
		Base::eraseComponent(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearFaces()
	{
		Base::clearComponents();
	}

	/** Iterator Member functions **/

	FaceIterator faceBegin() {return Base::componentBegin();}
	FaceIterator faceEnd() {return Base::componentEnd();}
	ConstFaceIterator faceBegin() const {return Base::componentBegin();}
	ConstFaceIterator faceEnd() const {return Base::componentEnd();}
	FaceRangeIterator faceIterator() {return Base::componentIterator();}
	ConstFaceRangeIterator faceIterator() const {return Base::componentIterator();}

protected:
	void updateFaceReferences(const Face* oldBase, const Face* newBase)
	{
		Base::updateComponentReferences(oldBase, newBase);
	}

	void updateFaceReferencesAfterCompact(const Face* base, const std::vector<int>& newIndices)
	{
		Base::updateComponentReferencesAfterCompact(base, newIndices);
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

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_FACE_REFERENCES_H
