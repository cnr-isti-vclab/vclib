/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "triangle_bit_flags.h"

namespace mgp::prop {

/**
 * @brief  TriangleBitFlags::userBitFlag returns the value of the bit given in input. The bit is
 * checked to be greater or equal than the first user bit available in this class (11) and less
 * than 32.
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
 * checked to be greater or equal than the first user bit available in this class (11) and less
 * than 32.
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
 * is checked to be greater or equal than the first user bit available in this class (11) and less
 * than 32.
 *
 * @param bit: the position of the bit that will be reset.
 */
void TriangleBitFlags::clearUserBit(unsigned int bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::clearUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief TriangleBitFlags::isEdgeOnBorder0 returns whether the Edge 0 of the Triangle is marked as
 * on border.
 *
 * @return true if the Edge 0 of the Triangle is on border, false otherwise.
 */
inline bool TriangleBitFlags::isEdgeOnBorder0() const
{
	return BitFlags::isOnBorder();
}

/**
 * @brief TriangleBitFlags::isEdgeOnBorder1 returns whether the Edge 1 of the Triangle is marked as
 * on border.
 *
 * @return true if the Edge 1 of the Triangle is on border, false otherwise.
 */
inline bool TriangleBitFlags::isEdgeOnBorder1() const
{
	return flagValue(BORDER1);
}

/**
 * @brief TriangleBitFlags::isEdgeOnBorder2 returns whether the Edge 2 of the Triangle is marked as
 * on border.
 *
 * @return true if the Edge 2 of the Triangle is on border, false otherwise.
 */
inline bool TriangleBitFlags::isEdgeOnBorder2() const
{
	return flagValue(BORDER2);
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
	return isEdgeOnBorder0() || isEdgeOnBorder1() || isEdgeOnBorder2();
}

inline bool TriangleBitFlags::isEdgeSelected0() const
{
	return flagValue(EDGESEL0);
}

inline bool TriangleBitFlags::isEdgeSelected1() const
{
	return flagValue(EDGESEL1);
}

inline bool TriangleBitFlags::isEdgeSelected2() const
{
	return flagValue(EDGESEL2);
}

inline bool TriangleBitFlags::isEdgeSelected(unsigned int i) const
{
	assert(i < 3);
	return flagValue(EDGESEL0 << i);
}

inline bool TriangleBitFlags::isEdgeFaux0() const
{
	return flagValue(FAUX0);
}

inline bool TriangleBitFlags::isEdgeFaux1() const
{
	return flagValue(FAUX1);
}

inline bool TriangleBitFlags::isEdgeFaux2() const
{
	return flagValue(FAUX2);
}

inline bool TriangleBitFlags::isEdgeFaux(unsigned int i) const
{
	assert(i < 3);
	return flagValue(FAUX0 << i);
}

inline bool TriangleBitFlags::isAnyEdgeFaux() const
{
	return isEdgeFaux0() || isEdgeFaux1() || isEdgeFaux2();
}

inline void TriangleBitFlags::setEdgeOnBorder0()
{
	BitFlags::setOnBorder();
}

inline void TriangleBitFlags::setEdgeOnBorder1()
{
	setFlag(BORDER1);
}

inline void TriangleBitFlags::setEdgeOnBorder2()
{
	setFlag(BORDER2);
}

inline void TriangleBitFlags::setEdgeOnBorder(unsigned int i)
{
	assert(i < 3);
	setFlag(BORDER << i);
}

inline void TriangleBitFlags::setEdgeSelected0()
{
	setFlag(EDGESEL0);
}

inline void TriangleBitFlags::setEdgeSelected1()
{
	setFlag(EDGESEL1);
}

inline void TriangleBitFlags::setEdgeSelected2()
{
	setFlag(EDGESEL2);
}

inline void TriangleBitFlags::setEdgeSelected(unsigned int i)
{
	assert(i < 3);
	setFlag(EDGESEL0 << i);
}

inline void TriangleBitFlags::setEdgeFaux0()
{
	setFlag(FAUX0);
}

inline void TriangleBitFlags::setEdgeFaux1()
{
	setFlag(FAUX1);
}

inline void TriangleBitFlags::setEdgeFaux2()
{
	setFlag(FAUX2);
}

inline void TriangleBitFlags::setEdgeFaux(unsigned int i)
{
	assert(i < 3);
	setFlag(FAUX0 << i);
}

inline void TriangleBitFlags::clearEdgeOnBorder0()
{
	BitFlags::clearOnBorder();
}

inline void TriangleBitFlags::clearEdgeOnBorder1()
{
	clearFlag(BORDER1);
}

inline void TriangleBitFlags::clearEdgeOnBorder2()
{
	clearFlag(BORDER2);
}

inline void TriangleBitFlags::clearEdgeOnBorder(unsigned int i)
{
	assert(i < 3);
	clearFlag(BORDER << i);
}

inline void TriangleBitFlags::clearAllEdgeOnBorder()
{
	clearEdgeOnBorder0();
	clearEdgeOnBorder1();
	clearEdgeOnBorder2();
}

inline void TriangleBitFlags::clearEdgeSelected0()
{
	clearFlag(EDGESEL0);
}

inline void TriangleBitFlags::clearEdgeSelected1()
{
	clearFlag(EDGESEL1);
}

inline void TriangleBitFlags::clearEdgeSelected2()
{
	clearFlag(EDGESEL2);
}

inline void TriangleBitFlags::clearEdgeSelected(unsigned int i)
{
	assert(i < 3);
	clearFlag(EDGESEL0 << i);
}

inline void TriangleBitFlags::clearEdgeFaux0()
{
	clearFlag(FAUX0);
}

inline void TriangleBitFlags::clearEdgeFaux1()
{
	clearFlag(FAUX1);
}

inline void TriangleBitFlags::clearEdgeFaux2()
{
	clearFlag(FAUX2);
}

inline void TriangleBitFlags::clearEdgeFaux(unsigned int i)
{
	assert(i < 3);
	clearFlag(FAUX0 << i);
}

inline void TriangleBitFlags::clearAllEdgeFaux()
{
	clearEdgeFaux0();
	clearEdgeFaux1();
	clearEdgeFaux2();
}

} // namespace mgp::prop
