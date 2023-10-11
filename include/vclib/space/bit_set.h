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

#ifndef VCL_SPACE_BIT_SET_H
#define VCL_SPACE_BIT_SET_H

#include <stdexcept>
#include <string>

#include "bit_set/bit_proxy.h"

namespace vcl {

/**
 * @brief The BitSet class allows to treat an integral type as an array of
 * booleans of a guaranteed size.
 *
 * This class is necessary w.r.t. the std::bitset because the realization of the
 * std::bitset class is implementation defined, and the standard does not
 * constraint the size of the bitset itself.
 *
 * This class guarantess that the sizeof(BitSet<T>) is equal to sizeof(T).
 *
 * @tparam T: The type to use as a storage for the bits. It must satisfy the
 * std::integral concept.
 *
 * @ingroup space
 */
template<std::integral T>
class BitSet
{
    T bits = static_cast<T>(0);

public:
    /**
     * @brief The number of the bits of the BitSet.
     */
    static constexpr std::size_t SIZE = sizeof(T) * 8;

    /**
     * @brief Empty constructor. All the Bits of the BitSet are set to false.
     */
    BitSet() {}

    /**
     * @brief Returns the number of bits of the BitSet.
     * @return the number of bits of the BitSet.
     */
    constexpr std::size_t size() const { return SIZE; }

    /**
     * @brief Returns the i-th bit value of the BitSet
     * @param[in] i: the position of the bit
     * @return the boolean value of the bit
     */
    bool operator[](uint i) const
    {
        assert(i < SIZE);
        return bits & (1 << i);
    }

    /**
     * @brief Returns a reference of the i-th bit value of the BitSet
     * @param[in] i: the position of the bit
     * @return a reference of the boolean value of the bit
     */
    BitProxy<T> operator[](uint i)
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
    bool at(uint i) const
    {
        if (i < SIZE)
            return bits & (1 << i);
        else
            throw std::out_of_range(std::to_string(i) + " out of range.");
    }

    /**
     * @brief Returns a reference of the i-th bit value of the BitSet, with
     * bounds checking.
     * @param[in] i: the position of the bit
     * @throws std::out_of_range if i is >= BitSet<T>::SIZE
     * @return a reference of the boolean value of the bit
     */
    BitProxy<T> at(uint i)
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
    bool all() const { return bits == ~static_cast<T>(0); }

    /**
     * @brief Returns true if any of the bits of the BitSet are set to true.
     * @return true if any of the bits of the BitSet are set to true.
     */
    bool any() const { return !none(); }

    /**
     * @brief Returns true if none of the bits of the BitSet is set to true.
     * @return true if none of the bits of the BitSet is set to true.
     */
    bool none() const { return bits == static_cast<T>(0); }

    /**
     * @brief Sets all the bits to true
     * @return `*this`
     */
    BitSet<T> set()
    {
        bits = ~static_cast<T>(0);
        return *this;
    }

    /**
     * @brief Sets the bit at position i to b
     * @param[in] i: the position of the bit
     * @param[in] b: the boolena value to assign to the bit
     * @return `*this`
     */
    BitSet<T> set(uint i, bool b = true)
    {
        at(i) = b;
        return *this;
    }

    /**
     * @brief Sets all the bits to false
     * @return `*this`
     */
    BitSet<T> reset()
    {
        bits = 0;
        return *this;
    }

    /**
     * @brief Sets the bit at position i to false
     * @param[in] i: the position of the bit
     * @return `*this`
     */
    BitSet<T> reset(uint i)
    {
        at(i) = false;
        return *this;
    }

    /**
     * @brief Flips all the bits of the BitSet
     * @return `*this`
     */
    BitSet<T> flip()
    {
        bits = ~bits;
        return *this;
    }

    /**
     * @brief Flips the bit at position i
     * @param[in] i: the position of the bit
     * @return `*this`
     */
    BitSet<T> flip(uint i)
    {
        at(i) = !at(i);
        return *this;
    }
};

/* Specialization Aliases */

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

#endif // VCL_SPACE_BIT_SET_H
