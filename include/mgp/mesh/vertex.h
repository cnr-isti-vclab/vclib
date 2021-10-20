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

template <typename, typename>
class Container;

}

namespace vert {

template<class T>
class Component : public T
{
};

}

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
