/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_container.h"

namespace mgp::mesh {

/**
 * @brief Container::Container Empty constructor that creates an empty container of Faces.
 */
template<class T>
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
template<class T>
const typename Container<T, IfIsFace<T>>::FaceType&
Container<T, IfIsFace<T>>::face(unsigned int i) const
{
	return faces[i];
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
template<class T>
typename Container<T, IfIsFace<T>>::FaceType&
Container<T, IfIsFace<T>>::face(unsigned int i)
{
	return faces[i];
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
template<class T>
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
template<class T>
unsigned int Container<T, IfIsFace<T>>::faceContainerSize() const
{
	return faces.size();
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
template<class T>
void Container<T, IfIsFace<T> >::deleteFace(unsigned int i)
{
	faces[i].setDeleted();
	fn--;
}

template<class T>
typename Container<T, IfIsFace<T>>::FaceIterator
Container<T, IfIsFace<T>>::faceBegin(bool jumpDeleted)
{
	auto it = faces.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted faces, and the first face is deleted, we need
		// to move forward until we find the first non-deleted face
		while (it != faces.end() && it->isDeleted()) {
			++it;
		}
	}
	return FaceIterator(it, faces, jumpDeleted);
}

template<class T>
typename Container<T, IfIsFace<T>>::FaceIterator Container<T, IfIsFace<T>>::faceEnd()
{
	return FaceIterator(faces.end(), faces);
}

template<class T>
typename Container<T, IfIsFace<T>>::ConstFaceIterator
Container<T, IfIsFace<T>>::faceBegin(bool jumpDeleted) const
{
	if (jumpDeleted) {
		auto it = faces.begin();
		while (it->isDeleted()) {
			++it;
		}
		return ConstFaceIterator(it, faces, jumpDeleted);
	}
	else
		return ConstFaceIterator(faces.begin(), faces, jumpDeleted);
}

template<class T>
typename Container<T, IfIsFace<T>>::ConstFaceIterator Container<T, IfIsFace<T>>::faceEnd() const
{
	return ConstFaceIterator(faces.end(), faces);
}

template<class T>
typename Container<T, IfIsFace<T>>::FaceRangeIterator
Container<T, IfIsFace<T>>::faceIterator(bool jumpDeleted)
{
	return FaceRangeIterator(
		*this, jumpDeleted, &FaceContainer::faceBegin, &FaceContainer::faceEnd);
}

template<class T>
typename Container<T, IfIsFace<T>>::ConstFaceRangeIterator
Container<T, IfIsFace<T>>::faceIterator(bool jumpDeleted) const
{
	return ConstFaceRangeIterator(
		*this, jumpDeleted, &FaceContainer::faceBegin, &FaceContainer::faceEnd);
}

template<class T>
unsigned int Container<T, IfIsFace<T>>::addFace()
{
	faces.push_back(FaceType());
	++fn;
	faces[faces.size() - 1]._id = faces.size() - 1;
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::setContainerPointer(faces[faces.size() - 1]);
		OptionalFaceContainer::resize(faces.size());
	}
	return faces[faces.size() - 1]._id;
}

template<class T>
void Container<T, IfIsFace<T>>::reserveFaces(unsigned int size)
{
	faces.reserve(size);
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::reserve(size);
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
std::vector<int> mgp::mesh::Container<T, IfIsFace<T> >::compactFaces()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	std::vector<int> newIndices;
	unsigned int k = 0;
	for (unsigned int i = 0; i < faces.size(); ++i){
		if (!faces[i].isDeleted()){
			faces[k] = faces[i];
			faces[k]._id = k;
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = -1;
		}
	}
	faces.resize(k);
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		OptionalFaceContainer::compact(newIndices);
	}
	return newIndices;
}

template<class T>
template<class Vertex>
void Container<T, IfIsFace<T>>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	for (FaceType& f : faceIterator()) {
		f.updateVertexReferences(oldBase, newBase);
	}
}

template<class T>
template<class Vertex>
void Container<T, IfIsFace<T>>::updateVertexReferencesAfterCompact(
	const Vertex* base,
	const std::vector<int>& newIndices)
{
	for (FaceType& f : faceIterator()) {
		f.updateVertexReferencesAfterCompact(base, newIndices);
	}
}

} // namespace mgp::mesh
