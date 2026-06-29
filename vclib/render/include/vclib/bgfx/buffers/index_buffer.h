// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_BUFFERS_INDEX_BUFFER_H
#define VCL_BGFX_BUFFERS_INDEX_BUFFER_H

#include "generic_buffer.h"

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
class IndexBuffer : public GenericBuffer<bgfx::IndexBufferHandle>
{
    using Base = GenericBuffer<bgfx::IndexBufferHandle>;

public:
    using Base::swap;

    /**
     * @brief Empty constructor.
     *
     * It creates an invalid IndexBuffer object.
     */
    IndexBuffer() = default;

    /**
     * @brief Creates the index buffer and sets the data for rendering or
     * compute.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
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
    void create(
        const void*        bufferIndices,
        const uint         bufferSize,
        PrimitiveType      type,
        bgfx::Access::Enum access    = bgfx::Access::Read,
        bgfx::ReleaseFn    releaseFn = nullptr)
    {
        if (bufferSize != 0) {
            uint64_t flags = flagsForType(type);
            flags |= flagsForAccess(access);
            create(
                bgfx::makeRef(
                    bufferIndices, bufferSize * sizeOf(type), releaseFn),
                flags);
        }
        else {
            if (releaseFn)
                releaseFn((void*) bufferIndices, nullptr);
            destroy();
        }
    }

    /**
     * @brief Creates the index buffer and sets the data for rendering or
     * compute.
     *
     * The access type is assumed to be Read.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferIndices: the data to be copied in the index buffer.
     * @param[in] bufferSize: the size of the bufferIndices.
     * @param[in] type: the type of the elements.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void create(
        const void*     bufferIndices,
        const uint      bufferSize,
        PrimitiveType   type,
        bgfx::ReleaseFn releaseFn)
    {
        create(bufferIndices, bufferSize, type, bgfx::Access::Read, releaseFn);
    }

    /**
     * @brief Creates the index buffer and sets the data for rendering or
     * compute.
     *
     * The type is assumed to be UINT, and the access type is assumed to be
     * Read.
     *
     * @note The data must be available for two bgfx::frame calls, then it is
     * safe to release the data. If you cannot guarantee this, you must provide
     * a release function that will be called automatically when the data is no
     * longer needed.
     *
     * @param[in] bufferIndices: the data to be copied in the index buffer.
     * @param[in] bufferSize: the size of the bufferIndices.
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void create(
        const void*     bufferIndices,
        const uint      bufferSize,
        bgfx::ReleaseFn releaseFn = nullptr)
    {
        create(
            bufferIndices,
            bufferSize,
            PrimitiveType::UINT,
            bgfx::Access::Read,
            releaseFn);
    }

    /**
     * @brief Creates the index buffer and sets the data.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @param[in] indices: the memory containing the data.
     * @param[in] flags: the flags for the buffer.
     */
    void create(const bgfx::Memory* indices, uint64_t flags = BGFX_BUFFER_NONE)
    {
        destroy();

        mHandle = bgfx::createIndexBuffer(indices, flags);
        assert(bgfx::isValid(mHandle));
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
        if (bgfx::isValid(mHandle)) {
            if (stage == UINT_NULL) {
                bgfx::setIndexBuffer(mHandle);
            }
            else {
                bgfx::setBuffer(stage, mHandle, access);
            }
        }
    }

    /**
     * @brief Bind a portion of the index buffer to the rendering pipeline.
     *
     * The index buffer is bound to the rendering pipeline, starting from the
     * firstIndex and using numIndices indices.
     *
     * @param[in] firstIndex: the first index to be used.
     * @param[in] numIndices: the number of indices to be used.
     */
    void bind(uint firstIndex, uint numIndices) const
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::setIndexBuffer(mHandle, firstIndex, numIndices);
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_INDEX_BUFFER_H
