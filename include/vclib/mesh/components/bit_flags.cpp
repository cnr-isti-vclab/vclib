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

/**
 * @brief Constructor that initializes the bits to false.
 */
template<typename El, bool O>
BitFlags<El, O>::BitFlags()
{
	if constexpr (!Base::IS_VERTICAL) {
		init();
	}
}

/**
 * @private
 * @brief Initializes the bits to false.
 *
 * It is made in the init function since the component could be not available
 * during construction (e.g. if the component is optional and not enabled).
 *
 * This member function is hidden by the element that inherits this class.
 */
template<typename El, bool O>
void BitFlags<El, O>::init()
{
	flags().reset();
}

/**
 * @private
 * @brief Returns `true` if the component is available, `false` otherwise.
 *
 * This member function can return `false` only if the component is optional,
 * and it is not enabled.
 *
 * This member function is hidden by the element that inherits this class.
 *
 * @return `true` if the component is available, `false` otherwise.
 */
template<typename El, bool O>
bool BitFlags<El, O>::isAvailable() const
{
	return Base::isAvailable(this);
}

/**
 * @brief Returns whether the current Element is deleted or not.
 * @return true if the Element is deleted, false otherwise.
 */
template<typename El, bool O>
bool BitFlags<El, O>::deleted() const
{
	return flags()[DELETED];
}

/**
 * @brief Accesses the 'selected' bit of this Element, returning a reference to
 * it.
 * @return a reference to the 'selected' bit of this Element.
 */
template<typename El, bool O>
auto BitFlags<El, O>::selected() -> BitProxy<FT>
{
	return flags()[SELECTED];
}

/**
 * @brief Returns whether the current Element is selected or not.
 * @return true if the Element is selected, false otherwise.
 */
template<typename El, bool O>
bool BitFlags<El, O>::selected() const
{
	return flags()[SELECTED];
}

/**
 * @brief Accesses the 'onBorder' bit of this Element, returning a reference to
 * it.
 * @return a reference to the 'onBorder' bit of this Element.
 */
template<typename El, bool O>
auto BitFlags<El, O>::onBorder() -> BitProxy<FT>
{
	return flags()[BORDER];
}

/**
 * @brief Returns whether the current Element is on border or not.
 * @return true if the Element is on border, false otherwise.
 */
template<typename El, bool O>
bool BitFlags<El, O>::onBorder() const
{
	return flags()[BORDER];
}

/**
 * @brief Accesses the 'visited' bit of this Element, returning a reference to
 * it.
 * @return a reference to the 'visited' bit of this Element.
 */
template<typename El, bool O>
auto BitFlags<El, O>::visited() -> BitProxy<FT>
{
	return flags()[VISITED];
}

/**
 * @brief Returns whether the current Element has been visited or not.
 * @return true if the Element has been visited, false otherwise.
 */
template<typename El, bool O>
bool BitFlags<El, O>::visited() const
{
	return flags()[VISITED];
}

/**
 * @brief Returns a reference to the value of the user bit of this Element given
 * in input. The bit is checked to be less than the total number of assigned
 * user bits, which in this class is 4.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 3].
 * @return a reference to the desired user bit.
 */
template<typename El, bool O>
bool BitFlags<El, O>::userBit(uint bit) const
{
	assert(bit < USER_BITS_NUMBER);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Returns the boolean value of the user bit of this Element given in
 * input. The bit is checked to be less than the total number of assigned user
 * bits, which in this class is 4.
 *
 * @param[in] bit: the position of the bit, in the interval [0 - 3], that will
 * be returned by reference.
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<typename El, bool O>
auto BitFlags<El, O>::userBit(uint bit) -> BitProxy<FT>
{
	assert(bit < USER_BITS_NUMBER);
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Unsets all the flags of this Element and sets them to `false`,
 * **except the deleted flag**, which needs to be manually reset.
 */
template<typename El, bool O>
void BitFlags<El, O>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
	deletedBit() = isD;
}

/**
 * @brief Sets all the flags of this element to the values contained in the
 * integer input parameter, that represents the bit flags of the VCG library.
 * @param[in] f: bit flags in the format of the VCG library.
 */
template<typename El, bool O>
void BitFlags<El, O>::importFromVCGFlags(int f)
{
	resetBitFlags();
	if (f & 0x0010)
		visited() = true;
	if (f & 0x0020)
		selected() = true;
	if (f & 0x0100)
		onBorder() = true;
}

/**
 * @brief Returns the bit flags of this element in the format of the VCG
 * library.
 *
 * @return an integer representing the bit flags of this element in the format
 * of the VCG library.
 */
template<typename El, bool O>
int BitFlags<El, O>::exportToVCGFlags() const
{
	int f = 0;
	if (visited())
		f &= 0x0010;
	if (selected())
		f &= 0x0020;
	if (onBorder())
		f &= 0x0100;
	return f;
}

template<typename El, bool O>
auto BitFlags<El, O>::deletedBit() -> BitProxy<FT>
{
	return flags()[DELETED];
}

template<typename El, bool O>
template<typename Element>
void BitFlags<El, O>::importFrom(const Element& e)
{
	if constexpr (HasBitFlags<Element>) {
		resetBitFlags();
		if constexpr (
			HasPolygonBitFlags<Element> || HasTriangleBitFlags<Element>)
		{
			deletedBit() = e.deleted();
			selected() = e.selected();
			visited() = e.visited();
			onBorder() = e.onBorder();
			const uint UM = std::min(USER_BITS_NUMBER, e.USER_BITS_NUMBER);
			for (uint i = 0; i < UM; ++i)
				userBit(i) = e.userBit(i);
		}
		else {
			flags() = e.flags();
		}

	}
}

template<typename El, bool O>
auto BitFlags<El, O>::flags()-> BitSet<FT>&
{
	return Base::data(this);
}

template<typename El, bool O>
auto BitFlags<El, O>::flags() const -> BitSet<FT>
{
	return Base::data(this);
}

} // namespace vcl::comp
