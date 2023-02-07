/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "bit_flags.h"

namespace vcl::comp {

template<typename El, bool o>
void BitFlags<El, o>::init()
{
	flags() = 0;
}

/**
 * @brief Returns whether this Element is marked as deleted.
 *
 * @return `true` if this Element is deleted, `false` otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::isDeleted() const
{
	return flagValue(DELETED);
}

/**
 * @brief Returns whether this Element is marked as selected.
 *
 * @return `true` if this Element is seleced, `false` otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::isSelected() const
{
	return flagValue(SELECTED);
}

/**
 * @brief Returns whether this Element is marked to be on border.
 *
 * @return `true` if this Element is marked to be on border, `false` otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::isOnBorder() const
{
	return flagValue(BORDER);
}

/**
 * @brief Returns the value of the user bit of this Element given in input. The bit is checked
 * to be less than the total number of assigned user bits, which in this class is 29.
 *
 * @param[in] bit: the position of the bit that will be returned.
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::userBitFlag(uint bit) const
{
	return userBitFlag(bit, FIRST_USER_BIT);
}

/**
 * @brief Marks as selected this Element.
 */
template<typename El, bool o>
void BitFlags<El, o>::setSelected()
{
	setFlag(SELECTED);
}

/**
 * @brief Marks as on-border this Element.
 */
template<typename El, bool o>
void BitFlags<El, o>::setOnBorder()
{
	setFlag(BORDER);
}

/**
 * @brief Sets to `true` the value of the bit of this Element given in input. The bit is checked
 * to be less than the total number of assigned user bits, which in this class is 29.
 *
 * @param[in] bit: the position of the bit that will be set.
 */
template<typename El, bool o>
void BitFlags<El, o>::setUserBit(uint bit)
{
	setUserBit(bit, FIRST_USER_BIT);
}

/**
 * @brief Unsets all the flags of this Element and sets them to `false`, **except the deleted
 * flag**, which needs to be manually reset.
 */
template<typename El, bool o>
void BitFlags<El, o>::unsetAllFlags()
{
	bool isD = isDeleted();
	flags()  = 0;
	if (isD)
		setFlag(DELETED);
}

/**
 * @brief Marks as non-selected this Element.
 */
template<typename El, bool o>
void BitFlags<El, o>::unsetSelected()
{
	unsetFlag(SELECTED);
}

/**
 * @brief Marks as non-on-border this Element.
 */
template<typename El, bool o>
void BitFlags<El, o>::unsetOnBorder()
{
	unsetFlag(BORDER);
}

/**
 * @brief Sets to `false` the value of the user bit of this Element given in input. The bit is
 * checked to be less than the total number of assigned user bits, which in this class is 29.
 *
 * @param[in] bit: the position of the bit that will be reset.
 */
template<typename El, bool o>
void BitFlags<El, o>::unsetUserBit(uint bit)
{
	unsetUserBit(bit, FIRST_USER_BIT);
}

template<typename El, bool o>
void BitFlags<El, o>::importFromVCGFlags(int f)
{
	unsetAllFlags();
	if (f & 0x0020)
		setSelected();
	if (f & 0x0100)
		setOnBorder();
}

template<typename El, bool o>
int BitFlags<El, o>::exportToVCGFlags() const
{
	int f = 0;
	if (isSelected())
		f &= 0x0020;
	if (isOnBorder())
		f &= 0x0100;
	return f;
}

/**
 * @brief Marks as deleted this Element.
 */
template<typename El, bool o>
void BitFlags<El, o>::setDeleted()
{
	setFlag(DELETED);
}

/**
 * @brief Marks as non-deleted this Element.
 */
template<typename El, bool o>
void BitFlags<El, o>::unsetDeleted()
{
	unsetFlag(DELETED);
}

/**
 * @brief Returns the value of the given flag (value of the enum).
 *
 * @param flag: value of the flag to return.
 * @return whether the flag is `true` or `false`.
 */
template<typename El, bool o>
bool BitFlags<El, o>::flagValue(uint flag) const
{
	return flags() & flag;
}

/**
 * @brief Sets to `true` the value of the given flag (value of the enum).
 *
 * @param[in] flag: value of the flag to set.
 */
template<typename El, bool o>
void BitFlags<El, o>::setFlag(uint flag)
{
	flags() |= flag;
}

/**
 * @brief Sets to `false` the value of the given flag (value of the enum).
 *
 * @param[in] flag: value of the flag to reset.
 */
template<typename El, bool o>
void BitFlags<El, o>::unsetFlag(uint flag)
{
	flags() &= ~flag;
}

/**
 * @brief Returns the value of the bit given in input. The bit is checked
 * to be less than the total number of assigned user bits.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param[in] bit: position of the bit
 * @param[in] firstBit: first user bit available (must be < than bit)
 * @return the value of the bit
 */
template<typename El, bool o>
bool BitFlags<El, o>::userBitFlag(uint bit, uint firstBit) const
{
	assert(bit < 32 - firstBit);
	uint flag = 1 << (firstBit + bit);
	return flagValue(flag);
}

/**
 * @brief Sets to `true` the value of the bit given in input. The bit is
 * checked to be less than the total number of assigned user bits.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param[in] bit: position of the bit
 * @param[in] firstBit: first user bit available (must be < than bit)
 */
template<typename El, bool o>
void BitFlags<El, o>::setUserBit(uint bit, uint firstBit)
{
	assert(bit < 32 - firstBit);
	uint flag = 1 << (firstBit + bit);
	setFlag(flag);
}

/**
 * @brief Sets to `false` the value of the bit given in input. he bit is
 * checked to be less than the total number of assigned user bits.
 *
 * This member function is meant to be used by derived clases that may have a FIRST_USER_BIT
 * different from the one of this class, and it is useful to avoid code duplication,
 * In fact the value of firstBit should be exactly the first available bit that can be used as
 * custom bit by the user.
 *
 * @param[in] bit: position of the bit
 * @param[in] firstBit: first user bit available (must be < than bit)
 */
template<typename El, bool o>
void BitFlags<El, o>::unsetUserBit(uint bit, uint firstBit)
{
	assert(bit < 32 - firstBit);
	uint flag = 1 << (firstBit + bit);
	unsetFlag(flag);
}

template<typename El, bool o>
template<typename Element>
void BitFlags<El, o>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		unsetAllFlags();
		if constexpr (HasPolygonBitFlags<Element>) {
			// todo
		}
		else if constexpr (HasTriangleBitFlags<Element>) {
			// todo
		}
		else {
			flags() = e.flags();
		}
	}
}

template<typename El, bool o>
int& BitFlags<El, o>::flags()
{
	return data.template get<El>(this);
}

template<typename El, bool o>
const int& BitFlags<El, o>::flags() const
{
	return data.template get<El>(this);
}

} // namespace vcl::comp
