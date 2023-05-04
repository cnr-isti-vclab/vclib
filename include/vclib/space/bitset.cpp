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

#include "bitset.h"

namespace vcl {

/**
 * @brief Empty constructor. All the Bits of the BitSet are set to false.
 */
template <std::integral T>
BitSet<T>::BitSet()
{
}

/**
 * @brief Returns the number of bits of the BitSet.
 * @return the number of bits of the BitSet.
 */
template<std::integral T>
constexpr std::size_t BitSet<T>::size() const
{
	return SIZE;
}

template<std::integral T>
bool BitSet<T>::operator[](uint i) const
{
	assert(i < SIZE);
	return bits & (1 << i);
}

template<std::integral T>
BitProxy BitSet<T>::operator[](uint i)
{
	assert(i < SIZE);
	return BitProxy(bits, i);
}



} // namespace vcl
