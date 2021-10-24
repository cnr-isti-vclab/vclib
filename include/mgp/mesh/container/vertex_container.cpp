/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_container.h"

namespace mgp::mesh {

template<class T>
Container<T, IfIsVertex<T>>::Container()
{
}

template<class T>
const typename Container<T, IfIsVertex<T>>::VertexType&
mgp::mesh::Container<T, IfIsVertex<T>>::vertex(unsigned int i) const
{
	return vertices[i];
}

template<class T>
typename Container<T, IfIsVertex<T>>::VertexType&
mgp::mesh::Container<T, IfIsVertex<T>>::vertex(unsigned int i)
{
	return vertices[i];
}

template<class T>
unsigned int Container<T, IfIsVertex<T>>::vertexNumber() const
{
	return vn;
}

template<class T>
void Container<T, IfIsVertex<T>>::reserveVertices(unsigned int size)
{
	vertices.reserve(size);
	if constexpr (common::hasOptionalInfo<VertexType>::value) {
		optionalComponentsVector.reserve(size);
	}
}

template<class T>
template<class U>
common::ReturnIfHasOptionalColor<U, void> Container<T, IfIsVertex<T>>::enableVertexColor()
{
	optionalComponentsVector.enableColor(vertexNumber());
}

template<class T>
template<class U>
common::ReturnIfHasOptionalMutableBitFlags<U, void>
Container<T, IfIsVertex<T>>::enableVertexMutableBitFlags()
{
	optionalComponentsVector.enableMutableBitFlags(vertexNumber());
}

template<class T>
template<class U>
common::ReturnIfHasOptionalNormal<U, void> Container<T, IfIsVertex<T>>::enableVertexNormal()
{
	optionalComponentsVector.enableNormal(vertexNumber());
}

template<class T>
template<class U>
common::ReturnIfHasOptionalScalar<U, void> Container<T, IfIsVertex<T>>::enableVertexScalar()
{
	optionalComponentsVector.enableScalar(vertexNumber());
}

template<class T>
typename Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>::VertexIterator
Container<T, IfIsVertex<T>>::vertexBegin(bool jumpDeleted)
{
	if (jumpDeleted) {
		auto it = vertices.begin();
		while (it->isDeleted()) {
			++it;
		}
		return VertexIterator(it, vertices, jumpDeleted);
	}
	else
		return VertexIterator(vertices.begin(), vertices, jumpDeleted);
}

template<class T>
typename Container<T, IfIsVertex<T>>::VertexIterator Container<T, IfIsVertex<T>>::vertexEnd()
{
	return VertexIterator(vertices.end(), vertices);
}

template<class T>
typename Container<T, IfIsVertex<T>>::ConstVertexIterator
Container<T, IfIsVertex<T>>::vertexBegin(bool jumpDeleted) const
{
	if (jumpDeleted) {
		auto it = vertices.begin();
		while (it->isDeleted()) {
			++it;
		}
		return ConstVertexIterator(it, vertices, jumpDeleted);
	}
	else
		return ConstVertexIterator(vertices.begin(), vertices, jumpDeleted);
}

template<class T>
typename Container<T, IfIsVertex<T>>::ConstVertexIterator
Container<T, IfIsVertex<T>>::vertexEnd() const
{
	return ConstVertexIterator(vertices.end(), vertices);
}

template<class T>
unsigned int Container<T, IfIsVertex<T>>::addVertex()
{
	vertices.push_back(VertexType());
	++vn;
	vertices[vertices.size() - 1]._id = vertices.size() - 1;
	if constexpr (common::hasOptionalInfo<VertexType>::value) {
		vertices[vertices.size() - 1].setContainerPointer(&optionalComponentsVector);
		optionalComponentsVector.resize(vertices.size());
	}
	return vertices[vertices.size() - 1]._id;
}

template<class T>
typename Container<T, IfIsVertex<T>>::VertexRangeIterator
Container<T, IfIsVertex<T>>::vertexIterator(bool jumpDeleted)
{
	return VertexRangeIterator(
		*this, jumpDeleted, &VertexContainer::vertexBegin, &VertexContainer::vertexEnd);
}

template<class T>
typename Container<T, IfIsVertex<T>>::ConstVertexRangeIterator
Container<T, IfIsVertex<T>>::vertexIterator(bool jumpDeleted) const
{
	return ConstVertexRangeIterator(
		*this, jumpDeleted, &VertexContainer::vertexBegin, &VertexContainer::vertexEnd);
}

} // namespace mgp::mesh
