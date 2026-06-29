// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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

public:
    using Base::swap;

    /**
     * @brief Empty constructor.
     *
     * It creates an invalid VertexBuffer object.
     */
    VertexBuffer() = default;

    /**
     * @brief Creates the vertex buffer and sets the data for rendering or
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
    void create(
        const void*        bufferData,
        const uint         vertNum,
        bgfx::Attrib::Enum attrib,
        uint               attribNumPerVertex,
        PrimitiveType      attribType,
        bool               normalize = false,
        bgfx::Access::Enum access    = bgfx::Access::Read,
        bgfx::ReleaseFn    releaseFn = nullptr)
    {
        if (vertNum != 0) {
            uint64_t flags = flagsForType(attribType, attribNumPerVertex);
            flags |= flagsForAccess(access);

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
                flags);
        }
        else {
            if (releaseFn)
                releaseFn((void*) bufferData, nullptr);
            destroy();
        }
    }

    /**
     * @brief Creates the vertex buffer and sets the data for rendering or
     * compute.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * The access type is assumed to be Read.
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
        const uint         vertNum,
        bgfx::Attrib::Enum attrib,
        uint               attribNumPerVertex,
        PrimitiveType      attribType,
        bool               normalize,
        bgfx::ReleaseFn    releaseFn = nullptr)
    {
        create(
            bufferData,
            vertNum,
            attrib,
            attribNumPerVertex,
            attribType,
            normalize,
            bgfx::Access::Read,
            releaseFn);
    }

    /**
     * @brief Creates the vertex buffer and sets the data for rendering or
     * compute.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * The access type is assumed to be Read, and the data is not normalized.
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
     * @param[in] releaseFn: the release function to be called when the data is
     * no longer needed.
     */
    void create(
        const void*        bufferData,
        const uint         vertNum,
        bgfx::Attrib::Enum attrib,
        uint               attribNumPerVertex,
        PrimitiveType      attribType,
        bgfx::ReleaseFn    releaseFn)
    {
        create(
            bufferData,
            vertNum,
            attrib,
            attribNumPerVertex,
            attribType,
            false,
            bgfx::Access::Read,
            releaseFn);
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
     */
    void create(
        const bgfx::Memory*       data,
        const bgfx::VertexLayout& layout,
        uint64_t                  flags = BGFX_BUFFER_NONE)
    {
        destroy();

        mHandle = bgfx::createVertexBuffer(data, layout, flags);
        assert(bgfx::isValid(mHandle));
    }

    /**
     * @brief Bind the vertex buffer to the rendering pipeline.
     *
     * If the buffer is invalid, it is not bound.
     *
     * @param[in] stream: the stream (or stage, in case of compute) to which the
     * vertex buffer is bound.
     */
    void bind(uint stream) const
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::setVertexBuffer(stream, mHandle);
        }
    }

    /**
     * @brief Bind the vertex buffer to the compute shader.
     *
     * If the buffer is invalid, it is not bound.
     *
     * @param[in] stage: the stage to which the vertex buffer is bound.
     * @param[in] access: the access type for the buffer.
     *
     * @note This function is used to bind the vertex buffer to a compute
     * shader. It is similar to the bind function, but it uses the
     * bgfx::setBuffer function to bind the buffer.
     */
    void bindCompute(uint stage, bgfx::Access::Enum access = bgfx::Access::Read)
        const
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::setBuffer(stage, mHandle, access);
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_VERTEX_BUFFER_H
