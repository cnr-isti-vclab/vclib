// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_SPACE_CORE_VECTOR_BIT_VECTOR_H
#define VCL_SPACE_CORE_VECTOR_BIT_VECTOR_H

#include "../bit_proxy.h"

#include <vclib/base.h>

#include <cstdint>
#include <vector>

namespace vcl {

/**
 * @brief The BitVector class is a memory-efficient container for boolean
 * values, guaranteeing predictable internal storage and alignment for direct
 * GPU mapping.
 *
 * While `std::vector<bool>` is a space-efficient specialization of
 * `std::vector`, its internal representation is implementation-defined. This
 * prevents obtaining a raw pointer to its underlying data (it lacks the
 * `.data()` method), making it unsuitable for direct upload to GPU buffers or
 * interoperability with C APIs without expensive manual repacking.
 *
 * `BitVector` overcomes this by explicitly managing bits inside a contiguous
 * `std::vector<uint8_t>`. It guarantees that the bits are tightly packed into
 * bytes and allows direct access to the underlying storage via the `bytes()`
 * and `data()` methods.
 *
 * The template parameter `MSB` allows specifying the bit ordering within each
 * byte.
 *
 * @tparam MSB: If false (default), bit index 0 is mapped to the least
 * significant bit of the first byte. If true, bit index 0 is mapped to
 * the most significant bit. This mirrors the behavior of `vcl::BitSet`.
 *
 * @ingroup space_core
 */
template<bool MSB = false>
class BitVector
{
    std::vector<uint8_t> mBytes;
    std::size_t          mSize = 0;

    static constexpr uint bitIndex(uint i)
    {
        return MSB ? (7 - (i % 8)) : (i % 8);
    }

public:
    BitVector() = default;

    /**
     * @brief Constructs a BitVector with the given size.
     * @param[in] size: The number of bits to allocate.
     * @param[in] value: The initial boolean value of each bit.
     */
    explicit BitVector(std::size_t size, bool value = false) :
            mBytes((size + 7) / 8, value ? 0xFF : 0x00), mSize(size)
    {
    }

    /**
     * @brief Returns the number of bits in the vector.
     */
    std::size_t size() const { return mSize; }

    /**
     * @brief Checks if the vector is empty (size == 0).
     */
    bool empty() const { return mSize == 0; }

    /**
     * @brief Resizes the vector to contain `count` bits.
     * @param[in] count: The new size in bits.
     * @param[in] value: The boolean value to initialize new bits if the vector
     * is expanded.
     */
    void resize(std::size_t count, bool value = false)
    {
        mBytes.resize((count + 7) / 8, value ? 0xFF : 0x00);
        mSize = count;
    }

    /**
     * @brief Clears the vector, making it empty.
     */
    void clear()
    {
        mBytes.clear();
        mSize = 0;
    }

    /**
     * @brief Pushes a new bit to the end of the vector.
     * @param[in] value: The boolean value of the new bit.
     */
    void push_back(bool value)
    {
        if (mSize % 8 == 0) {
            mBytes.push_back(0);
        }
        BitProxy<uint8_t>(mBytes.back(), bitIndex(mSize)) = value;
        mSize++;
    }

    /**
     * @brief Accesses the bit at the given index.
     * @param[in] i: The index of the bit.
     * @return the boolean value of the bit.
     */
    bool operator[](std::size_t i) const
    {
        return mBytes[i / 8] & (1 << bitIndex(i));
    }

    /**
     * @brief Accesses the bit at the given index, returning a proxy for
     * assignment.
     * @param[in] i: The index of the bit.
     * @return a proxy representing the bit.
     */
    BitProxy<uint8_t> operator[](std::size_t i)
    {
        return BitProxy<uint8_t>(mBytes[i / 8], bitIndex(i));
    }

    /**
     * @brief Returns the underlying byte storage.
     * @return a constant reference to the underlying std::vector of bytes.
     */
    const std::vector<uint8_t>& bytes() const { return mBytes; }

    /**
     * @brief Returns a pointer to the underlying byte storage.
     * @return a constant pointer to the underlying byte array.
     */
    const uint8_t* data() const { return mBytes.data(); }

    /**
     * @brief Returns the number of allocated bytes.
     */
    std::size_t byteSize() const { return mBytes.size(); }

    /**
     * @brief Replaces the underlying byte storage directly.
     * @param[in] bytes: The new vector of bytes.
     * @param[in] exactSizeInBits: Optional. If specified, sets the bit size
     * precisely. Otherwise, it assumes the vector is fully utilized
     * (bytes.size() * 8).
     */
    void setBytes(
        const std::vector<uint8_t>& bytes,
        std::size_t                 exactSizeInBits = 0)
    {
        mBytes = bytes;
        mSize  = exactSizeInBits > 0 ? exactSizeInBits : bytes.size() * 8;
    }

    /**
     * @brief Replaces the underlying byte storage directly (move semantics).
     * @param[in] bytes: The new vector of bytes.
     * @param[in] exactSizeInBits: Optional. If specified, sets the bit size
     * precisely. Otherwise, it assumes the vector is fully utilized
     * (bytes.size() * 8).
     */
    void setBytes(std::vector<uint8_t>&& bytes, std::size_t exactSizeInBits = 0)
    {
        mBytes = std::move(bytes);
        mSize  = exactSizeInBits > 0 ? exactSizeInBits : bytes.size() * 8;
    }
};

} // namespace vcl

#endif // VCL_SPACE_CORE_VECTOR_BIT_VECTOR_H
