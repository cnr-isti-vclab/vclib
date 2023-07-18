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

#include "adjacent_faces.h"

namespace vcl::comp {

/**
 * @private
 * @brief Initializes the container of adjacent faces to nullptr.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::init()
{
	Base::init(this);
}

/**
 * @private
 * @brief Returns `true` if the component is enabled, `false` otherwise.
 * This member function can return `false` only if the component is optional.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is enabled, `false` otherwise.
 */
template<typename Face, int N, bool TT, typename El, bool O>
bool AdjacentFaces<Face, N, TT, El, O>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns the number of adjacent faces of this element.
 * @return The number of adjacent faces of this element.
 */
template<typename Face, int N, bool TT, typename El, bool O>
uint AdjacentFaces<Face, N, TT, El, O>::adjFacesNumber() const
{
	return Base::container(this).size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent face of this
 * element.
 *
 * You can use this function to set the i-th adjacent face:
 *
 * @code{.cpp}
 * e.adjFace(2) = &m.face(k); // the second adj face of e will point to the k-th
 *                            // face of the mesh.
 * @endcode
 *
 * @param[in] i: the position of the required adjacent face in the container;
 * the value must be between 0 and the number of adj faces.
 * @return The pointer to the i-th adjacent face of this element.
 */
template<typename Face, int N, bool TT, typename El, bool O>
Face*& AdjacentFaces<Face, N, TT, El, O>::adjFace(uint i)
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent face of this element.
 * @param[in] i: the position of the required adjacent face in the container;
 * the value must be between 0 and the number of adj faces.
 * @return The pointer to the i-th adjacent face of this element.
 */
template<typename Face, int N, bool TT, typename El, bool O>
const Face* AdjacentFaces<Face, N, TT, El, O>::adjFace(uint i) const
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent face of this
 * element but using as index the module between i and the number of adjacent
 * faces. You can use this function if you need to get the "next adjacent face
 * after position k", without check if it is less than the number of adj faces.
 * Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent face
 * auto* next = e.adjFaceMod(k+1); // the adj face next to k, that may also be
 *                                 // at pos 0
 * auto* last = e.adjFaceMod(-1); // the adj face in position
 *                                // adjFaceNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent face in the container,
 * w.r.t. the position 0; value is modularized on adjFaceNumber().
 * @return The pointer to the required adjacent face of this element.
 */
template<typename Face, int N, bool TT, typename El, bool O>
Face*& AdjacentFaces<Face, N, TT, El, O>::adjFaceMod(int i)
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Same of adjFaceMod, but returns a const Pointer to the adjacent face.
 * @param[in] i: the position of the required adjacent face in the container,
 * w.r.t. the position 0; value is modularized on adjFaceNumber().
 * @return The pointer to the required adjacent face of this element.
 */
template<typename Face, int N, bool TT, typename El, bool O>
const Face* AdjacentFaces<Face, N, TT, El, O>::adjFaceMod(int i) const
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Sets the i-th adjacent face of this element.
 * @param[in] f: The pointer to the adjacent face to set to this element.
 * @param[in] i: the position in the container on which set the adj face; the
 * value must be between 0 and the number of adj faces.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::setAdjFace(Face* f, uint i)
{
	Base::container(this).set(f, i);
}

/**
 * @brief Sets all the adjacent faces of this element.
 *
 * If the size of the container is static, the size of the input vector must be
 * the same one of the container.
 *
 * @param[in] list: vector of adjacent faces to set.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::setAdjFaces(
	const std::vector<Face*>& list)
{
	Base::container(this).set(list);
}

/**
 * @brief Returns `true` if the container of adjacent faces contains the given
 * face, `false` otherwise.
 *
 * @param[in] e: the pointer to the face to search.
 * @return `true` if the container of adjacent faces contains the given face,
 * `false` otherwise.
 */
template<typename Face, int N, bool TT, typename El, bool O>
bool AdjacentFaces<Face, N, TT, El, O>::containsAdjFace(const Face* f) const
{
	return Base::container(this).contains(f);
}

/**
 * @brief Returns an iterator to the first adjacent face in the container of
 * this component that is equal to the given face. If no such adjacent face is
 * found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the face to search.
 * @return an iterator pointing to the first adjacent face equal to the given
 * face, or end if no such adjacent face is found.
 */
template<typename Face, int N, bool TT, typename El, bool O>
typename AdjacentFaces<Face, N, TT, El, O>::AdjacentFaceIterator
AdjacentFaces<Face, N, TT, El, O>::findAdjFace(const Face* f)
{
	return Base::container(this).find(f);
}

/**
 * @brief Returns a const iterator to the first adjacent face in the container
 * of this component that is equal to the given edge. If no such adjacent face
 * is found, past-the-end iterator is returned.
 *
 * @param[in] e: the pointer to the face to search.
 * @return a const iterator pointing to the first adjacent face equal to the
 * given face, or end if no such adjacent face is found.
 */
template<typename Face, int N, bool TT, typename El, bool O>
typename AdjacentFaces<Face, N, TT, El, O>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N, TT, El, O>::findAdjFace(const Face* f) const
{
	return Base::container(this).find(f);
}

/**
 * @brief Returns the index of the given adjacent face in the container of this
 * element. If the given adjacent face is not in the container, returns
 * UINT_NULL.
 *
 * @param[in] e: the pointer to the adjacent face to search.
 * @return the index of the given adjacent face, or UINT_NULL if it is not
 * found.
 */
template<typename Face, int N, bool TT, typename El, bool O>
uint AdjacentFaces<Face, N, TT, El, O>::indexOfAdjFace(const Face* f) const
{
	return Base::container(this).indexOf(f);
}

/**
 * @brief Resize the container of the adjacent faces to the given size.
 * @note This function is available only if the container of the Adjacent Faces
 * is has dynamic size.
 * @param[in] n: The new size of the adjacent faces container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::resizeAdjFaces(uint n)
	requires(N < 0 && !TT)
{
	Base::container(this).resize(n);
}

/**
 * @brief Pushes in the back of the container the given adjacent face.
 * @note This function is available only if the container of the Adjacent Faces
 * component has dynamic size.
 * @param[in] e: The pointer to the adjacent face to push in the back of the
 * container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::pushAdjFace(Face* f)
	requires(N < 0 && !TT)
{
	Base::container(this).pushBack(f);
}

/**
 * @brief Inserts the given adjacent face in the container at the given
 * position.
 * @note This function is available only if the container of the Adjacent Faces
 * component has dynamic size.
 * @param[in] i: The position in the container where to insert the adjacent
 * face.
 * @param[in] e: The pointer to the adjacent face to insert in the container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::insertAdjFace(uint i, Face* f)
	requires(N < 0 && !TT)
{
	Base::container(this).insert(i, f);
}

/**
 * @brief Removes the adjacent face at the given position from the container.
 * @note This function is available only if the container of the Adjacent Faces
 * component has dynamic size.
 * @param[in] i: The position of the adjacent face to remove from the
 * container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::eraseAdjFace(uint i)
	requires(N < 0 && !TT)
{
	Base::container(this).erase(i);
}

/**
 * @brief Clears the container of adjacent faces, making it empty.
 * @note This function is available only if the container of the Adjacent Faces
 * component has dynamic size.
 */
template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::clearAdjFaces()
	requires(N < 0 && !TT)
{
	Base::container(this).clear();
}

/**
 * @brief Returns an iterator to the first adjacent face in the container of
 * this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
typename AdjacentFaces<Face, N, TT, El, O>::AdjacentFaceIterator
AdjacentFaces<Face, N, TT, El, O>::adjFaceBegin()
{
	return Base::container(this).begin();
}

/**
 * @brief Returns an iterator to the end of the container of this component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
typename AdjacentFaces<Face, N, TT, El, O>::AdjacentFaceIterator
AdjacentFaces<Face, N, TT, El, O>::adjFaceEnd()
{
	return Base::container(this).end();
}

/**
 * @brief Returns a const iterator to the first adjacent face in the container
 * of this component.
 *
 * @return an iterator pointing to the begin of this container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
typename AdjacentFaces<Face, N, TT, El, O>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N, TT, El, O>::adjFaceBegin() const
{
	return Base::container(this).begin();
}

/**
 * @brief Returns a const iterator to the end of the container of this
 * component.
 *
 * @return an iterator pointing to the end of this container.
 */
template<typename Face, int N, bool TT, typename El, bool O>
typename AdjacentFaces<Face, N, TT, El, O>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N, TT, El, O>::adjFaceEnd() const
{
	return Base::container(this).end();
}

/**
 * @brief Returns a lightweight view object that stores the begin and end
 * iterators of the container of adjacent faces of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (auto* adjFace : el.adjFaces()) {
 *     // Do something with adjFace
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent faces.
 */
template<typename Face, int N, bool TT, typename El, bool O>
auto AdjacentFaces<Face, N, TT, El, O>::adjFaces()
{
	return View(adjFaceBegin(), adjFaceEnd());
}

/**
 * @brief Returns a lightweight const view object that stores the begin and end
 * iterators of the container of adjacent faces of the element. The view object
 * exposes the iterators trough the `begin()` and `end()` member functions, and
 * therefore the returned object can be used in range-based for loops:
 *
 * @code{.cpp}
 * for (const auto* adjFace : el.adjFaces()) {
 *     // Do something read-only with adjFace
 * }
 * @endcode
 *
 * @return a lightweight view object that can be used in range-based for loops
 * to iterate over adjacent faces.
 */
template<typename Face, int N, bool TT, typename El, bool O>
auto AdjacentFaces<Face, N, TT, El, O>::adjFaces() const
{
	return View(adjFaceBegin(), adjFaceEnd());
}

template<typename Face, int N, bool TT, typename El, bool O>
template<typename Element>
void AdjacentFaces<Face, N, TT, El, O>::importFrom(const Element&)
{
}

template<typename Face, int N, bool TT, typename El, bool O>
template<typename Element, typename ElFType>
void AdjacentFaces<Face, N, TT, El, O>::importPointersFrom(
	const Element& e,
	Face*          base,
	const ElFType* ebase)
{
	if constexpr (HasAdjacentFaces<Element>) {
		if (isAdjacentFacesEnabledOn(e)) {
			if constexpr (N > 0) {
				// same static size
				if constexpr (N == Element::ADJ_FACE_NUMBER) {
					importPtrsFrom(e, base, ebase);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::ADJ_FACE_NUMBER < 0) {
					if (e.adjFacesNumber() == N) {
						importPtrsFrom(e, base, ebase);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic
					// size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first,
				// then import
				resize(e.adjFacesNumber());
				importPtrsFrom(e, base, ebase);
			}
		}
	}
}

template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::updatePointers(
	const Face* oldBase,
	const Face* newBase)
{
	Base::updateElementPointers(oldBase, newBase, this);
}

template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::updatePointers(
	const Face*             base,
	const std::vector<uint>& newIndices)
{
	Base::updateElementPointers(base, newIndices, this);
}

template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::resize(uint n) requires (N < 0)
{
	Base::container(this).resize(n);
}

template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::pushBack(Face* f) requires (N < 0)
{
	Base::container(this).pushBack(f);
}

template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::insert(uint i, Face* f) requires (N < 0)
{
	Base::container(this).insert(i, f);
}

template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::erase(uint i) requires (N < 0)
{
	Base::container(this).erase(i);
}

template<typename Face, int N, bool TT, typename El, bool O>
void AdjacentFaces<Face, N, TT, El, O>::clear() requires (N < 0)
{
	Base::container(this).clear();
}

template<typename Face, int N, bool TT, typename El, bool O>
template<typename Element, typename ElFType>
void AdjacentFaces<Face, N, TT, El, O>::importPtrsFrom(
	const Element& e,
	Face*          base,
	const ElFType* ebase)
{
	if (ebase != nullptr && base != nullptr) {
		for (uint i = 0; i < e.adjFacesNumber(); ++i) {
			if (e.adjFace(i) != nullptr) {
				adjFace(i) = base + (e.adjFace(i) - ebase);
			}
		}
	}
}

/**
 * @brief Checks if the given Element has AdjacentFaces available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that satisfies
 * the ElementConcept.
 * @return `true` if the element has AdjacentFaces available, `false` otherwise.
 */
bool isAdjacentFacesEnabledOn(const ElementConcept auto& element)
{
	return isComponentEnabledOn<ADJACENT_FACES>(element);
}

} // namespace vcl::comp
