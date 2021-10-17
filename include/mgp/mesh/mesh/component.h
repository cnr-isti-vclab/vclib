/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_COMPONENT_H
#define MGP_MESH_MESH_COMPONENT_H

#include "../vertex.h"
#include "../face.h"

namespace mgp {

namespace mesh {

template<class T>
class Component
{
};

template<class... Args>
class Component< mgp::Vertex<Args...> >
{
public:
	std::vector<mgp::Vertex<Args...>> vertices;
};

template<class... Args>
class Component< mgp::Face<Args...> >
{
public:
	std::vector<mgp::Face<Args...>> faces;
};

}

}

#endif // MGP_MESH_MESH_COMPONENT_H
