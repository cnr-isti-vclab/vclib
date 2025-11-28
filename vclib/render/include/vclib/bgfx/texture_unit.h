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

#include <vclib/base.h>
#include <vclib/space/core.h>

#include <bgfx/bgfx.h>
#include <bimg/bimg.h>

namespace vcl {

/**
 * // TODO: rename this class
 * @class TextureUnit
 * @brief Manages a BGFX texture.
 *
 * This class is a RAII wrapper for a bgfx::TextureHandle. It handles the
 * creation, destruction, and binding of a texture.
 *
 * This class is move-only, as it represents a unique GPU resource.
 */
class TextureUnit
{
    bgfx::TextureHandle mTextureHandle = BGFX_INVALID_HANDLE;

public:
    /**
     * @brief Default constructor.
     *
     * Creates an empty and invalid TextureUnit object.
     * A call to set() is required before the object can be used.
     */
    TextureUnit() = default;

    /**
     * @brief Deleted copy constructor.
     *
     * Copying a TextureUnit is disallowed to prevent duplicate ownership of a
     * single GPU resource.
     */
    TextureUnit(const TextureUnit& other) = delete;

    /**
     * @brief Move constructor.
     *
     * Transfers ownership of the texture resources from another TextureUnit.
     * The other TextureUnit is left in an invalid state.
     *
     * @param[in] other: The TextureUnit object to move from.
     */
    TextureUnit(TextureUnit&& other) noexcept { swap(other); }

    /**
     * @brief Destructor.
     *
     * Destroys the managed BGFX texture if it is valid, releasing the
     * associated GPU resource.
     */
    ~TextureUnit()
    {
        if (bgfx::isValid(mTextureHandle))
            bgfx::destroy(mTextureHandle);
    }

    /**
     * @brief Deleted copy assignment operator.
     *
     * Copy-assigning a TextureUnit is disallowed to prevent duplicate
     * ownership of a single GPU resource.
     */
    TextureUnit& operator=(const TextureUnit& other) = delete;

    /**
     * @brief Move assignment operator.
     *
     * Transfers ownership of the texture resources from another TextureUnit.
     * The other TextureUnit is left in an invalid state.
     *
     * @param[in] other: The TextureUnit object to move from.
     * @return A reference to this object.
     */
    TextureUnit& operator=(TextureUnit&& other) noexcept
    {
        swap(other);
        return *this;
    }

    /**
     * @brief Swaps the content of this object with another TextureUnit.
     *
     * @param[in] other: The other TextureUnit object to swap with.
     */
    void swap(TextureUnit& other)
    {
        using std::swap;
        swap(mTextureHandle, other.mTextureHandle);
    }

    /**
     * @brief Swaps two TextureUnit objects.
     * @param[in] a: The first TextureUnit.
     * @param[in] b: The second TextureUnit.
     */
    friend void swap(TextureUnit& a, TextureUnit& b) { a.swap(b); }

    /**
     * @brief Checks if the TextureUnit holds valid BGFX texture handle.
     *
     * @return true if the texture handle is valid, false otherwise.
     */
    bool isValid() const { return bgfx::isValid(mTextureHandle); }

    /**
     * @brief Creates a 2D texture from raw pixel data.
     *
     * This is a convenience method that creates a standard 2D RGBA8 texture.
     * Any existing texture data in this object will be destroyed.
     *
     * @param[in] data: Pointer to the raw texture data.
     * @param[in] size: The width and height of the texture.
     * @param[in] hasMips: Indicates if the provided data includes mipmaps.
     * @param[in] flags: BGFX texture and sampler creation flags.
     * @param[in] releaseFn: Optional callback function to release the data
     * pointer when BGFX is finished with it.
     */
    void set(
        const void*         data,
        const vcl::Point2i& size,
        bool                hasMips   = false,
        uint64_t            flags     = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
        bgfx::ReleaseFn     releaseFn = nullptr)
    {
        uint32_t sz = bimg::imageGetSize(
            nullptr,
            size.x(),
            size.y(),
            1,
            false,
            hasMips,
            1,
            bimg::TextureFormat::RGBA8);
        set(bgfx::makeRef(data, sz, releaseFn),
            size,
            hasMips,
            1,
            bgfx::TextureFormat::RGBA8,
            flags);
    }

    /**
     * @brief Creates a 2D texture from a bgfx::Memory reference.
     *
     * This is the primary method for texture creation, offering full control
     * over format, layers, and mipmaps. Any existing texture data in this
     * object will be destroyed.
     *
     * @param[in] texture: Pointer to a bgfx::Memory object containing the
     * texture data.
     * @param[in] size: The width and height of the texture.
     * @param[in] hasMips: Indicates if the provided data includes mipmaps.
     * @param[in] nLayers: The number of layers for a texture array. Use 1 for a
     * standard 2D texture.
     * @param[in] format: The format of the texture data.
     * @param[in] flags: BGFX texture and sampler creation flags.
     */
    void set(
        const bgfx::Memory*       texture,
        const vcl::Point2i&       size,
        bool                      hasMips,
        uint                      nLayers,
        bgfx::TextureFormat::Enum format = bgfx::TextureFormat::RGBA8,
        uint64_t                  flags = BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE)
    {
        if (bgfx::isValid(mTextureHandle))
            bgfx::destroy(mTextureHandle);

        mTextureHandle = bgfx::createTexture2D(
            size.x(), size.y(), hasMips, nLayers, format, flags, texture);
    }

    /**
     * @brief Binds the texture to a texture stage for rendering.
     *
     * This method should be called within the rendering loop before submitting
     * a draw call that uses this texture.
     *
     * @param[in] stage: The texture stage (sampler index) to bind to.
     * @param[in] samplerHandle: The sampler uniform handle in the shader.
     * @param[in] samplerFlags: Optional BGFX sampler flags to override the
     * flags set during creation. If set to UINT32_MAX, the creation flags are
     * used.
     */
    void bind(
        uint                stage,
        bgfx::UniformHandle samplerHandle,
        uint                samplerFlags = UINT32_MAX) const
    {
        if (bgfx::isValid(mTextureHandle) && bgfx::isValid(samplerHandle)) {
            bgfx::setTexture(
                stage, samplerHandle, mTextureHandle, samplerFlags);
        }
    }

    /**
     * @brief Generates BGFX sampler flags based on the texture's filtering
     * and wrapping modes.
     *
     * @param[in] tex: The TextureDescriptor object to derive sampler flags from.
     * @return The generated BGFX sampler flags.
     */
    static uint samplerFlagsFromTextrue(const TextureDescriptor& tex)
    {
        using enum TextureDescriptor::MinificationFilter;
        using enum TextureDescriptor::WrapMode;

        uint flags = BGFX_SAMPLER_NONE;

        // set minification filter - bgfx default is linear
        if (tex.minFilter() == NEAREST ||
            tex.minFilter() == NEAREST_MIPMAP_LINEAR ||
            tex.minFilter() == NEAREST_MIPMAP_NEAREST)
            flags |= BGFX_SAMPLER_MIN_POINT;

        // set mipmap filter - bgfx default is linear
        if (tex.minFilter() == NEAREST_MIPMAP_NEAREST ||
            tex.minFilter() == LINEAR_MIPMAP_NEAREST)
            flags |= BGFX_SAMPLER_MIP_POINT;

        // set magnification filter - bgfx default is linear
        if (tex.magFilter() == TextureDescriptor::MagnificationFilter::NEAREST)
            flags |= BGFX_SAMPLER_MAG_POINT;

        // set wrap modes - bgfx default is repeat
        if (tex.wrapU() == CLAMP_TO_EDGE)
            flags |= BGFX_SAMPLER_U_CLAMP;
        else if (tex.wrapU() == MIRRORED_REPEAT)
            flags |= BGFX_SAMPLER_U_MIRROR;

        if (tex.wrapV() == CLAMP_TO_EDGE)
            flags |= BGFX_SAMPLER_V_CLAMP;
        else if (tex.wrapV() == MIRRORED_REPEAT)
            flags |= BGFX_SAMPLER_V_MIRROR;

        return flags;
    }
};

} // namespace vcl

#endif // VCL_BGFX_TEXTURE_UNIT_H
