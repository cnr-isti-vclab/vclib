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

#include "component_references.h"

namespace mgp::components {

class VertexReferencesTriggerer
{
};

template<class Vertex, int N>
class VertexReferences : protected ComponentReferences<Vertex, N>, public VertexReferencesTriggerer
{
	using Base = ComponentReferences<Vertex, N>;
public:
	static const int VERTEX_NUMBER = Base::COMPONENT_NUMBER;

	/** Iterator Types declaration **/

	using VertexIterator = typename Base::ComponentIterator;
	using ConstVertexIterator = typename Base::ConstComponentIterator;
	using VertexRangeIterator = typename Base::ComponentRangeIterator;
	using ConstVertexRangeIterator = typename Base::ConstComponentRangeIterator;

	/** Constructor **/

	VertexReferences () : ComponentReferences<Vertex, N>() {}

	/** Member functions **/

	unsigned int vertexNumber() const { return Base::componentNumber(); }
	using Base::sizeMod;

	Vertex*& v (unsigned int i){ return Base::c(i); }
	const Vertex* v  (unsigned int i) const {return Base::c(i);}

	void setVertex(Vertex* v, unsigned int i) {Base::setComponent(v, i);}
	void setVertices(const std::vector<Vertex*>& list) {Base::setComponents(list);}

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushVertex(Vertex* v) { Base::pushComponent(v); }

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, Vertex* v)
	{
		Base::insertComponent(i, v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i)
	{
		Base::eraseComponent(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearVertices()
	{
		Base::clearComponents();
	}

	/** Iterator Member functions **/

	VertexIterator vertexBegin() {return Base::componentBegin();}
	VertexIterator vertexEnd() {return Base::componentEnd();}
	ConstVertexIterator vertexBegin() const {return Base::componentBegin();}
	ConstVertexIterator vertexEnd() const {return Base::componentEnd();}
	VertexRangeIterator vertexIterator() {return Base::componentIterator();}
	ConstVertexRangeIterator vertexIterator() const {return Base::componentIterator();}

protected:
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
	{
		Base::updateComponentReferences(oldBase, newBase);
	}
};

template<class Vertex>
class TriVertexReferences : public VertexReferences<Vertex, 3>
{
private:
	using B = VertexReferences<Vertex, 3>;

public:
	Vertex*&      v0() { return B::compRefs[0]; }
	Vertex*&      v1() { return B::compRefs[1]; }
	Vertex*&      v2() { return B::compRefs[2]; }
	const Vertex* v0() const { return B::compRefs[0]; }
	const Vertex* v1() const { return B::compRefs[1]; }
	const Vertex* v2() const { return B::compRefs[2]; }

	void setV0(Vertex* v) { B::compRefs[0] = v; }
	void setV1(Vertex* v) { B::compRefs[1] = v; }
	void setV2(Vertex* v) { B::compRefs[2] = v; }
};

template<typename T>
using hasVertexReferencesT = std::is_base_of<VertexReferencesTriggerer, T>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return hasVertexReferencesT<T>::value;
}

} // namespace mgp::components

#endif // MGP_MESH_COMPONENTS_VERTEX_REFERENCES_H
