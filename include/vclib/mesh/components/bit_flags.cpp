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

template<typename C, typename El, bool o>
BitFlagsT<C, El, o>::BitFlagsT()
{
	if constexpr (!IS_VERTICAL) {
		init();
	}
}

template<typename C, typename El, bool o>
void BitFlagsT<C, El, o>::init()
{
	flags().reset();
}

template<typename C, typename El, bool o>
bool BitFlagsT<C, El, o>::isEnabled() const
{
	return data.template isComponentEnabled<El>(static_cast<const C*>(this));
}

template<typename Component, typename ElementType, bool optional>
bool BitFlagsT<Component, ElementType, optional>::deleted() const
{
	return flags()[DELETED];
}

template<typename Component, typename ElementType, bool optional>
BitProxy<int> BitFlagsT<Component, ElementType, optional>::selected()
{
	return flags()[SELECTED];
}

template<typename Component, typename ElementType, bool optional>
bool BitFlagsT<Component, ElementType, optional>::selected() const
{
	return flags()[SELECTED];
}

template<typename Component, typename ElementType, bool optional>
BitProxy<int> BitFlagsT<Component, ElementType, optional>::onBorder()
{
	return flags()[BORDER];
}

template<typename Component, typename ElementType, bool optional>
bool BitFlagsT<Component, ElementType, optional>::onBorder() const
{
	return flags()[BORDER];
}

/**
 * @brief Returns the value of the user bit of this Element given in input. The bit is checked
 * to be less than the total number of assigned user bits, which in this class is 28.
 *
 * @param[in] bit: the position of the bit that will be returned.
 * @return `true` if the required bit is enabled, `false` otherwise.
 */
template<typename C, typename El, bool o>
bool BitFlagsT<C, El, o>::userBit(uint bit) const
{
	return flags()[bit + FIRST_USER_BIT];
}

template<typename Component, typename ElementType, bool optional>
BitProxy<int> BitFlagsT<Component, ElementType, optional>::userBit(uint bit)
{
	return flags()[bit + FIRST_USER_BIT];
}

/**
 * @brief Unsets all the flags of this Element and sets them to `false`, **except the deleted
 * flag**, which needs to be manually reset.
 */
template<typename C, typename El, bool o>
void BitFlagsT<C, El, o>::resetBitFlags()
{
	bool isD = deleted();
	flags().reset();
	deleted() = isD;
}

template<typename C, typename El, bool o>
void BitFlagsT<C, El, o>::importFromVCGFlags(int f)
{
	resetBitFlags();
	if (f & 0x0020)
		selected() = true;
	if (f & 0x0100)
		onBorder() = true;
}

template<typename C, typename El, bool o>
int BitFlagsT<C, El, o>::exportToVCGFlags() const
{
	int f = 0;
	if (selected())
		f &= 0x0020;
	if (onBorder())
		f &= 0x0100;
	return f;
}

template<typename Component, typename ElementType, bool optional>
BitProxy<int> BitFlagsT<Component, ElementType, optional>::deleted()
{
	return flags()[DELETED];
}

template<typename C, typename El, bool o>
template<typename Element>
void BitFlagsT<C, El, o>::importFrom(const Element& e)
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

template<typename C, typename El, bool o>
BitSet<int>& BitFlagsT<C, El, o>::flags()
{
	return data.template get<El>(static_cast<C*>(this));
}

template<typename C, typename El, bool o>
BitSet<int> BitFlagsT<C, El, o>::flags() const
{
	return data.template get<El>(static_cast<const C*>(this));
}

} // namespace vcl::comp
