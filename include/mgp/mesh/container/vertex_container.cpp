/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_container.h"

namespace mgp::mesh {

/**
 * @brief Container::Container Empty constructor that creates an empty container of Vertices.
 */
template<class T>
Container<T, IfIsVertex<T>>::Container()
{
}

/**
 * @brief Container::vertex Returns a const reference of the vertex at the i-th position
 * in the Vertex Container of the Mesh, which will be the vertex having id = i.
 *
 * This function does not perform any sanity check: if i is less than vertexContainerSize(), this
 * function will return a valid Vertex reference (note that the Vertex may have been flagged as
 * deleted).
 *
 * @param i: the id of the vertex that will be returned.
 */
template<class T>
const typename Container<T, IfIsVertex<T>>::VertexType&
Container<T, IfIsVertex<T>>::vertex(unsigned int i) const
{
	return vertices[i];
}

/**
 * @brief Container::vertex Returns a reference of the vertex at the i-th position
 * in the Vertex Container of the Mesh, which will be the vertex having id = i.
 *
 * This function does not perform any sanity check: if i is less than vertexContainerSize(), this
 * function will return a valid Vertex reference (note that the Vertex may have been flagged as
 * deleted).
 *
 * @param i: the id of the vertex that will be returned.
 */
template<class T>
typename Container<T, IfIsVertex<T>>::VertexType&
Container<T, IfIsVertex<T>>::vertex(unsigned int i)
{
	return vertices[i];
}

/**
 * @brief Container::vertexNumber Returns the number of **non-deleted** vertices contained in the
 * Vertex container of the Mesh.
 *
 * If vertexNumber() != vertexContainerSize(), it means that there are some vertices that are
 * flagged as deleted.
 *
 * @return the number of non-deleted vertices of the Mesh.
 */
template<class T>
unsigned int Container<T, IfIsVertex<T>>::vertexNumber() const
{
	return vn;
}

/**
 * @brief Container::vertexContainerSize Returns the number of vertices (also deleted) contained in
 * the Vertex container of the Mesh.
 *
 * If vertexNumber() != vertexContainerSize(), it means that there are some vertices that are
 * flagged as deleted.
 *
 * @return the number of all the vertices contained in the Mesh.
 */
template<class T>
unsigned int Container<T, IfIsVertex<T> >::vertexContainerSize() const
{
	return vertices.size();
}

template<class T>
unsigned int mgp::mesh::Container<T, IfIsVertex<T> >::deletedVertexNumber() const
{
	return vertexContainerSize() - vertexNumber();
}

/**
 * @brief Container::deleteVertex Marks as deleted the vertex with the given id.
 *
 * This member function does not perform any reallocation of the vertices: the deleted vertices
 * will stay in the Vertex Container, but will be marked as deleted.
 *
 * Deleted vertices are automatically jumped by the iterators provided by the Vertex Container.
 *
 * @param i: the id of the vertex that will be marked as deleted.
 */
template<class T>
void Container<T, IfIsVertex<T> >::deleteVertex(unsigned int i)
{
	vertices[i].setDeleted();
	--vn;
}

template<class T>
typename Container<T, IfIsVertex<T>>::VertexIterator
Container<T, IfIsVertex<T>>::vertexBegin(bool jumpDeleted)
{
	auto it = vertices.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted vertices, and the first vertex is deleted, we need
		// to move forward until we find the first non-deleted vertex
		while (it != vertices.end() && it->isDeleted()) {
			++it;
		}
	}
	return VertexIterator(it, vertices, jumpDeleted);
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

/**
 * @brief Container::addVertices adds nVertices to the Vertex Container of the mesh.
 *
 * Returns the id of the first added vertex.
 *
 * @param nVertices
 * @return the id of the first added vertex.
 */
template<class T>
unsigned int Container<T, IfIsVertex<T> >::addVertices(unsigned int nVertices)
{
	unsigned int baseId = vertices.size();
	vertices.resize(vertices.size() + nVertices);
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::resize(vertices.size());
	}
	for (unsigned int i = baseId; i < vertices.size(); ++i){
		vertices[i]._id = i;
		if constexpr (vert::hasOptionalInfo<VertexType>()) {
			OptionalVertexContainer::setContainerPointer(vertices[i]);
		}
	}
	return baseId;
}

template<class T>
void Container<T, IfIsVertex<T>>::reserveVertices(unsigned int size)
{
	vertices.reserve(size);
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::reserve(size);
	}
}

/**
 * @brief mgp::mesh::Container::compact compacts the vertex container, keeping only the non-deleted
 * vertices.
 *
 * All the ids of the vertices will be updated.
 *
 * @return a vector that tells, for each old id, the new id of the vertex. Will contain -1 if the
 * vertex has been deleted.
 */
template<class T>
std::vector<int> mgp::mesh::Container<T, IfIsVertex<T> >::compactVertices()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	std::vector<int> newIndices(vertices.size());
	unsigned int k = 0;
	for (unsigned int i = 0; i < vertices.size(); ++i){
		if (!vertices[i].isDeleted()){
			vertices[k] = vertices[i];
			vertices[k]._id = k;
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = -1;
		}
	}
	vertices.resize(k);
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::compact(newIndices);
	}
	return newIndices;
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

template<class T>
template<class Face>
void Container<T, IfIsVertex<T>>::updateFaceReferencesAfterCompact(
	const Face* base,
	const std::vector<int>& newIndices)
{
	if constexpr (mgp::components::hasFaceReferences<T>()) {
		for (VertexType& v : vertexIterator()) {
			v.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
	else if constexpr (mgp::components::hasOptionalFaceReferences<T>()){
		if (OptionalVertexContainer::isPerVertexAdjacentFacesEnabled()) {
			for (VertexType& v : vertexIterator()) {
				v.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

} // namespace mgp::mesh
