/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_container.h"

namespace mgp::mesh {

/**
 * @brief Container::Container Empty constructors that created an empty container of Vertices.
 */
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
unsigned int Container<T, IfIsVertex<T> >::vertexContainerSize() const
{
	return vertices.size();
}



template<class T>
typename Container<T, IfIsVertex<T>>::VertexIterator
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

template<class T>
unsigned int Container<T, IfIsVertex<T>>::addVertex()
{
	vertices.push_back(VertexType());
	++vn;
	vertices[vertices.size() - 1]._id = vertices.size() - 1;
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::setContainerPointer(vertices[vertices.size() - 1]);
		OptionalVertexContainer::resize(vertices.size());
	}
	return vertices[vertices.size() - 1]._id;
}

template<class T>
void Container<T, IfIsVertex<T>>::reserveVertices(unsigned int size)
{
	vertices.reserve(size);
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::reserve(size);
	}
}

template<class T>
template<typename  Face>
void Container<T, IfIsVertex<T>>::updateFaceReferences(const Face* oldBase, const Face* newBase)
{
	if constexpr (mgp::components::hasFaceReferences<T>()) {
		for (VertexType& v : vertexIterator()) {
			v.updateFaceReferences(oldBase, newBase);
		}
	}
	else if constexpr (mgp::components::hasOptionalFaceReferences<T>()){
		if (OptionalVertexContainer::isPerVertexAdjacentFacesEnabled()) {
			for (VertexType& v : vertexIterator()) {
				v.updateFaceReferences(oldBase, newBase);
			}
		}
	}
}

} // namespace mgp::mesh
