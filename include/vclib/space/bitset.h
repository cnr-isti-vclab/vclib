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

#ifndef VCL_SPACE_BITSET_H
#define VCL_SPACE_BITSET_H

#include <concepts>

#include <vclib/misc/bit_proxy.h>

namespace vcl {

/**
 * @brief The BitSet class allows to treat an integral type as an array of booleans of a guaranteed
 * size.
 *
 * This class is necessary w.r.t. the std::bitset because the realization of the std::bitset class
 * is implementation defined, and the standard does not constraint the size of the bitset itself.
 *
 * This class guarantess that the sizeof(BitSet<T>) is equal to sizeof(T).
 *
 * @tparam T: The type to use as a storage for the bits. It must satisfy the std::integral concept.
 *
 * @ingroup space
 */
template<std::integral T>
class BitSet
{
public:
	BitSet();

	static constexpr std::size_t SIZE = sizeof(T) * 8;

	constexpr std::size_t size() const;

	bool operator[](uint i) const;
	BitProxy operator[](uint i);
	
	bool at(uint i) const;
	BitProxy at(uint i);
	
	bool all() const;
	bool any() const;
	bool none() const;

private:
	T bits = 0;
};

/**
 * @brief BitSet8 is a BitSet of 8 bits.
 *
 * @ingroup space
 */
using BitSet8  = BitSet<char>;

/**
 * @brief BitSet16 is a BitSet of 16 bits.
 *
 * @ingroup space
 */
using BitSet16 = BitSet<short>;

/**
 * @brief BitSet32 is a BitSet of 32 bits.
 *
 * @ingroup space
 */
using BitSet32 = BitSet<int>;

/**
 * @brief BitSet64 is a BitSet of 64 bits.
 *
 * @ingroup space
 */
using BitSet64 = BitSet<std::size_t>;

} // namespace vcl

#include "bitset.cpp"

#endif // VCL_SPACE_BITSET_H
