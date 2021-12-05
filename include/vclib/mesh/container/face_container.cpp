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
 * @brief Returns a const reference of the face at the i-th position in the Face Container of the
 * Mesh, which will be the face having index = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the face that will be returned.
 */
template<typename T>
const typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i) const
{
	return Base::vec[i];
}

/**
 * @brief Returns a reference of the face at the i-th position in the Face Container of the Mesh,
 * which will be the face having index = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the face that will be returned.
 */
template<typename T>
typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i)
{
	return Base::vec[i];
}

/**
 * @brief Returns the number of **non-deleted** faces contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return The number of non-deleted faces of the Mesh.
 */
template<typename T>
uint FaceContainer<T>::faceNumber() const
{
	return fn;
}

/**
 * @brief Returns the number of faces (also deleted) contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return The number of all the faces contained in the Mesh.
 */
template<typename T>
uint FaceContainer<T>::faceContainerSize() const
{
	return Base::vec.size();
}

/**
 * @brief Returns the number of deleted faces in the Face container, that is faceContainerSize() -
 * faceNumber().
 *
 * @return The number of deleted faces in the container.
 */
template<typename T>
uint FaceContainer<T>::deletedFaceNumber() const
{
	return faceContainerSize() - faceNumber();
}

/**
 * @brief Marks as deleted the face with the given id.
 *
 * This member function does not perform any reallocation of the faces: the deleted faces
 * will stay in the Face Container, but will be marked as deleted.
 *
 * Deleted faces are automatically jumped by the iterators provided by the Face Container.
 *
 * @param[in] i: the id of the face that will be marked as deleted.
 */
template<typename T>
void FaceContainer<T>::deleteFace(uint i)
{
	Base::vec[i].setDeleted();
	fn--;
}

/**
 * @brief Marks as deleted the given face, before asserting that the face belongs to this container.
 *
 * This member function does not perform any reallocation of the faces: the deleted faces
 * will stay in the Face Container, but will be marked as deleted.
 *
 * Deleted faces are automatically jumped by the iterators provided by the Face Container.
 *
 * @param[in] f: the pointer of the face that will be marked as deleted.
 */
template<typename T>
void FaceContainer<T>::deleteFace(const FaceType* f)
{
	deleteFace(index(f));
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the face with the given
 * index.
 *
 * Complexity: O(n), with n the number of faces in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a face of the container.
 * @return The index that the face with index i would have if this container would be compact.
 */
template<typename T>
uint FaceContainer<T>::faceIndexIfCompact(uint i) const
{
	if (Base::vec.size() == fn)
		return i;
	else {
		uint cnt = 0;
		for (uint ii = 0; ii < i; ii++) {
			if (!Base::vec[ii].isDeleted())
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
 *
 * @return A vector containing, for each face index, its index if the container would be compact.
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

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted faces of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return An iterator the the first face of the container.
 */
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

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<typename T>
typename FaceContainer<T>::FaceIterator FaceContainer<T>::faceEnd()
{
	return FaceIterator(Base::vec.end(), Base::vec);
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted faces of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return A const iterator the the first face of the container.
 */
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

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<typename T>
typename FaceContainer<T>::ConstFaceIterator FaceContainer<T>::faceEnd() const
{
	return ConstFaceIterator(Base::vec.end(), Base::vec);
}

/**
 * @brief Returns a small utility object that allows to iterate over the faces of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the faces using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (Face& f : m.faces()){
 *     // do something with this face
 * }
 * @endcode
 *
 * The iterator used to iterate over faces is automatically initialized to jump deleted faces of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<typename T>
typename FaceContainer<T>::FaceRangeIterator FaceContainer<T>::faces(bool jumpDeleted)
{
	return FaceRangeIterator(
		*this,
		jumpDeleted && Base::vec.size() != fn,
		&FaceContainer::faceBegin,
		&FaceContainer::faceEnd);
}

/**
 * @brief Returns a small utility object that allows to iterate over the faces of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the faces using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (const Face& f : m.faces()){
 *     // do something with this face
 * }
 * @endcode
 *
 * The iterator used to iterate over faces is automatically initialized to jump deleted faces of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted faces.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
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
 * @brief Enables all the optional components associated to the Face type contained in the
 * FaceContainer.
 */
template<typename T>
void FaceContainer<T>::enableAllPerFaceOptionalComponents()
{
	if constexpr (face::hasOptionalAdjacentEdges<T>())
		enablePerFaceAdjacentEdges();
	if constexpr (face::hasOptionalAdjacentFaces<T>())
		enablePerFaceAdjacentFaces();
	if constexpr (face::hasOptionalColor<T>())
		enablePerFaceColor();
	if constexpr (face::hasOptionalMark<T>())
		enablePerFaceMark();
	if constexpr (face::hasOptionalMutableBitFlags<T>())
		enablePerFaceMutableBitFlags();
	if constexpr (face::hasOptionalNormal<T>())
		enablePerFaceNormal();
	if constexpr (face::hasOptionalPrincipalCurvature<T>())
		enablePerFacePrincipalCurvature();
	if constexpr (face::hasOptionalScalar<T>())
		enablePerFaceScalar();
	if constexpr (face::hasOptionalWedgeColors<T>())
		enablePerFaceWedgeColors();
	if constexpr (face::hasOptionalWedgeTexCoords<T>())
		enablePerFaceWedgeTexCoords();
}

/**
 * @brief Disables all the optional components associated to the Face type contained in the
 * FaceContainer.
 */
template<typename T>
void FaceContainer<T>::disableAllPerFaceOptionalComponents()
{
	if constexpr (face::hasOptionalAdjacentEdges<T>())
		disablePerFaceAdjacentEdges();
	if constexpr (face::hasOptionalAdjacentFaces<T>())
		disablePerFaceAdjacentFaces();
	if constexpr (face::hasOptionalColor<T>())
		disablePerFaceColor();
	if constexpr (face::hasOptionalMark<T>())
		disablePerFaceMark();
	if constexpr (face::hasOptionalMutableBitFlags<T>())
		disablePerFaceMutableBitFlags();
	if constexpr (face::hasOptionalNormal<T>())
		disablePerFaceNormal();
	if constexpr (face::hasOptionalPrincipalCurvature<T>())
		disablePerFacePrincipalCurvature();
	if constexpr (face::hasOptionalScalar<T>())
		disablePerFaceScalar();
	if constexpr (face::hasOptionalWedgeColors<T>())
		disablePerFaceWedgeColors();
	if constexpr (face::hasOptionalWedgeTexCoords<T>())
		disablePerFaceWedgeTexCoords();
}

/**
 * @brief Checks if the face Optional Adjacent Edges is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 *
 * @return true if the Optional AdjacentEdges is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalAdjacentEdges<U>(), bool)
	FaceContainer<T>::isPerFaceAdjacentEdgesEnabled() const
{
	return Base::optionalVec.isAdjacentEdgesEnabled();
}

/**
 * @brief Enable the Optional Adjacent Edges of the face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the adjacent edges number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Adjacent Edges number is tied to the number of vertices.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalAdjacentEdges<U>(), void)
	FaceContainer<T>::enablePerFaceAdjacentEdges()
{
	Base::optionalVec.enableAdjacentEdges(Base::vec.size());
	static const int N = T::VERTEX_NUMBER;
	if constexpr (N < 0) {
		for (T& f : faces()) {
			f.resizeAdjEdges(f.vertexNumber());
		}
	}
}

/**
 * @brief Disables the Optional Adjacent Edges of the face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalAdjacentEdges<U>(), void)
	FaceContainer<T>::disablePerFaceAdjacentEdges()
{
	Base::optionalVec.disableAdjacentEdges();
}

/**
 * @brief Checks if the face Optional Adjacent Faces is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 *
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
 * @brief Enable the Optional Adjacent Faces of the face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the adjacent faces number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
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
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalAdjacentFaces<U>(), void)
FaceContainer<T>::disablePerFaceAdjacentFaces()
{
	Base::optionalVec.disableAdjacentFaces();
}

/**
 * @brief Checks if the face Optional Color is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalColor component.
 *
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
 *
 * @note This function is available only if the Face Element has the OptionalColor Component.
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
 *
 * @note This function is available only if the Face Element has the OptionalColor Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalColor<U>(), void)
FaceContainer<T>::disablePerFaceColor()
{
	Base::optionalVec.disableColor();
}

/**
 * @brief Checks if the face Optional Mark is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalMark component.
 *
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
 *
 * @note This function is available only if the Face Element has the OptionalMark Component.
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
 *
 * @note This function is available only if the Face Element has the OptionalMark Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMark<U>(), void)
FaceContainer<T>::disablePerFaceMark()
{
	Base::optionalVec.disableMark();
}

/**
 * @brief Checks if the face Optional Mutable Bit Flags is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalMutableBitFlags
 * Component.
 *
 * @return true if the Optional Mutable Bit Flags component is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), bool)
FaceContainer<T>::isPerFaceMutableBitFlagsEnabled() const
{
	return Base::optionalVec.isMutableBitFlagsEnabled();
}

/**
 * @brief Enables the Optional Mutable Flags of the face.
 *
 * @note This function is available only if the Face Element has the OptionalMutableFlags Component.
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
 *
 * @note This function is available only if the Face Element has the OptionalMutableBitFlags
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), void)
FaceContainer<T>::disablePerFaceMutableBitFlags()
{
	Base::optionalVec.disableMutableBitFlags();
}

/**
 * @brief Checks if the face Optional Normal is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 *
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
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
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
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalNormal<U>(), void)
FaceContainer<T>::disablePerFaceNormal()
{
	Base::optionalVec.disableNormal();
}

/**
 * @brief Checks if the face Optional PrincipalCurvature is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 *
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
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
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
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalPrincipalCurvature<U>(), void)
FaceContainer<T>::disablePerFacePrincipalCurvature()
{
	Base::optionalVec.disablePrincipalCurvature();
}

/**
 * @brief Checks if the face Optional Scalar is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalScalar Component.
 *
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
 *
 * @note This function is available only if the Face Element has the OptionalScalar Component.
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
 *
 * @note This function is available only if the Face Element has the OptionalScalar Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalScalar<U>(), void)
FaceContainer<T>::disablePerFaceScalar()
{
	Base::optionalVec.disableScalar();
}

/**
 * @brief Checks if the face Optional WedgeColors Faces is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors Component.
 *
 * @return true if the Optional WedgeColors is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeColors<U>(), bool)
FaceContainer<T>::isPerFaceWedgeColorsEnabled() const
{
	return Base::optionalVec.isWedgeColorsEnabled();
}

/**
 * @brief Enable the Optional Wedge Colors of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge colors number will
 * be the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge Colors number is tied to the number of vertices.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeColors<U>(), void)
FaceContainer<T>::enablePerFaceWedgeColors()
{
	Base::optionalVec.enableWedgeColors(Base::vec.size());
	static const int N = T::VERTEX_NUMBER;
	if constexpr(N < 0) {
		for (T& f : faces()) {
			f.resizeWedgeColors(f.vertexNumber());
		}
	}
}

/**
 * @brief Disables the Optional Wedge Colors of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeColors<U>(), void)
FaceContainer<T>::disablePerFaceWedgeColors()
{
	Base::optionalVec.disableWedgeColors();
}

/**
 * @brief Checks if the face Optional WedgeTexCoords Faces is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 * @return true if the Optional WedgeTexCoords is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeTexCoords<U>(), bool)
FaceContainer<T>::isPerFaceWedgeTexCoordsEnabled() const
{
	return Base::optionalVec.isWedgeTexCoordsEnabled();
}

/**
 * @brief Enables the Optional Wedge TexCoords of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge texcoord number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge TexCoords number is tied to the number of vertices.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeTexCoords<U>(), void)
FaceContainer<T>::enablePerFaceWedgeTexCoords()
{
	Base::optionalVec.enableWedgeTexCoords(Base::vec.size());
	static const int N = T::VERTEX_NUMBER;
	if constexpr (N < 0) {
		for (T& f : faces()) {
			f.resizeWedgeTexCoords(f.vertexNumber());
		}
	}
}

/**
 * @brief Disables the Optional WedgeTex Coords of the face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeTexCoords<U>(), void)
FaceContainer<T>::disablePerFaceWedgeTexCoords()
{
	Base::optionalVec.disableWedgeTexCoords();
}

/**
 * @brief Adds a custom component of type K to the Face, having the given name.
 *
 * @note This function is available only if the Face Element has the CustomComponents
 * Component.
 *
 * @tparam K: the type of the custom component added to the Face.
 * @param[in] name: the name of the custom component added to the Face.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(face::hasCustomComponents<U>(), void)
FaceContainer<T>::addPerFaceCustomComponent(const std::string& name)
{
	Base::optionalVec.template addNewComponent<K>(name, faceContainerSize());
}

/**
 * @brief Returns the index of the given face.
 * @param f: face pointer.
 * @return The index of f.
 */
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
 * @brief Adds nFaces to the Face Container of the mesh.
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
		Base::optionalVec.resize(Base::vec.size());
		for (uint i = baseId; i < Base::vec.size(); ++i) {
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
 * @brief Compacts the face container, keeping only the non-deleted faces.
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
	if constexpr (vcl::face::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (FaceType& f : faces()) {
				f.updateFaceReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (vcl::face::hasAdjacentFaces<T>()) {
		for (FaceType& f : faces()) {
			f.updateFaceReferences(oldBase, newBase);
		}
	}
}

template<typename T>
void FaceContainer<T>::updateFaceReferencesAfterCompact(
	const T*                base,
	const std::vector<int>& newIndices)
{
	if constexpr (vcl::face::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (FaceType& f : faces()) {
				f.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (vcl::face::hasAdjacentFaces<T>()) {
		for (FaceType& f : faces()) {
			f.updateFaceReferencesAfterCompact(base, newIndices);
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

template<typename T>
template<typename Mesh>
void FaceContainer<T>::enableOptionalComponentsOf(const Mesh& m)
{
	// unfortunately, this function cannot be shortened in a smart way

	// if edges are enabled in the other Mesh
	if constexpr (hasFaces<Mesh>()) {
		using FaceType = T;
		using MFaceType = typename Mesh::FaceType;

		// disable all the optional components, we will enable them again depending on Mesh
		disableAllPerFaceOptionalComponents();

		// if this Face type has optional adjacent edges
		if constexpr(face::hasOptionalAdjacentEdges<FaceType>()) {
			// if also the other Mesh Face type has optional adjacent edges
			if constexpr (face::hasOptionalAdjacentEdges<MFaceType>()){
				// if they are enabled on the other Mesh, enable also here
				if (m.isPerFaceAdjacentEdgesEnabled()){
					enablePerFaceAdjacentEdges();
				}
			}
			else {
				// if the other Mesh has *non-optional* adjacent edges, I need to enable it on this
				// Face container
				if constexpr(face::hasAdjacentEdges<MFaceType>()) {
					enablePerFaceAdjacentEdges();
				}
			}
		}
		if constexpr(face::hasOptionalAdjacentFaces<FaceType>()) {
			if constexpr (face::hasOptionalAdjacentFaces<MFaceType>()){
				if (m.isPerFaceAdjacentFacesEnabled()){
					enablePerFaceAdjacentFaces();
				}
			}
			else {
				if constexpr(face::hasAdjacentFaces<MFaceType>()) {
					enablePerFaceAdjacentFaces();
				}
			}
		}
		if constexpr(face::hasOptionalColor<FaceType>()) {
			if constexpr (face::hasOptionalColor<MFaceType>()){
				if (m.isPerFaceColorEnabled()){
					enablePerFaceColor();
				}
			}
			else {
				if constexpr(face::hasColor<MFaceType>()) {
					enablePerFaceColor();
				}
			}
		}
		if constexpr(face::hasOptionalMark<FaceType>()) {
			if constexpr (face::hasOptionalMark<MFaceType>()){
				if (m.isPerFaceMarkEnabled()){
					enablePerFaceMark();
				}
			}
			else {
				if constexpr(face::hasMark<MFaceType>()) {
					enablePerFaceMark();
				}
			}
		}
		if constexpr(face::hasOptionalMutableBitFlags<FaceType>()) {
			if constexpr (face::hasOptionalMutableBitFlags<MFaceType>()){
				if (m.isPerFaceMutableBitFlagsEnabled()){
					enablePerFaceMutableBitFlags();
				}
			}
			else {
				if constexpr(face::hasMutableBitFlags<MFaceType>()) {
					enablePerFaceMutableBitFlags();
				}
			}
		}
		if constexpr(face::hasOptionalNormal<FaceType>()) {
			if constexpr (face::hasOptionalNormal<MFaceType>()){
				if (m.isPerFaceNormalEnabled()){
					enablePerFaceNormal();
				}
			}
			else {
				if constexpr(face::hasNormal<MFaceType>()) {
					enablePerFaceNormal();
				}
			}
		}
		if constexpr(face::hasOptionalPrincipalCurvature<FaceType>()) {
			if constexpr (face::hasOptionalPrincipalCurvature<MFaceType>()){
				if (m.isPerFacePrincipalCurvatureEnabled()){
					enablePerFacePrincipalCurvature();
				}
			}
			else {
				if constexpr(face::hasPrincipalCurvature<MFaceType>()) {
					enablePerFacePrincipalCurvature();
				}
			}
		}
		if constexpr(face::hasOptionalScalar<FaceType>()) {
			if constexpr (face::hasOptionalScalar<MFaceType>()){
				if (m.isPerFaceScalarEnabled()){
					enablePerFaceScalar();
				}
			}
			else {
				if constexpr(face::hasScalar<MFaceType>()) {
					enablePerFaceScalar();
				}
			}
		}
		if constexpr(face::hasOptionalWedgeColors<FaceType>()) {
			if constexpr (face::hasOptionalWedgeColors<MFaceType>()){
				if (m.isPerFaceWedgeColorsEnabled()){
					enablePerFaceWedgeColors();
				}
			}
			else {
				if constexpr(face::hasWedgeColors<MFaceType>()) {
					enablePerFaceWedgeColors();
				}
			}
		}
		if constexpr(face::hasOptionalWedgeTexCoords<FaceType>()) {
			if constexpr (face::hasOptionalWedgeTexCoords<MFaceType>()){
				if (m.isPerFaceWedgeTexCoordsEnabled()){
					enablePerFaceWedgeTexCoords();
				}
			}
			else {
				if constexpr(face::hasWedgeTexCoords<MFaceType>()) {
					enablePerFaceWedgeTexCoords();
				}
			}
		}
	}
}

template<typename T>
template<typename Mesh>
void FaceContainer<T>::importFrom(const Mesh& m)
{
	clearFaces();
	if constexpr (hasFaces<Mesh>()) {
		addFaces(m.faceContainerSize());
		unsigned int fid = 0;
		for (const typename Mesh::FaceType& f : m.faces(false)){
			face(fid).importFrom(f);
			if constexpr (face::hasAdjacentFaces<T>()) {
				face(fid).importFaceReferencesFrom(f, Base::vec.data(), &m.face(0));
			}
			++fid;
		}
	}
}

template<typename T>
template<typename Mesh, typename Vertex>
void FaceContainer<T>::importVertexReferencesFrom(const Mesh& m, Vertex* base)
{
	if constexpr(hasVertices<Mesh>() && hasFaces<Mesh>()) {
		for (uint i = 0; i < faceContainerSize(); ++i){
			face(i).importVertexReferencesFrom(m.face(i), base, &m.vertex(0));
		}
	}
}

template<typename T>
template<typename Mesh, typename Edge>
void FaceContainer<T>::importEdgeReferencesFrom(const Mesh& m, Edge* base)
{
	if constexpr(hasEdges<Mesh>() && hasFaces<Mesh>()) {
		for (uint i = 0; i < faceContainerSize(); ++i){
			face(i).importEdgeReferencesFrom(m.face(i), base, &m.edge(0));
		}
	}
}

} // namespace vcl::mesh
