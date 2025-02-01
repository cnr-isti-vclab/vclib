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

#ifndef VCL_BGFX_BUFFERS_DYNAMIC_VERTEX_BUFFER_H
#define VCL_BGFX_BUFFERS_DYNAMIC_VERTEX_BUFFER_H

#include <vclib/types.h>

#include <bgfx/bgfx.h>

#include <utility>

namespace vcl {

/**
 * @brief The DynamicVertexBuffer manages the lifetime of a
 * bgfx::DynamicVertexBufferHandle.
 *
 * It provides an interface to set the dynamic vertex buffer data and bind it to
 * the rendering pipeline. The dynamic vertex buffer can be used for rendering
 * or for compute shaders.
 *
 * @todo provide here the differences between a vertex buffer and a dynamic
 * vertex buffer.
 *
 * @note A DynamicVertexBuffer can be moved but not copied (a copy would require
 * to create a new bgfx::DynamicVertexBufferHandle, that can be done only having
 * access to the data). Any class that contains a DynamicVertexBuffer should
 * implement the copy constructor and the copy assignment operator.
 */
class DynamicVertexBuffer
{
    bgfx::DynamicVertexBufferHandle mVertexBufferHandle = BGFX_INVALID_HANDLE;

public:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid DynamicVertexBuffer object.
     */
    DynamicVertexBuffer() = default;

    // Copying a DynamicVertexBuffer is not allowed
    DynamicVertexBuffer(const DynamicVertexBuffer& other) = delete;

    /**
     * @brief Move constructor.
     *
     * The other DynamicVertexBuffer is left in an invalid state.
     *
     * @param[in] other: the other DynamicVertexBuffer object.
     */
    DynamicVertexBuffer(DynamicVertexBuffer&& other) noexcept { swap(other); }

    /**
     * @brief Destructor.
     *
     * If the DynamicVertexBuffer is valid, the bgfx::DynamicVertexBufferHandle
     * is destroyed.
     */
    ~DynamicVertexBuffer()
    {
        if (bgfx::isValid(mVertexBufferHandle))
            bgfx::destroy(mVertexBufferHandle);
    }

    // Copying a DynamicVertexBuffer is not allowed
    DynamicVertexBuffer& operator=(const DynamicVertexBuffer& other) = delete;

    /**
     * @brief Move assignment operator.
     *
     * The other DynamicVertexBuffer is left in an invalid state.
     *
     * @param[in] other: the other DynamicVertexBuffer object.
     * @return a reference to this object.
     */
    DynamicVertexBuffer& operator=(DynamicVertexBuffer&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swap the content of this object with another DynamicVertexBuffer object.
     *
     * @param[in] other: the other DynamicVertexBuffer object.
     */
    void swap(DynamicVertexBuffer& other)
    {
        using std::swap;
        swap(mVertexBufferHandle, other.mVertexBufferHandle);
    }

    friend void swap(DynamicVertexBuffer& a, DynamicVertexBuffer& b) { a.swap(b); }

    /**
     * @brief Check if the DynamicVertexBuffer is valid.
     *
     * @return true if the DynamicVertexBuffer is valid, false otherwise.
     */
    bool isValid() const { return bgfx::isValid(mVertexBufferHandle); }

    void create(
        uint                      size,
        const bgfx::VertexLayout& layout,
        ushort                    flags = BGFX_BUFFER_NONE)
    {
        mVertexBufferHandle = bgfx::createDynamicVertexBuffer(size, layout, flags);
    }

    void update(uint startIndex, const bgfx::Memory* data)
    {
        if (bgfx::isValid(mVertexBufferHandle)) {
            bgfx::update(mVertexBufferHandle, startIndex, data);
        }
    }

    /**
     * @brief Bind the dynamic vertex buffer to the rendering pipeline.
     *
     * @param[in] stream: the stream to which the vertex buffer is bound.
     */
    void bind(uint stream) const
    {
        if (bgfx::isValid(mVertexBufferHandle)) {
            bgfx::setVertexBuffer(stream, mVertexBufferHandle);
        }
    }

private:
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

#endif // VCL_BGFX_BUFFERS_DYNAMIC_VERTEX_BUFFER_H
