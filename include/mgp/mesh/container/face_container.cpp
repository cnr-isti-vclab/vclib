/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_container.h"

namespace mgp::mesh {

/**
 * @brief Container::Container Empty constructor that creates an empty container of Faces.
 */
template<typename T>
Container<T, IfIsFace<T>>::Container()
{
}

/**
 * @brief Container::face Returns a const reference of the face at the i-th position
 * in the Face Container of the Mesh, which will be the face having id = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param i: the id of the face that will be returned.
 */
template<typename T>
const typename Container<T, IfIsFace<T>>::FaceType&
Container<T, IfIsFace<T>>::face(unsigned int i) const
{
	return facesVec[i];
}

/**
 * @brief Container::face Returns a reference of the face at the i-th position
 * in the Face Container of the Mesh, which will be the face having id = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param i: the id of the face that will be returned.
 */
template<typename T>
typename Container<T, IfIsFace<T>>::FaceType&
Container<T, IfIsFace<T>>::face(unsigned int i)
{
	return facesVec[i];
}

/**
 * @brief Container::faceNumber Returns the number of **non-deleted** faces contained in the
 * Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return the number of non-deleted faces of the Mesh.
 */
template<typename T>
unsigned int Container<T, IfIsFace<T>>::faceNumber() const
{
	return fn;
}

/**
 * @brief Container::faceContainerSize Returns the number of faces (also deleted) contained in
 * the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return the number of all the faces contained in the Mesh.
 */
template<typename T>
unsigned int Container<T, IfIsFace<T>>::faceContainerSize() const
{
	return facesVec.size();
}

template<typename T>
unsigned int Container<T, IfIsFace<T> >::deletedFaceNumber() const
{
	return faceContainerSize() - faceNumber();
}

/**
 * @brief Container::deleteFace Marks as deleted the face with the given id.
 *
 * This member function does not perform any reallocation of the faces: the deleted faces
 * will stay in the Face Container, but will be marked as deleted.
 *
 * Deleted faces are automatically jumped by the iterators provided by the Face Container.
 *
 * @param i: the id of the face that will be marked as deleted.
 */
template<typename T>
void Container<T, IfIsFace<T> >::deleteFace(unsigned int i)
{
	facesVec[i].setDeleted();
	fn--;
}

/**
 * @brief Container::faceIdIfCompact is an utility member functions that returns the id of
 * an element if the container would be compact, that is the number of non-deleted elements before
 * the face with the given id.
 *
 * Complexity: O(n), n number of faces in the container.
 *
 * This function does not perform any sanity check.
 *
 * @param id
 * @return
 */
template<typename T>
unsigned int Container<T, IfIsFace<T> >::faceIdIfCompact(unsigned int id) const
{
	if (facesVec.size() == fn)
		return id;
	else {
		unsigned int cnt = 0;
		for (unsigned int i = 0; i < id; i++){
			if (!facesVec[i].isDeleted())
				++cnt;
		}
		return cnt;
	}
}

template<typename T>
typename Container<T, IfIsFace<T>>::FaceIterator
Container<T, IfIsFace<T>>::faceBegin(bool jumpDeleted)
{
	auto it = facesVec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted faces, and the first face is deleted, we need
		// to move forward until we find the first non-deleted face
		while (it != facesVec.end() && it->isDeleted()) {
			++it;
		}
	}
	return FaceIterator(it, facesVec, jumpDeleted);
}

template<typename T>
typename Container<T, IfIsFace<T>>::FaceIterator Container<T, IfIsFace<T>>::faceEnd()
{
	return FaceIterator(facesVec.end(), facesVec);
}

template<typename T>
typename Container<T, IfIsFace<T>>::ConstFaceIterator
Container<T, IfIsFace<T>>::faceBegin(bool jumpDeleted) const
{
	if (jumpDeleted) {
		auto it = facesVec.begin();
		while (it != facesVec.end() && it->isDeleted()) {
			++it;
		}
		return ConstFaceIterator(it, facesVec, jumpDeleted);
	}
	else
		return ConstFaceIterator(facesVec.begin(), facesVec, jumpDeleted);
}

template<typename T>
typename Container<T, IfIsFace<T>>::ConstFaceIterator Container<T, IfIsFace<T>>::faceEnd() const
{
	return ConstFaceIterator(facesVec.end(), facesVec);
}

template<typename T>
typename Container<T, IfIsFace<T>>::FaceRangeIterator
Container<T, IfIsFace<T>>::faces(bool jumpDeleted)
{
	return FaceRangeIterator(
		*this, jumpDeleted, &FaceContainer::faceBegin, &FaceContainer::faceEnd);
}

template<typename T>
typename Container<T, IfIsFace<T>>::ConstFaceRangeIterator
Container<T, IfIsFace<T>>::faces(bool jumpDeleted) const
{
	return ConstFaceRangeIterator(
		*this, jumpDeleted, &FaceContainer::faceBegin, &FaceContainer::faceEnd);
}

/**
 * @brief Container::enableFaceAdjacentFaces enable the Optional Adjacent Faces of the face.
 * @note This function is available **only if the Face Element has the OptionalAdjacentFaces
 * Component**.
 *
 * @note If the size of the Face is dynamic (N < 0), when enabled, the adjacent faces number will be
 * the same of the vertex number for each face of the container. This is because, for Faces,
 * Adjacent Faces number is tied to the number of vertices.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalAdjacentFaces<U, void>
Container<T, IfIsFace<T>>::enablePerFaceAdjacentFaces()
{
	OptionalFaceContainer::enablePerFaceAdjacentFaces();
	static const int N = T::VERTEX_NUMBER;
	if (N < 0) {
		for (T& f : faces()) {
			f.resizeAdjFaces(f.vertexNumber());
		}
	}
}

template<typename T>
void mgp::mesh::Container<T, IfIsFace<T> >::clearFaces()
{
	facesVec.clear();
	fn = 0;
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::clear();
	}
}

template<typename T>
unsigned int Container<T, IfIsFace<T>>::addFace()
{
	T* oldB = facesVec.data();
	facesVec.push_back(FaceType());
	T* newB = facesVec.data();
	++fn;
	facesVec[facesVec.size() - 1]._id = facesVec.size() - 1;
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::setContainerPointer(facesVec[facesVec.size() - 1]);
		OptionalFaceContainer::resize(facesVec.size());
	}
	updateFaceReferences(oldB, newB);
	return facesVec[facesVec.size() - 1]._id;
}

/**
 * @brief Container::addFaces adds nFaces to the Face Container of the mesh.
 *
 * Returns the id of the first added face.
 *
 * @param nFaces
 * @return the id of the first added face.
 */
template<typename T>
unsigned int mgp::mesh::Container<T, IfIsFace<T> >::addFaces(unsigned int nFaces)
{
	unsigned int baseId = facesVec.size();
	T* oldB = facesVec.data();
	facesVec.resize(facesVec.size() + nFaces);
	T* newB = facesVec.data();
	fn += nFaces;
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::resize(facesVec.size());
	}
	for (unsigned int i = baseId; i < facesVec.size(); ++i){
		facesVec[i]._id = i;
		if constexpr (face::hasOptionalInfo<FaceType>()) {
			OptionalFaceContainer::setContainerPointer(facesVec[i]);
		}
	}
	updateFaceReferences(oldB, newB);
	return baseId;
}

template<typename T>
void Container<T, IfIsFace<T>>::reserveFaces(unsigned int size)
{
	T* oldB = facesVec.data();
	facesVec.reserve(size);
	T* newB = facesVec.data();
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::reserve(size);
	}
	updateFaceReferences(oldB, newB);
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
std::vector<int> mgp::mesh::Container<T, IfIsFace<T> >::compactFaces()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	std::vector<int> newIndices(facesVec.size());
	unsigned int k = 0;
	for (unsigned int i = 0; i < facesVec.size(); ++i){
		if (!facesVec[i].isDeleted()){
			facesVec[k] = facesVec[i];
			facesVec[k]._id = k;
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = -1;
		}
	}
	facesVec.resize(k);
	T* base = facesVec.data();
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::compact(newIndices);
	}
	updateFaceReferencesAfterCompact(base, newIndices);
	return newIndices;
}

template<typename T>
void Container<T, IfIsFace<T> >::updateFaceReferences(const T* oldBase, const T* newBase)
{
	if constexpr (mgp::face::hasAdjacentFaces<T>()) {
		for (FaceType& f : faces()) {
			f.updateFaceReferences(oldBase, newBase);
		}
	}
	else if constexpr(mgp::face::hasOptionalAdjacentFaces<T>()) {
		if (OptionalFaceContainer::isPerFaceAdjacentFacesEnabled()) {
			for (FaceType& f : faces()) {
				f.updateFaceReferences(oldBase, newBase);
			}
		}
	}
}

template<typename T>
void Container<T, IfIsFace<T> >::updateFaceReferencesAfterCompact(
	const T* base,
	const std::vector<int>& newIndices)
{
	if constexpr (mgp::face::hasAdjacentFaces<T>()) {
		for (FaceType& f : faces()) {
			f.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
	else if constexpr(mgp::face::hasOptionalAdjacentFaces<T>()) {
		if (OptionalFaceContainer::isPerFaceAdjacentFacesEnabled()) {
			for (FaceType& f : faces()) {
				f.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

template<typename T>
template<typename Vertex>
void Container<T, IfIsFace<T>>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	for (FaceType& f : faces()) {
		f.updateVertexReferences(oldBase, newBase);
	}
}

template<typename T>
template<typename Vertex>
void Container<T, IfIsFace<T>>::updateVertexReferencesAfterCompact(
	const Vertex* base,
	const std::vector<int>& newIndices)
{
	for (FaceType& f : faces()) {
		f.updateVertexReferencesAfterCompact(base, newIndices);
	}
}

} // namespace mgp::mesh
