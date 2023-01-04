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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
#define VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasBitFlags concept is satisfied only if a Element class provides the member functions
 * specified in this concept. These member functions allows to access to a BitFlag component of a
 * given element.
 */
template<typename T>
concept HasBitFlags = requires(
	T o,
	const T& co)
{
	{ co.isDeleted() } -> std::same_as<bool>;
	{ co.isSelected() } -> std::same_as<bool>;
	{ co.isOnBorder() } -> std::same_as<bool>;
	{ co.userBitFlag(uint()) } -> std::same_as<bool>;

	{ o.setSelected() } -> std::same_as<void>;
	{ o.setUserBit(uint()) } -> std::same_as<void>;

	{ o.unsetAllFlags() } -> std::same_as<void>;
	{ o.unsetSelected() } -> std::same_as<void>;
	{ o.unsetUserBit(uint()) } -> std::same_as<void>;
};

/**
 * @brief Concept for internal use - contains all the flags member functions that are contained on
 * both the BitFlags components for Face Elements.
 */
template<typename T>
concept FaceBitFlagsConcept = HasBitFlags<T> &&
	requires(
		T o,
		const T& co)
{
	{ co.isEdgeOnBorder(uint()) } -> std::same_as<bool>;
	{ co.isAnyEdgeOnBorder() } -> std::same_as<bool>;

	{ co.isEdgeSelected(uint()) } -> std::same_as<bool>;
	{ co.isAnyEdgeSelected() } -> std::same_as<bool>;

	{ co.isEdgeFaux(uint()) } -> std::same_as<bool>;
	{ co.isAnyEdgeFaux() } -> std::same_as<bool>;

	{ o.setEdgeOnBorder(uint()) } -> std::same_as<void>;
	{ o.setEdgeSelected(uint()) } -> std::same_as<void>;
	{ o.setEdgeFaux(uint()) } -> std::same_as<void>;

	{ o.unsetEdgeOnBorder(uint()) } -> std::same_as<void>;
	{ o.unsetAllEdgesOnBorder() } -> std::same_as<void>;
	{ o.unsetEdgeSelected(uint()) } -> std::same_as<void>;
	{ o.unsetAllEdgesSelected() } -> std::same_as<void>;
	{ o.unsetEdgeFaux(uint()) } -> std::same_as<void>;
	{ o.unsetAllEdgesFaux() } -> std::same_as<void>;

	{ o.importFromVCGFlags(int())} -> std::same_as<void>;
	{ co.exportToVCGFlags() } -> std::same_as<int>;
};

/**
 * @brief HasPolygonBitFlags concept is satisfied only if a Element class (that should be a Face)
 * provides the member functions specified in this concept. These member functions allows to access
 * to PolygonBitFlags component of a given element.
 */
template<typename T>
concept HasPolygonBitFlags = FaceBitFlagsConcept<T> &&
	requires(T o)
{
	{ o.__polygonBitFlags() } -> std::same_as<void>;
};

/**
 * @brief HasTriangleBitFlags concept is satisfied only if a Element class (that should be a Face)
 * provides the member functions specified in this concept. These member functions allows to access
 * to TriangleBitFlags component of a given element.
 */
template<typename T>
concept HasTriangleBitFlags = FaceBitFlagsConcept<T> &&
	requires(T o)
{
	{ o.__triangleBitFlags() } -> std::same_as<void>;
};

/**
 * @brief HasFaceBitFlags concept is satisfied if one between HasPolygonBitFlags and
 * HasTriangleBitFlags concept is satisfied. This concept allows to make sure that a Face element
 * has proper FaceBitFlags (Triangle or Polygon).
 */
template<typename T>
concept HasFaceBitFlags = HasPolygonBitFlags<T> || HasTriangleBitFlags<T>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_BIT_FLAGS_H
