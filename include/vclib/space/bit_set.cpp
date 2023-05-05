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

#include "bit_set.h"

#include <stdexcept>
#include <string>

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

/**
 * @brief Returns the i-th bit value of the BitSet
 * @param[in] i: the position of the bit
 * @return the boolean value of the bit
 */
template<std::integral T>
bool BitSet<T>::operator[](uint i) const
{
	assert(i < SIZE);
	return bits & (1 << i);
}

/**
 * @brief Returns a reference of the i-th bit value of the BitSet
 * @param[in] i: the position of the bit
 * @return a reference of the boolean value of the bit
 */
template<std::integral T>
BitProxy<T> BitSet<T>::operator[](uint i)
{
	assert(i < SIZE);
	return BitProxy(bits, i);
}

/**
 * @brief Returns the i-th bit value of the BitSet, with bounds checking.
 * @param[in] i: the position of the bit
 * @throws std::out_of_range if i is >= BitSet<T>::SIZE
 * @return the boolean value of the bit
 */
template<std::integral T>
bool BitSet<T>::at(uint i) const
{
	if (i < SIZE)
		return bits & (1 << i);
	else
		throw std::out_of_range(std::to_string(i) + " out of range.");
}

/**
 * @brief Returns a reference of the i-th bit value of the BitSet, with bounds checking.
 * @param[in] i: the position of the bit
 * @throws std::out_of_range if i is >= BitSet<T>::SIZE
 * @return a reference of the boolean value of the bit
 */
template<std::integral T>
BitProxy<T> BitSet<T>::at(uint i)
{
	if (i < SIZE)
		return BitProxy(bits, i);
	else
		throw std::out_of_range(std::to_string(i) + " out of range.");
}

/**
 * @brief Returns true if all the bits of the BitSet are set to true.
 * @return true if all the bits of the BitSet are set to true.
 */
template<std::integral T>
bool BitSet<T>::all() const
{
	return ((bits+1) & bits) == 0;
}

/**
 * @brief Returns true if any of the bits of the BitSet are set to true.
 * @return true if any of the bits of the BitSet are set to true.
 */
template<std::integral T>
bool BitSet<T>::any() const
{
	return !none();
}

/**
 * @brief Returns true if none of the bits of the BitSet is set to true.
 * @return true if none of the bits of the BitSet is set to true.
 */
template<std::integral T>
bool BitSet<T>::none() const
{
	return bits == 0;
}

} // namespace vcl
