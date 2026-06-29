// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_BUFFERS_OWNED_OR_REF_BUFFER_H
#define VCL_BGFX_BUFFERS_OWNED_OR_REF_BUFFER_H

#include <stdexcept>
#include <variant>

namespace vcl {

/**
 * @brief A class template to manage a buffer that can be either owned (T) or
 * referenced (const T*).
 *
 * This class wraps a std::variant<T, const T*> and provides a unified
 * interface for both cases. Copy constructor and assignment are deleted to
 * match the semantics of other buffers in the library.
 *
 * @tparam T The type of the buffer (e.g., VertexBuffer).
 */
template<typename T>
class OwnedOrRefBuffer
{
    std::variant<T, const T*> mBuffer;

public:
    /// Default constructor creates an empty (invalid) buffer.
    OwnedOrRefBuffer() = default;

    // Copy operations are deleted
    OwnedOrRefBuffer(const OwnedOrRefBuffer&) = delete;

    // Move operations are allowed
    OwnedOrRefBuffer(OwnedOrRefBuffer&& other) noexcept = default;

    ~OwnedOrRefBuffer() = default;

    OwnedOrRefBuffer& operator=(const OwnedOrRefBuffer&) = delete;

    OwnedOrRefBuffer& operator=(OwnedOrRefBuffer&& other) noexcept = default;

    /**
     * @brief Construct with an owned buffer.
     */
    explicit OwnedOrRefBuffer(T&& owned) : mBuffer(std::move(owned)) {}

    /**
     * @brief Construct with a referenced (non-owned) buffer.
     */
    explicit OwnedOrRefBuffer(const T* ref) : mBuffer(ref) {}

    /**
     * @brief Get a const reference to the underlying buffer.
     * @return A const reference to the buffer (dereferenced if referenced).
     * @throws std::runtime_error if the buffer is not valid.
     */
    const T& operator()() const
    {
        if (std::holds_alternative<T>(mBuffer)) {
            return std::get<T>(mBuffer);
        }
        else {
            const T* ptr = std::get<const T*>(mBuffer);
            if (!ptr) {
                throw std::runtime_error {"OwnedOrRefBuffer: buffer is null"};
            }
            return *ptr;
        }
    }

    /**
     * @brief Get a const reference to the underlying buffer.
     * @return A const reference to the buffer (dereferenced if referenced).
     * @throws std::runtime_error if the buffer is not valid.
     */
    const T& get() const { return operator()(); }

    /**
     * @brief Check if the buffer is currently valid (either owned or referenced
     * with non-null pointer).
     */
    explicit operator bool() const { return isValid(); }

    /**
     * @brief Check if the buffer is currently owned (as a value).
     */
    bool isOwned() const { return std::holds_alternative<T>(mBuffer); }

    /**
     * @brief Check if the buffer is currently referenced.
     */
    bool isReferenced() const
    {
        return std::holds_alternative<const T*>(mBuffer);
    }

    /**
     * @brief Set as owned.
     */
    void setOwned(T&& val = T()) { mBuffer = std::move(val); }

    /**
     * @brief Set as referenced.
     */
    void setReferenced(const T* ptr) { mBuffer = ptr; }

    /**
     * @brief Check if the buffer is valid (either owned and valid, or
     * referenced with non-null pointer that is valid).
     * @return true if the buffer is valid, false otherwise.
     */
    bool isValid() const
    {
        if (std::holds_alternative<T>(mBuffer)) {
            return std::get<T>(mBuffer).isValid();
        }
        else {
            const T* ptr = std::get<const T*>(mBuffer);
            if (ptr)
                return ptr->isValid();
            return false;
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_OWNED_OR_REF_BUFFER_H
