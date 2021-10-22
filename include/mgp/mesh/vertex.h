/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_H
#define MGP_MESH_VERTEX_H

#include "vertex/common.h"
#include "vertex/coordinate.h"
#include "vertex/face_adjacency_ref.h"

namespace mgp {

namespace mesh {

// Container class declaration
template <typename, typename>
class Container;

}

namespace vert {

/**
 * @brief The Component fallback class, will be used when the template argument given to the Vertex
 * is not one of the known Components.
 *
 * This class will just make the given template argument a Base of the Vertex, becoming a feature of
 * the Vertex.
 */
template<class T>
class Component : public T
{
};

}

// Dummy class used to detect the a mgp::Vertex regardless of its template arguments
class VertexTriggerer {
};

template<class... Args>
class Vertex : public VertexTriggerer, public vert::Component<Args>...
{
	template<typename, typename> friend class mesh::Container;
protected:
	Vertex() {}
};

}

#endif // MGP_MESH_VERTEX_H
