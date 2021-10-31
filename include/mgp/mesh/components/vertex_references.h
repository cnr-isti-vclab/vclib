/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"

namespace mgp::components {

namespace internal {

template<int M, typename T>
using ReturnIfIsVector = typename std::enable_if<(M < 0), T>::type;
template<int M, typename T>
using ReturnIfIsArray = typename std::enable_if<(M >= 0), T>::type;

} // namespace internal

class VertexReferencesTriggerer
{
};

template<class Vertex, int N>
class VertexReferences : public VertexReferencesTriggerer
{
private:
	// if we use the vector, the size of the array will be 0
	// actually the array will never be used and will not use memory, it's just for declaration
	static const int ARRAY_SIZE = N >= 0 ? N : 0;

	// the Container type will be array or vector, depending on N value
	using Container = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>,
		typename std::vector<Vertex*>>::type;

public:
	static const int VERTEX_NUMBER = N;

	/** Iterator Types declaration **/

	// if using array, will be the array iterator, the vector iterator otherwise
	using VertexIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>::iterator,
		typename std::vector<Vertex*>::iterator>::type;

	using ConstVertexIterator = typename std::conditional<
		(N >= 0),
		typename std::array<Vertex*, ARRAY_SIZE>::const_iterator,
		typename std::vector<Vertex*>::const_iterator>::type;

	using VertexRangeIterator = RangeIterator<Container, VertexIterator>;
	using ConstVertexRangeIterator = ConstRangeIterator<Container, ConstVertexIterator>;

	/** Constructor **/

	VertexReferences();

	/** Member functions **/

	unsigned int vertexNumber() const;
	unsigned int sizeMod(unsigned int i) const;

	Vertex*&      v(unsigned int i);
	const Vertex* v(unsigned int i) const;

	void setVertex(Vertex* v, unsigned int i);
	void setVertices(const std::vector<Vertex*>& list);

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushVertex(Vertex* v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, Vertex* v);

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i);

	/** Iterator Member functions **/

	VertexIterator vertexBegin();
	VertexIterator vertexEnd();
	ConstVertexIterator vertexBegin() const;
	ConstVertexIterator vertexEnd() const;
	VertexRangeIterator vertexIterator();
	ConstVertexRangeIterator vertexIterator() const;

protected:
	Container refs;

	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
};

template<class Vertex>
class TriVertexReferences : public VertexReferences<Vertex, 3>
{
private:
	using B = VertexReferences<Vertex, 3>;

public:
	Vertex*&      v0() { return B::refs[0]; }
	Vertex*&      v1() { return B::refs[1]; }
	Vertex*&      v2() { return B::refs[2]; }
	const Vertex* v0() const { return B::refs[0]; }
	const Vertex* v1() const { return B::refs[1]; }
	const Vertex* v2() const { return B::refs[2]; }

	void setV0(Vertex* v) { B::refs[0] = v; }
	void setV1(Vertex* v) { B::refs[1] = v; }
	void setV2(Vertex* v) { B::refs[2] = v; }
};

template<typename T>
using hasVertexReferencesT = std::is_base_of<VertexReferencesTriggerer, T>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return hasVertexReferencesT<T>::value;
}

} // namespace mgp::components

#include "vertex_references.cpp"

#endif // MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H
