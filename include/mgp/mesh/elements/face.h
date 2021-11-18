/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_FACE_H
#define MGP_MESH_ELEMENTS_FACE_H

#include "face_components.h"
#include "face_components_optional.h"

namespace mgp::mesh {

// FaceContainer class declaration
template<typename>
class FaceContainer;

} // namespace mgp::mesh

namespace mgp {

// Dummy class used to detect the a mgp::Face regardless of its template arguments
class FaceTriggerer
{
};

template<typename... Args>
class Face : public FaceTriggerer, public Args...
{
	template<typename>
	friend class mesh::FaceContainer;

	// Vertex references component of the Face
	using VRefs = typename Face::VertexReferences;
	using VertexType = typename VRefs::VertexType;

	static const int NV = VRefs::VERTEX_NUMBER; // If dynamic, NV will be -1

public:
	Face() {}

	Face(const std::vector<VertexType*>& list);

	template<typename... V>
	Face(V... args);

	unsigned int id() const;

	void setVertices(const std::vector<VertexType*>& list);

	template<typename... V>
	void setVertices(V... args);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> resizeVertices(unsigned int n);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> pushVertex(VertexType* v);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, VertexType* v);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i);

	template<int U = NV>
	comp::internal::ReturnIfIsVector<U, void> clearVertices();

protected:
	unsigned int _id = 0;
};

} // namespace mgp

#include "face.cpp"

#endif // MGP_MESH_ELEMENTS_FACE_H
