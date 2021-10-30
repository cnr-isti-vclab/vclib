/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_OPTIONAL_ADJACENT_FACES_REF_H
#define MGP_MESH_VERTEX_OPTIONAL_ADJACENT_FACES_REF_H

#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"
#include "common.h"

namespace mgp::vert {

template<typename T>
class OptionalAdjacenctFacesRefTrigger
{
};

template<class Face, typename T>
class OptionalAdjacenctFacesRef :
		public OptionalAdjacenctFacesRefTrigger<T>,
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
using hasOptionalAdjacenctFacesT = std::is_base_of<OptionalAdjacenctFacesRefTrigger<T>, T>;

template<typename U, typename T>
using ReturnIfHasOptionalAdjacenctFaces =
	typename std::enable_if<hasOptionalAdjacenctFacesT<U>::value, T>::type;

template<typename T>
bool constexpr hasOptionalAdjacenctFaces()
{
	return hasOptionalAdjacenctFacesT<T>::value;
}

} // namespace mgp::vert

#endif // MGP_MESH_VERTEX_OPTIONAL_ADJACENT_FACES_REF_H
