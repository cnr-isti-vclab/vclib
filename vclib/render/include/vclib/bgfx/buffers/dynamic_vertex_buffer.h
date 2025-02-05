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

#include "generic_buffer.h"

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
class DynamicVertexBuffer :
        public GenericBuffer<bgfx::DynamicVertexBufferHandle>
{
    using Base = GenericBuffer<bgfx::DynamicVertexBufferHandle>;

    bool mCompute = false;

public:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid DynamicVertexBuffer object.
     */
    DynamicVertexBuffer() = default;

    /**
     * @brief Swap the content of this object with another DynamicVertexBuffer
     * object.
     *
     * @param[in] other: the other DynamicVertexBuffer object.
     */
    void swap(DynamicVertexBuffer& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mCompute, other.mCompute);
    }

    friend void swap(DynamicVertexBuffer& a, DynamicVertexBuffer& b)
    {
        a.swap(b);
    }

    /**
     * @brief Check if the VertexBuffer is used for compute shaders.
     *
     * @return true if the VertexBuffer is used for compute shaders, false
     * otherwise.
     */
    bool isCompute() const { return mCompute; }

    void set(
        uint               vertNum,
        bgfx::Attrib::Enum attrib,
        uint               numElements,
        PrimitiveType      type,
        bool               normalize = false,
        uint64_t           flags = BGFX_BUFFER_ALLOW_RESIZE)
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(attrib, numElements, attribType(type), normalize)
            .end();

        set(vertNum, layout, flags);
    }

    /**
     * @brief Set the dynamic vertex buffer data for rendering.
     *
     * @param[in] vertNum: the number of vertices in the buffer.
     * @param[in] layout: the vertex layout.
     * @param[in] compute: if true, the buffer is used for compute shaders.
     * @param[in] flags: the flags for the buffer.
     */
    void set(
        uint                      vertNum,
        const bgfx::VertexLayout& layout,
        uint64_t                  flags = BGFX_BUFFER_ALLOW_RESIZE)
    {
        if (bgfx::isValid(mHandle))
            bgfx::destroy(mHandle);

        mHandle  = bgfx::createDynamicVertexBuffer(vertNum, layout, flags);
        mCompute = false;
    }

    void setForCompute(
        uint                      vertNum,
        const bgfx::VertexLayout& layout,
        bgfx::Access::Enum        access = bgfx::Access::Read,
        uint64_t                  flags  = BGFX_BUFFER_ALLOW_RESIZE)
    {
        if (bgfx::isValid(mHandle))
            bgfx::destroy(mHandle);

        flags |= flagsForAccess(access);

        mHandle = bgfx::createDynamicVertexBuffer(vertNum, layout, flags);
        mCompute = true;
    }

    void update(uint startIndex, const bgfx::Memory* data)
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::update(mHandle, startIndex, data);
        }
    }

    /**
     * @brief Bind the dynamic vertex buffer to the rendering pipeline.
     *
     * @param[in] stream: the stream (or stage, in case of compute) to which the
     * dynamic vertex buffer is bound.
     * @param[in] access: the access type for the buffer (only for compute).
     */
    void bind(uint stream, bgfx::Access::Enum access = bgfx::Access::Read) const
    {
        if (bgfx::isValid(mHandle)) {
            if (!mCompute)
                bgfx::setVertexBuffer(stream, mHandle);
            else
                bgfx::setBuffer(stream, mHandle, access);
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_DYNAMIC_VERTEX_BUFFER_H
