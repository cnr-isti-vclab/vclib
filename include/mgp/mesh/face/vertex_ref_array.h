/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_VERTEX_REF_ARRAY_H
#define MGP_MESH_FACE_VERTEX_REF_ARRAY_H

#include <array>
#include <assert.h>

#include <mgp/misc/type_traits.h>

namespace mgp {
namespace face {

template<class Vertex, int N>
class VertexRefsArray
{
public:
	constexpr static unsigned int N_VERTICES = N;

	VertexRefsArray() : refs({nullptr}) {}

	Vertex*& v(unsigned int i) {assert(i < N); return refs[i];}
	const Vertex* v(unsigned int i) const {assert(i < N); return refs[i];}

	void setVertex(Vertex* v, unsigned int i) {assert(i < N); refs[i] = v;};

	void setVertices(const std::initializer_list<Vertex*>& list)
	{
		// todo: static assert
		unsigned int i = 0;
		for(const auto& v : list) {
			setVertex(v, i);
			++i;
		}
	}

protected:
	std::array<Vertex*, N> refs;
};

template<class Vertex>
class TriangleVertexRefsArray : public VertexRefsArray<Vertex, 3>
{
	Vertex*& v0() {return VertexRefsArray<Vertex, 3>::refs[0];}
	Vertex*& v1() {return VertexRefsArray<Vertex, 3>::refs[1];}
	Vertex*& v2() {return VertexRefsArray<Vertex, 3>::refs[2];}
	const Vertex* v0() const {return VertexRefsArray<Vertex, 3>::refs[0];}
	const Vertex* v1() const {return VertexRefsArray<Vertex, 3>::refs[1];}
	const Vertex* v2() const {return VertexRefsArray<Vertex, 3>::refs[2];}

	void setV0(Vertex* v) {VertexRefsArray<Vertex, 3>::refs[0] = v;}
	void setV1(Vertex* v) {VertexRefsArray<Vertex, 3>::refs[1] = v;}
	void setV2(Vertex* v) {VertexRefsArray<Vertex, 3>::refs[2] = v;}
};

// Detector to check if a class has VertexRefsArray()

template<typename T>
using hasVertexRefsArray_t = decltype(std::declval<const T&>().v(std::declval<int>()));

template <typename T>
using hasVertexRefsArray = typename detector<hasVertexRefsArray_t, void, T>::type;

} // namespace face
} // namespace mgp

#endif // MGP_MESH_FACE_VERTEX_REF_ARRAY_H
