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

#ifndef VCL_BGFX_BUFFERS_INDIRECT_BUFFER_H
#define VCL_BGFX_BUFFERS_INDIRECT_BUFFER_H

#include "generic_buffer.h"

namespace vcl {

class IndirectBuffer : public GenericBuffer<bgfx::IndirectBufferHandle>
{
    using Base = GenericBuffer<bgfx::IndirectBufferHandle>;

public:
    using Base::swap;

    /**
     * @brief Empty constructor.
     *
     * It creates an invalid IndirectBuffer object.
     */
    IndirectBuffer() = default;

    /**
     * @brief Creates the indirect buffer with the given number of indirect
     * calls.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @param[in] num: the number of indirect calls.
     */
    void create(uint num)
    {
        if (bgfx::isValid(mHandle))
            bgfx::destroy(mHandle);

        mHandle = bgfx::createIndirectBuffer(num);
    }

    /**
     * @brief Returns the handle of the indirect buffer.
     * @return the handle of the indirect buffer.
     */
    bgfx::IndirectBufferHandle handle() const { return mHandle; }

    void bind(uint stage, bgfx::Access::Enum access) const
    {
        if (bgfx::isValid(mHandle))
            bgfx::setBuffer(stage, mHandle, access);
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_INDIRECT_BUFFER_H
