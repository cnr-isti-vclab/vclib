// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_BUFFERS_DYNAMIC_INDEX_BUFFER_H
#define VCL_BGFX_BUFFERS_DYNAMIC_INDEX_BUFFER_H

#include "generic_buffer.h"

namespace vcl {

/**
 * @brief The DynamicIndexBuffer manages the lifetime of a
 * bgfx::DynamicIndexBufferHandle.
 *
 * It provides an interface to set the dynamic index buffer data and bind it to
 * the rendering pipeline. The dynamic index buffer can be used for rendering
 * or for compute shaders.
 *
 * @todo provide here the differences between a index buffer and a dynamic
 * index buffer.
 *
 * @note A DynamicIndexBuffer can be moved but not copied (a copy would require
 * to create a new bgfx::DynamicIndexBufferHandle, that can be done only having
 * access to the data). Any class that contains a DynamicIndexBuffer should
 * implement the copy constructor and the copy assignment operator.
 */
class DynamicIndexBuffer : public GenericBuffer<bgfx::DynamicIndexBufferHandle>
{
    using Base = GenericBuffer<bgfx::DynamicIndexBufferHandle>;

public:
    using Base::swap;

    /**
     * @brief Empty constructor.
     *
     * It creates an invalid DynamicIndexBuffer object.
     */
    DynamicIndexBuffer() = default;

    void create(uint size, ushort flags = BGFX_BUFFER_NONE)
    {
        destroy();
        if (size != 0)
            mHandle = bgfx::createDynamicIndexBuffer(size, flags);
    }

    void update(uint startIndex, const bgfx::Memory* data)
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::update(mHandle, startIndex, data);
        }
    }

    /**
     * @brief Bind the dynamic index buffer to the rendering pipeline.
     */
    void bind() const
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::setIndexBuffer(mHandle);
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_DYNAMIC_INDEX_BUFFER_H
