/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_H
#define MGP_MESH_VERTEX_H

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

namespace mgp::vert {

/**
 * @brief The Property fallback class, will be used when the template argument given to the Vertex
 * is not one of the known Components.
 *
 * This class will just make the given template argument a Base of the Vertex, becoming a feature of
 * the Vertex.
 */
template<class T>
class Property : public T
{
};

} // namespace mgp::vert

namespace mgp {

// Dummy class used to detect the a mgp::Vertex regardless of its template arguments
class VertexTriggerer
{
};

template<class... Args>
class Vertex : public VertexTriggerer, public vert::Property<Args>...
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

#endif // MGP_MESH_VERTEX_H
