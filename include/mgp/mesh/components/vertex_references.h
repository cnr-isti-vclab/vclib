/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include <array>
#include <assert.h>
#include <variant>
#include <vector>

namespace mgp::components {

class VertexRefereferencesTriggerer {};

template<class Vertex, int N>
class VertexReferences : public VertexRefereferencesTriggerer
{
public:
	static const int FACE_SIZE = N > 0 ? N : 0;
	VertexReferences();

	unsigned int vertexNumber() const {return N;}
	unsigned int sizeMod(unsigned int i) const {return i%N;}

	Vertex*&      v(unsigned int i);
	const Vertex* v(unsigned int i) const;

	void setVertex(Vertex* v, unsigned int i);
	void setVertices(const std::vector<Vertex*> &list);

protected:
	std::variant<std::array<Vertex*, FACE_SIZE>, std::vector<Vertex*>> refs;

	// id 0 if use the array, 1 if we use the vector
	static const int VARIANT_ID = N > 0 ? 0 : 1;

	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
};

template<class Vertex>
class TriangleVertexRefs : public VertexReferences<Vertex, 3>
{
private:
	using B = VertexReferences<Vertex, 3>;
public:
	Vertex*&      v0() { return std::get<0>(B::refs)[0]; }
	Vertex*&      v1() { return std::get<0>(B::refs)[1]; }
	Vertex*&      v2() { return std::get<0>(B::refs)[2]; }
	const Vertex* v0() const { return std::get<0>(B::refs)[0]; }
	const Vertex* v1() const { return std::get<0>(B::refs)[1]; }
	const Vertex* v2() const { return std::get<0>(B::refs)[2]; }

	void setV0(Vertex* v) { std::get<0>(B::refs)[0] = v; }
	void setV1(Vertex* v) { std::get<0>(B::refs)[1] = v; }
	void setV2(Vertex* v) { std::get<0>(B::refs)[2] = v; }
};

template <typename  T>
using hasVertexReferencesT = std::is_base_of<VertexRefereferencesTriggerer, T>;

template <typename  T>
bool constexpr hasVertexReferences() {return hasVertexReferencesT<T>::value;}

} // namespace mgp::components

#include "vertex_references.cpp"

#endif // MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H
