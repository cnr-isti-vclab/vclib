/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_container.h"

namespace mgp::mesh {

/**
 * @brief Empty constructor that creates an empty container of Vertices.
 */
template<typename T>
VertexContainer<T>::VertexContainer()
{
}

/**
 * @brief Returns a const reference of the vertex at the i-th position
 * in the Vertex Container of the Mesh, which will be the vertex having id = i.
 *
 * This function does not perform any sanity check: if i is less than vertexContainerSize(), this
 * function will return a valid Vertex reference (note that the Vertex may have been flagged as
 * deleted).
 *
 * @param i: the id of the vertex that will be returned.
 */
template<typename T>
const typename VertexContainer<T>::VertexType&
VertexContainer<T>::vertex(unsigned int i) const
{
	return Base::vec[i];
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
typename VertexContainer<T>::VertexType&
VertexContainer<T>::vertex(unsigned int i)
{
	return Base::vec[i];
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
unsigned int VertexContainer<T>::vertexNumber() const
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
unsigned int VertexContainer<T>::vertexContainerSize() const
{
	return Base::vec.size();
}

template<typename T>
unsigned int mgp::mesh::VertexContainer<T>::deletedVertexNumber() const
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
void VertexContainer<T>::deleteVertex(unsigned int i)
{
	Base::vec[i].setDeleted();
	--vn;
}

template<typename T>
void VertexContainer<T>::deleteVertex(const VertexType* v)
{
	deleteVertex(index(v));
}

/**
 * @brief This is an utility member function that returns the id of an element if the container
 * would be compact, that is the number of non-deleted elements before the vertex with the given id.
 *
 * Complexity: O(n), n number of vertices in the container.
 *
 * This function does not perform any sanity check.
 *
 * @param id
 * @return
 */
template<typename T>
unsigned int mgp::mesh::VertexContainer<T>::vertexIndexIfCompact(unsigned int id) const
{
	if (Base::vec.size() == vn)
		return id;
	else {
		unsigned int cnt = 0;
		for (unsigned int i = 0; i < id; i++){
			if (!Base::vec[i].isDeleted())
				++cnt;
		}
		return cnt;
	}
}

template<typename T>
typename VertexContainer<T>::VertexIterator
VertexContainer<T>::vertexBegin(bool jumpDeleted)
{
	auto it = Base::vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted vertices, and the first vertex is deleted, we need
		// to move forward until we find the first non-deleted vertex
		while (it != Base::vec.end() && it->isDeleted()) {
			++it;
		}
	}
	return VertexIterator(it, Base::vec, jumpDeleted && Base::vec.size() != vn);
}

template<typename T>
typename VertexContainer<T>::VertexIterator VertexContainer<T>::vertexEnd()
{
	return VertexIterator(Base::vec.end(), Base::vec);
}

template<typename T>
typename VertexContainer<T>::ConstVertexIterator
VertexContainer<T>::vertexBegin(bool jumpDeleted) const
{
	auto it = Base::vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted vertices, and the first vertex is deleted, we need
		// to move forward until we find the first non-deleted vertex
		while (it != Base::vec.end() && it->isDeleted()) {
			++it;
		}
	}
	return ConstVertexIterator(it, Base::vec, jumpDeleted && Base::vec.size() != vn);
}

template<typename T>
typename VertexContainer<T>::ConstVertexIterator
VertexContainer<T>::vertexEnd() const
{
	return ConstVertexIterator(Base::vec.end(), Base::vec);
}

template<typename T>
typename VertexContainer<T>::VertexRangeIterator
VertexContainer<T>::vertices(bool jumpDeleted)
{
	return VertexRangeIterator(
		*this, jumpDeleted && Base::vec.size() != vn, &VertexContainer::vertexBegin, &VertexContainer::vertexEnd);
}

template<typename T>
typename VertexContainer<T>::ConstVertexRangeIterator
VertexContainer<T>::vertices(bool jumpDeleted) const
{
	return ConstVertexRangeIterator(
		*this, jumpDeleted, &VertexContainer::vertexBegin, &VertexContainer::vertexEnd);
}

/**
 * @brief VertexOptionalContainer::isPerVertexAdjacentFacesEnabled checks if the vertex Optional
 * Adjacent Faces component is enabled. This function is available **only if the Vertex Element has
 * the OptionalAdjacentFaces Component**.
 * @return true if the Optional Adjacent Faces is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalAdjacentFaces<U, bool>
VertexContainer<T>::isPerVertexAdjacentFacesEnabled() const
{
	return Base::optionalVec.isAdjacentFacesEnabled();
}

/**
 * @brief VertexOptionalContainer::enablePerVertexAdjacentFaces enables the Optional Adjacent Faces
 * of the vertex.
 * This function is available **only if the Vertex Element has the OptionalAdjacentFaces
 * Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalAdjacentFaces<U, void>
VertexContainer<T>::enablePerVertexAdjacentFaces()
{
	Base::optionalVec.enableAdjacentFaces(vertexContainerSize());
}

/**
 * @brief VertexOptionalContainer::disablePerVertexAdjacentFaces disables the Optional Adjacent
 * Faces of the vertex. This function is available **only if the Vertex Element has the
 * OptionalAdjacentFaces Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalAdjacentFaces<U, void>
VertexContainer<T>::disablePerVertexAdjacentFaces()
{
	Base::optionalVec.disableAdjacentFaces();
}

/**
 * @brief VertexOptionalContainer::isPerVertexAdjacentVerticesEnabled checks if the vertex Optional
 * Adjacent Vertices component is enabled. This function is available **only if the Vertex Element
 * has the OptionalAdjacentVertices Component**.
 * @return true if the Optional Adjacent Vertices is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalAdjacentVertices<U, bool>
VertexContainer<T>::isPerVertexAdjacentVerticesEnabled() const
{
	return Base::optionalVec.isVertexReferencesEnabled();
}

/**
 * @brief VertexOptionalContainer::enablePerVertexAdjacentVertices enables the Optional Adjacent
 * Vertices of the vertex. This function is available **only if the Vertex Element has the
 * OptionalAdjacentVertices Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalAdjacentVertices<U, void>
VertexContainer<T>::enablePerVertexAdjacentVertices()
{
	Base::optionalVec.enableVertexReferences(vertexContainerSize());
}

/**
 * @brief VertexOptionalContainer::disablePerVertexAdjacentVertices disables the Optional Adjacent
 * Vertices of the vertex. This function is available **only if the Vertex Element has the
 * OptionalAdjacentVertices Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalAdjacentVertices<U, void>
VertexContainer<T>::disablePerVertexAdjacentVertices()
{
	Base::optionalVec.disableVertexReferences();
}

/**
 * @brief VertexOptionalContainer::isPerVertexColorEnabled checks if the vertex Optional Color is
 * enabled. This function is available **only if the Vertex Element has the OptionalColor
 * Component**.
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalColor<U, bool>
VertexContainer<T>::isPerVertexColorEnabled() const
{
	return Base::optionalVec.isColorEnabled();
}

/**
 * @brief Container::enableVertexColor enables the Optional Color of the vertex.
 * This function is available **only if the Vertex Element has the OptionalColor Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalColor<U, void>
VertexContainer<T>::enablePerVertexColor()
{
	Base::optionalVec.enableColor(vertexContainerSize());
}

/**
 * @brief Container::disableVertexColor disables the Optional Color of the vertex.
 * This function is available **only if the Vertex Element has the OptionalColor Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalColor<U, void>
VertexContainer<T>::disablePerVertexColor()
{
	Base::optionalVec.disableColor();
}

/**
 * @brief VertexOptionalContainer::isPerVertexMarkEnabled checks if the vertex Optional Mark is
 * enabled. This function is available **only if the Vertex Element has the OptionalMark
 * Component**.
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalMark<U, bool>
VertexContainer<T>::isPerVertexMarkEnabled() const
{
	return Base::optionalVec.isMarkEnabled();
}

/**
 * @brief Container::enableVertexMark enables the Optional Mark of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMark Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalMark<U, void>
VertexContainer<T>::enablePerVertexMark()
{
	Base::optionalVec.enableMark(vertexContainerSize());
}

/**
 * @brief Container::disableVertexMark disables the Optional Mark of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMark Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalMark<U, void>
VertexContainer<T>::disablePerVertexMark()
{
	Base::optionalVec.disableMark();
}

/**
 * @brief VertexOptionalContainer::isPerVertexMutableBitFlagsEnabled checks if the vertex Optional
 * Mutable Bit Flags is enabled. This function is available **only if the Vertex Element has the
 * OptionalMutableBitFlags Component**.
 * @return true if the Optional Mutable Bit Flags component is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalMutableBitFlags<U, bool>
VertexContainer<T>::isPerVertexMutableBitFlagsEnabled() const
{
	Base::optionalVec.isMutableBitFlagsEnabled();
}

/**
 * @brief Container::enableVertexMutableFlags enables the Optional Mutable Flags of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMutableBitFlags
 * Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalMutableBitFlags<U, void>
VertexContainer<T>::enablePerVertexMutableBitFlags()
{
	Base::optionalVec.enableMutableBitFlags(vertexContainerSize());
}

/**
 * @brief Container::disableVertexMutableFlags disables the Optional Mutable Flags of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMutableBitFlags
 * Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalMutableBitFlags<U, void>
VertexContainer<T>::disablePerVertexMutableBitFlags()
{
	Base::optionalVec.disableMutableBitFlags();
}

/**
 * @brief VertexOptionalContainer::isPerVertexNormalEnabled checks if the vertex Optional Normal is
 * enabled. This function is available **only if the Vertex Element has the OptionalNormal
 * Component**.
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalNormal<U, bool>
VertexContainer<T>::isPerVertexNormalEnabled() const
{
	return Base::optionalVec.isNormalEnabled();
}

/**
 * @brief Container::enableVertexNormal enables the Optional Normal of the vertex.
 * This function is available **only if the Vertex Element has the OptionalNormal Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalNormal<U, void>
VertexContainer<T>::enablePerVertexNormal()
{
	Base::optionalVec.enableNormal(vertexContainerSize());
}

/**
 * @brief Container::disableVertexNormal disables the Optional Normal of the vertex.
 * This function is available **only if the Vertex Element has the OptionalNormal Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalNormal<U, void>
VertexContainer<T>::disablePerVertexNormal()
{
	Base::optionalVec.disableNormal();
}

/**
 * @brief VertexOptionalContainer::isPerVertexScalarEnabled checks if the vertex Optional Scalar is
 * enabled. This function is available **only if the Vertex Element has the OptionalScalar
 * Component**.
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalScalar<U, bool>
VertexContainer<T>::isPerVertexScalarEnabled() const
{
	return Base::optionalVec.isScalarEnabled();
}

/**
 * @brief Container::enableVertexScalar enables the Optional Scalar of the vertex.
 * This function is available **only if the Vertex Element has the OptionalScalar Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalScalar<U, void>
VertexContainer<T>::enablePerVertexScalar()
{
	Base::optionalVec.enableScalar(vertexContainerSize());
}

/**
 * @brief Container::disableVertexScalar disables the Optional Scalar of the vertex.
 * This function is available **only if the Vertex Element has the OptionalScalar Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalScalar<U, void>
VertexContainer<T>::disablePerVertexScalar()
{
	Base::optionalVec.disableScalar();
}

/**
 * @brief VertexOptionalContainer::isPerVertexTexCoordEnabled checks if the vertex Optional TexCoord
 * is enabled. This function is available **only if the Vertex Element has the OptionalTexCoord
 * Component**.
 * @return true if the Optional TexCoord is enabled, false otherwise.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalTexCoord<U, bool>
VertexContainer<T>::isPerVertexTexCoordEnabled() const
{
	return Base::optionalVec.isTexCoordEnabled();
}

/**
 * @brief Container::enableVertexTexCoord enables the Optional TexCoord of the vertex.
 * This function is available **only if the Vertex Element has the OptionalTexCoord Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalTexCoord<U, void>
VertexContainer<T>::enablePerVertexTexCoord()
{
	Base::optionalVec.enableTexCoord(vertexContainerSize());
}

/**
 * @brief Container::disableVertexTexCoord disables the Optional TexCoord of the vertex.
 * This function is available **only if the Vertex Element has the OptionalTexCoord Component**.
 */
template<typename T>
template<typename U>
vert::ReturnIfHasOptionalTexCoord<U, void>
VertexContainer<T>::disablePerVertexTexCoord()
{
	Base::optionalVec.disableTexCoord();
}

/**
 * @brief VertexOptionalContainer::addPerVertexCustomComponent
 */
template<typename T>
template<typename K, typename U>
vert::ReturnIfHasCustomComponents<U, void>
VertexContainer<T>::addPerVertexCustomComponent(
	const std::string& name)
{
	Base::optionalVec.template addNewComponent<K>(name, vertexContainerSize());
}

template<typename T>
void VertexContainer<T>::clearVertices()
{
	Base::vec.clear();
	vn = 0;
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		Base::optionalVec.clear();
	}
}

template<typename T>
unsigned int VertexContainer<T>::index(const VertexType* v) const
{
	assert(!Base::vec.empty() && v >= Base::vec.data() && v <= &Base::vec.back());
	return v - Base::vec.data();
}

template<typename T>
unsigned int VertexContainer<T>::addVertex()
{
	T* oldB = Base::vec.data();
	Base::vec.push_back(VertexType());
	T* newB = Base::vec.data();
	++vn;
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		setContainerPointer(Base::vec[Base::vec.size() - 1]);
		Base::optionalVec.resize(Base::vec.size());
	}
	updateAfterAllocation(oldB, newB);
	return Base::vec.size() - 1;
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
unsigned int VertexContainer<T>::addVertices(unsigned int nVertices)
{
	unsigned int baseId = Base::vec.size();
	T* oldB = Base::vec.data();
	Base::vec.resize(Base::vec.size() + nVertices);
	T* newB = Base::vec.data();
	vn+=nVertices;
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		Base::optionalVec.resize(Base::vec.size());
	}
	for (unsigned int i = baseId; i < Base::vec.size(); ++i){
		if constexpr (vert::hasOptionalInfo<VertexType>()) {
			setContainerPointer(Base::vec[i]);
		}
	}
	updateAfterAllocation(oldB, newB);
	return baseId;
}

template<typename T>
void VertexContainer<T>::reserveVertices(unsigned int size)
{
	T* oldB = Base::vec.data();
	Base::vec.reserve(size);
	T* newB = Base::vec.data();
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		Base::optionalVec.reserve(size);
	}
	updateAfterAllocation(oldB, newB);
}

template<typename T>
void mgp::mesh::VertexContainer<T>::setContainerPointer(VertexType& v)
{
	v.setContainerPointer((Base*)this);
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
std::vector<int> mgp::mesh::VertexContainer<T>::compactVertices()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	std::vector<int> newIndices(Base::vec.size());
	unsigned int k = 0;
	for (unsigned int i = 0; i < Base::vec.size(); ++i){
		if (!Base::vec[i].isDeleted()){
			Base::vec[k] = Base::vec[i];
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = -1;
		}
	}
	Base::vec.resize(k);
	T* base = Base::vec.data();
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		Base::optionalVec.compact(newIndices);
	}
	updateVertexReferencesAfterCompact(base, newIndices);
	return newIndices;
}

template<typename T>
void VertexContainer<T>::updateAfterAllocation(const T* oldBase, const T* newBase)
{
	if (oldBase != newBase) {
		updateContainerPointers();
		updateVertexReferences(oldBase, newBase);
	}
}

/**
 * @brief After a reallocation, it is needed always to update the container pointers of all the
 * elements, because the assignment operator of the OptionalInfo component (which stores the pointer
 * of the container) does not copy the container pointer for security reasons.
 */
template<typename T>
void VertexContainer<T>::updateContainerPointers()
{
	if constexpr (vert::hasOptionalInfo<VertexType>()) {
		// all the vertices must point to the right container - also the deleted ones
		for (VertexType& v : vertices(false)) {
			setContainerPointer(v);
		}
	}
}

template<typename T>
void VertexContainer<T>::updateVertexReferences(const T* oldBase, const T* newBase)
{
	if constexpr (mgp::vert::hasAdjacentVertices<T>()) {
		for (VertexType& v : vertices()) {
			v.updateVertexReferences(oldBase, newBase);
		}
	}
	else if constexpr(mgp::vert::hasOptionalAdjacentVertices<T>()) {
		if (Base::optionalVec.isAdjacentVerticesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateVertexReferences(oldBase, newBase);
			}
		}
	}
}

template<typename T>
void VertexContainer<T>::updateVertexReferencesAfterCompact(
	const T* base,
	const std::vector<int>& newIndices)
{
	if constexpr (mgp::vert::hasAdjacentVertices<T>()) {
		for (VertexType& v : vertices()) {
			v.updateVertexReferencesAfterCompact(base, newIndices);
		}
	}
	else if constexpr (mgp::vert::hasOptionalAdjacentVertices<T>()){
		if (Base::optionalVec.isAdjacentVerticesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateVertexReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

template<typename T>
template<typename  Face>
void VertexContainer<T>::updateFaceReferences(const Face* oldBase, const Face* newBase)
{
	if constexpr (mgp::vert::hasAdjacentFaces<T>()) {
		for (VertexType& v : vertices()) {
			v.updateFaceReferences(oldBase, newBase);
		}
	}
	else if constexpr (mgp::vert::hasOptionalAdjacentFaces<T>()){
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateFaceReferences(oldBase, newBase);
			}
		}
	}
}

template<typename T>
template<typename Face>
void VertexContainer<T>::updateFaceReferencesAfterCompact(
	const Face* base,
	const std::vector<int>& newIndices)
{
	if constexpr (mgp::vert::hasAdjacentFaces<T>()) {
		for (VertexType& v : vertices()) {
			v.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
	else if constexpr (mgp::vert::hasOptionalAdjacentFaces<T>()){
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

} // namespace mgp::mesh
