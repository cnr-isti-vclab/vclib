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
template<FaceConcept T>
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
template<FaceConcept T>
const typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i) const
{
	return Base::element(i);
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
template<FaceConcept T>
typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** faces contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return The number of non-deleted faces of the Mesh.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of faces (also deleted) contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some faces that are
 * flagged as deleted.
 *
 * @return The number of all the faces contained in the Mesh.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted faces in the Face container, that is faceContainerSize() -
 * faceNumber().
 *
 * @return The number of deleted faces in the container.
 */
template<FaceConcept T>
uint FaceContainer<T>::deletedFaceNumber() const
{
	return Base::deletedElementNumber();
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
template<FaceConcept T>
void FaceContainer<T>::deleteFace(uint i)
{
	Base::deleteElement(i);
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
template<FaceConcept T>
void FaceContainer<T>::deleteFace(const FaceType* f)
{
	Base::deleteElement(f);
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
template<FaceConcept T>
uint FaceContainer<T>::faceIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
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
template<FaceConcept T>
std::vector<int> FaceContainer<T>::faceCompactIndices() const
{
	return Base::elementCompactIndices();
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
template<FaceConcept T>
typename FaceContainer<T>::FaceIterator FaceContainer<T>::faceBegin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<FaceConcept T>
typename FaceContainer<T>::FaceIterator FaceContainer<T>::faceEnd()
{
	return Base::elementEnd();
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
template<FaceConcept T>
typename FaceContainer<T>::ConstFaceIterator FaceContainer<T>::faceBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<FaceConcept T>
typename FaceContainer<T>::ConstFaceIterator FaceContainer<T>::faceEnd() const
{
	return Base::elementEnd();
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
template<FaceConcept T>
typename FaceContainer<T>::FaceRangeIterator FaceContainer<T>::faces(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
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
template<FaceConcept T>
typename FaceContainer<T>::ConstFaceRangeIterator FaceContainer<T>::faces(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the Face type contained in the
 * FaceContainer.
 */
template<FaceConcept T>
void FaceContainer<T>::enableAllPerFaceOptionalComponents()
{
	if constexpr (face::HasOptionalAdjacentEdges<T>)
		enablePerFaceAdjacentEdges();
	if constexpr (face::HasOptionalAdjacentFaces<T>)
		enablePerFaceAdjacentFaces();
	if constexpr (face::HasOptionalColor<T>)
		enablePerFaceColor();
	if constexpr (face::HasOptionalMark<T>)
		enablePerFaceMark();
	if constexpr (face::HasOptionalNormal<T>)
		enablePerFaceNormal();
	if constexpr (face::HasOptionalPrincipalCurvature<T>)
		enablePerFacePrincipalCurvature();
	if constexpr (face::HasOptionalScalar<T>)
		enablePerFaceScalar();
	if constexpr (face::HasOptionalWedgeColors<T>)
		enablePerFaceWedgeColors();
	if constexpr (face::HasOptionalWedgeTexCoords<T>)
		enablePerFaceWedgeTexCoords();
}

/**
 * @brief Disables all the optional components associated to the Face type contained in the
 * FaceContainer.
 */
template<FaceConcept T>
void FaceContainer<T>::disableAllPerFaceOptionalComponents()
{
	if constexpr (face::HasOptionalAdjacentEdges<T>)
		disablePerFaceAdjacentEdges();
	if constexpr (face::HasOptionalAdjacentFaces<T>)
		disablePerFaceAdjacentFaces();
	if constexpr (face::HasOptionalColor<T>)
		disablePerFaceColor();
	if constexpr (face::HasOptionalMark<T>)
		disablePerFaceMark();
	if constexpr (face::HasOptionalNormal<T>)
		disablePerFaceNormal();
	if constexpr (face::HasOptionalPrincipalCurvature<T>)
		disablePerFacePrincipalCurvature();
	if constexpr (face::HasOptionalScalar<T>)
		disablePerFaceScalar();
	if constexpr (face::HasOptionalWedgeColors<T>)
		disablePerFaceWedgeColors();
	if constexpr (face::HasOptionalWedgeTexCoords<T>)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalAdjacentEdges<U>, bool)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalAdjacentEdges<U>, void)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalAdjacentEdges<U>, void)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalAdjacentFaces<U>, bool)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalAdjacentFaces<U>, void)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalAdjacentFaces<U>, void)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalColor<U>, bool)
FaceContainer<T>::isPerFaceColorEnabled() const
{
	return Base::optionalVec.isColorEnabled();
}

/**
 * @brief Enables the Optional Color of the face.
 *
 * @note This function is available only if the Face Element has the OptionalColor Component.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalColor<U>, void)
FaceContainer<T>::enablePerFaceColor()
{
	Base::optionalVec.enableColor(faceContainerSize());
}

/**
 * @brief Disables the Optional Color of the face.
 *
 * @note This function is available only if the Face Element has the OptionalColor Component.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalColor<U>, void)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalMark<U>, bool)
FaceContainer<T>::isPerFaceMarkEnabled() const
{
	return Base::optionalVec.isMarkEnabled();
}

/**
 * @brief Enables the Optional Mark of the face.
 *
 * @note This function is available only if the Face Element has the OptionalMark Component.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalMark<U>, void)
FaceContainer<T>::enablePerFaceMark()
{
	Base::optionalVec.enableMark(faceContainerSize());
}

/**
 * @brief Disables the Optional Mark of the face.
 *
 * @note This function is available only if the Face Element has the OptionalMark Component.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalMark<U>, void)
FaceContainer<T>::disablePerFaceMark()
{
	Base::optionalVec.disableMark();
}

/**
 * @brief Checks if the face Optional Normal is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 *
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalNormal<U>, bool)
FaceContainer<T>::isPerFaceNormalEnabled() const
{
	return Base::optionalVec.isNormalEnabled();
}

/**
 * @brief Enables the Optional Normal of the face.
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalNormal<U>, void)
FaceContainer<T>::enablePerFaceNormal()
{
	Base::optionalVec.enableNormal(faceContainerSize());
}

/**
 * @brief Disables the Optional Normal of the face.
 *
 * @note This function is available only if the Face Element has the OptionalNormal Component.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalNormal<U>, void)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalPrincipalCurvature<U>, bool)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalPrincipalCurvature<U>, void)
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasOptionalPrincipalCurvature<U>, void)
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
template<FaceConcept T>
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
template<FaceConcept T>
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
template<FaceConcept T>
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
template<FaceConcept T>
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeColors<U>(), void)
FaceContainer<T>::enablePerFaceWedgeColors()
{
	Base::optionalVec.enableWedgeColors(Base::vec.size());
	static const int N = T::VERTEX_NUMBER;
	if constexpr (N < 0) {
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
template<FaceConcept T>
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
template<FaceConcept T>
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
template<FaceConcept T>
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
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::hasOptionalWedgeTexCoords<U>(), void)
FaceContainer<T>::disablePerFaceWedgeTexCoords()
{
	Base::optionalVec.disableWedgeTexCoords();
}

/**
 * @brief Checks if faces have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @return `true` if the Face Element has a custom component with the given name.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasCustomComponents<U>, bool)
FaceContainer<T>::hasPerFaceCustomComponent(const std::string& name) const
{
	return Base::optionalVec.componentExists(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of any type associated
 * to the Face Element.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom components.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasCustomComponents<U>, std::vector<std::string>)
FaceContainer<T>::getAllPerFaceCustomComponentNames() const
{
	return Base::optionalVec.allComponentNames();
}

/**
 * @brief Checks if the custom component of the Face Element having the given name has the same type
 * of the given template argument type of this function.
 *
 * For example, the following code checks if the component called `cc` is of type `double`:
 * @code{.cpp}
 * if (m.isPerFaceCustomComponentOfType<double>("cc")) {
 *   ...
 * }
 * @endcode
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component to check.
 * @param[in] name: the name of the custom component to check.
 * @return `true` if the custom component is of the same type of the template argument.
 */
template<FaceConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(face::HasCustomComponents<U>, bool)
	FaceContainer<T>::isPerFaceCustomComponentOfType(const std::string& name) const
{
	return Base::optionalVec.template isComponentOfType<K>(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the Face
 * Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.getPerFaceCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<FaceConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(face::HasCustomComponents<U>, std::vector<std::string>)
	FaceContainer<T>::getPerFaceCustomComponentNamesOfType() const
{
	return Base::optionalVec.template allComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the Face, having the given name.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component added to the Face.
 * @param[in] name: the name of the custom component added to the Face.
 */
template<FaceConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(face::HasCustomComponents<U>, void)
FaceContainer<T>::addPerFaceCustomComponent(const std::string& name)
{
	Base::optionalVec.template addNewComponent<K>(name, faceContainerSize());
}

/**
 * @brief Deletes the custom component of the given name from the Face Element.
 *
 * The function does nothing if the custom component does not exists.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component that will be removed from the Face.
 */
template<FaceConcept T>
template<typename U>
VCL_ENABLE_IF(face::HasCustomComponents<U>, void)
FaceContainer<T>::deletePerFaceCustomComponent(const std::string& name)
{
	Base::optionalVec.deleteComponent(name);
}

/**
 * @brief Returns a vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **references** to the custom component, therefore you can
 * modify the custom component by modifying the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a face custom component named "cc" of type int:
 *
 * @code{.cpp}
 * auto handle = m.getPerFaceCustomComponentVectorHandle<int>("cc");
 * for (Face& f : m.faces() {
 *    handle[m.index(f)] = 5; // f.customComponent<int>("cc") == 5
 *    assert(f.customComponent<int>("cc") == 5);
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 * @param name: name of the custom component on which return the handle.
 */
template<FaceConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(
	face::HasCustomComponents<U>,
	CustomComponentVectorHandle<K>)
	FaceContainer<T>::getPerFaceCustomComponentVectorHandle(const std::string& name)
{
	std::vector<std::any>& cc = Base::optionalVec.template componentVector<K>(name);
	CustomComponentVectorHandle<K> v(cc);
	return v;
}

/**
 * @brief Returns a const vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **const references** to the custom component, therefore you
 * can access to the custom component by accessing the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a face custom component named "cc" of type int:
 *
 * @code{.cpp}
 * // access to the const handle by making const the template parameter:
 * auto handle = m.getPerFaceCustomComponentVectorHandle<const int>("cc");
 * int sum = 0;
 * for (const Face& f : m.faces() {
 *    sum += handle[m.index(f)];
 *    // handle[m.index(f)] = 5; // not allowed, because the handle is const
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 * @param name: name of the custom component on which return the handle.
 */
template<FaceConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(
	face::HasCustomComponents<U>,
	ConstCustomComponentVectorHandle<K>)
	FaceContainer<T>::getPerFaceCustomComponentVectorHandle(const std::string& name) const
{
	const std::vector<std::any>& cc = Base::optionalVec.template componentVector<K>(name);
	ConstCustomComponentVectorHandle<K> v(cc);
	return cc;
}

/**
 * @brief Returns the index of the given face.
 * @param f: face pointer.
 * @return The index of f.
 */
template<FaceConcept T>
uint FaceContainer<T>::index(const FaceType* f) const
{
	return Base::index(f);
}

template<FaceConcept T>
void vcl::mesh::FaceContainer<T>::clearFaces()
{
	Base::clearElements();
}

template<FaceConcept T>
uint FaceContainer<T>::addFace()
{
	return Base::addElement();
}

/**
 * @brief Adds nFaces to the Face Container of the mesh.
 *
 * Returns the id of the first added face.
 *
 * @param nFaces
 * @return the id of the first added face.
 */
template<FaceConcept T>
uint vcl::mesh::FaceContainer<T>::addFaces(uint nFaces)
{
	return Base::addElements(nFaces);
}

template<FaceConcept T>
void FaceContainer<T>::reserveFaces(uint size)
{
	Base::reserveElements(size);
}

/**
 * @brief Compacts the face container, keeping only the non-deleted faces.
 *
 * @return a vector that tells, for each old face index, the new index of the face. Will contain -1
 * if the face has been deleted.
 */
template<FaceConcept T>
std::vector<int> vcl::mesh::FaceContainer<T>::compactFaces()
{
	return Base::compactElements();
}

template<FaceConcept T>
template<typename Mesh>
void FaceContainer<T>::enableOptionalComponentsOf(const Mesh& m)
{
	// if faces are enabled in the other Mesh
	if constexpr (hasFaces<Mesh>()) {
		using MFaceContainer = typename Mesh::FaceContainer::Base;

		Base::enableOptionalComponentsOf((const MFaceContainer&)m);
	}
}

template<FaceConcept T>
template<typename Mesh>
void FaceContainer<T>::importFrom(const Mesh& m)
{
	if constexpr (hasFaces<Mesh>()) {
		using MFaceContainer = typename Mesh::FaceContainer::Base;

		Base::importFrom((const MFaceContainer&)m);
	}
}

template<FaceConcept T>
template<typename Mesh, typename Vertex>
void FaceContainer<T>::importVertexReferencesFrom(const Mesh& m, Vertex* base)
{
	if constexpr (hasVertices<Mesh>() && hasFaces<Mesh>()) {
		using MFaceContainer = typename Mesh::FaceContainer::Base;

		Base::importVertexReferencesFrom((const MFaceContainer&)m, base, &m.vertex(0));
	}
}

template<FaceConcept T>
template<typename Mesh>
void FaceContainer<T>::importFaceReferencesFrom(const Mesh& m, T *base)
{
	if constexpr (hasFaces<Mesh>()) {
		using MFaceContainer = typename Mesh::FaceContainer::Base;

		Base::importFaceReferencesFrom((const MFaceContainer&)m, base, &m.face(0));
	}
}

template<FaceConcept T>
template<typename Mesh, typename Edge>
void FaceContainer<T>::importEdgeReferencesFrom(const Mesh& m, Edge* base)
{
	if constexpr (hasEdges<Mesh>() && hasFaces<Mesh>()) {
		using MFaceContainer = typename Mesh::FaceContainer::Base;

		Base::importEdgeReferencesFrom((const MFaceContainer&)m, base, &m.edge(0));
	}
}

} // namespace vcl::mesh
