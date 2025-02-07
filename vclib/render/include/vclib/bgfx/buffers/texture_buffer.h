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

#ifndef VCL_BGFX_BUFFERS_TEXTURE_BUFFER_H
#define VCL_BGFX_BUFFERS_TEXTURE_BUFFER_H

#include "generic_buffer.h"

namespace vcl {

class TextureBuffer : public GenericBuffer<bgfx::TextureHandle>
{
    using Base = GenericBuffer<bgfx::TextureHandle>;

    bgfx::TextureFormat::Enum mFormat = bgfx::TextureFormat::Count;

public:
    TextureBuffer() = default;

    /**
     * @brief Swap the content of this object with another TextureBuffer object.
     *
     * @param[in] other: the other TextureBuffer object.
     */
    void swap(TextureBuffer& other)
    {
        using std::swap;
        Base::swap(other);
        swap(mFormat, other.mFormat);
    }

    friend void swap(TextureBuffer& a, TextureBuffer& b) { a.swap(b); }

    /**
     * @brief Creates the texture buffer and sets the data (if given) for
     * compute shaders.
     *
     * If the buffer is already created (@ref isValid() returns `true`), it is
     * destroyed and a new one is created.
     *
     * @param[in] width: the width of the texture.
     * @param[in] height: the height of the texture.
     * @param[in] format: the format of the texture.
     * @param[in] flags: the flags for the texture.
     * @param[in] hasMips: if true, the texture has mipmaps.
     * @param[in] nLayers: the number of layers of the texture.
     * @param[in] data: the memory containing the data.
     */
    void create(
        ushort                    width,
        ushort                    height,
        bgfx::TextureFormat::Enum format,
        uint64_t                  flags   = BGFX_TEXTURE_NONE,
        bool                      hasMips = false,
        uint                      nLayers = 1,
        const bgfx::Memory*       data    = nullptr)
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::destroy(mHandle);
        }

        mFormat = format;

        mHandle = bgfx::createTexture2D(
            width, height, hasMips, nLayers, format, flags, data);
    }

    /**
     * @brief Bind the texture buffer to the compute shader.
     *
     * If the texture buffer is valid, it is bound to the compute shader with
     * the specified stage and the given access type (Read, Write, Read Write).
     * If the texture buffer is not valid, nothing happens.
     *
     * @param[in] stage: the stage to which the buffer is bound.
     * @param[in] access: the access type for the buffer.
     * @param[in] mipLevel: the mip level of the texture.
     */
    void bind(
        uint               stage,
        bgfx::Access::Enum access   = bgfx::Access::Read,
        uint8_t            mipLevel = 0) const
    {
        if (bgfx::isValid(mHandle)) {
            bgfx::setImage(stage, mHandle, mipLevel, access, mFormat);
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_BUFFERS_TEXTURE_BUFFER_H
