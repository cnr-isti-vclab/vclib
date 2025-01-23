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

#ifndef VCL_BGFX_BUFFERS_VERTEX_BUFFER_H
#define VCL_BGFX_BUFFERS_VERTEX_BUFFER_H

#include <vclib/types.h>

#include <bgfx/bgfx.h>

#include <utility>

namespace vcl {

class VertexBuffer
{
    bgfx::VertexBufferHandle mVertexBufferHandle = BGFX_INVALID_HANDLE;

public:
    VertexBuffer() = default;

    VertexBuffer(const VertexBuffer& other) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept
    {
        mVertexBufferHandle = other.mVertexBufferHandle;
        other.mVertexBufferHandle = BGFX_INVALID_HANDLE;
    }

    ~VertexBuffer()
    {
        if (bgfx::isValid(mVertexBufferHandle))
            bgfx::destroy(mVertexBufferHandle);
    }

    VertexBuffer& operator=(const VertexBuffer& other) = delete;

    VertexBuffer& operator=(VertexBuffer&& other) noexcept
    {
        mVertexBufferHandle = other.mVertexBufferHandle;
        other.mVertexBufferHandle = BGFX_INVALID_HANDLE;
        return *this;
    }

    void swap(VertexBuffer& other)
    {
        using std::swap;
        swap(mVertexBufferHandle, other.mVertexBufferHandle);
    }

    friend void swap(VertexBuffer& a, VertexBuffer& b) { a.swap(b); }

    void set(
        const void*            bufferData,
        const uint             bufferSize,
        bgfx::Attrib::Enum     attrib,
        uint                   numElements,
        bgfx::AttribType::Enum attribType,
        bool                   normalize = false,
        bgfx::ReleaseFn        releaseFn = nullptr,
        uint64_t               flags     = BGFX_BUFFER_NONE)
    {
        bgfx::VertexLayout layout;
        layout.begin().add(attrib, numElements, attribType, normalize).end();

        set(layout,
            bgfx::makeRef(
                bufferData, bufferSize * attribTypeSize(attribType), releaseFn),
            flags);
    }

    void set(
        const bgfx::VertexLayout& layout,
        const bgfx::Memory* vertices,
        uint64_t flags = BGFX_BUFFER_NONE)
    {
        if (bgfx::isValid(mVertexBufferHandle))
            bgfx::destroy(mVertexBufferHandle);

        mVertexBufferHandle = bgfx::createVertexBuffer(vertices, layout, flags);
    }

    void bind(uint stream) const
    {
        if (bgfx::isValid(mVertexBufferHandle))
            bgfx::setVertexBuffer(stream, mVertexBufferHandle);
    }

    static int attribTypeSize(bgfx::AttribType::Enum type)
    {
        switch(type)
        {
            case bgfx::AttribType::Uint8: return 1;
            case bgfx::AttribType::Uint10: return 2;
            case bgfx::AttribType::Int16: return 2;
            case bgfx::AttribType::Half: return 2;
            case bgfx::AttribType::Float: return 4;
            default: return 0;
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_VERTEX_BUFFER_H
