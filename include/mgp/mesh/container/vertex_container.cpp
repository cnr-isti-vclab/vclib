/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_container.h"

namespace mgp::mesh {

/**
 * @brief Container::Container Empty constructor that creates an empty container of Vertices.
 */
template<typename T>
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
template<typename T>
const typename Container<T, IfIsVertex<T>>::VertexType&
Container<T, IfIsVertex<T>>::vertex(unsigned int i) const
{
	return vertsVec[i];
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
template<typename T>
typename Container<T, IfIsVertex<T>>::VertexType&
Container<T, IfIsVertex<T>>::vertex(unsigned int i)
{
	return vertsVec[i];
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
template<typename T>
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
template<typename T>
unsigned int Container<T, IfIsVertex<T> >::vertexContainerSize() const
{
	return vertsVec.size();
}

template<typename T>
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
template<typename T>
void Container<T, IfIsVertex<T> >::deleteVertex(unsigned int i)
{
	vertsVec[i].setDeleted();
	--vn;
}

/**
 * @brief Container::vertexIdIfCompact is an utility member function that returns the id of
 * an element if the container would be compact, that is the number of non-deleted elements before
 * the vertex with the given id.
 *
 * Complexity: O(n), n number of vertices in the container.
 *
 * This function does not perform any sanity check.
 *
 * @param id
 * @return
 */
template<typename T>
unsigned int mgp::mesh::Container<T, IfIsVertex<T> >::vertexIdIfCompact(unsigned int id) const
{
	if (vertsVec.size() == vn)
		return id;
	else {
		unsigned int cnt = 0;
		for (unsigned int i = 0; i < id; i++){
			if (!vertsVec[i].isDeleted())
				++cnt;
		}
		return cnt;
	}
}

template<typename T>
typename Container<T, IfIsVertex<T>>::VertexIterator
Container<T, IfIsVertex<T>>::vertexBegin(bool jumpDeleted)
{
	auto it = vertsVec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted vertices, and the first vertex is deleted, we need
		// to move forward until we find the first non-deleted vertex
		while (it != vertsVec.end() && it->isDeleted()) {
			++it;
		}
	}
	return VertexIterator(it, vertsVec, jumpDeleted && vertsVec.size() != vn);
}

template<typename T>
typename Container<T, IfIsVertex<T>>::VertexIterator Container<T, IfIsVertex<T>>::vertexEnd()
{
	return VertexIterator(vertsVec.end(), vertsVec);
}

template<typename T>
typename Container<T, IfIsVertex<T>>::ConstVertexIterator
Container<T, IfIsVertex<T>>::vertexBegin(bool jumpDeleted) const
{
	auto it = vertsVec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted vertices, and the first vertex is deleted, we need
		// to move forward until we find the first non-deleted vertex
		while (it != vertsVec.end() && it->isDeleted()) {
			++it;
		}
	}
	return ConstVertexIterator(it, vertsVec, jumpDeleted && vertsVec.size() != vn);
}

template<typename T>
typename Container<T, IfIsVertex<T>>::ConstVertexIterator
Container<T, IfIsVertex<T>>::vertexEnd() const
{
	return ConstVertexIterator(vertsVec.end(), vertsVec);
}

template<typename T>
typename Container<T, IfIsVertex<T>>::VertexRangeIterator
Container<T, IfIsVertex<T>>::vertices(bool jumpDeleted)
{
	return VertexRangeIterator(
		*this, jumpDeleted && vertsVec.size() != vn, &VertexContainer::vertexBegin, &VertexContainer::vertexEnd);
}

template<typename T>
typename Container<T, IfIsVertex<T>>::ConstVertexRangeIterator
Container<T, IfIsVertex<T>>::vertices(bool jumpDeleted) const
{
	return ConstVertexRangeIterator(
		*this, jumpDeleted, &VertexContainer::vertexBegin, &VertexContainer::vertexEnd);
}

template<typename T>
void Container<T, IfIsVertex<T> >::clearVertices()
{
	vertsVec.clear();
	vn = 0;
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::clear();
	}
}

template<typename T>
unsigned int Container<T, IfIsVertex<T> >::index(const VertexType* v) const
{
	assert(!vertsVec.empty() && v >= vertsVec.data() && v <= &vertsVec.back());
	return v - vertsVec.data();
}

template<typename T>
unsigned int Container<T, IfIsVertex<T>>::addVertex()
{
	T* oldB = vertsVec.data();
	vertsVec.push_back(VertexType());
	T* newB = vertsVec.data();
	++vn;
	vertsVec[vertsVec.size() - 1]._id = vertsVec.size() - 1;
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::setContainerPointer(vertsVec[vertsVec.size() - 1]);
		OptionalVertexContainer::resize(vertsVec.size());
	}
	updateVertexReferences(oldB, newB);
	return vertsVec[vertsVec.size() - 1]._id;
}

/**
 * @brief Container::addVertices adds nVertices to the Vertex Container of the mesh.
 *
 * Returns the id of the first added vertex.
 *
 * @param nVertices
 * @return the id of the first added vertex.
 */
template<typename T>
unsigned int Container<T, IfIsVertex<T> >::addVertices(unsigned int nVertices)
{
	unsigned int baseId = vertsVec.size();
	T* oldB = vertsVec.data();
	vertsVec.resize(vertsVec.size() + nVertices);
	T* newB = vertsVec.data();
	vn+=nVertices;
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::resize(vertsVec.size());
	}
	for (unsigned int i = baseId; i < vertsVec.size(); ++i){
		vertsVec[i]._id = i;
		if constexpr (vert::hasOptionalInfo<VertexType>()) {
			OptionalVertexContainer::setContainerPointer(vertsVec[i]);
		}
	}
	updateVertexReferences(oldB, newB);
	return baseId;
}

template<typename T>
void Container<T, IfIsVertex<T>>::reserveVertices(unsigned int size)
{
	T* oldB = vertsVec.data();
	vertsVec.reserve(size);
	T* newB = vertsVec.data();
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::reserve(size);
	}
	updateVertexReferences(oldB, newB);
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
template<typename T>
std::vector<int> mgp::mesh::Container<T, IfIsVertex<T> >::compactVertices()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	std::vector<int> newIndices(vertsVec.size());
	unsigned int k = 0;
	for (unsigned int i = 0; i < vertsVec.size(); ++i){
		if (!vertsVec[i].isDeleted()){
			vertsVec[k] = vertsVec[i];
			vertsVec[k]._id = k;
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = -1;
		}
	}
	vertsVec.resize(k);
	T* base = vertsVec.data();
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		OptionalVertexContainer::compact(newIndices);
	}
	updateVertexReferencesAfterCompact(base, newIndices);
	return newIndices;
}

template<typename T>
void Container<T, IfIsVertex<T> >::updateVertexReferences(const T* oldBase, const T* newBase)
{
	if constexpr (mgp::vert::hasAdjacentVertices<T>()) {
		for (VertexType& v : vertices()) {
			v.updateVertexReferences(oldBase, newBase);
		}
	}
	else if constexpr(mgp::vert::hasOptionalAdjacentVertices<T>()) {
		if (OptionalVertexContainer::isPerVertexAdjacentVerticesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateVertexReferences(oldBase, newBase);
			}
		}
	}
}

template<typename T>
void Container<T, IfIsVertex<T> >::updateVertexReferencesAfterCompact(
	const T* base,
	const std::vector<int>& newIndices)
{
	if constexpr (mgp::vert::hasAdjacentVertices<T>()) {
		for (VertexType& v : vertices()) {
			v.updateVertexReferencesAfterCompact(base, newIndices);
		}
	}
	else if constexpr (mgp::vert::hasOptionalAdjacentVertices<T>()){
		if (OptionalVertexContainer::isPerVertexAdjacentVerticesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateVertexReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

template<typename T>
template<typename  Face>
void Container<T, IfIsVertex<T>>::updateFaceReferences(const Face* oldBase, const Face* newBase)
{
	if constexpr (mgp::vert::hasAdjacentFaces<T>()) {
		for (VertexType& v : vertices()) {
			v.updateFaceReferences(oldBase, newBase);
		}
	}
	else if constexpr (mgp::vert::hasOptionalAdjacentFaces<T>()){
		if (OptionalVertexContainer::isPerVertexAdjacentFacesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateFaceReferences(oldBase, newBase);
			}
		}
	}
}

template<typename T>
template<typename Face>
void Container<T, IfIsVertex<T>>::updateFaceReferencesAfterCompact(
	const Face* base,
	const std::vector<int>& newIndices)
{
	if constexpr (mgp::vert::hasAdjacentFaces<T>()) {
		for (VertexType& v : vertices()) {
			v.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
	else if constexpr (mgp::vert::hasOptionalAdjacentFaces<T>()){
		if (OptionalVertexContainer::isPerVertexAdjacentFacesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

} // namespace mgp::mesh
