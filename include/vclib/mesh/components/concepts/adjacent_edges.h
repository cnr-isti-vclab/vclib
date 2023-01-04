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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasAdjacentEdges concept is satisfied only if a Element class provides the types and
 * member functions specified in this concept. These types and member functions allow to access to
 * an AdjacentEdges component of a given element.
 *
 * Note that this concept does not discriminate between the Horizontal AdjacentEdges component and
 * the vertical OptionalAdjacentEdges component, therefore it does not guarantee that a template
 * Element type that satisfies this concept provides AdjacentEdges component at runtime (it is
 * guaranteed only that the proper member functions are available at compile time).
 *
 * To be completely sure that AdjacentEdges is available at runtime, you need to call the member
 * function `isAdjEdgesEnabled()`.
 */
template<typename T>
concept HasAdjacentEdges = requires(
	T o,
	const T& co,
	typename T::AdjacentEdgeType e,
	std::vector<typename T::AdjacentEdgeType*> v)
{
	T::ADJ_EDGE_NUMBER;
	typename T::AdjacentEdgeType;
	typename T::AdjacentEdgeIterator;
	typename T::ConstAdjacentEdgeIterator;
	typename T::AdjacentEdgeRangeIterator;
	typename T::ConstAdjacentEdgeRangeIterator;

	{ o.adjEdgesNumber() } -> std::same_as<uint>;
	{ o.adjEdge(uint()) } -> std::same_as<typename T::AdjacentEdgeType*&>;
	{ co.adjEdge(uint()) } -> std::same_as<const typename T::AdjacentEdgeType*>;
	{ o.adjEdgeMod(int()) } -> std::same_as<typename T::AdjacentEdgeType*&>;
	{ co.adjEdgeMod(int()) } -> std::same_as<const typename T::AdjacentEdgeType*>;
	{ o.setAdjEdge(&e, uint()) } -> std::same_as<void>;
	{ o.setAdjEdges(v) } -> std::same_as<void>;
	{ co.containsAdjEdge(&e) } -> std::same_as<bool>;

	{ o.findAdjEdge(&e) } -> std::same_as<typename T::AdjacentEdgeIterator>;
	{ co.findAdjEdge(&e) } -> std::same_as<typename T::ConstAdjacentEdgeIterator>;
	{ co.indexOfAdjEdge(&e) } -> std::same_as<int>;
	{ co.isAdjEdgesEnabled() } -> std::same_as<bool>;

	{ o.adjEdgeBegin() } -> std::same_as<typename T::AdjacentEdgeIterator>;
	{ o.adjEdgeEnd() } -> std::same_as<typename T::AdjacentEdgeIterator>;
	{ co.adjEdgeBegin() } -> std::same_as<typename T::ConstAdjacentEdgeIterator>;
	{ co.adjEdgeEnd() } -> std::same_as<typename T::ConstAdjacentEdgeIterator>;
	{ o.adjEdges() } -> std::same_as<typename T::AdjacentEdgeRangeIterator>;
	{ co.adjEdges() } -> std::same_as<typename T::ConstAdjacentEdgeRangeIterator>;
};

/**
 * @brief HasOptionalAdjacentEdges concept is satisfied only if a class satisfies the
 * HasAdjacentEdges concept and has the additional member function '__optionalAdjEdges()', which is
 * the discriminator between the non-optional and optional component.
 */
template<typename T>
concept HasOptionalAdjacentEdges = HasAdjacentEdges<T> && requires(T o)
{
	{ o.__optionalAdjEdges() } -> std::same_as<void>;
};

/**
 * @brief HasRightNumberOfAdjacentEdges concept
 *
 * This concept is designed to be used with Face components, where the number of adjacent edges must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * This concept is satisfied only if static number of adjacent edges is the same of the static
 * number of vertices.
 */
template<typename T>
concept HasRightNumberOfAdjacentEdges = T::VERTEX_NUMBER == T::ADJ_EDGE_NUMBER;

/**
 * @brief SanityCheckAdjacentEdges concept
 *
 * This concept is designed to be used with Face components, where the number of adjacent edges must
 * be consistent w.r.t. the number of vertices of the face.
 *
 * It is satisfied if:
 * - the component does *not* have adjacent edges, or
 * - in case it has adjacent edges, they have the same number of vertices of the face.
 */
template<typename T>
concept SanityCheckAdjacentEdges = !HasAdjacentEdges<T> || HasRightNumberOfAdjacentEdges<T>;

/* Detector function to check if a class has AdjacentEdges enabled */

template <typename T>
bool isAdjacentEdgesEnabledOn(const T& element)
{
	if constexpr (HasOptionalAdjacentEdges<T>) {
		return element.isAdjEdgesEnabled();
	}
	else {
		return HasAdjacentEdges<T>;
	}
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_ADJACENT_EDGES_H
