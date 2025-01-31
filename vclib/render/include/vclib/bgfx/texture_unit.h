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

#ifndef VCL_BGFX_TEXTURE_UNIT_H
#define VCL_BGFX_TEXTURE_UNIT_H

#include <vclib/space/core/point.h>
#include <vclib/types.h>

#include <bgfx/bgfx.h>

namespace vcl {

class TextureUnit
{
    bgfx::TextureHandle mTextureHandle = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle mUniformHandle = BGFX_INVALID_HANDLE;

public:
    /**
     * @brief Empty constructor.
     *
     * It creates an invalid TextureUnit object.
     */
    TextureUnit() = default;

    // Copying an TextureUnit is not allowed
    TextureUnit(const TextureUnit& other) = delete;

    /**
     * @brief Move constructor.
     *
     * The other TextureUnit is left in an invalid state.
     *
     * @param[in] other: the other TextureUnit object.
     */
    TextureUnit(TextureUnit&& other) noexcept { swap(other); }

    /**
     * @brief Destructor.
     *
     * It destroys the TextureUnit.
     */
    ~TextureUnit()
    {
        if (bgfx::isValid(mTextureHandle))
            bgfx::destroy(mTextureHandle);
        if (bgfx::isValid(mUniformHandle))
            bgfx::destroy(mUniformHandle);
    }

    // Copying a TextureUnit is not allowed
    TextureUnit& operator=(const TextureUnit& other) = delete;

    /**
     * @brief Move assignment operator.
     *
     * The other TextureUnit is left in an invalid state.
     *
     * @param[in] other: the other TextureUnit object.
     * @return a reference to this object.
     */
    TextureUnit& operator=(TextureUnit&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swap the content of this object with another TextureUnit object.
     *
     * @param[in] other: the other TextureUnit object.
     */
    void swap(TextureUnit& other)
    {
        using std::swap;
        swap(mTextureHandle, other.mTextureHandle);
        swap(mUniformHandle, other.mUniformHandle);
    }

    friend void swap(TextureUnit& a, TextureUnit& b) { a.swap(b); }

    /**
     * @brief Check if the TextureUnit is valid.
     *
     * @return true if the TextureUnit is valid, false otherwise.
     */
    bool isValid() const
    {
        return bgfx::isValid(mTextureHandle) && bgfx::isValid(mUniformHandle);
    }

    void set(
        const void*               data,
        const vcl::Point2i&       sizes,
        const std::string&        samplerName,
        bool                      hasMips = false,
        bgfx::ReleaseFn           releaseFn = nullptr)
    {
        set(bgfx::makeRef(data, sizes.x() * sizes.y() * 4, releaseFn),
            sizes,
            samplerName,
            hasMips,
            1);
    }

    void set(
        const bgfx::Memory* texture,
        const vcl::Point2i& sizes,
        const std::string& samplerName,
        bool hasMips,
        uint nLayers,
        bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8,
        uint flags = BGFX_TEXTURE_NONE)
    {
        if (bgfx::isValid(mTextureHandle))
            bgfx::destroy(mTextureHandle);
        if (bgfx::isValid(mUniformHandle))
            bgfx::destroy(mUniformHandle);

        mTextureHandle = bgfx::createTexture2D(
            sizes.x(), sizes.y(), hasMips, nLayers, format, flags, texture);
        mUniformHandle = bgfx::createUniform(
            samplerName.c_str(), bgfx::UniformType::Sampler);
    }

    void bind(uint stage) const
    {
        if (bgfx::isValid(mTextureHandle) && bgfx::isValid(mUniformHandle)) {
            bgfx::setTexture(stage, mUniformHandle, mTextureHandle);
        }
    }

};

} // namespace vcl

#endif // VCL_BGFX_TEXTURE_UNIT_H
