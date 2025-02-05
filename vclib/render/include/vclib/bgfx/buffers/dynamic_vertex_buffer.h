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

    /**
     * @brief Creates the dynamic vertex buffer data for rendering, with the
     * layout given by the vertex attributes and without any data.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @param[in] vertNum: the number of vertices in the buffer.
     * @param[in] attrib: the attribute to which the data of the buffer refers.
     * @param[in] attribNumPerVertex: the number of attributes per vertex.
     * @param[in] attribType: the type of the attributes.
     * @param[in] normalize: if true, the data is normalized.
     * @param[in] allowResize: if true, the buffer can be resized.
     */
    void create(
        uint               vertNum,
        bgfx::Attrib::Enum attrib,
        uint               attribNumPerVertex,
        PrimitiveType      attribType,
        bool               normalize   = false,
        bool               allowResize = true)
    {
        uint64_t flags =
            allowResize ? BGFX_BUFFER_ALLOW_RESIZE : BGFX_BUFFER_NONE;

        bgfx::VertexLayout layout;
        layout.begin()
            .add(attrib, attribNumPerVertex, attributeType(attribType), normalize)
            .end();

        create(vertNum, layout, true, flags);
    }
    /**
     * @brief Creates the dynamic vertex buffer data for rendering, with the
     * layout given by the vertex attributes and with the given data.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @param[in] bufferData: the data to be copied in the vertex buffer.
     * @param[in] vertNum: the number of vertices in the buffer.
     * @param[in] attrib: the attribute to which the data of the buffer refers.
     * @param[in] attribNumPerVertex: the number of attributes per vertex.
     * @param[in] attribType: the type of the attributes.
     * @param[in] normalize: if true, the data is normalized.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     * @param[in] allowResize: if true, the buffer can be resized.
     */
    void create(
        const void*        bufferData,
        uint               vertNum,
        bgfx::Attrib::Enum attrib,
        uint               attribNumPerVertex,
        PrimitiveType      attribType,
        bool               normalize   = false,
        bgfx::ReleaseFn    releaseFn   = nullptr,
        bool               allowResize = true)
    {
        uint64_t flags =
            allowResize ? BGFX_BUFFER_ALLOW_RESIZE : BGFX_BUFFER_NONE;

        bgfx::VertexLayout layout;
        layout.begin()
            .add(attrib, attribNumPerVertex, attributeType(attribType), normalize)
            .end();

        create(vertNum, layout, false, flags);
        update(bufferData, vertNum, attribNumPerVertex, attribType, 0, releaseFn);
    }

    /**
     * @brief Creates the dynamic vertex buffer data for rendering, with the
     * given layout and without any data.
     *
     * @param[in] vertNum: the number of vertices in the buffer.
     * @param[in] layout: the vertex layout.
     * @param[in] compute: if true, the buffer is used for compute shaders.
     * @param[in] flags: the flags for the buffer.
     */
    void create(
        uint                      vertNum,
        const bgfx::VertexLayout& layout,
        bool                      compute = false,
        uint64_t                  flags   = BGFX_BUFFER_NONE)
    {
        if (bgfx::isValid(mHandle))
            bgfx::destroy(mHandle);

        mHandle  = bgfx::createDynamicVertexBuffer(vertNum, layout, flags);
        mCompute = compute;
    }

    /**
     * @brief Updates the dynamic vertex buffer with the given data.
     *
     * If the buffer is not valid, the data is not updated.
     * If the buffer was created with the allowResize flag set to true, the
     * buffer can be resized to fit the new data. Otherwise, only the data that
     * fits the buffer is updated.
     *
     * @param[in] bufferData: the data to be copied in the vertex buffer.
     * @param[in] vertNum: the number of vertices in the buffer.
     * @param[in] attribNumPerVertex: the number of attributes per vertex.
     * @param[in] attribType: the type of the attributes.
     * @param[in] startIndex: the index of the first vertex to be updated.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void update(
        const void*     bufferData,
        uint            vertNum,
        uint            attribNumPerVertex,
        PrimitiveType   attribType,
        uint            startIndex = 0,
        bgfx::ReleaseFn releaseFn  = nullptr)
    {
        const bgfx::Memory* data = bgfx::makeRef(
            bufferData,
            vertNum * attribNumPerVertex * sizeOf(attribType),
            releaseFn);

        update(startIndex, data);
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
