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

#include "triangle_bit_flags.h"

namespace vcl::comp {

/**
 * @brief Returns the value of the bit given in input. The bit is checked to be less than the
 * total number of assigned user bits, which in this class is 21.
 *
 * @param[in] bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
inline bool TriangleBitFlags::userBitFlag(uint bit) const
{
	// using FIRST_USER_BIT of this class
	return BitFlags::userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief TriangleBitFlags::setUserBit sets to true the value of the bit given in input. The bit is
 * checked to be less than the total number of assigned user bits, which in this class is 21.
 *
 * @param bit: the position of the bit that will be set.
 */
inline void TriangleBitFlags::setUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief TriangleBitFlags::clearUserBit sets to false the value of the bit given in input. The bit
 * is checked to be less than the total number of assigned user bits, which in this class is 21.
 *
 * @param bit: the position of the bit that will be reset.
 */
void TriangleBitFlags::unsetUserBit(uint bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::unsetUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief TriangleBitFlags::isEdgeOnBorder returns whether the ith Edge of the Triangle is marked as
 * on border.
 *
 * @param[in] i: id of the edge, must be < 3.
 * @return true if the ith Edge of the Triangle is on border, false otherwise.
 */
inline bool TriangleBitFlags::isEdgeOnBorder(uint i) const
{
	assert(i < 3);
	return flagValue(BORDER << i);
}

inline bool TriangleBitFlags::isAnyEdgeOnBorder() const
{
	return isEdgeOnBorder(0) || isEdgeOnBorder(1) || isEdgeOnBorder(2);
}

inline bool TriangleBitFlags::isEdgeSelected(uint i) const
{
	assert(i < 3);
	return flagValue(EDGESEL0 << i);
}

bool TriangleBitFlags::isAnyEdgeSelected() const
{
	return isEdgeSelected(0) || isEdgeSelected(1) || isEdgeSelected(2);
}

inline bool TriangleBitFlags::isEdgeFaux(uint i) const
{
	assert(i < 3);
	return flagValue(FAUX0 << i);
}

inline bool TriangleBitFlags::isAnyEdgeFaux() const
{
	return isEdgeFaux(0) || isEdgeFaux(1) || isEdgeFaux(2);
}

inline void TriangleBitFlags::setEdgeOnBorder(uint i)
{
	assert(i < 3);
	setFlag(BORDER << i);
}

inline void TriangleBitFlags::setEdgeSelected(uint i)
{
	assert(i < 3);
	setFlag(EDGESEL0 << i);
}

inline void TriangleBitFlags::setEdgeFaux(uint i)
{
	assert(i < 3);
	setFlag(FAUX0 << i);
}

inline void TriangleBitFlags::unsetEdgeOnBorder(uint i)
{
	assert(i < 3);
	unsetFlag(BORDER << i);
}

inline void TriangleBitFlags::unsetAllEdgesOnBorder()
{
	unsetEdgeOnBorder(0);
	unsetEdgeOnBorder(1);
	unsetEdgeOnBorder(2);
}

inline void TriangleBitFlags::unsetEdgeSelected(uint i)
{
	assert(i < 3);
	unsetFlag(EDGESEL0 << i);
}

inline void TriangleBitFlags::unsetAllEdgesSelected()
{
	unsetEdgeSelected(0);
	unsetEdgeSelected(1);
	unsetEdgeSelected(2);
}

inline void TriangleBitFlags::unsetEdgeFaux(uint i)
{
	assert(i < 3);
	unsetFlag(FAUX0 << i);
}

inline void TriangleBitFlags::unsetAllEdgeFaux()
{
	unsetEdgeFaux(0);
	unsetEdgeFaux(1);
	unsetEdgeFaux(2);
}

template<typename Element>
void TriangleBitFlags::importFrom(const Element& e)
{
	if constexpr (hasBitFlags<Element>()) {
		unsetAllFlags();
		if constexpr (hasPolygonBitFlags<Element>()) {
			// todo
		}
		else if constexpr (hasTriangleBitFlags<Element>()) {
			flags = e.BitFlags::flags;
		}
		else {
			// todo
		}
	}
}

} // namespace vcl::comp
