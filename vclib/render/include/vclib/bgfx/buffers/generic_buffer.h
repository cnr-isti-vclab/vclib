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

#ifndef VCL_BGFX_BUFFERS_GENERIC_BUFFER_H
#define VCL_BGFX_BUFFERS_GENERIC_BUFFER_H

#include <vclib/base.h>

#include <bgfx/bgfx.h>

#include <utility>

namespace vcl {

/**
 * @brief The GenericBuffer manages the lifetime of a bgfx BufferHandle.
 *
 * @note A bgfx Buffer can be moved but not copied (a copy would require to
 * create a new Buffer Handle, that can be done only having access
 * to the data). Any class that contains a GenericBuffer (or a derived class)
 * should implement the copy constructor and the copy assignment operator.
 */
template<typename BufferHandleType>
class GenericBuffer
{
protected:
    BufferHandleType mHandle = BGFX_INVALID_HANDLE;

public:
    /**
     * @brief Check if the Buffer is valid.
     *
     * @return true if the Buffer is valid, false otherwise.
     */
    bool isValid() const { return bgfx::isValid(mHandle); }

    /**
     * @brief Destroy the Buffer.
     */
    void destroy()
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::destroy(mHandle);
            mHandle = BGFX_INVALID_HANDLE;
        }
    }

protected:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid GenericBuffer object.
     */
    GenericBuffer() = default;

    // Copying a GenericBuffer is not allowed
    GenericBuffer(const GenericBuffer& other) = delete;

    /**
     * @brief Move constructor.
     *
     * The other GenericBuffer is left in an invalid state.
     *
     * @param[in] other: the other GenericBuffer object.
     */
    GenericBuffer(GenericBuffer&& other) noexcept { swap(other); }

    /**
     * @brief Destructor.
     *
     * If the GenericBuffer is valid, the bgfx Buffer Handle is destroyed.
     */
    ~GenericBuffer() { destroy(); }

    // Copying a GenericBuffer is not allowed
    GenericBuffer& operator=(const GenericBuffer& other) = delete;

    /**
     * @brief Move assignment operator.
     *
     * The other GenericBuffer is left in an invalid state.
     *
     * @param[in] other: the other GenericBuffer object.
     * @return a reference to this object.
     */
    GenericBuffer& operator=(GenericBuffer&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swap the content of this object with another Buffer object.
     *
     * @param[in] other: the other Buffer object.
     */
    void swap(GenericBuffer& other)
    {
        using std::swap;
        swap(mHandle, other.mHandle);
    }

    friend void swap(GenericBuffer& a, GenericBuffer& b) { a.swap(b); }

protected:
    static uint64_t flagsForAccess(bgfx::Access::Enum access)
    {
        switch (access) {
        case bgfx::Access::Read: return BGFX_BUFFER_COMPUTE_READ;
        case bgfx::Access::Write: return BGFX_BUFFER_COMPUTE_WRITE;
        case bgfx::Access::ReadWrite: return BGFX_BUFFER_COMPUTE_READ_WRITE;
        default: return BGFX_BUFFER_NONE;
        }
    }

    static bgfx::AttribType::Enum attributeType(PrimitiveType type)
    {
        switch (type) {
        case PrimitiveType::CHAR:
        case PrimitiveType::UCHAR: return bgfx::AttribType::Uint8;
        case PrimitiveType::SHORT:
        case PrimitiveType::USHORT: return bgfx::AttribType::Int16;
        case PrimitiveType::FLOAT: return bgfx::AttribType::Float;
        default:
            assert(0); // not supported
            return bgfx::AttribType::Count;
        }
    }

    static uint64_t flagsForType(PrimitiveType type)
    {
        switch (type) {
        case PrimitiveType::INT:
        case PrimitiveType::UINT: return BGFX_BUFFER_INDEX32;
        case PrimitiveType::FLOAT:
            return BGFX_BUFFER_COMPUTE_FORMAT_32X1 |
                   BGFX_BUFFER_COMPUTE_TYPE_FLOAT;
        case PrimitiveType::DOUBLE: assert(0); // not supported
        default: return BGFX_BUFFER_NONE;
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_GENERIC_BUFFER_H
