/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#include <vclib/views/mesh.h>
#include <vclib/space/polygon.h>

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Faces.
 */
template<FaceConcept T>
FaceContainer<T>::FaceContainer()
{
}

/**
 * @brief Returns a const reference of the Face at the i-th position in the Face Container of the
 * Mesh, which will be the Face having index = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the Face that will be returned.
 */
template<FaceConcept T>
const typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the Face at the i-th position in the Face Container of the Mesh,
 * which will be the Face having index = i.
 *
 * This function does not perform any sanity check: if i is less than faceContainerSize(), this
 * function will return a valid Face reference (note that the Face may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the Face that will be returned.
 */
template<FaceConcept T>
typename FaceContainer<T>::FaceType& FaceContainer<T>::face(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** Faces contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some Faces that are
 * flagged as deleted.
 *
 * @return the number of non-deleted Faces of the Mesh.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of Faces (also deleted) contained in the Face container of the Mesh.
 *
 * If faceNumber() != faceContainerSize(), it means that there are some Faces that are
 * flagged as deleted.
 *
 * @return the number of all the Faces contained in the Mesh.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted Faces in the Face container, that is faceContainerSize() -
 * faceNumber().
 *
 * @return The number of deleted Faces in the container.
 */
template<FaceConcept T>
uint FaceContainer<T>::deletedFaceNumber() const
{
	return Base::deletedElementNumber();
}

template<FaceConcept T>
uint FaceContainer<T>::addFace()
{
	return Base::addElement();
}

template<FaceConcept T>
template<typename... V>
uint FaceContainer<T>::addFace(V... args) requires (sizeof...(args) >= 3)
{
	uint  fid = addFace();
	Face& f   = face(fid);

	constexpr uint n = sizeof...(args);

	if constexpr (T::VERTEX_NUMBER < 0) {
		f.resizeVertices(n);
	}
	else {
		static_assert(n == T::VERTEX_NUMBER, "Wrong number of vertices in Mesh::addFace.");
	}

	addFaceHelper(f, args...);
	return fid;
}

template<FaceConcept T>
template<typename Iterator>
uint FaceContainer<T>::addFace(Iterator begin, Iterator end)
{
	if (begin == end) return UINT_NULL;
	uint n = std::distance(begin, end);

	uint fid = UINT_NULL;

	assert(n >= 3);
	if (n < 3) return UINT_NULL;

	if constexpr (T::VERTEX_NUMBER < 0) {
		fid = addFace();
		face(fid).resizeVertices(n);
	}
	else {
		assert(n == T::VERTEX_NUMBER);
		if (n == T::VERTEX_NUMBER)
			fid = addFace();
	}

	if (fid != UINT_NULL) {
		Face& f = face(fid);

		unsigned int i = 0;
		for (Iterator it = begin; it != end; ++it) {
			if constexpr (std::integral<typename Iterator::value_type>)
				f.vertex(i) = &Base::parentMesh->vertex(*it);
			else
				f.vertex(i) = *it;
			++i;
		}
	}
	return fid;
}

/**
 * @brief Add an arbitrary number of n Faces, returning the id of the first added Face.
 *
 * This means that, if you want to add 5 Faces and this member function returns 4, the added
 * Faces will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the FaceContainer, the function
 * will automatically take care of updating all the Face pointers contained in the Mesh.
 *
 * @param n: the number of Faces to add to the mesh.
 * @return the id of the first added Face.
 */
template<FaceConcept T>
uint FaceContainer<T>::addFaces(uint n)
{
	return Base::addElements(n);
}

/**
 * @brief Reserve a number of Faces in the container of Faces. This is useful when you know
 * (or you have an idea) of how much Faces are going to add into a newly of existing mesh.
 * Calling this function before any addFace() call will avoid unuseful reallocations of the
 * container, saving execution time.
 *
 * The filosofy of this function is similar to the one of the
 * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve) function of the
 * [std::vector class](https://en.cppreference.com/w/cpp/container/vector).
 *
 * If the call of this function will cause a reallocation of the Face container, the function
 * will automatically take care of updating all the Face pointers contained in the Mesh.
 *
 * @param n: the new capacity of the Face container.
 */
template<FaceConcept T>
void FaceContainer<T>::reserveFaces(uint n)
{
	Base::reserveElements(n);
}

/**
 * @brief Compacts the FaceContainer, removing all the Faces marked as deleted. Faces indices
 * will change accordingly. The function will automatically take care of updating all the Face
 * pointers contained in the Mesh.
 */
template<FaceConcept T>
void FaceContainer<T>::compactFaces()
{
	Base::compactElements();
}

/**
 * @brief Marks as deleted the Face with the given id.
 *
 * This member function does not perform any reallocation of the Faces: the deleted Faces
 * will stay in the Face Container, but will be marked as deleted.
 *
 * Deleted Faces are automatically jumped by the iterators provided by the Face Container.
 *
 * @param[in] i: the id of the Face that will be marked as deleted.
 */
template<FaceConcept T>
void FaceContainer<T>::deleteFace(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given Face, before asserting that the Face belongs to this container.
 *
 * This member function does not perform any reallocation of the Faces: the deleted Faces
 * will stay in the Face Container, but will be marked as deleted.
 *
 * Deleted Faces are automatically jumped by the iterators provided by the Face Container.
 *
 * @param[in] e: the pointer of the Face that will be marked as deleted.
 */
template<FaceConcept T>
void FaceContainer<T>::deleteFace(const FaceType* e)
{
	Base::deleteElement(e);
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the Face with the given
 * index.
 *
 * Complexity: O(n), with n the number of Faces in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a Face of the container.
 * @return The index that the Face with index i would have if this container would be compact.
 */
template<FaceConcept T>
uint FaceContainer<T>::faceIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual Face index, the new index that the Face
 * would have in a compacted container. For each deleted Face index, the value of the vector will
 * be -1.
 *
 * This is useful if you need to know the indices of the Faces that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each Face index, its index if the container would be compact.
 */
template<FaceConcept T>
std::vector<int> FaceContainer<T>::faceCompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted Faces of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted Faces.
 * @return An iterator the the first Face of the container.
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
 * The iterator is automatically initialized to jump deleted Faces of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted Faces.
 * @return A const iterator the the first Face of the container.
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
 * @brief Returns a small view object that allows to iterate over the Faces of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the Faces using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (Face& e : m.faces()){
 *     // do something with this Face
 * }
 * @endcode
 *
 * The iterator used to iterate over Faces is automatically initialized to jump deleted Faces of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted Faces.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<FaceConcept T>
auto FaceContainer<T>::faces(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small view object that allows to iterate over the Faces of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the Faces using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (const Face& e : m.faces()){
 *     // do something with this Face
 * }
 * @endcode
 *
 * The iterator used to iterate over Faces is automatically initialized to jump deleted Faces of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted Faces.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<FaceConcept T>
auto FaceContainer<T>::faces(bool jumpDeleted) const
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
	Base::enableAllOptionalComponents();
}

/**
 * @brief Disables all the optional components associated to the Face type contained in the
 * FaceContainer.
 */
template<FaceConcept T>
void FaceContainer<T>::disableAllPerFaceOptionalComponents()
{
	Base::disableAllOptionalComponents();
}

/**
 * @brief Checks if the Face Optional AdjacentEdges is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 *
 * @return true if the Optional AdjacentEdges is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceAdjacentEdgesEnabled() const requires face::HasOptionalAdjacentEdges<T>
{
	return Base::template isOptionalComponentEnabled<typename T::AdjacentEdges>();
}

/**
 * @brief Enable the Optional AdjacentEdges of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the adjacent edges number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Adjacent Edges number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentEdges>();
}

/**
 * @brief Disables the Optional AdjacentEdges of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentEdges
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentEdges>();
}

/**
 * @brief Checks if the Face Optional AdjacentFaces is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 *
 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceAdjacentFacesEnabled() const requires face::HasOptionalAdjacentFaces<T>
{
	return Base::template isOptionalComponentEnabled<typename T::AdjacentFaces>();
}

/**
 * @brief Enable the Optional AdjacentFaces of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the adjacent faces number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Adjacent Faces number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentFaces>();
}

/**
 * @brief Disables the Optional AdjacentFaces of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalAdjacentFaces
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentFaces>();
}

/**
 * @brief Checks if the Face Optional Color is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalColor
 * Component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceColorEnabled() const requires face::HasOptionalColor<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Color>();
}

/**
 * @brief Enable the Optional Color of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalColor
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceColor() requires face::HasOptionalColor<T>
{
	Base::template enableOptionalComponent<typename T::Color>();
}

/**
 * @brief Disables the Optional Color of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalColor
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceColor() requires face::HasOptionalColor<T>
{
	Base::template disableOptionalComponent<typename T::Color>();
}

/**
 * @brief Checks if the Face Optional Mark is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalMark
 * Component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceMarkEnabled() const requires face::HasOptionalMark<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Mark>();
}

/**
 * @brief Enable the Optional Mark of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalMark
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceMark() requires face::HasOptionalMark<T>
{
	Base::template enableOptionalComponent<typename T::Mark>();
}

/**
 * @brief Disables the Optional Mark of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalMark
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceMark() requires face::HasOptionalMark<T>
{
	Base::template disableOptionalComponent<typename T::Mark>();
}

/**
 * @brief Checks if the Face Optional Normal is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalNormal
 * Component.
 *
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceNormalEnabled() const requires face::HasOptionalNormal<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Normal>();
}

/**
 * @brief Enable the Optional Normal of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalNormal
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceNormal() requires face::HasOptionalNormal<T>
{
	Base::template enableOptionalComponent<typename T::Normal>();
}

/**
 * @brief Disables the Optional Normal of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalNormal
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceNormal() requires face::HasOptionalNormal<T>
{
	Base::template disableOptionalComponent<typename T::Normal>();
}

/**
 * @brief Checks if the Face Optional PrincipalCurvature is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 *
 * @return true if the Optional PrincipalCurvature is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFacePrincipalCurvatureEnabled() const requires face::HasOptionalPrincipalCurvature<T>
{
	return Base::template isOptionalComponentEnabled<typename T::PrincipalCurvature>();
}

/**
 * @brief Enable the Optional PrincipalCurvature of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFacePrincipalCurvature() requires face::HasOptionalPrincipalCurvature<T>
{
	Base::template enableOptionalComponent<typename T::PrincipalCurvature>();
}

/**
 * @brief Disables the Optional PrincipalCurvature of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalPrincipalCurvature
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFacePrincipalCurvature() requires face::HasOptionalPrincipalCurvature<T>
{
	Base::template disableOptionalComponent<typename T::PrincipalCurvature>();
}

/**
 * @brief Checks if the Face Optional Quality is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalQuality
 * Component.
 *
 * @return true if the Optional Quality is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceQualityEnabled() const requires face::HasOptionalQuality<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Quality>();
}

/**
 * @brief Enable the Optional Quality of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalQuality
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceQuality() requires face::HasOptionalQuality<T>
{
	Base::template enableOptionalComponent<typename T::Quality>();
}

/**
 * @brief Disables the Optional Quality of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalQuality
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceQuality() requires face::HasOptionalQuality<T>
{
	Base::template disableOptionalComponent<typename T::Quality>();
}

/**
 * @brief Checks if the Face Optional WedgeColors is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors
 * Component.
 *
 * @return true if the Optional WedgeColors is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceWedgeColorsEnabled() const requires face::HasOptionalWedgeColors<T>
{
	return Base::template isOptionalComponentEnabled<typename T::WedgeColors>();
}

/**
 * @brief Enable the Optional WedgeColors of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge colors number will
 * be the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge Colors number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>
{
	Base::template enableOptionalComponent<typename T::WedgeColors>();
}

/**
 * @brief Disables the Optional WedgeColors of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeColors
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>
{
	Base::template disableOptionalComponent<typename T::WedgeColors>();
}

/**
 * @brief Checks if the Face Optional WedgeTexCoords is enabled.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 *
 * @return true if the Optional WedgeTexCoords is enabled, false otherwise.
 */
template<FaceConcept T>
bool FaceContainer<T>::isPerFaceWedgeTexCoordsEnabled() const requires face::HasOptionalWedgeTexCoords<T>
{
	return Base::template isOptionalComponentEnabled<typename T::WedgeTexCoords>();
}

/**
 * @brief Enable the Optional WedgeTexCoords of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 *
 * @note If the Face is polygonal (dynamic size, N < 0), when enabled, the wedge texcoord number
 * will be the same of the vertex number for each face of the container. This is because, for Faces,
 * Wedge TexCoords number is tied to the number of vertices.
 */
template<FaceConcept T>
void FaceContainer<T>::enablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>
{
	Base::template enableOptionalComponent<typename T::WedgeTexCoords>();
}

/**
 * @brief Disables the Optional WedgeTexCoords of the Face.
 *
 * @note This function is available only if the Face Element has the OptionalWedgeTexCoords
 * Component.
 */
template<FaceConcept T>
void FaceContainer<T>::disablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>
{
	Base::template disableOptionalComponent<typename T::WedgeTexCoords>();
}

/**
 * @brief Checks if Faces have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @return `true` if the Face Element has a custom component with the given name.
 */
template<FaceConcept T>
bool FaceContainer<T>::hasPerFaceCustomComponent(const std::string& name) const
	requires face::HasCustomComponents<T>
{
	return Base::hasElemCustomComponent(name);
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
std::vector<std::string> FaceContainer<T>::perFaceCustomComponentNames() const
	requires face::HasCustomComponents<T>
{
	return Base::elemCustomComponentNames();
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
 *
 * @param[in] name: the name of the custom component to check.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return `true` if the custom component is of the same type of the template argument.
 */
template<FaceConcept T>
template<typename K>
bool FaceContainer<T>::isPerFaceCustomComponentOfType(const std::string& name) const
	requires face::HasCustomComponents<T>
{
	return Base::template isElemCustomComponentOfType<K>(name);
}

/**
 * @brief Returns the std::type_index of the custom component of the Face Element having the given
 * input name.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component to get the std::type_index from.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return The std::type_index of the custom component having the given input name.
 */
template<vcl::FaceConcept T>
std::type_index FaceContainer<T>::perFaceCustomComponentType(const std::string& name) const
	requires face::HasCustomComponents<T>
{
	return Base::elemComponentType(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the Edge
 * Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.perFaceCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<FaceConcept T>
template<typename K>
std::vector<std::string> FaceContainer<T>::perFaceCustomComponentNamesOfType() const
	requires face::HasCustomComponents<T>
{
	return Base::template elemCustomComponentNamesOfType<K>();
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
template<typename K>
void FaceContainer<T>::addPerFaceCustomComponent(const std::string& name)
	requires face::HasCustomComponents<T>
{
	Base::template addElemCustomComponent<K>(name);
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
void FaceContainer<T>::deletePerFaceCustomComponent(const std::string& name)
	requires face::HasCustomComponents<T>
{
	Base::deleteElemCustomComponent(name);
}

/**
 * @brief Returns a vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **references** to the custom component, therefore you can
 * modify the custom component by modifying the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a Face custom component named "cc" of type int:
 *
 * @code{.cpp}
 * auto handle = m.getPerFaceCustomComponentVectorHandle<int>("cc");
 * for (Face& e : m.faces() {
 *    handle[m.index(e)] = 5; // e.customComponent<int>("cc") == 5
 *    assert(e.customComponent<int>("cc") == 5);
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 *
 * @param[in] name: name of the custom component on which return the handle.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return a vector handle that allows to access to the custom component.
 */
template<FaceConcept T>
template<typename K>
CustomComponentVectorHandle<K>
FaceContainer<T>::perFaceCustomComponentVectorHandle(const std::string& name)
	requires face::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

/**
 * @brief Returns a const vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **const references** to the custom component, therefore you
 * can access to the custom component by accessing the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a Face custom component named "cc" of type int:
 *
 * @code{.cpp}
 * // access to the const handle by making const the template parameter:
 * auto handle = m.perFaceCustomComponentVectorHandle<const int>("cc");
 * int sum = 0;
 * for (const Face& e : m.faces() {
 *    sum += handle[m.index(e)];
 *    // handle[m.index(e)] = 5; // not allowed, because the handle is const
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note This function is available only if the Face Element has the CustomComponents Component.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 * @param[in] name: name of the custom component on which return the handle.
 *
 * @return a const vector handle that allows to access to the custom component.
 */
template<FaceConcept T>
template<typename K>
ConstCustomComponentVectorHandle<K>
FaceContainer<T>::perFaceCustomComponentVectorHandle(const std::string& name) const
	requires face::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

/**
 * @brief This function manages the case where we try to import into a TriMesh a PolyMesh
 * Faces have been already imported, but without vertex pointers and other components that
 * depend on the number of vertices (e.g. wedges)
 */
template<FaceConcept F>
template<typename OthMesh>
void FaceContainer<F>::manageImportTriFromPoly(const OthMesh& m)
{
	if constexpr (HasFaceContainer<OthMesh>) {
		using ParentMesh = typename Base::ParentMeshType;
		using VertexType = typename ParentMesh::VertexType;
		using MVertexType = typename OthMesh::VertexType;
		using MCoordType = typename MVertexType::CoordType;
		using MFaceType = typename OthMesh::FaceType;

		using VertexContainer = typename ParentMesh::VertexContainer;

		// if this is not a triangle mesh nor a polygon mesh (meaning that we can't control the
		// number of vertex pointers in this mesh), and this mesh does not have the same
		// number of vertex pointers of the other, it means that we don't know how to convert
		// these type of meshes (e.g. we don't know how to convert a polygon mesh into a quad
		// mesh, or convert a quad mesh into a pentagonal mesh...)
		static_assert(
			!(FaceType::VERTEX_NUMBER != 3 && FaceType::VERTEX_NUMBER > 0 &&
			  FaceType::VERTEX_NUMBER != MFaceType::VERTEX_NUMBER),
			"Cannot import from that type of Mesh. Don't know how to convert faces.");

		// we need to manage conversion from poly or faces with cardinality > 3 (e.g. quads) to
		// triangle meshes. In this case, we triangulate the polygon using the earcut algorithm.
		if constexpr (
			FaceType::VERTEX_NUMBER == 3 &&
			(MFaceType::VERTEX_NUMBER > 3 || MFaceType::VERTEX_NUMBER < 0)) {

			VertexType* base = &Base::parentMesh->vertex(0);
			const MVertexType* mvbase = &m.vertex(0);

			for (const MFaceType& mf : m.faces()) {
				// if the current face has the same number of vertices of this faces (3), then
				// the vertex pointers have been correctly imported from the import pointers
				// function. The import pointers function does nothing when importing from a face
				// with at least 4 vertices
				if (mf.vertexNumber() != FaceType::VERTEX_NUMBER) {
					// triangulate mf; the first triangle of the triangulation will be
					// this->face(m.index(mf));
					// the other triangles will be added at the end of the container
					std::vector<uint> tris =
						Polygon<MCoordType>::earCut(mf.vertices() | vcl::views::coords);
					FaceType& f = face(m.index(mf));
					importTriPointersHelper(f, mf, base, mvbase, tris, 0);

					// number of other faces to add
					uint nf = tris.size() / 3 - 1;
					uint fid = addFaces(nf);

					uint i = 3; // index that cycles into tris
					for (; fid < faceContainerSize(); ++fid) {
						FaceType& f = face(fid);
						importTriPointersHelper(f, mf, base, mvbase, tris, i);
						i+=3;
					}
				}
			}
		}
	}
}

template<FaceConcept F>
void FaceContainer<F>::addFaceHelper(F&)
{
	// base case: no need to add any other vertices
}

template<FaceConcept F>
template<typename... V>
void FaceContainer<F>::addFaceHelper(F& f, typename F::VertexType* v, V... args)
{
	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = v;   // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

template<FaceConcept F>
template<typename... V>
void FaceContainer<F>::addFaceHelper(F& f, uint vid, V... args)
{
	// position on which add the vertex
	const std::size_t n = f.vertexNumber() - sizeof...(args) - 1;
	f.vertex(n)         = &Base::parentMesh->vertex(vid); // set the vertex
	addFaceHelper(f, args...); // set the remanining vertices, recursive variadics
}

template<FaceConcept F>
template<typename MFaceType, typename VertexType, typename MVertexType>
void FaceContainer<F>::importTriPointersHelper(
	FaceType&                f,
	const MFaceType&         mf,
	VertexType*              base,
	const MVertexType*       mvbase,
	const std::vector<uint>& tris,
	uint                     basetri)
{
	f.importFrom(mf); // import all the components from mf
	for (uint i = basetri, j = 0; i < basetri+3; i++, j++) {
		f.vertex(j) = base + (mf.vertex(tris[i]) - mvbase);

		// wedge colors
		if constexpr(face::HasWedgeColors<FaceType> && face::HasWedgeColors<MFaceType>) {
			if (comp::isWedgeColorsEnabledOn(f) && comp::isWedgeColorsEnabledOn(mf)) {
				f.wedgeColor(j) = mf.wedgeColor(tris[i]);
			}
		}

		// wedge texcoords
		if constexpr(face::HasWedgeTexCoords<FaceType> && face::HasWedgeTexCoords<MFaceType>) {
			if (comp::isWedgeTexCoordsEnabledOn(f) && comp::isWedgeTexCoordsEnabledOn(mf)) {
				f.wedgeTexCoord(j) =
					mf.wedgeTexCoord(tris[i])
						.template cast<typename FaceType::WedgeTexCoordType::ScalarType>();
			}
		}
	}
}

} // namespace vcl::mesh
