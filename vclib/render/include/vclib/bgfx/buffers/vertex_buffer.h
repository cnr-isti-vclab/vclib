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
    bool mCompute = false;

public:
    VertexBuffer() = default;

    VertexBuffer(const VertexBuffer& other) = delete;

    VertexBuffer(VertexBuffer&& other) noexcept
    {
        mVertexBufferHandle = other.mVertexBufferHandle;
        mCompute = other.mCompute;
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
        mCompute = other.mCompute;
        other.mVertexBufferHandle = BGFX_INVALID_HANDLE;
        return *this;
    }

    void swap(VertexBuffer& other)
    {
        using std::swap;
        swap(mVertexBufferHandle, other.mVertexBufferHandle);
        swap(mCompute, other.mCompute);
    }

    friend void swap(VertexBuffer& a, VertexBuffer& b) { a.swap(b); }

    void set(
        const void*        bufferData,
        const uint         bufferSize,
        bgfx::Attrib::Enum attrib,
        uint               numElements,
        PrimitiveType      type,
        bool               normalize = false,
        bgfx::ReleaseFn    releaseFn = nullptr)
    {
        bgfx::VertexLayout layout;
        layout.begin().add(attrib, numElements, attribType(type), normalize).end();

        set(layout,
            bgfx::makeRef(
                bufferData,
                bufferSize * sizeOf(type),
                releaseFn));
    }

    void setForCompute(
        const void*        bufferData,
        const uint         bufferSize,
        bgfx::Attrib::Enum attrib,
        uint               numElements,
        PrimitiveType      type,
        bool               normalize = false,
        bgfx::Access::Enum access    = bgfx::Access::Read,
        bgfx::ReleaseFn    releaseFn = nullptr)
    {
        uint64_t flags = flagsForAccess(access);

        bgfx::VertexLayout layout;
        layout.begin()
            .add(attrib, numElements, attribType(type), normalize)
            .end();

        set(layout,
            bgfx::makeRef(
                bufferData, bufferSize * sizeOf(type), releaseFn),
            true,
            flags);
    }

    void set(
        const bgfx::VertexLayout& layout,
        const bgfx::Memory* vertices,
        bool compute = false,
        uint64_t flags = BGFX_BUFFER_NONE)
    {
        if (bgfx::isValid(mVertexBufferHandle))
            bgfx::destroy(mVertexBufferHandle);

        mVertexBufferHandle = bgfx::createVertexBuffer(vertices, layout, flags);
        mCompute = compute;
    }

    void bind(uint stream, bgfx::Access::Enum access = bgfx::Access::Read) const
    {
        if (bgfx::isValid(mVertexBufferHandle)) {
            if (!mCompute)
                bgfx::setVertexBuffer(stream, mVertexBufferHandle);
            else
                bgfx::setBuffer(stream, mVertexBufferHandle, access);
        }
    }

private:
    static uint64_t flagsForAccess(bgfx::Access::Enum access)
    {
        switch (access) {
        case bgfx::Access::Read: return BGFX_BUFFER_COMPUTE_READ;
        case bgfx::Access::Write: return BGFX_BUFFER_COMPUTE_WRITE;
        case bgfx::Access::ReadWrite: return BGFX_BUFFER_COMPUTE_READ_WRITE;
        default: return BGFX_BUFFER_NONE;
        }
    }

    static bgfx::AttribType::Enum attribType(PrimitiveType type)
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
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_VERTEX_BUFFER_H
