/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_FACE_ADJ_REF_H
#define MGP_MESH_VERTEX_FACE_ADJ_REF_H

#include <vector>

#include "common.h"

namespace mgp::vert {

template<class Face>
class AdjacenctFacesRef
{
public:
	using AdjacentFacesIterator      = typename std::vector<Face*>::iterator;
	using ConstAdjacentFacesIterator = typename std::vector<Face*>::const_iterator;

	void         addAdjacentFace(Face* f) { refs.push_back(f); };
	void         clearAdjacentFaces() { refs.clear(); }
	unsigned int adjacentFacesNumber() { return refs.size(); }
	Face*&       adjacentFace(unsigned int i) { return refs[i]; }
	const Face*  adjacentFace(unsigned int i) const { return refs[i]; }

	AdjacentFacesIterator      adjacentFacesBegin() { return refs.begin(); }
	AdjacentFacesIterator      adjacentFacesEnd() { return refs.end(); }
	ConstAdjacentFacesIterator adjacentFacesBegin() const { return refs.begin(); }
	ConstAdjacentFacesIterator adjacentFacesEnd() const { return refs.end(); }

private:
	std::vector<Face*> refs;
};

} // namespace mgp::vert

#endif // MGP_MESH_VERTEX_FACE_ADJ_REF_H
