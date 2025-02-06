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

#include "generic_buffer.h"

namespace vcl {

/**
 * @brief The VertexBuffer manages the lifetime of a bgfx::VertexBufferHandle.
 *
 * It provides an interface to set the vertex buffer data and bind it to the
 * rendering pipeline. The vertex buffer can be used for rendering or for
 * compute shaders.
 *
 * @note A VertexBuffer can be moved but not copied (a copy would require to
 * create a new bgfx::VertexBufferHandle, that can be done only having access
 * to the data). Any class that contains a VertexBuffer should implement the
 * copy constructor and the copy assignment operator.
 */
class VertexBuffer : public GenericBuffer<bgfx::VertexBufferHandle>
{
    using Base = GenericBuffer<bgfx::VertexBufferHandle>;

    bool mCompute = false;

public:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid VertexBuffer object.
     */
    VertexBuffer() = default;

    /**
     * @brief Swap the content of this object with another VertexBuffer object.
     *
     * @param[in] other: the other VertexBuffer object.
     */
    void swap(VertexBuffer& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mCompute, other.mCompute);
    }

    friend void swap(VertexBuffer& a, VertexBuffer& b) { a.swap(b); }

    /**
     * @brief Check if the VertexBuffer is used for compute shaders.
     *
     * @return true if the VertexBuffer is used for compute shaders, false
     * otherwise.
     */
    bool isCompute() const { return mCompute; }

    /**
     * @brief Creates the vertex buffer and sets the data for rendering.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferData: the data to be copied in the vertex buffer.
     * @param[in] vertNum: the number of vertices in the buffer.
     * @param[in] attrib: the attribute to which the data of the buffer refers.
     * @param[in] attribNumPerVertex: the number of attributes per vertex.
     * @param[in] attribType: the type of the attributes.
     * @param[in] normalize: if true, the data is normalized.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void create(
        const void*        bufferData,
        uint               vertNum,
        bgfx::Attrib::Enum attrib,
        uint               attribNumPerVertex,
        PrimitiveType      attribType,
        bool               normalize = false,
        bgfx::ReleaseFn    releaseFn = nullptr)
    {
        bgfx::VertexLayout layout;
        layout.begin()
            .add(
                attrib,
                attribNumPerVertex,
                attributeType(attribType),
                normalize)
            .end();

        create(
            bgfx::makeRef(
                bufferData,
                vertNum * attribNumPerVertex * sizeOf(attribType),
                releaseFn),
            layout);
    }

    /**
     * @brief Creates the vertex buffer and sets the data for compute shaders.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferData: the data to be copied in the vertex buffer.
     * @param[in] vertNum: the number of vertices in the buffer.
     * @param[in] attrib: the attribute to which the data of the buffer refers.
     * @param[in] attribNumPerVertex: the number of attributes per vertex.
     * @param[in] attribType: the type of the attributes.
     * @param[in] normalize: if true, the data is normalized.
     * @param[in] access: the access type for the buffer.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void createForCompute(
        const void*        bufferData,
        const uint         vertNum,
        bgfx::Attrib::Enum attrib,
        uint               attribNumPerVertex,
        PrimitiveType      attribType,
        bool               normalize = false,
        bgfx::Access::Enum access    = bgfx::Access::Read,
        bgfx::ReleaseFn    releaseFn = nullptr)
    {
        uint64_t flags = flagsForAccess(access);

        bgfx::VertexLayout layout;
        layout.begin()
            .add(
                attrib,
                attribNumPerVertex,
                attributeType(attribType),
                normalize)
            .end();

        create(
            bgfx::makeRef(
                bufferData,
                vertNum * attribNumPerVertex * sizeOf(attribType),
                releaseFn),
            layout,
            flags,
            true);
    }

    /**
     * @brief Creates the vertex buffer and sets the data.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @param[in] data: the memory containing the data.
     * @param[in] layout: the vertex layout.
     * @param[in] flags: the flags for the buffer.
     * @param[in] compute: if true, the buffer is used for compute shaders.
     */
    void create(
        const bgfx::Memory*       data,
        const bgfx::VertexLayout& layout,
        uint64_t                  flags   = BGFX_BUFFER_NONE,
        bool                      compute = false)
    {
        if (bgfx::isValid(mHandle))
            bgfx::destroy(mHandle);

        mHandle  = bgfx::createVertexBuffer(data, layout, flags);
        mCompute = compute;
    }

    /**
     * @brief Bind the vertex buffer to the rendering pipeline.
     *
     * If the buffer is invalid, it is not bound.
     *
     * @param[in] stream: the stream (or stage, in case of compute) to which the
     * vertex buffer is bound.
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

#endif // VCL_BGFX_BUFFERS_VERTEX_BUFFER_H
