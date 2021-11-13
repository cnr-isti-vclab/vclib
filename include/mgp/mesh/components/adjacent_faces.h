/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_ADJACENT_FACES_H
#define MGP_MESH_COMPONENTS_ADJACENT_FACES_H

#include "element_references.h"

namespace mgp::comp {

class AdjacentFacesTriggerer
{
};

template<class Face, int N>
/**
 * @brief The AdjacentFaces class is a container of Face references. It could be used by any
 * Element to save adjacencies information (also the Face element itself).
 *
 * It is a random access container having static or dynamic size, depending on the value of N (a
 * negative number means dynamic).
 */
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
	AdjacentFaceRangeIterator      adjFaceIterator() { return Base::rangeIterator(); }
	ConstAdjacentFaceRangeIterator adjFaceIterator() const { return Base::rangeIterator(); }

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

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_ADJACENT_FACES_H
