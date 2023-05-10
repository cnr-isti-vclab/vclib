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

#include "bit_flags.h"

namespace vcl::comp {

template<typename El, bool o>
BitFlags<El, o>::BitFlags()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

template<typename El, bool o>
void BitFlags<El, o>::init()
{
	flags().reset();
}

template<typename El, bool o>
bool BitFlags<El, o>::isEnabled() const
{
	return Base::isEnabled(this);
}

/**
 * @brief Returns whether the current Element is deleted or not.
 * @return true if the Element is deleted, false otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::deleted() const
{
	return flags()[DELETED];
}

/**
 * @brief Accesses the 'selected' bit of this Element, returning a reference to it.
 * @return a reference to the 'selected' bit of this Element.
 */
template<typename El, bool o>
BitProxy<typename BitFlags<El, o>::FT> BitFlags<El, o>::selected()
{
	return flags()[SELECTED];
}

/**
 * @brief Returns whether the current Element is selected or not.
 * @return true if the Element is selected, false otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::selected() const
{
	return flags()[SELECTED];
}

/**
 * @brief Accesses the 'onBorder' bit of this Element, returning a reference to it.
 * @return a reference to the 'onBorder' bit of this Element.
 */
template<typename El, bool o>
BitProxy<typename BitFlags<El, o>::FT> BitFlags<El, o>::onBorder()
{
	return flags()[BORDER];
}

/**
 * @brief Returns whether the current Element is on border or not.
 * @return true if the Element is on border, false otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::onBorder() const
{
	return flags()[BORDER];
}

/**
 * @brief Accesses the 'visited' bit of this Element, returning a reference to it.
 * @return a reference to the 'visited' bit of this Element.
 */
template<typename El, bool o>
BitProxy<typename BitFlags<El, o>::FT> BitFlags<El, o>::visited()
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Element has been visited or not.
 * @return true if the Element has been visited, false otherwise.
 */
template<typename El, bool o>
bool BitFlags<El, o>::visited() const
{
	return flags()[VISITED];
}

/**
 * @brief Returns a reference to the value of the user bit of this Element given in input. The bit
 * is checked to be less than the total number of assigned user bits, which in this class is 4.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 3].
 * @return a reference to the desired user bit.
 */
template<typename El, bool o>
bool BitFlags<El, o>::userBit(uint bit) const
{
	assert(bit < N_USER_BITS);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Returns the boolean value of the user bit of this Element given in input. The bit
 * is checked to be less than the total number of assigned user bits, which in this class is 4.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 3], that will be returned by
 * reference.
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<typename El, bool o>
BitProxy<typename BitFlags<El, o>::FT> BitFlags<El, o>::userBit(uint bit)
{
	assert(bit < N_USER_BITS);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Unsets all the flags of this Element and sets them to `false`, **except the deleted
 * flag**, which needs to be manually reset.
 */
template<typename El, bool o>
void BitFlags<El, o>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
	deleted() = isD;
}

template<typename El, bool o>
void BitFlags<El, o>::importFromVCGFlags(int f)
{
	resetBitFlags();
	if (f & 0x0020)
		selected() = true;
	if (f & 0x0100)
		onBorder() = true;
}

template<typename El, bool o>
int BitFlags<El, o>::exportToVCGFlags() const
{
	int f = 0;
	if (selected())
		f &= 0x0020;
	if (onBorder())
		f &= 0x0100;
	return f;
}

template<typename El, bool o>
BitProxy<typename BitFlags<El, o>::FT> BitFlags<El, o>::deleted()
{
	return flags()[DELETED];
}

template<typename El, bool o>
template<typename Element>
void BitFlags<El, o>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		resetBitFlags();
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
BitSet<typename BitFlags<El, o>::FT>& BitFlags<El, o>::flags()
{
	return Base::data(this);
}

template<typename El, bool o>
BitSet<typename BitFlags<El, o>::FT> BitFlags<El, o>::flags() const
{
	return Base::data(this);
}

} // namespace vcl::comp
