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

#ifndef VCL_BGFX_BUFFERS_INDEX_BUFFER_H
#define VCL_BGFX_BUFFERS_INDEX_BUFFER_H

#include <vclib/types.h>

#include <bgfx/bgfx.h>

#include <utility>

namespace vcl {

class IndexBuffer
{
    bgfx::IndexBufferHandle mIndexBufferHandle = BGFX_INVALID_HANDLE;

public:
    IndexBuffer() = default;

    IndexBuffer(const IndexBuffer& other) = delete;

    IndexBuffer(IndexBuffer&& other) noexcept
    {
        mIndexBufferHandle = other.mIndexBufferHandle;
        other.mIndexBufferHandle = BGFX_INVALID_HANDLE;
    }

    ~IndexBuffer()
    {
        if (bgfx::isValid(mIndexBufferHandle))
            bgfx::destroy(mIndexBufferHandle);
    }

    IndexBuffer& operator=(const IndexBuffer& other) = delete;

    IndexBuffer& operator=(IndexBuffer&& other) noexcept
    {
        mIndexBufferHandle = other.mIndexBufferHandle;
        other.mIndexBufferHandle = BGFX_INVALID_HANDLE;
        return *this;
    }

    void swap(IndexBuffer& other)
    {
        using std::swap;
        swap(mIndexBufferHandle, other.mIndexBufferHandle);
    }

    friend void swap(IndexBuffer& a, IndexBuffer& b) { a.swap(b); }

    void set(
        const void*   bufferIndices,
        const uint    bufferSize,
        PrimitiveType type,
        uint64_t      flags = BGFX_BUFFER_NONE)
    {
        flags |= flagsForType(type);
        set(bgfx::makeRef(bufferIndices, bufferSize * sizeOf(type)), flags);
    }

    void set(const bgfx::Memory* indices, uint64_t flags = BGFX_BUFFER_NONE)
    {
        mIndexBufferHandle = bgfx::createIndexBuffer(indices, flags);
    }

    void bind() const
    {
        bgfx::setIndexBuffer(mIndexBufferHandle);
    }

    void bindForCompute(
        uint8_t            stage,
        bgfx::Access::Enum access = bgfx::Access::Read) const
    {
        bgfx::setBuffer(stage, mIndexBufferHandle, access);
    }

private:
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

#endif // VCL_BGFX_BUFFERS_INDEX_BUFFER_H
