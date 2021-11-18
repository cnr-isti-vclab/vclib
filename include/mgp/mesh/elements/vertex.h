/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_VERTEX_H
#define MGP_MESH_ELEMENTS_VERTEX_H

#include "vertex_components.h"
#include "vertex_components_optional.h"

namespace mgp::mesh {

// VertexContainr class declaration
template<typename>
class VertexContainer;

} // namespace mgp::mesh

namespace mgp {

// Dummy class used to detect the a mgp::Vertex regardless of its template arguments
class VertexTriggerer
{
};

template<typename... Args>
class Vertex : public VertexTriggerer, public Args...
{
	template<typename>
	friend class mesh::VertexContainer;

public:
	unsigned int id() const { return _id; }
protected:
	Vertex() {}
	unsigned int _id = 0;
};

} // namespace mgp

#endif // MGP_MESH_ELEMENTS_VERTEX_H
