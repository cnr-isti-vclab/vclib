/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include "element_references.h"

namespace vcl::comp {

class VertexReferencesTriggerer
{
};

template<typename Vertex, int N>
class VertexReferences : protected ElementReferences<Vertex, N>, public VertexReferencesTriggerer
{
	using Base = ElementReferences<Vertex, N>;

public:
	using VertexType = Vertex;

	static const int VERTEX_NUMBER = Base::CONTAINER_SIZE;

	/** Iterator Types declaration **/

	using VertexIterator           = typename Base::GCIterator;
	using ConstVertexIterator      = typename Base::ConstGCIterator;
	using VertexRangeIterator      = typename Base::GCRangeIterator;
	using ConstVertexRangeIterator = typename Base::ConstGCRangeIterator;

	/** Constructor **/

	VertexReferences() : Base() {}

	/** Member functions **/

	unsigned int vertexNumber() const { return Base::size(); }

	Vertex*&      vertex(unsigned int i) { return Base::at(i); }
	const Vertex* vertex(unsigned int i) const { return Base::at(i); }
	Vertex*&      vertexMod(int i) { return Base::atMod(i); }
	const Vertex* vertexMod(int i) const { return Base::atMod(i); }

	void setVertex(Vertex* v, unsigned int i) { Base::set(v, i); }
	void setVertices(const std::vector<Vertex*>& list) { Base::set(list); }

	using Base::contains;

	/** Member functions specific for vector **/

	template<int U = N>
	internal::ReturnIfIsVector<U, void> resizeVertices(unsigned int n)
	{
		Base::resize(n);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> pushVertex(Vertex* v)
	{
		Base::pushBack(v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> insertVertex(unsigned int i, Vertex* v)
	{
		Base::insert(i, v);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> eraseVertex(unsigned int i)
	{
		Base::erase(i);
	}

	template<int U = N>
	internal::ReturnIfIsVector<U, void> clearVertices()
	{
		Base::clear();
	}

	/** Iterator Member functions **/

	VertexIterator           vertexBegin() { return Base::begin(); }
	VertexIterator           vertexEnd() { return Base::end(); }
	ConstVertexIterator      vertexBegin() const { return Base::begin(); }
	ConstVertexIterator      vertexEnd() const { return Base::end(); }
	VertexRangeIterator      vertices() { return Base::rangeIterator(); }
	ConstVertexRangeIterator vertices() const { return Base::rangeIterator(); }

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

template<typename Vertex>
class TriVertexReferences : public VertexReferences<Vertex, 3>
{
private:
	using B = VertexReferences<Vertex, 3>;

public:
	Vertex*&      v0() { return B::container[0]; }
	Vertex*&      v1() { return B::container[1]; }
	Vertex*&      v2() { return B::container[2]; }
	const Vertex* v0() const { return B::container[0]; }
	const Vertex* v1() const { return B::container[1]; }
	const Vertex* v2() const { return B::container[2]; }

	void setV0(Vertex* v) { B::container[0] = v; }
	void setV1(Vertex* v) { B::container[1] = v; }
	void setV2(Vertex* v) { B::container[2] = v; }
};

template<typename T>
using hasVertexReferencesT = std::is_base_of<VertexReferencesTriggerer, T>;

template<typename T>
bool constexpr hasVertexReferences()
{
	return hasVertexReferencesT<T>::value;
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
