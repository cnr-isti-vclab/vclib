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

/**
 * @brief The IndexBuffer manages the lifetime of a bgfx::IndexBufferHandle.
 *
 * It provides an interface to set the index buffer data and bind it to the
 * rendering pipeline. The index buffer can be used for rendering or for
 * compute shaders.
 *
 * @note An IndexBuffer can be moved but not copied (a copy would require to
 * create a new bgfx::IndexBufferHandle, that can be done only having access
 * to the data). Any class that contains an IndexBuffer should implement the
 * copy constructor and the copy assignment operator.
 */
class IndexBuffer
{
    bgfx::IndexBufferHandle mIndexBufferHandle = BGFX_INVALID_HANDLE;
    bool mCompute = false;

public:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid IndexBuffer object.
     */
    IndexBuffer() = default;

    // Copying an IndexBuffer is not allowed
    IndexBuffer(const IndexBuffer& other) = delete;

    /**
     * @brief Move constructor.
     *
     * The other IndexBuffer is left in an invalid state.
     *
     * @param[in] other: the other IndexBuffer object.
     */
    IndexBuffer(IndexBuffer&& other) noexcept
    {
        swap(other);
    }

    /**
     * @brief Destructor.
     *
     * It destroys the bgfx::IndexBufferHandle.
     */
    ~IndexBuffer()
    {
        if (bgfx::isValid(mIndexBufferHandle))
            bgfx::destroy(mIndexBufferHandle);
    }

    // Copying a IndexBuffer is not allowed
    IndexBuffer& operator=(const IndexBuffer& other) = delete;

    /**
     * @brief Move assignment operator.
     *
     * The other IndexBuffer is left in an invalid state.
     *
     * @param[in] other: the other IndexBuffer object.
     * @return a reference to this object.
     */
    IndexBuffer& operator=(IndexBuffer&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swap the content of this object with another IndexBuffer object.
     *
     * @param[in] other: the other IndexBuffer object.
     */
    void swap(IndexBuffer& other)
    {
        using std::swap;
        swap(mIndexBufferHandle, other.mIndexBufferHandle);
        swap(mCompute, other.mCompute);
    }

    friend void swap(IndexBuffer& a, IndexBuffer& b) { a.swap(b); }

    /**
     * @brief Check if the IndexBuffer is valid.
     *
     * @return true if the IndexBuffer is valid, false otherwise.
     */
    bool isValid() const { return bgfx::isValid(mIndexBufferHandle); }

    /**
     * @brief Check if the IndexBuffer is used for compute shaders.
     *
     * @return true if the IndexBuffer is used for compute shaders, false
     * otherwise.
     */
    bool isCompute() const { return mCompute; }

    /**
     * @brief Set the index buffer data.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferIndices: the data to be copied in the index buffer.
     * @param[in] bufferSize: the size of the bufferIndices.
     * @param[in] is32Bit: if true, the bufferIndices are 32-bit integers.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void set(
        const void*     bufferIndices,
        const uint      bufferSize,
        bool            is32Bit   = true,
        bgfx::ReleaseFn releaseFn = nullptr)
    {
        uint64_t flags = is32Bit ? BGFX_BUFFER_INDEX32 : BGFX_BUFFER_NONE;
        uint size = is32Bit ? 4 : 2;
        set(bgfx::makeRef(bufferIndices, bufferSize * size, releaseFn),
            false,
            flags);
    }

    /**
     * @brief Set the index buffer data for compute shaders.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferIndices: the data to be copied in the index buffer.
     * @param[in] bufferSize: the size of the bufferIndices.
     * @param[in] type: the type of the elements.
     * @param[in] access: the access type for the buffer.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void setForCompute(
        const void*   bufferIndices,
        const uint    bufferSize,
        PrimitiveType type,
        bgfx::Access::Enum access = bgfx::Access::Read,
        bgfx::ReleaseFn releaseFn = nullptr)
    {
        uint64_t flags = flagsForType(type);
        flags |= flagsForAccess(access);
        set(bgfx::makeRef(bufferIndices, bufferSize * sizeOf(type), releaseFn),
            true,
            flags);
    }

    /**
     * @brief Set the index buffer data.
     *
     * @param[in] indices: the memory containing the data.
     * @param[in] compute: if true, the buffer is used for compute shaders.
     * @param[in] flags: the flags for the buffer.
     */
    void set(
        const bgfx::Memory* indices,
        bool                compute = false,
        uint64_t            flags   = BGFX_BUFFER_NONE)
    {
        mIndexBufferHandle = bgfx::createIndexBuffer(indices, flags);
        mCompute = compute;
    }

    /**
     * @brief Bind the index buffer to the rendering pipeline.
     *
     * If the stage is not specified, the index buffer is bound to the rendering
     * pipeline. Otherwise, it is bound to the compute shader with the specified
     * stage and the given access type (Read, Write, ReadWrite).
     *
     * @param[in] stage: the stage to which the buffer is bound.
     * @param[in] access: the access type for the buffer.
     */
    void bind(
        uint               stage  = UINT_NULL,
        bgfx::Access::Enum access = bgfx::Access::Read) const
    {
        if (bgfx::isValid(mIndexBufferHandle)) {
            if (stage == UINT_NULL) {
                bgfx::setIndexBuffer(mIndexBufferHandle);
            }
            else {
                bgfx::setBuffer(stage, mIndexBufferHandle, access);
            }
        }
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

    static uint64_t flagsForAccess(bgfx::Access::Enum access)
    {
        switch (access) {
        case bgfx::Access::Read: return BGFX_BUFFER_COMPUTE_READ;
        case bgfx::Access::Write: return BGFX_BUFFER_COMPUTE_WRITE;
        case bgfx::Access::ReadWrite: return BGFX_BUFFER_COMPUTE_READ_WRITE;
        default: return BGFX_BUFFER_NONE;
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_INDEX_BUFFER_H
