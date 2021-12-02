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

#include "mutable_bit_flags.h"

namespace vcl::comp {

/**
 * @brief MutableBitFlags::isVisitedM returns whether the Element is marked as visited.
 *
 * @return true if the Element is visited, false otherwise.
 */
inline bool MutableBitFlags::isVisitedM() const
{
	return flagValueM(VISITED);
}

/**
 * @brief MutableBitFlags::userBitFlagM returns the value of the bit given in input. The bit is
 * checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 *
 * @param bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
inline bool MutableBitFlags::userBitFlagM(uint bit) const
{
	return userBitFlagM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief MutableBitFlags::setVisitedM marks as visited the Element.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 */
inline void MutableBitFlags::setVisitedM() const
{
	setFlagM(VISITED);
}

/**
 * @brief MutableBitFlags::setUserBitM sets to true the value of the bit given in input. The bit is
 * checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: the position of the bit that will be set.
 */
inline void MutableBitFlags::setUserBitM(uint bit) const
{
	setUserBitM(bit, FIRST_MUTABLE_USER_BIT);
}

/**
 * @brief MutableBitFlags::clearAllFlagsM clear all the flags and sets them to false.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 */
inline void MutableBitFlags::clearAllFlagsM() const
{
	mutableFlags = 0;
}

/**
 * @brief MutableBitFlags::clearVisitedM marks as non-visited the Element.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 */
inline void MutableBitFlags::clearVisitedM() const
{
	clearFlagM(VISITED);
}

/**
 * @brief MutableBitFlags::clearUserBitM sets to false the value of the bit given in input. The bit
 * is checked to be greater or equal than the first user bit available in this class (1) and less
 * than 32.
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: the position of the bit that will be reset.
 */
inline void MutableBitFlags::clearUserBitM(uint bit) const
{
	clearUserBitM(bit, FIRST_MUTABLE_USER_BIT);
}

template<typename Element>
void MutableBitFlags::importFrom(const Element& e)
{
	if constexpr (hasMutableBitFlags<Element>()) {
		if (isMutableBitFlagsEnabled(e)) {
			mutableFlags = e.MutableBitFlags::mutableFlags;
		}
	}
}

/**
 * @brief MutableBitFlags::flagValueM returns the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to return.
 * @return whether the flag is true or false.
 */
inline bool MutableBitFlags::flagValueM(uint flag) const
{
	return mutableFlags & flag;
}

/**
 * @brief MutableBitFlags::setFlagM sets to true the value of the given flag (value of the enum).
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param flag: value of the flag to set.
 */
inline void MutableBitFlags::setFlagM(uint flag) const
{
	mutableFlags |= flag;
}

/**
 * @brief MutableBitFlags::clearFlagM sets to false the value of the given flag (value of the enum).
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param flag: value of the flag to reset.
 */
inline void MutableBitFlags::clearFlagM(uint flag) const
{
	mutableFlags &= ~flag;
}

/**
 * @brief MutableBitFlags::userBitFlagM returns the value of the bit given in input. The bit is
 * checked to be greateror equal  than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 * @return the value of the bit
 */
inline bool MutableBitFlags::userBitFlagM(uint bit, uint firstBit) const
{
	assert(bit >= firstBit && bit < 32); (void) firstBit;
	uint flag = 1 << bit;
	return flagValueM(flag);
}

/**
 * @brief MutableBitFlags::setUserBitM sets to true the value of the bit given in input. The bit is
 * checked to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
inline void MutableBitFlags::setUserBitM(uint bit, uint firstBit) const
{
	assert(bit >= firstBit && bit < 32); (void) firstBit;
	uint flag = 1 << bit;
	setFlagM(flag);
}

/**
 * @brief MutableBitFlags::clearUserBitM sets to false the value of the bit given in input. The bit
 * is checked to be greater or equal than the firstBit and less than 32.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @warning this function sets a **mutable** member. Const-ness is not guaranteed.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
inline void MutableBitFlags::clearUserBitM(uint bit, uint firstBit) const
{
	assert(bit >= firstBit && bit < 32); (void) firstBit;
	uint flag = 1 << bit;
	clearFlagM(flag);
}

} // namespace vcl::comp
