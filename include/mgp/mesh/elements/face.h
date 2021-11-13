/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_FACE_H
#define MGP_MESH_ELEMENTS_FACE_H

#include "face_components.h"
#include "face_components_optional.h"

namespace mgp::mesh {

// Container class declaration
template<typename, typename>
class Container;

// FaceOptionalContainer class declaration
template<typename, typename>
class FaceOptionalContainer;

} // namespace mgp::mesh

namespace mgp {

// Dummy class used to detect the a mgp::Face regardless of its template arguments
class FaceTriggerer
{
};

template<class... Args>
class Face : public FaceTriggerer, public Args...
{
	template<typename, typename>
	friend class mesh::Container;

	template<typename, typename>
	friend class mesh::FaceOptionalContainer;

	// Vertex references component of the Face
	using VRefs = typename Face::VertexReferences;
	static const int NV = VRefs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	unsigned int id() const;

	template<typename Vertex>
	void setVertices(const std::vector<Vertex*>& list);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> resizeVertices(unsigned int n);

	template<typename Vertex, int U = NV>
	comp::internal::ReturnIfIsVector<U, void> pushVertex(Vertex* v);

	template<typename Vertex, int U = NV>
	comp::internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, Vertex* v);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> clearVertices();

protected:
	Face() {}
	unsigned int _id = 0;
};

} // namespace mgp

#include "face.cpp"

#endif // MGP_MESH_ELEMENTS_FACE_H
