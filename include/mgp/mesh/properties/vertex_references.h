/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_PROPERTIES_VERTEX_REFERENCES_H
#define MGP_MESH_PROPERTIES_VERTEX_REFERENCES_H

#include <array>
#include <assert.h>
#include <type_traits>
#include <vector>

#include "../iterators/range_iterator.h"

#include "element_references.h"

namespace mgp::prop {

class VertexReferencesTriggerer
{
};

template<class Vertex, int N>
class VertexReferences : protected ElementReferences<Vertex, N>, public VertexReferencesTriggerer
{
	using Base = ElementReferences<Vertex, N>;

public:
	static const int VERTEX_NUMBER = Base::COMPONENT_NUMBER;

	/** Iterator Types declaration **/

	using VertexIterator           = typename Base::ComponentIterator;
	using ConstVertexIterator      = typename Base::ConstComponentIterator;
	using VertexRangeIterator      = typename Base::ComponentRangeIterator;
	using ConstVertexRangeIterator = typename Base::ConstComponentRangeIterator;

	/** Constructor **/

	VertexReferences() : ElementReferences<Vertex, N>() {}

	/** Member functions **/

	unsigned int vertexNumber() const { return Base::elementNumber(); }
	using Base::sizeMod;

	Vertex*&      v(unsigned int i) { return Base::element(i); }
	const Vertex* v(unsigned int i) const { return Base::element(i); }

	void setVertex(Vertex* v, unsigned int i) { Base::setElement(v, i); }
	void setVertices(const std::vector<Vertex*>& list) { Base::setElements(list); }

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushVertex(Vertex* v)
	{
		Base::pushElement(v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, Vertex* v)
	{
		Base::insertElement(i, v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i)
	{
		Base::eraseElement(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearVertices()
	{
		Base::clearElements();
	}

	/** Iterator Member functions **/

	VertexIterator           vertexBegin() { return Base::elementBegin(); }
	VertexIterator           vertexEnd() { return Base::elementEnd(); }
	ConstVertexIterator      vertexBegin() const { return Base::elementBegin(); }
	ConstVertexIterator      vertexEnd() const { return Base::elementEnd(); }
	VertexRangeIterator      vertexIterator() { return Base::elementIterator(); }
	ConstVertexRangeIterator vertexIterator() const { return Base::elementIterator(); }

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
	{
		Base::updateElementReferences(oldBase, newBase);
	}

	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices)
	{
		Base::updateElementReferencesAfterCompact(base, newIndices);
	}
};

template<class Vertex>
class TriVertexReferences : public VertexReferences<Vertex, 3>
{
private:
	using B = VertexReferences<Vertex, 3>;

public:
	Vertex*&      v0() { return B::elemRefs[0]; }
	Vertex*&      v1() { return B::elemRefs[1]; }
	Vertex*&      v2() { return B::elemRefs[2]; }
	const Vertex* v0() const { return B::elemRefs[0]; }
	const Vertex* v1() const { return B::elemRefs[1]; }
	const Vertex* v2() const { return B::elemRefs[2]; }

	void setV0(Vertex* v) { B::elemRefs[0] = v; }
	void setV1(Vertex* v) { B::elemRefs[1] = v; }
	void setV2(Vertex* v) { B::elemRefs[2] = v; }
};

template<typename T>
using hasVertexReferencesT = std::is_base_of<VertexReferencesTriggerer, T>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return hasVertexReferencesT<T>::value;
}

} // namespace mgp::prop

#endif // MGP_MESH_PROPERTIES_VERTEX_REFERENCES_H
