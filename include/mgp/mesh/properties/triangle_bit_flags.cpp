/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "triangle_bit_flags.h"

namespace mgp::prop {

/**
 * @brief  TriangleBitFlags::userBitFlag returns the value of the bit given in input. The bit is
 * checked to be less than the total number of assigned user bits, which in this class is 21.
 *
 * @param bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
inline bool TriangleBitFlags::userBitFlag(unsigned int bit) const
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
inline void TriangleBitFlags::setUserBit(unsigned int bit)
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
void TriangleBitFlags::clearUserBit(unsigned int bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::clearUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief TriangleBitFlags::isEdgeOnBorder returns whether the ith Edge of the Triangle is marked as
 * on border.
 *
 * @param i: id of the edge, must be < 3.
 * @return true if the ith Edge of the Triangle is on border, false otherwise.
 */
inline bool TriangleBitFlags::isEdgeOnBorder(unsigned int i) const
{
	assert(i < 3);
	return flagValue(BORDER << i);
}

inline bool TriangleBitFlags::isAnyEdgeOnBorder() const
{
	return isEdgeOnBorder(0) || isEdgeOnBorder(1) || isEdgeOnBorder(2);
}

inline bool TriangleBitFlags::isEdgeSelected(unsigned int i) const
{
	assert(i < 3);
	return flagValue(EDGESEL0 << i);
}

bool TriangleBitFlags::isAnyEdgeSelected()
{
	return isEdgeSelected(0) || isEdgeSelected(1) || isEdgeSelected(2);
}

inline bool TriangleBitFlags::isEdgeFaux(unsigned int i) const
{
	assert(i < 3);
	return flagValue(FAUX0 << i);
}

inline bool TriangleBitFlags::isAnyEdgeFaux() const
{
	return isEdgeFaux(0) || isEdgeFaux(1) || isEdgeFaux(2);
}

inline void TriangleBitFlags::setEdgeOnBorder(unsigned int i)
{
	assert(i < 3);
	setFlag(BORDER << i);
}

inline void TriangleBitFlags::setEdgeSelected(unsigned int i)
{
	assert(i < 3);
	setFlag(EDGESEL0 << i);
}

inline void TriangleBitFlags::setEdgeFaux(unsigned int i)
{
	assert(i < 3);
	setFlag(FAUX0 << i);
}

inline void TriangleBitFlags::clearEdgeOnBorder(unsigned int i)
{
	assert(i < 3);
	clearFlag(BORDER << i);
}

inline void TriangleBitFlags::clearAllEdgeOnBorder()
{
	clearEdgeOnBorder(0);
	clearEdgeOnBorder(1);
	clearEdgeOnBorder(2);
}

inline void TriangleBitFlags::clearEdgeSelected(unsigned int i)
{
	assert(i < 3);
	clearFlag(EDGESEL0 << i);
}

inline void TriangleBitFlags::clearEdgeFaux(unsigned int i)
{
	assert(i < 3);
	clearFlag(FAUX0 << i);
}

inline void TriangleBitFlags::clearAllEdgeFaux()
{
	clearEdgeFaux(0);
	clearEdgeFaux(1);
	clearEdgeFaux(2);
}

} // namespace mgp::prop
