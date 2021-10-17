/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_VERTEX_REF_ARRAY_H
#define MGP_MESH_FACE_VERTEX_REF_ARRAY_H

#include <array>
#include <assert.h>

namespace mgp {
namespace face {

template<class Vertex, int N>
class VertexRefArray
{
public:
	void setVertexReference(Vertex* v, int i) {assert(i < N); refs[i] = v;};

private:
	std::array<Vertex*, N> refs;
};

} // namespace face
} // namespace mgp

#endif // MGP_MESH_FACE_VERTEX_REF_ARRAY_H
