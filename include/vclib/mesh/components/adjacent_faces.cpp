/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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
 * @brief Creates a Container of Adjacent Faces, stored as Face Pointers.
 * If the size of the container is static, all the values will be set to nullptr.
 * If the size is dynamic, the number of values in the container is zero.
 */
template<typename Face, int N, typename El, bool h>
AdjacentFaces<Face, N, El, h>::AdjacentFaces() : Base()
{
}

/**
 * @brief Returns the number of adjacent faces of this element.
 * @return The number of adjacent faces of this element.
 */
template<typename Face, int N, typename El, bool h>
uint AdjacentFaces<Face, N, El, h>::adjFacesNumber() const
{
	return Base::container(this).size();
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent face of this element.
 *
 * You can use this function to set the i-th adjacent face:
 *
 * @code{.cpp}
 * e.adjFace(2) = &m.face(k); // the second adj face of e will point to the k-th face of the mesh.
 * @endcode
 *
 * @param[in] i: the position of the required adjacent face in the container; the value must be
 * between 0 and the number of adj faces.
 * @return The pointer to the i-th adjacent face of this element.
 */
template<typename Face, int N, typename El, bool h>
Face*& AdjacentFaces<Face, N, El, h>::adjFace(uint i)
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a const pointer to the i-th adjacent face of this element.
 * @param[in] i: the position of the required adjacent face in the container; the value must be
 * between 0 and the number of adj faces.
 * @return The pointer to the i-th adjacent face of this element.
 */
template<typename Face, int N, typename El, bool h>
const Face* AdjacentFaces<Face, N, El, h>::adjFace(uint i) const
{
	return Base::container(this).at(i);
}

/**
 * @brief Returns a reference of the pointer to the i-th adjacent face of this element but using as
 * index the module between i and the number of adjacent faces. You can use this function if you
 * need to get the "next adjacent face after position k", without check if it is less than the
 * number of adj faces. Works also for negative numbers:
 *
 * @code{.cpp}
 * k = pos; // some position of an adjacent face
 * auto* next = e.adjFaceMod(k+1); // the adj face next to k, that may also be at pos 0
 * auto* last = e.adjFaceMod(-1); // the adj face in position adjFaceNumber()-1
 * @endcode
 *
 * @param[in] i: the position of the required adjacent face in the container, w.r.t. the position 0;
 * value is modularized on adjFaceNumber().
 * @return The pointer to the required adjacent face of this element.
 */
template<typename Face, int N, typename El, bool h>
Face*& AdjacentFaces<Face, N, El, h>::adjFaceMod(int i)
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Same of adjFaceMod, but returns a const Pointer to the adjacent face.
 * @param[in] i: the position of the required adjacent face in the container, w.r.t. the position 0;
 * value is modularized on adjFaceNumber().
 * @return The pointer to the required adjacent face of this element.
 */
template<typename Face, int N, typename El, bool h>
const Face* AdjacentFaces<Face, N, El, h>::adjFaceMod(int i) const
{
	return Base::container(this).atMod(i);
}

/**
 * @brief Sets the i-th adjacent face of this element.
 * @param[in] f: The pointer to the adjacent face to set to this element.
 * @param[in] i: the position in the container on which set the adj face; the value must be
 * between 0 and the number of adj faces.
 */
template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::setAdjFace(Face* f, uint i)
{
	Base::container(this).set(f, i);
}

/**
 * @brief Sets all the adjacent faces of this element.
 *
 * If the size of the container is static, the size of the input vector must be the same one of the
 * container.
 *
 * @param[in] list: vector of adjacent faces to set.
 */
template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::setAdjFaces(const std::vector<Face*>& list)
{
	Base::container(this).set(list);
}

template<typename Face, int N, typename El, bool h>
bool AdjacentFaces<Face, N, El, h>::containsAdjFace(const Face* f) const
{
	return Base::container(this).contains(f);
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::AdjacentFaceIterator
AdjacentFaces<Face, N, El, h>::findAdjFace(const Face* f)
{
	return Base::container(this).find(f);
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N, El, h>::findAdjFace(const Face* f) const
{
	return Base::container(this).find(f);
}

template<typename Face, int N, typename El, bool h>
int AdjacentFaces<Face, N, El, h>::indexOfAdjFace(const Face* f) const
{
	return Base::container(this).indexOf(f);
}

/**
 * @brief Resize the container of the adjacent faces to the given size.
 * @note This function is available only if the container of the Adjacent Faces is has dynamic
 * size.
 * @param n
 */
template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::resizeAdjFaces(uint n)
	requires(N < 0)
{
	Base::container(this).resize(n);
}

template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::pushAdjFace(Face* f)
	requires(N < 0)
{
	Base::container(this).pushBack(f);
}

template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::insertAdjFace(uint i, Face* f)
	requires(N < 0)
{
	Base::container(this).insert(i, f);
}

template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::eraseAdjFace(uint i)
	requires(N < 0)
{
	Base::container(this).erase(i);
}

template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::clearAdjFaces()
	requires(N < 0)
{
	Base::container(this).clear();
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::AdjacentFaceIterator AdjacentFaces<Face, N, El, h>::adjFaceBegin()
{
	return Base::container(this).begin();
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::AdjacentFaceIterator AdjacentFaces<Face, N, El, h>::adjFaceEnd()
{
	return Base::container(this).end();
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N, El, h>::adjFaceBegin() const
{
	return Base::container(this).begin();
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::ConstAdjacentFaceIterator
AdjacentFaces<Face, N, El, h>::adjFaceEnd() const
{
	return Base::container(this).end();
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::AdjacentFaceRangeIterator AdjacentFaces<Face, N, El, h>::adjFaces()
{
	return Base::container(this).rangeIterator();
}

template<typename Face, int N, typename El, bool h>
typename AdjacentFaces<Face, N, El, h>::ConstAdjacentFaceRangeIterator
AdjacentFaces<Face, N, El, h>::adjFaces() const
{
	return Base::container(this).rangeIterator();
}

template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::updateFaceReferences(const Face* oldBase, const Face* newBase)
{
	Base::updateElementReferences(oldBase, newBase, this);
}

template<typename Face, int N, typename El, bool h>
void AdjacentFaces<Face, N, El, h>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	Base::updateElementReferencesAfterCompact(base, newIndices, this);
}

template<typename Face, int N, typename El, bool h>
template<typename Element>
void AdjacentFaces<Face, N, El, h>::importFrom(const Element&)
{
}

template<typename Face, int N, typename El, bool h>
template<typename Element, typename ElFType>
void AdjacentFaces<Face, N, El, h>::importFaceReferencesFrom(
	const Element& e,
	Face*          base,
	const ElFType* ebase)
{
	if constexpr (HasAdjacentFaces<Element>) {
		if (isAdjacentFacesEnabledOn(e)) {
			if constexpr (N > 0) {
				// same static size
				if constexpr (N == Element::ADJ_FACE_NUMBER) {
					importReferencesFrom(e, base, ebase);
				}
				// from dynamic to static, but dynamic size == static size
				else if constexpr (Element::ADJ_FACE_NUMBER < 0) {
					if (e.adjFacesNumber() == N) {
						importReferencesFrom(e, base, ebase);
					}
				}
				else {
					// do not import in this case: cannot import from dynamic size != static size
				}
			}
			else {
				// from static/dynamic to dynamic size: need to resize first, then import
				resizeAdjFaces(e.adjFacesNumber());
				importReferencesFrom(e, base, ebase);
			}
		}
	}
}

template<typename Face, int N, typename El, bool h>
template<typename Element, typename ElFType>
void AdjacentFaces<Face, N, El, h>::importReferencesFrom(
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

} // namespace vcl::comp
