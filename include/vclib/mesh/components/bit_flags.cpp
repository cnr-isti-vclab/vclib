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

#include "bit_flags.h"

namespace vcl::comp {

/**
 * @brief BitFlags::isDeleted returns whether the Element is marked as deleted.
 *
 * @return true if the Element is deleted, false otherwise.
 */
inline bool BitFlags::isDeleted() const
{
	return flagValue(DELETED);
}

/**
 * @brief BitFlags::isSelected returns whether the Element is marked as selected.
 *
 * @return true if the Element is seleced, false otherwise.
 */
inline bool BitFlags::isSelected() const
{
	return flagValue(SELECTED);
}

/**
 * @brief BitFlags::isOnBorder returns whether the Element is marked to be on border.
 *
 * @return true if the Element is marked to be on border, false otherwise.
 */
inline bool BitFlags::isOnBorder() const
{
	return flagValue(BORDER);
}

/**
 * @brief BitFlags::userBitFlag returns the value of the bit given in input. The bit is checked
 * to be less than the total number of assigned user bits, which in this class is 29.
 *
 * @param bit: the position of the bit that will be returned.
 * @return true if the required bit is enabled, false otherwise.
 */
inline bool BitFlags::userBitFlag(unsigned int bit) const
{
	return userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief BitFlags::setSelected marks as selected the Element.
 */
inline void BitFlags::setSelected()
{
	setFlag(SELECTED);
}

/**
 * @brief BitFlags::setOnBorder marks as on-border the Element.
 */
inline void BitFlags::setOnBorder()
{
	setFlag(BORDER);
}

/**
 * @brief BitFlags::setUserBit sets to true the value of the bit given in input. The bit is checked
 * to be less than the total number of assigned user bits, which in this class is 29.
 *
 * @param bit: the position of the bit that will be set.
 */
inline void BitFlags::setUserBit(unsigned int bit)
{
	setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief BitFlags::clearAllFlags clear all the flags and sets them to false, **except the deleted
 * flag**, which needs to be manually reset.
 */
inline void BitFlags::clearAllFlags()
{
	bool isD = isDeleted();
	flags    = 0;
	if (isD)
		setFlag(DELETED);
}

/**
 * @brief BitFlags::clearSelected marks as non-selected the Element.
 */
inline void BitFlags::clearSelected()
{
	clearFlag(SELECTED);
}

/**
 * @brief BitFlags::clearOnBorder marks as non-on-border the Element.
 */
inline void BitFlags::clearOnBorder()
{
	clearFlag(BORDER);
}

/**
 * @brief BitFlags::clearUserBit sets to false the value of the bit given in input. The bit is
 * checked to be less than the total number of assigned user bits, which in this class is 29.
 *
 * @param bit: the position of the bit that will be reset.
 */
inline void BitFlags::clearUserBit(unsigned int bit)
{
	clearUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief BitFlags::setDeleted marks as deleted the Element.
 */
inline void BitFlags::setDeleted()
{
	setFlag(DELETED);
}

/**
 * @brief BitFlags::clearDeleted marks as non-deleted the Element.
 */
inline void BitFlags::clearDeleted()
{
	clearFlag(DELETED);
}

/**
 * @brief BitFlags::flagValue returns the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to return.
 * @return whether the flag is true or false.
 */
inline bool BitFlags::flagValue(unsigned int flag) const
{
	return flags & flag;
}

/**
 * @brief BitFlags::setFlag sets to true the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to set.
 */
inline void BitFlags::setFlag(unsigned int flag)
{
	flags |= flag;
}

/**
 * @brief BitFlags::clearFlag sets to false the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to reset.
 */
inline void BitFlags::clearFlag(unsigned int flag)
{
	flags &= ~flag;
}

/**
 * @brief BitFlags::userBitFlag returns the value of the bit given in input. The bit is checked
 * to be less than the total number of assigned user bits.
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
inline bool BitFlags::userBitFlag(unsigned int bit, unsigned int firstBit) const
{
	assert(bit < 32 - firstBit);
	unsigned int flag = 1 << (firstBit + bit);
	return flagValue(flag);
}

/**
 * @brief BitFlags::setUserBit sets to true the value of the bit given in input. he bit is checked
 * to be less than the total number of assigned user bits.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
inline void BitFlags::setUserBit(unsigned int bit, unsigned int firstBit)
{
	assert(bit < 32 - firstBit);
	unsigned int flag = 1 << (firstBit + bit);
	setFlag(flag);
}

/**
 * @brief BitFlags::clearUserBit sets to false the value of the bit given in input. he bit is
 * checked to be less than the total number of assigned user bits.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param bit: position of the bit
 * @param firstBit: first user bit available (must be < than bit)
 */
inline void BitFlags::clearUserBit(unsigned int bit, unsigned int firstBit)
{
	assert(bit < 32 - firstBit);
	unsigned int flag = 1 << (firstBit + bit);
	clearFlag(flag);
}

} // namespace vcl::comp
