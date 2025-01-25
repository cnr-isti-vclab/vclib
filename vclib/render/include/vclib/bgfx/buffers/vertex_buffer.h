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
class VertexBuffer
{
    bgfx::VertexBufferHandle mVertexBufferHandle = BGFX_INVALID_HANDLE;
    bool mCompute = false;

public:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid VertexBuffer object.
     */
    VertexBuffer() = default;

    // Copying a VertexBuffer is not allowed
    VertexBuffer(const VertexBuffer& other) = delete;

    /**
     * @brief Move constructor.
     *
     * The other VertexBuffer is left in an invalid state.
     *
     * @param[in] other: the other VertexBuffer object.
     */
    VertexBuffer(VertexBuffer&& other) noexcept
    {
        swap(other);
    }

    /**
     * @brief Destructor.
     *
     * If the VertexBuffer is valid, the bgfx::VertexBufferHandle is destroyed.
     */
    ~VertexBuffer()
    {
        if (bgfx::isValid(mVertexBufferHandle))
            bgfx::destroy(mVertexBufferHandle);
    }

    // Copying a VertexBuffer is not allowed
    VertexBuffer& operator=(const VertexBuffer& other) = delete;

    /**
     * @brief Move assignment operator.
     *
     * The other VertexBuffer is left in an invalid state.
     *
     * @param[in] other: the other VertexBuffer object.
     * @return a reference to this object.
     */
    VertexBuffer& operator=(VertexBuffer&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swap the content of this object with another VertexBuffer object.
     *
     * @param[in] other: the other VertexBuffer object.
     */
    void swap(VertexBuffer& other)
    {
        using std::swap;
        swap(mVertexBufferHandle, other.mVertexBufferHandle);
        swap(mCompute, other.mCompute);
    }

    friend void swap(VertexBuffer& a, VertexBuffer& b) { a.swap(b); }

    /**
     * @brief Check if the VertexBuffer is valid.
     *
     * @return true if the VertexBuffer is valid, false otherwise.
     */
    bool isValid() const { return bgfx::isValid(mVertexBufferHandle); }

    /**
     * @brief Check if the VertexBuffer is used for compute shaders.
     *
     * @return true if the VertexBuffer is used for compute shaders, false
     * otherwise.
     */
    bool isCompute() const { return mCompute; }

    /**
     * @brief Set the vertex buffer data for rendering.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferData: the data to be copied in the vertex buffer.
     * @param[in] bufferSize: the size of the bufferData.
     * @param[in] attrib: the attribute to which the data refers.
     * @param[in] numElements: the number of elements for each vertex.
     * @param[in] type: the type of the elements.
     * @param[in] normalize: if true, the data is normalized.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
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

    /**
     * @brief Set the vertex buffer data for compute shaders.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferData: the data to be copied in the vertex buffer.
     * @param[in] bufferSize: the size of the bufferData.
     * @param[in] attrib: the attribute to which the data refers.
     * @param[in] numElements: the number of elements for each vertex.
     * @param[in] type: the type of the elements.
     * @param[in] normalize: if true, the data is normalized.
     * @param[in] access: the access type for the buffer.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
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

    /**
     * @brief Set the vertex buffer data.
     *
     * @param[in] layout: the vertex layout.
     * @param[in] data: the memory containing the data.
     * @param[in] compute: if true, the buffer is used for compute shaders.
     * @param[in] flags: the flags for the buffer.
     */
    void set(
        const bgfx::VertexLayout& layout,
        const bgfx::Memory* data,
        bool compute = false,
        uint64_t flags = BGFX_BUFFER_NONE)
    {
        if (bgfx::isValid(mVertexBufferHandle))
            bgfx::destroy(mVertexBufferHandle);

        mVertexBufferHandle = bgfx::createVertexBuffer(data, layout, flags);
        mCompute = compute;
    }

    /**
     * @brief Bind the vertex buffer to the rendering pipeline.
     *
     * @param[in] stream: the stream (or stage, in case of compute) to which the
     * vertex buffer is bound.
     * @param[in] access: the access type for the buffer (only for compute).
     */
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
