/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define MGP_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include "element_references.h"

namespace mgp::comp {

class AdjacentVerticesTriggerer
{
};

template<class Vertex, int N>
class AdjacentVertices : protected ElementReferences<Vertex, N>, public AdjacentVerticesTriggerer
{
	using Base = ElementReferences<Vertex, N>;

public:
	static const int ADJ_VERTEX_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using AdjacentVertexIterator           = typename Base::GCIterator;
	using ConstAdjacentVertexIterator      = typename Base::ConstGCIterator;
	using AdjacentVertexRangeIterator      = typename Base::GCRangeIterator;
	using ConstAdjacentVertexRangeIterator = typename Base::ConstGCRangeIterator;

	/** Constructor **/

	AdjacentVertices() : Base() {}

	/** Member functions **/

	unsigned int adjVerticesNumber() const { return Base::size(); }

	Vertex*&      adjVert(unsigned int i) { return Base::at(i); }
	const Vertex* adjVert(unsigned int i) const { return Base::at(i); }
	Vertex*&      adjVertMod(int i) { return Base::atMod(i); }
	const Vertex* adjVertMod(int i) const { return Base::atMod(i); }

	void setAdjVertex(Vertex* v, unsigned int i) { Base::set(v, i); }
	void setAdjVertices(const std::vector<Vertex*>& list) { Base::set(list); }

	bool containsAdjVertex(const Vertex* v) const { return Base::contains(v); }

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeAdjVertices(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushAdjVertex(Vertex* v)
	{
		Base::pushBack(v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertAdjVertex(unsigned int i, Vertex* v)
	{
		Base::insert(i, v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseAdjVertex(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearAdjVertices()
	{
		Base::clear();
	}

	/** Iterator Member functions **/

	AdjacentVertexIterator           adjVertexBegin() { return Base::begin(); }
	AdjacentVertexIterator           adjVertexEnd() { return Base::end(); }
	ConstAdjacentVertexIterator      adjVertexBegin() const { return Base::begin(); }
	ConstAdjacentVertexIterator      adjVertexEnd() const { return Base::end(); }
	AdjacentVertexRangeIterator      adjVertexIterator() { return Base::rangeIterator(); }
	ConstAdjacentVertexRangeIterator adjVertexIterator() const { return Base::rangeIterator(); }

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

template<typename T>
using hasAdjacentVerticesT = std::is_base_of<AdjacentVerticesTriggerer, T>;

template<typename T>
bool constexpr hasAdjacentVertices()
{
	return hasAdjacentVerticesT<T>::value;
}

} // namespace mgp::comp

#endif // MGP_MESH_COMPONENTS_ADJACENT_VERTICES_H
