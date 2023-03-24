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

#ifndef VCL_CONCEPT_MESH_COMPONENT_ADJACENT_FACES_H
#define VCL_CONCEPT_MESH_COMPONENT_ADJACENT_FACES_H

#include <vector>

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasAdjacentFaces concept is satisfied only if a Element class provides the types and
 * member functions specified in this concept. These types and member functions allow to access to
 * an AdjacentFaces component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal AdjacentFaces component and
 * the vertical OptionalAdjacentFaces component, therefore it does not guarantee that a template
 * Element type that satisfies this concept provides AdjacentFaces component at runtime (it is
 * guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that AdjacentFaces is available at runtime, you need to call the member
 * function `isAdjFacesEnabled()`.
 */
template<typename T>
concept HasAdjacentFaces = requires(
	T o,
	const T& co,
	typename T::AdjacentFaceType f,
	std::vector<typename T::AdjacentFaceType*> v)
{
	T::ADJ_FACE_NUMBER;
	typename T::AdjacentFaceType;
	typename T::AdjacentFaceIterator;
	typename T::ConstAdjacentFaceIterator;
	typename T::AdjacentFaceRange;
	typename T::ConstAdjacentFaceRange;

	{ o.adjFacesNumber() } -> std::same_as<uint>;
	{ o.adjFace(uint()) } -> std::same_as<typename T::AdjacentFaceType*&>;
	{ co.adjFace(uint()) } -> std::same_as<const typename T::AdjacentFaceType*>;
	{ o.adjFaceMod(int()) } -> std::same_as<typename T::AdjacentFaceType*&>;
	{ co.adjFaceMod(int()) } -> std::same_as<const typename T::AdjacentFaceType*>;
	{ o.setAdjFace(&f, uint()) } -> std::same_as<void>;
	{ o.setAdjFaces(v) } -> std::same_as<void>;
	{ co.containsAdjFace(&f) } -> std::same_as<bool>;

	{ o.findAdjFace(&f) } -> std::same_as<typename T::AdjacentFaceIterator>;
	{ co.findAdjFace(&f) } -> std::same_as<typename T::ConstAdjacentFaceIterator>;
	{ co.indexOfAdjFace(&f) } -> std::same_as<int>;
	{ co.isAdjFacesEnabled() } -> std::same_as<bool>;

	{ o.adjFaceBegin() } -> std::same_as<typename T::AdjacentFaceIterator>;
	{ o.adjFaceEnd() } -> std::same_as<typename T::AdjacentFaceIterator>;
	{ co.adjFaceBegin() } -> std::same_as<typename T::ConstAdjacentFaceIterator>;
	{ co.adjFaceEnd() } -> std::same_as<typename T::ConstAdjacentFaceIterator>;
	{ o.adjFaces() } -> std::same_as<typename T::AdjacentFaceRange>;
	{ co.adjFaces() } -> std::same_as<typename T::ConstAdjacentFaceRange>;
};

/**
 * @brief HasAdjacentFacesComponent concept is used to discriminate between the AdjacentFaces (or
 * OptionalAdjacentFaces) component, and the FaceHalfEdgeReferences component, which using half
 * edges in a Dcel Mesh data structure, allows to access to adjacent faces. This concept is intended
 * only for internal use, useful to check that a Face does not have both AdjacentFaces and
 * FaceHalfEdgeReferences components.
 */
template<typename T>
concept HasAdjacentFacesComponent = requires(T o)
{
	requires HasAdjacentFaces<T>;
	typename T::AdjacentFacesComponent;
};

/**
 * @brief HasOptionalAdjacentFaces concept is satisfied only if a class satisfies the
 * HasAdjacentFacesComponent concept and the static boolean constant IS_OPTIONAL is set to true.
 */
template<typename T>
concept HasOptionalAdjacentFaces =
	HasAdjacentFacesComponent<T> && IsOptionalComponent<typename T::AdjacentFacesComponent>;

/**
 * @brief HasRightNumberOfAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of adjacent faces must
 * be consisted w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent faces is the same of the static
 * number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentFaces = T::VERTEX_NUMBER == T::ADJ_FACE_NUMBER;

/**
 * @brief SanityCheckAdjacentFaces concept
 *
 * This concept is designed to be used with Face components, where the number of adjacent faces must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have adjacent faces;
 * - in case it has adjacent faces, they have the same number of vertices of the face.
 */
template<typename T>
concept SanityCheckAdjacentFaces = !HasAdjacentFaces<T> || HasRightNumberOfAdjacentFaces<T>;

/* Detector function to check if a class has AdjacentFaces enabled */

template <typename T>
bool isAdjacentFacesEnabledOn(const T& element)
{
	if constexpr (HasOptionalAdjacentFaces<T>) {
		return element.isAdjFacesEnabled();
	}
	else {
		return HasAdjacentFaces<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_CONCEPT_MESH_COMPONENT_ADJACENT_FACES_H
