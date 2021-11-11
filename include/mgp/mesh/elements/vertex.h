/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_ELEMENTS_VERTEX_H
#define MGP_MESH_ELEMENTS_VERTEX_H

#include "vertex_components.h"
#include "vertex_components_optional.h"

namespace mgp::mesh {

// Container class declaration
template<typename, typename>
class Container;

// VertexOptionalContainer class declaration
template<typename, typename>
class VertexOptionalContainer;

} // namespace mgp::mesh

namespace mgp {

// Dummy class used to detect the a mgp::Vertex regardless of its template arguments
class VertexTriggerer
{
};

template<class... Args>
class Vertex : public VertexTriggerer, public Args...
{
	template<typename, typename>
	friend class mesh::Container;

	template<typename, typename>
	friend class mesh::VertexOptionalContainer;
public:
	unsigned int id() const { return _id; }
protected:
	Vertex() {}
	unsigned int _id = 0;
};

} // namespace mgp

#endif // MGP_MESH_ELEMENTS_VERTEX_H
