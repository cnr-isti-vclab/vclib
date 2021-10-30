/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_OPTIONAL_ADJACENT_FACES_REF_H
#define MGP_MESH_VERTEX_OPTIONAL_ADJACENT_FACES_REF_H

#include <type_traits>
#include <vector>

#include "common.h"
#include "../iterators/range_iterator.h"

namespace mgp::vert {

template <typename T>
class OptionalAdjacenctFacesRefTrigger
{
};

template<class Face, typename T>
class OptionalAdjacenctFacesRef : public OptionalAdjacenctFacesRefTrigger<T>, public virtual OptionalInfo<T>
{
private:
	typedef OptionalInfo<T> B;
public:
	using AdjacentFacesIterator      = typename std::vector<Face*>::iterator;
	using ConstAdjacentFacesIterator = typename std::vector<Face*>::const_iterator;
	using AdjacentFaceRangeIterator  = RangeIterator<std::vector<Face*>, AdjacentFacesIterator>;
	using ConstAdjacentFaceRangeIterator =
		ConstRangeIterator<std::vector<Face*>, ConstAdjacentFacesIterator>;

	void         addAdjacentFace(Face* f) { /*B::contPtr->*/ };
	void         clearAdjacentFaces() { /*refs.clear();*/ }
	unsigned int adjacentFacesNumber() { /*return refs.size();*/ }
	Face*&       adjacentFace(unsigned int i) { /*return refs[i];*/ }
	const Face*  adjacentFace(unsigned int i) const { /*return refs[i];*/ }

	AdjacentFacesIterator      adjacentFacesBegin() { /*return refs.begin();*/ }
	AdjacentFacesIterator      adjacentFacesEnd() { /*return refs.end();*/ }
	ConstAdjacentFacesIterator adjacentFacesBegin() const { /*return refs.begin();*/ }
	ConstAdjacentFacesIterator adjacentFacesEnd() const { /*return refs.end();*/ }

	AdjacentFaceRangeIterator adjacentFacesIterator()
	{
		//return AdjacentFaceRangeIterator(refs, &adjacentFacesBegin, &adjacentFacesEnd);
	}

	ConstAdjacentFaceRangeIterator adjacentFacesIterator() const
	{
		//return ConstAdjacentFaceRangeIterator(refs, &adjacentFacesBegin, &adjacentFacesEnd);
	}
};

}

#endif // MGP_MESH_VERTEX_OPTIONAL_ADJACENT_FACES_REF_H
