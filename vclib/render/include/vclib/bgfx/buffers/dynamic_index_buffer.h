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
