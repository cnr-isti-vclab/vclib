/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_COMPONENT_H
#define MGP_MESH_FACE_COMPONENT_H

#include <iostream>

#include "vertex_ref_array.h"

namespace mgp {

namespace face {

template<class T>
class Component : public T
{
};

template<class Vertex, int N>
class Component< face::VertexRefArray<Vertex, N> > : public face::VertexRefArray<Vertex, N>
{
};

}

}

#endif // MGP_MESH_VERTEX_COMPONENT_H
