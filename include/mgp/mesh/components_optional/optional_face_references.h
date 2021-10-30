/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H
#define MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H

#include <type_traits>
#include <vector>

#include "optional_info.h"

#include "../iterators/range_iterator.h"

namespace mgp::components {

template<typename T>
class OptionalAdjacentFacesRefTrigger
{
};

template<class Face, typename T>
class OptionalAdjacentFacesRef :
		public OptionalAdjacentFacesRefTrigger<T>,
		public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;

public:
	using FaceType                   = Face;
	using AdjacentFacesIterator      = typename std::vector<Face*>::iterator;
	using ConstAdjacentFacesIterator = typename std::vector<Face*>::const_iterator;
	using AdjacentFaceRangeIterator  = RangeIterator<std::vector<Face*>, AdjacentFacesIterator>;
	using ConstAdjacentFaceRangeIterator =
		ConstRangeIterator<std::vector<Face*>, ConstAdjacentFacesIterator>;

	void         addAdjacentFace(Face* f) { B::contPtr->adjFacesRefs(thisId()).push_back(f); };
	void         clearAdjacentFaces() { B::contPtr->adjFacesRefs(thisId()).clear(); }
	unsigned int adjacentFacesNumber() { return B::contPtr->adjFacesRefs(thisId()).size(); }
	Face*&       adjacentFace(unsigned int i) { return B::contPtr->adjFacesRefs(thisId())[i]; }
	const Face* adjacentFace(unsigned int i) const { return B::contPtr->adjFacesRefs(thisId())[i]; }

	AdjacentFacesIterator adjacentFacesBegin()
	{
		return B::contPtr->adjFacesRefs(thisId()).begin();
	}
	AdjacentFacesIterator adjacentFacesEnd() { return B::contPtr->adjFacesRefs(thisId()).end(); }
	ConstAdjacentFacesIterator adjacentFacesBegin() const
	{
		return B::contPtr->adjFacesRefs(thisId()).begin();
	}
	ConstAdjacentFacesIterator adjacentFacesEnd() const
	{
		return B::contPtr->adjFacesRefs(thisId()).end();
	}

	AdjacentFaceRangeIterator adjacentFacesIterator()
	{
		return AdjacentFaceRangeIterator(
			B::contPtr->adjFacesRefs(thisId()), &adjacentFacesBegin, &adjacentFacesEnd);
	}

	ConstAdjacentFaceRangeIterator adjacentFacesIterator() const
	{
		return ConstAdjacentFaceRangeIterator(
			B::contPtr->adjFacesRefs(thisId()), &adjacentFacesBegin, &adjacentFacesEnd);
	}

private:
	unsigned int thisId() const { return ((T*) this)->id(); }
};

/**
 * Detector to check if a class has (inherits) OptionalAdjacenctFacesRef
 */

template<typename T>
using hasOptionalAdjacentFacesT = std::is_base_of<OptionalAdjacentFacesRefTrigger<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalAdjacentFaces =
	typename std::enable_if<hasOptionalAdjacentFacesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalAdjacentFaces()
{
	return hasOptionalAdjacentFacesT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_OPTIONAL_FACE_REFERENCES_H
