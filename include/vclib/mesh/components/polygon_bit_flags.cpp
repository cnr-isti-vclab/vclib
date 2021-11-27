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

#include "polygon_bit_flags.h"

namespace vcl::comp {

/**
 * @brief  Returns the value of the bit given in input. The bit is checked to be less than the total
 * number of assigned user bits, which in this class is 3.
 *
 * @param[in] bit: The position of the bit that will be returned.
 * @return `true` if the required bit is enabled, false otherwise.
 */
inline bool PolygonBitFlags::userBitFlag(uint bit) const
{
	// using FIRST_USER_BIT of this class
	return BitFlags::userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief Sets to `true` the value of the bit given in input. The bit is checked to be less than the
 * total number of assigned user bits, which in this class is 3.
 *
 * @param[in] bit: The position of the bit that will be set.
 */
inline void PolygonBitFlags::setUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief Sets to `false` the value of the bit given in input. The bit is checked to be less than
 * the total number of assigned user bits, which in this class is 3.
 *
 * @param[in] bit: The position of the bit that will be reset.
 */
inline void PolygonBitFlags::unsetUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::unsetUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief Returns whether the ith edge of this polygon is marked as on border.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 * @return `true` if the ith edge of the Polygon is on border, false otherwise.
 */
inline bool PolygonBitFlags::isEdgeOnBorder(uint i) const
{
	assert(i < 12);
	return flagValue(BORDER << i);
}

/**
 * @brief Returns `true` if at least one edge of this polygon is marked as on border.
 *
 * @return `true` if any edge is on border.
 */
inline bool PolygonBitFlags::isAnyEdgeOnBorder() const
{
	for (uint i = 0; i < 12; ++i)
		if (isEdgeOnBorder(i))
			return true;
	return false;
}

/**
 * @brief Returns whether the ith edge of this polygon is selected.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 * @return `true` if the ith edge is selected.
 */
inline bool PolygonBitFlags::isEdgeSelected(uint i) const
{
	assert(i < 12);
	return flagValue(EDGESEL0 << i);
}

/**
 * @brief Returns `true` if at least one edge of this polygon is selected.
 *
 * @return `true` if any edge is selected.
 */
inline bool PolygonBitFlags::isAnyEdgeSelected() const
{
	for (uint i = 0; i < 12; ++i)
		if (isEdgeSelected(i))
			return true;
	return false;
}

/**
 * @brief Returns whether the ith edge of this polygon is marked as faux.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 3. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 * @return `true` if the ith edge is faux.
 */
inline bool PolygonBitFlags::isEdgeFaux(uint i) const
{
	assert(i < 3);
	return flagValue(FAUX0 << i);
}

/**
 * @brief Returns `true` if at least one edge of this polygon is marked as faux.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @return `true` if any edge is selected.
 */
inline bool PolygonBitFlags::isAnyEdgeFaux() const
{
	return isEdgeFaux(0) || isEdgeFaux(1) || isEdgeFaux(2);
}

/**
 * @brief Sets the ith edge of this polygon as on border.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
inline void PolygonBitFlags::setEdgeOnBorder(uint i)
{
	assert(i < 12);
	setFlag(BORDER << i);
}

/**
 * @brief Sets the ith edge of this polygon as selected.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
inline void PolygonBitFlags::setEdgeSelected(uint i)
{
	assert(i < 12);
	setFlag(EDGESEL0 << i);
}

/**
 * @brief Sets the ith edge of this polygon as faux.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 3. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
inline void PolygonBitFlags::setEdgeFaux(uint i)
{
	assert(i < 3);
	setFlag(FAUX0 << i);
}

/**
 * @brief Marks as non-on-border the ith edge of this polygon.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
inline void PolygonBitFlags::unsetEdgeOnBorder(uint i)
{
	assert(i < 12);
	unsetFlag(BORDER << i);
}

/**
 * @brief Marks as non-on-border alle the edges of this polygon.
 */
inline void PolygonBitFlags::unsetAllEdgesOnBorder()
{
	for (uint i = 0; i < 12; ++i)
		unsetEdgeOnBorder(i);
}

/**
 * @brief Marks as non-selected the ith edge of this polygon.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 12. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
inline void PolygonBitFlags::unsetEdgeSelected(uint i)
{
	assert(i < 12);
	unsetFlag(EDGESEL0 << i);
}

/**
 * @brief Marks as non-selected alle the edges of this polygon.
 */
inline void PolygonBitFlags::unsetAllEdgesSelected()
{
	for (uint i = 0; i < 12; ++i)
		unsetEdgeSelected(i);
}

/**
 * @brief Marks as non-faux the ith edge of this polygon.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 *
 * @param[in] i: Index of the edge in the polygon, must be < 3. No sanity check is performed on
 * this parameter about the actual size of the polygon.
 */
inline void PolygonBitFlags::unsetEdgeFaux(uint i)
{
	assert(i < 3);
	unsetFlag(FAUX0 << i);
}

/**
 * @brief Marks as non-faux alle the edges of this polygon.
 *
 * @note Faux edges in Polygons make no sense. This function has been left in this Component
 * just for compatibility reasons between Triangle and Polygon meshes.
 */
inline void PolygonBitFlags::unsetAllEdgesFaux()
{
	unsetEdgeFaux(0);
	unsetEdgeFaux(1);
	unsetEdgeFaux(2);
}

} // namespace vcl::comp
