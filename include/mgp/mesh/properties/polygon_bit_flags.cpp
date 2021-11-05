/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "polygon_bit_flags.h"

namespace mgp::prop {

/**
 * @brief  PolygonBitFlags::userBitFlag returns the value of the bit given in input. The bit is
 * checked to be greater or equal than the first user bit available in this class (24) and less
 * than 32.
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
 * checked to be greater or equal than the first user bit available in this class (24) and less
 * than 32.
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
 * is checked to be greater or equal than the first user bit available in this class (24) and less
 * than 32.
 *
 * @param bit: the position of the bit that will be reset.
 */
inline void PolygonBitFlags::clearUserBit(unsigned int bit)
{
	// using FIRST_USER_BIT of this class
	BitFlags::clearUserBit(bit, FIRST_USER_BIT);
}

}
