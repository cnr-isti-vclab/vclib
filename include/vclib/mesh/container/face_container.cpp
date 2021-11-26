/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
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

#include "face_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Faces.
 */
template<typename T>
FaceContainer<T>::FaceContainer()
{
}

/**
 * @brief Returns a const reference of the face at the i-th position
 * in the Face Container of the Mesh, which will be the face having id = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param i: the id of the face that will be returned.
 */
template<typename T>
const typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i) const
{
	return Base::vec[i];
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
typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i)
{
	return Base::vec[i];
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
uint FaceContainer<T>::faceNumber() const
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
uint FaceContainer<T>::faceContainerSize() const
{
	return Base::vec.size();
}

template<typename T>
uint FaceContainer<T>::deletedFaceNumber() const
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
void FaceContainer<T>::deleteFace(uint i)
{
	Base::vec[i].setDeleted();
	fn--;
}

template<typename T>
void FaceContainer<T>::deleteFace(const FaceType* f)
{
	deleteFace(index(f));
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the face with the given id.
 *
 * Complexity: O(n), n number of faces in the container.
 *
 * This function does not perform any sanity check.
 *
 * @param id
 * @return
 */
template<typename T>
uint FaceContainer<T>::faceIndexIfCompact(uint id) const
{
	if (Base::vec.size() == fn)
		return id;
	else {
		uint cnt = 0;
		for (uint i = 0; i < id; i++) {
			if (!Base::vec[i].isDeleted())
				++cnt;
		}
		return cnt;
	}
}

/**
 * @brief Returns a vector that tells, for each actual face index, the new index that the face
 * would have in a compacted container. For each deleted face index, the value of the vector will
 * be -1.
 *
 * This is useful if you need to know the indices of the faces that they would have in a
 * compact container, without considering the deleted ones.
 * @return
 */
template<typename T>
std::vector<int> FaceContainer<T>::faceCompactIndices() const
{
	std::vector<int> newIndices(Base::vec.size());
	uint             k = 0;
	for (uint i = 0; i < Base::vec.size(); ++i) {
		if (!Base::vec[i].isDeleted()) {
			newIndices[i] = k;
			k++;
		}
		else {
			newIndices[i] = -1;
		}
	}
	return newIndices;
}

template<typename T>
typename FaceContainer<T>::FaceIterator FaceContainer<T>::faceBegin(bool jumpDeleted)
{
	auto it = Base::vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted faces, and the first face is deleted, we need
		// to move forward until we find the first non-deleted face
		while (it != Base::vec.end() && it->isDeleted()) {
			++it;
		}
	}
	return FaceIterator(it, Base::vec, jumpDeleted && Base::vec.size() != fn);
}

template<typename T>
typename FaceContainer<T>::FaceIterator FaceContainer<T>::faceEnd()
{
	return FaceIterator(Base::vec.end(), Base::vec);
}

template<typename T>
typename FaceContainer<T>::ConstFaceIterator FaceContainer<T>::faceBegin(bool jumpDeleted) const
{
	auto it = Base::vec.begin();
	if (jumpDeleted) {
		// if the user asked to jump the deleted faces, and the first face is deleted, we need
		// to move forward until we find the first non-deleted face
		while (it != Base::vec.end() && it->isDeleted()) {
			++it;
		}
	}
	return ConstFaceIterator(it, Base::vec, jumpDeleted && Base::vec.size() != fn);
}

template<typename T>
typename FaceContainer<T>::ConstFaceIterator FaceContainer<T>::faceEnd() const
{
	return ConstFaceIterator(Base::vec.end(), Base::vec);
}

template<typename T>
typename FaceContainer<T>::FaceRangeIterator FaceContainer<T>::faces(bool jumpDeleted)
{
	return FaceRangeIterator(
		*this,
		jumpDeleted && Base::vec.size() != fn,
		&FaceContainer::faceBegin,
		&FaceContainer::faceEnd);
}

template<typename T>
typename FaceContainer<T>::ConstFaceRangeIterator FaceContainer<T>::faces(bool jumpDeleted) const
{
	return ConstFaceRangeIterator(
		*this,
		jumpDeleted && Base::vec.size() != fn,
		&FaceContainer::faceBegin,
		&FaceContainer::faceEnd);
}

/**
 * @brief Checks if the face Optional Adjacent
 * Faces is enabled. This function is available **only if the Face Element has the
 * OptionalAdjacentFaces Component**.
 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalAdjacentFaces<U>(), bool)
FaceContainer<T>::isPerFaceAdjacentFacesEnabled() const
{
	return Base::optionalVec.isAdjacentFacesEnabled();
}

/**
 * @brief Container::enableFaceAdjacentFaces enable the Optional Adjacent Faces of the face.
 *
 * This function is available **only if the Face Element has the OptionalAdjacentFaces Component**.
 *
 * If the Face is polygonal (dynamic size, N < 0), when enabled, the adjacent faces number will be
 * the same of the vertex number for each face of the container. This is because, for Faces,
 * Adjacent Faces number is tied to the number of vertices.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalAdjacentFaces<U>(), void)
FaceContainer<T>::enablePerFaceAdjacentFaces()
{
	Base::optionalVec.enableAdjacentFaces(Base::vec.size());
	static const int N = T::VERTEX_NUMBER;
	if constexpr (N < 0) {
		for (T& f : faces()) {
			f.resizeAdjFaces(f.vertexNumber());
		}
	}
}

/**
 * @brief Disables the Optional Adjacent Faces of the face.
 * This function is available **only if the Face Element has the OptionalAdjacentFaces Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalAdjacentFaces<U>(), void)
FaceContainer<T>::disablePerFaceAdjacentFaces()
{
	Base::optionalVec.disableAdjacentFaces();
}

/**
 * @brief Checks if the face Optional Color is
 * enabled. This function is available **only if the Face Element has the OptionalColor
 * component**.
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalColor<U>(), bool)
FaceContainer<T>::isPerFaceColorEnabled() const
{
	return Base::optionalVec.isColorEnabled();
}

/**
 * @brief Enables the Optional Color of the face.
 * This function is available **only if the Face Element has the OptionalColor Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalColor<U>(), void)
FaceContainer<T>::enablePerFaceColor()
{
	Base::optionalVec.enableColor(faceContainerSize());
}

/**
 * @brief Disables the Optional Color of the face.
 * This function is available **only if the Face Element has the OptionalColor Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalColor<U>(), void)
FaceContainer<T>::disablePerFaceColor()
{
	Base::optionalVec.disableColor();
}

/**
 * @brief Checks if the face Optional Mark is
 * enabled. This function is available **only if the Face Element has the OptionalMark
 * component**.
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMark<U>(), bool)
FaceContainer<T>::isPerFaceMarkEnabled() const
{
	return Base::optionalVec.isMarkEnabled();
}

/**
 * @brief Enables the Optional Mark of the face.
 * This function is available **only if the Face Element has the OptionalMark Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMark<U>(), void)
FaceContainer<T>::enablePerFaceMark()
{
	Base::optionalVec.enableMark(faceContainerSize());
}

/**
 * @brief Disables the Optional Mark of the face.
 * This function is available **only if the Face Element has the OptionalMark Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMark<U>(), void)
FaceContainer<T>::disablePerFaceMark()
{
	Base::optionalVec.disableMark();
}

/**
 * @brief Checks if the face Optional
 * Mutable Bit Flags is enabled. This function is available **only if the Face Element has the
 * OptionalMutableBitFlags Component**.
 * @return true if the Optional Mutable Bit Flags component is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), bool)
FaceContainer<T>::isPerFaceMutableBitFlagsEnabled() const
{
	Base::optionalVec.isMutableBitFlagsEnabled();
}

/**
 * @brief Enables the Optional Mutable Flags of the face.
 * This function is available **only if the Face Element has the OptionalMutableFlags Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), void)
FaceContainer<T>::enablePerFaceMutableBitFlags()
{
	Base::optionalVec.enableMutableBitFlags(faceContainerSize());
}

/**
 * @brief Disables the Optional Mutable Flags of the face.
 * This function is available **only if the Face Element has the OptionalMutableBitFlags
 * Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), void)
FaceContainer<T>::disablePerFaceMutableBitFlags()
{
	Base::optionalVec.disableMutableBitFlags();
}

/**
 * @brief Checks if the face Optional Normal is
 * enabled. This function is available **only if the Face Element has the OptionalNormal
 * Component**.
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalNormal<U>(), bool)
FaceContainer<T>::isPerFaceNormalEnabled() const
{
	return Base::optionalVec.isNormalEnabled();
}

/**
 * @brief Enables the Optional Normal of the face.
 * This function is available **only if the Face Element has the OptionalNormal Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalNormal<U>(), void)
FaceContainer<T>::enablePerFaceNormal()
{
	Base::optionalVec.enableNormal(faceContainerSize());
}

/**
 * @brief Disables the Optional Normal of the face.
 * This function is available **only if the Face Element has the OptionalNormal Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalNormal<U>(), void)
FaceContainer<T>::disablePerFaceNormal()
{
	Base::optionalVec.disableNormal();
}

/**
 * @brief Checks if the face Optional PrincipalCurvature is
 * enabled. This function is available **only if the Face Element has the OptionalPrincipalCurvature
 * Component**.
 * @return true if the Optional PrincipalCurvature is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalPrincipalCurvature<U>(), bool)
FaceContainer<T>::isPerFacePrincipalCurvatureEnabled() const
{
	return Base::optionalVec.isPrincipalCurvatureEnabled();
}
/**
 * @brief Enables the Optional PrincipalCurvature of the face.
 * This function is available **only if the Face Element has the OptionalPrincipalCurvature
 * Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalPrincipalCurvature<U>(), void)
FaceContainer<T>::enablePerFacePrincipalCurvature()
{
	Base::optionalVec.enablePrincipalCurvature(faceContainerSize());
}

/**
 * @brief Disables the Optional PrincipalCurvature of the face.
 * This function is available **only if the Face Element has the OptionalPrincipalCurvature
 * Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalPrincipalCurvature<U>(), void)
FaceContainer<T>::disablePerFacePrincipalCurvature()
{
	Base::optionalVec.disablePrincipalCurvature();
}

/**
 * @brief Checks if the face Optional Scalar is
 * enabled. This function is available **only if the Face Element has the OptionalScalar
 * Component**.
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalScalar<U>(), bool)
FaceContainer<T>::isPerFaceScalarEnabled() const
{
	return Base::optionalVec.isScalarEnabled();
}
/**
 * @brief Enables the Optional Scalar of the face.
 * This function is available **only if the Face Element has the OptionalScalar Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalScalar<U>(), void)
FaceContainer<T>::enablePerFaceScalar()
{
	Base::optionalVec.enableScalar(faceContainerSize());
}

/**
 * @brief Disables the Optional Scalar of the face.
 * This function is available **only if the Face Element has the OptionalScalar Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalScalar<U>(), void)
FaceContainer<T>::disablePerFaceScalar()
{
	Base::optionalVec.disableScalar();
}

/**
 * @brief Checks if the face Optional WedgeColors
 * Faces is enabled. This function is available **only if the Face Element has the
 * OptionalWedgeColors Component**.
 * @return true if the Optional WedgeColors is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasWedgeColors<U>(), bool)
FaceContainer<T>::isPerFaceWedgeColorsEnabled() const
{
	return Base::optionalVec.isWedgeColorsEnabled();
}

/**
 * @brief Enable the Optional Wedge Colors of the face.
 * This function is available **only if the Face Element has the OptionalWedgeColors Component**.
 *
 * If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge colors number will be
 * the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge Colors number is tied to the number of vertices.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasWedgeColors<U>(), void)
FaceContainer<T>::enablePerFaceWedgeColors()
{
	Base::optionalVec.enableWedgeColors(Base::vec.size());
	static const int N = T::VERTEX_NUMBER;
	if (N < 0) {
		for (T& f : faces()) {
			f.resizeWedgeColors(f.vertexNumber());
		}
	}
}

/**
 * @brief Disables the Optional Wedge Colors of the face.
 * This function is available **only if the Face Element has the OptionalWedgeColors Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasWedgeColors<U>(), void)
FaceContainer<T>::disablePerFaceWedgeColors()
{
	Base::optionalVec.disableWedgeColors();
}

/**
 * @brief Checks if the face Optional WedgeTexCoords
 * Faces is enabled. This function is available **only if the Face Element has the
 * OptionalWedgeTexCoords Component**.
 * @return true if the Optional WedgeTexCoords is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasWedgeTexCoords<U>(), bool)
FaceContainer<T>::isPerFaceWedgeTexCoordsEnabled() const
{
	return Base::optionalVec.isWedgeTexCoordsEnabled();
}

/**
 * @brief Enables the Optional Wedge TexCoords of the face.
 *  This function is available **only if the Face Element has the OptionalWedgeTexCoords
 * Component**.
 *
 * If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge texcoord number will be
 * the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge TexCoords number is tied to the number of vertices.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasWedgeTexCoords<U>(), void)
FaceContainer<T>::enablePerFaceWedgeTexCoords()
{
	Base::optionalVec.enableWedgeTexCoords(Base::vec.size());
	static const int N = T::VERTEX_NUMBER;
	if (N < 0) {
		for (T& f : faces()) {
			f.resizeWedgeTexCoords(f.vertexNumber());
		}
	}
}

/**
 * @brief Disables the Optional WedgeTex Coords of the face.
 * This function is available **only if the Face Element has the OptionalWedgeTexCoords Component**.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasWedgeTexCoords<U>(), void)
FaceContainer<T>::disablePerFaceWedgeTexCoords()
{
	Base::optionalVec.disableWedgeTexCoords();
}

template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(face::hasCustomComponents<U>(), void)
FaceContainer<T>::addPerFaceCustomComponent(const std::string& name)
{
	Base::optionalVec.template addNewComponent<K>(name, faceContainerSize());
}

template<typename T>
uint FaceContainer<T>::index(const FaceType* f) const
{
	assert(!Base::vec.empty() && f >= Base::vec.data() && f <= &Base::vec.back());
	return f - Base::vec.data();
}

template<typename T>
void vcl::mesh::FaceContainer<T>::clearFaces()
{
	Base::vec.clear();
	fn = 0;
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		Base::optionalVec.clear();
	}
}

template<typename T>
uint FaceContainer<T>::addFace()
{
	T* oldB = Base::vec.data();
	Base::vec.push_back(FaceType());
	T* newB = Base::vec.data();
	++fn;
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		setContainerPointer(Base::vec[Base::vec.size() - 1]);
		Base::optionalVec.resize(Base::vec.size());
	}
	updateAfterAllocation(oldB, newB);
	return Base::vec.size() - 1;
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
uint vcl::mesh::FaceContainer<T>::addFaces(uint nFaces)
{
	uint baseId = Base::vec.size();
	T*   oldB   = Base::vec.data();
	Base::vec.resize(Base::vec.size() + nFaces);
	T* newB = Base::vec.data();
	fn += nFaces;
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		Base::optionalVec::resize(Base::vec.size());
	}
	for (uint i = baseId; i < Base::vec.size(); ++i) {
		Base::vec[i]._id = i;
		if constexpr (face::hasOptionalInfo<FaceType>()) {
			setContainerPointer(Base::vec[i]);
		}
	}
	updateAfterAllocation(oldB, newB);
	return baseId;
}

template<typename T>
void FaceContainer<T>::reserveFaces(uint size)
{
	T* oldB = Base::vec.data();
	Base::vec.reserve(size);
	T* newB = Base::vec.data();
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		Base::optionalVec.reserve(size);
	}
	updateAfterAllocation(oldB, newB);
}

template<typename T>
void FaceContainer<T>::setContainerPointer(FaceType& f)
{
	f.setContainerPointer((Base*) this);
}

/**
 * @brief compacts the face container, keeping only the non-deleted faces.
 *
 * @return a vector that tells, for each old face index, the new index of the face. Will contain -1
 * if the face has been deleted.
 */
template<typename T>
std::vector<int> vcl::mesh::FaceContainer<T>::compactFaces()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	uint k = 0;

	std::vector<int> newIndices = faceCompactIndices();
	for (uint i = 0; i < newIndices.size(); ++i) {
		if (newIndices[i] >= 0) {
			k = newIndices[i];
			if (i != k)
				Base::vec[k] = Base::vec[i];
		}
	}
	k++;
	Base::vec.resize(k);
	T* base = Base::vec.data();
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		Base::optionalVec.compact(newIndices);
	}
	updateFaceReferencesAfterCompact(base, newIndices);
	return newIndices;
}

template<typename T>
void FaceContainer<T>::updateAfterAllocation(const T* oldBase, const T* newBase)
{
	if (oldBase != newBase) {      // if has been reallocated
		updateContainerPointers(); // update all container pointers
		updateFaceReferences(oldBase, newBase);
	}
}

/**
 * @brief After a reallocation, it is needed always to update the container pointers of all the
 * elements, because the assignment operator of the OptionalInfo component (which stores the pointer
 * of the container) does not copy the container pointer for security reasons.
 */
template<typename T>
void FaceContainer<T>::updateContainerPointers()
{
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		// all the faces must point to the right container - also the deleted ones
		for (FaceType& f : faces(false)) {
			setContainerPointer(f);
		}
	}
}

template<typename T>
void FaceContainer<T>::updateFaceReferences(const T* oldBase, const T* newBase)
{
	if constexpr (vcl::face::hasAdjacentFaces<T>()) {
		for (FaceType& f : faces()) {
			f.updateFaceReferences(oldBase, newBase);
		}
	}
	else if constexpr (vcl::face::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (FaceType& f : faces()) {
				f.updateFaceReferences(oldBase, newBase);
			}
		}
	}
}

template<typename T>
void FaceContainer<T>::updateFaceReferencesAfterCompact(
	const T*                base,
	const std::vector<int>& newIndices)
{
	if constexpr (vcl::face::hasAdjacentFaces<T>()) {
		for (FaceType& f : faces()) {
			f.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
	else if constexpr (vcl::face::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (FaceType& f : faces()) {
				f.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
}

template<typename T>
template<typename Vertex>
void FaceContainer<T>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	for (FaceType& f : faces()) {
		f.updateVertexReferences(oldBase, newBase);
	}
}

template<typename T>
template<typename Vertex>
void FaceContainer<T>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	for (FaceType& f : faces()) {
		f.updateVertexReferencesAfterCompact(base, newIndices);
	}
}

} // namespace vcl::mesh
