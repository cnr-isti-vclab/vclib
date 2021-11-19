/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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
 * @brief  PolygonBitFlags::userBitFlag returns the value of the bit given in input. The bit is
 * checked to be less than the total number of assigned user bits, which in this class is 3.
 *
 * @param bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
inline bool PolygonBitFlags::userBitFlag(unsigned int bit) const
{
	// using FIRST_USER_BIT of this class
	return BitFlags::userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief PolygonBitFlags::setUserBit sets to true the value of the bit given in input. The bit is
 * checked to be less than the total number of assigned user bits, which in this class is 3.
 *
 * @param bit: the position of the bit that will be set.
 */
inline void PolygonBitFlags::setUserBit(unsigned int bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief PolygonBitFlags::clearUserBit sets to false the value of the bit given in input. The bit
 * is checked to be less than the total number of assigned user bits, which in this class is 3.
 *
 * @param bit: the position of the bit that will be reset.
 */
inline void PolygonBitFlags::clearUserBit(unsigned int bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::clearUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief TriangleBitFlags::isEdgeOnBorder returns whether the ith Edge of the Polygon is marked as
 * on border.
 *
 * @param i: id of the edge, must be < 12.
 * @return true if the ith Edge of the Polygon is on border, false otherwise.
 */
inline bool PolygonBitFlags::isEdgeOnBorder(unsigned int i) const
{
	assert(i < 12);
	return flagValue(BORDER << i);
}

inline bool PolygonBitFlags::isAnyEdgeOnBorder() const
{
	for (unsigned int i = 0; i < 12; ++i)
		if (isEdgeOnBorder(i))
			return true;
	return false;
}

inline bool PolygonBitFlags::isEdgeSelected(unsigned int i) const
{
	assert(i < 12);
	return flagValue(EDGESEL0 << i);
}

inline bool PolygonBitFlags::isAnyEdgeSelected() const
{
	for (unsigned int i = 0; i < 12; ++i)
		if (isEdgeSelected(i))
			return true;
	return false;
}

inline bool PolygonBitFlags::isEdgeFaux(unsigned int i) const
{
	assert(i < 3);
	return flagValue(FAUX0 << i);
}

inline bool PolygonBitFlags::isAnyEdgeFaux() const
{
	return isEdgeFaux(0) || isEdgeFaux(1) || isEdgeFaux(2);
}

inline void PolygonBitFlags::setEdgeOnBorder(unsigned int i)
{
	assert(i < 12);
	setFlag(BORDER << i);
}

inline void PolygonBitFlags::setEdgeSelected(unsigned int i)
{
	assert(i < 12);
	setFlag(EDGESEL0 << i);
}

inline void PolygonBitFlags::setEdgeFaux(unsigned int i)
{
	assert(i < 3);
	setFlag(FAUX0 << i);
}

inline void PolygonBitFlags::clearEdgeOnBorder(unsigned int i)
{
	assert(i < 12);
	clearFlag(BORDER << i);
}

inline void PolygonBitFlags::clearAllEdgeOnBorder()
{
	for (unsigned int i = 0; i < 12; ++i)
		clearEdgeOnBorder(i);
}

inline void PolygonBitFlags::clearEdgeSelected(unsigned int i)
{
	assert(i < 12);
	clearFlag(EDGESEL0 << i);
}

inline void PolygonBitFlags::clearAllEdgeSelected()
{
	for (unsigned int i = 0; i < 12; ++i)
		clearEdgeSelected(i);
}

inline void PolygonBitFlags::clearEdgeFaux(unsigned int i)
{
	assert(i < 3);
	clearFlag(FAUX0 << i);
}

inline void PolygonBitFlags::clearAllEdgeFaux()
{
	clearEdgeFaux(0);
	clearEdgeFaux(1);
	clearEdgeFaux(2);
}

} // namespace vcl::comp
