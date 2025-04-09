/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_SPACE_CORE_BIT_SET_H
#define VCL_SPACE_CORE_BIT_SET_H

#include "bit_set/bit_proxy.h"

#include <vclib/concepts/types.h>
#include <vclib/serialization.h>

#include <stdexcept>
#include <string>

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
 * @ingroup space_core
 */
template<std::integral T>
class BitSet
{
    T mBits = static_cast<T>(0);

public:
    /**
     * @brief The number of the bits of the BitSet.
     */
    static constexpr std::size_t SIZE = sizeof(T) * 8;

    /**
     * @brief Empty constructor. All the Bits of the BitSet are set to false.
     */
    BitSet() = default;

    /**
     * @brief Constructor from list of integral (or enum) values that represent
     * the indices of the true bits, allowing braces initialization.
     *
     * Creates a BitSet by setting to true the bits at the indices specified in
     * the list. All the other bits are set to false. Each value of the list
     * must be less than the number of bits of the BitSet.
     *
     * @param[in] l: the list of bit positions to set to true.
     */
    template<NonBoolIntegralOrEnum I>
    BitSet(std::initializer_list<I> l)
    {
        for (const auto& i : l) {
            if constexpr (std::is_enum_v<I>)
                at(toUnderlying(i)) = true;
            else
                at(i) = true;
        }
    }

    /**
     * @brief Constructor from a list of boolean values, allowing braces
     * initialization.
     *
     * Constructs the bitset from the list of boolean values. The size of the
     * list must be less or equal to the number of bits of the BitSet.
     *
     * @param[in] l: the list of boolean values to assign to the BitSet.
     */
    template<typename B>
    BitSet(std::initializer_list<B> l) requires std::same_as<bool, B>
    {
        if (l.size() > SIZE)
            throw std::invalid_argument(
                "BitSet: list size is greater than the number of bits of the "
                "BitSet");

        for (uint i = 0; const auto& b : l)
            at(i++) = b;
    }

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
        return mBits & (1 << i);
    }

    /**
     * @brief Returns a reference of the i-th bit value of the BitSet
     * @param[in] i: the position of the bit
     * @return a reference of the boolean value of the bit
     */
    BitProxy<T> operator[](uint i)
    {
        assert(i < SIZE);
        return BitProxy(mBits, i);
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
            return mBits & (1 << i);
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
            return BitProxy(mBits, i);
        else
            throw std::out_of_range(std::to_string(i) + " out of range.");
    }

    /**
     * @brief Returns true if all the bits of the BitSet are set to true.
     * @return true if all the bits of the BitSet are set to true.
     */
    bool all() const { return mBits == ~static_cast<T>(0); }

    /**
     * @brief Returns true if any of the bits of the BitSet are set to true.
     * @return true if any of the bits of the BitSet are set to true.
     */
    bool any() const { return !none(); }

    /**
     * @brief Returns true if none of the bits of the BitSet is set to true.
     * @return true if none of the bits of the BitSet is set to true.
     */
    bool none() const { return mBits == static_cast<T>(0); }

    /**
     * @brief Sets all the bits to true
     * @return `*this`
     */
    BitSet<T> set()
    {
        mBits = ~static_cast<T>(0);
        return *this;
    }

    /**
     * @brief Sets the bit at position i to b
     * @param[in] b: the boolena value to assign to the bit
     * @param[in] i: the position of the bit
     * @return `*this`
     */
    BitSet<T> set(bool b, uint i)
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
        mBits = 0;
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
        mBits = ~mBits;
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

    /**
     * @brief Returns the underlying integral value of the BitSet
     * @return the underlying integral value of the BitSet
     */
    T underlying() const { return mBits; }

    /**
     * @brief Returns a BitSet that is the result of the bitwise AND between
     * this BitSet and another BitSet.
     *
     * @param[in] other: the other BitSet
     * @return the result of the bitwise AND between this BitSet and another
     * BitSet.
     */
    BitSet<T> operator&(const BitSet<T>& other) const
    {
        return BitSet<T>(mBits & other.mBits);
    }

    /**
     * @brief Returns a BitSet that is the result of the bitwise OR between
     * this BitSet and another BitSet.
     *
     * @param[in] other: the other BitSet
     * @return the result of the bitwise OR between this BitSet and another
     * BitSet.
     */
    BitSet<T> operator|(const BitSet<T>& other) const
    {
        return BitSet<T>(mBits | other.mBits);
    }

    /**
     * @brief Returns a BitSet that is the result of the bitwise XOR between
     * this BitSet and another BitSet.
     *
     * @param[in] other: the other BitSet
     * @return the result of the bitwise XOR between this BitSet and another
     * BitSet.
     */
    BitSet<T> operator^(const BitSet<T>& other) const
    {
        return BitSet<T>(mBits ^ other.mBits);
    }

    /**
     * @brief Returns a BitSet that is the result of the bitwise NOT of this
     * BitSet.
     *
     * @return the result of the bitwise NOT of this BitSet.
     */
    BitSet<T> operator~() const { return BitSet<T>(~mBits); }

    /**
     * @brief Compound assignment operator that performs the bitwise AND between
     * this BitSet and another BitSet.
     *
     * @param[in] other: the other BitSet
     * @return `*this`
     */
    BitSet<T>& operator&=(const BitSet<T>& other)
    {
        mBits &= other.mBits;
        return *this;
    }

    /**
     * @brief Compound assignment operator that performs the bitwise OR between
     * this BitSet and another BitSet.
     *
     * @param[in] other: the other BitSet
     * @return `*this`
     */
    BitSet<T>& operator|=(const BitSet<T>& other)
    {
        mBits |= other.mBits;
        return *this;
    }

    /**
     * @brief Compound assignment operator that performs the bitwise XOR between
     * this BitSet and another BitSet.
     *
     * @param[in] other: the other BitSet
     * @return `*this`
     */
    BitSet<T>& operator^=(const BitSet<T>& other)
    {
        mBits ^= other.mBits;
        return *this;
    }

    /**
     * @brief Serializes the BitSet to the given output stream.
     * @param[in] os: The output stream.
     */
    void serialize(std::ostream& os) const { vcl::serialize(os, mBits); }

    /**
     * @brief Deserializes the BitSet from the given input stream.
     * @param[in] is: The input stream.
     */
    void deserialize(std::istream& is) { vcl::deserialize(is, mBits); }

    auto operator<=>(const BitSet<T>&) const = default;

    /// @private
    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const BitSet<U>& bs);

private:
    // constructor to initialize the BitSet with a given integral value
    BitSet(T bits) : mBits(bits) {}
};

/**
 * @brief Output stream operator for BitSet
 * @param[in] os: the output stream
 * @param[in] bs: the BitSet to output
 * @return the output stream
 */
template<typename T>
std::ostream& operator<<(std::ostream& os, const BitSet<T>& bs)
{
    os << "BitSet<" << typeid(T).name() << ">(";
    for (uint i = 0; i < bs.size(); i++)
        os << bs[i];
    os << ")";
    return os;
}

/* Specialization Aliases */

/**
 * @brief BitSet8 is a BitSet of 8 bits.
 *
 * @ingroup space_core
 */
using BitSet8 = BitSet<char>;

/**
 * @brief BitSet16 is a BitSet of 16 bits.
 *
 * @ingroup space_core
 */
using BitSet16 = BitSet<short>;

/**
 * @brief BitSet32 is a BitSet of 32 bits.
 *
 * @ingroup space_core
 */
using BitSet32 = BitSet<int>;

/**
 * @brief BitSet64 is a BitSet of 64 bits.
 *
 * @ingroup space_core
 */
using BitSet64 = BitSet<std::size_t>;

} // namespace vcl

#endif // VCL_SPACE_CORE_BIT_SET_H
