/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_VERTEX_H
#define MGP_MESH_VERTEX_H

#include "vertex/component.h"

namespace mgp {

class VertexID {
public:
	VertexID() : id(0) {}
protected:
	unsigned int id;
};

template<class... Args>
class Vertex : public VertexID, public vert::Component<Args>...
{
};

}

#endif // MGP_MESH_VERTEX_H
