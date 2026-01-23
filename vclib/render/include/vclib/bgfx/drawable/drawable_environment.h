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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_ENVIRONMENT_H
#define VCL_BGFX_DRAWABLE_DRAWABLE_ENVIRONMENT_H

#include "uniforms/drawable_environment_uniforms.h"

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/texture.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/render/drawable/drawable_object.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

#include <vclib/base.h>

#include <bgfx/bgfx.h>
#include <bx/allocator.h>

namespace vcl {

/**
 * @brief A class representing an environment for PBR rendering.
 *
 * It manages the loading and setup of environment maps, including
 * HDR images, cubemaps, irradiance maps, specular maps, and BRDF LUTs.
 */
class DrawableEnvironment
{
    enum class FileFormat { UNKNOWN, HDR, EXR, KTX, DDS };

    static const uint BRDF_LU_TEXTURE_SIZE = 1024;

    uint8_t mSpecularMips = 0;

    const Uniform mHdrSamplerUniform =
        Uniform("s_hdr", bgfx::UniformType::Sampler);
    const Uniform mEnvCubeSamplerUniform =
        Uniform("s_env0", bgfx::UniformType::Sampler);
    const Uniform mIrradianceCubeSamplerUniform =
        Uniform("s_irradiance", bgfx::UniformType::Sampler);
    const Uniform mSpecularCubeSamplerUniform =
        Uniform("s_specular", bgfx::UniformType::Sampler);
    const Uniform mBrdfLutSamplerUniform =
        Uniform("s_brdf_lut", bgfx::UniformType::Sampler);

    DrawableEnvironmentUniforms mDataUniforms;

    Texture mHdrTexture;
    Texture mCubeMapTexture;
    Texture mIrradianceTexture;
    Texture mSpecularTexture;
    Texture mBrdfLuTexture;

    const vcl::VertexBuffer mVertexBuffer = fullScreenTriangle();

public:
    /**
     * @brief Types of environment textures managed by the DrawableEnvironment
     * class.
     */
    enum class TextureType { RAW_CUBE, IRRADIANCE, SPECULAR, BRDF_LUT };

    DrawableEnvironment() = default;

    DrawableEnvironment(const std::string& imagePath, uint viewId = UINT_NULL);

    DrawableEnvironment(const DrawableEnvironment& other) = delete;

    DrawableEnvironment(DrawableEnvironment&& other) { swap(other); }

    ~DrawableEnvironment() = default;

    DrawableEnvironment& operator=(const DrawableEnvironment& other) = delete;

    DrawableEnvironment& operator=(DrawableEnvironment&& other)
    {
        swap(other);
        return *this;
    }

    void swap(DrawableEnvironment& other)
    {
        using std::swap;
        swap(mSpecularMips, other.mSpecularMips);
        swap(mHdrTexture, other.mHdrTexture);
        swap(mCubeMapTexture, other.mCubeMapTexture);
        swap(mIrradianceTexture, other.mIrradianceTexture);
        swap(mSpecularTexture, other.mSpecularTexture);
        swap(mBrdfLuTexture, other.mBrdfLuTexture);
    }

    friend void swap(DrawableEnvironment& first, DrawableEnvironment& second)
    {
        first.swap(second);
    }

    void drawBackground(uint viewId, const PBRViewerSettings& settings) const;

    void bindTexture(
        TextureType type,
        uint        stage,
        uint        samplerFlags = BGFX_SAMPLER_UVW_CLAMP) const;

    void bindUniforms() const { mDataUniforms.bind(); }

    void bindDataUniform(
        const float d1 = 0.0f,
        const float d3 = 0.0f) const;

    /**
     * @brief Checks if the environment is ready to be drawn.
     * @return true if the environment can be drawn, false otherwise.
     */
    bool canDraw() const { return mCubeMapTexture.isValid(); }

    /**
     * @brief Gets the number of mipmap levels in the specular environment map.
     * @return The number of mipmap levels in the specular environment map.
     */
    uint8_t specularMips() const { return mSpecularMips; }

private:
    FileFormat getFileFormat(const std::string& imagePath);

    bimg::ImageContainer* loadImage(std::string imagePath);

    void setAndGenerateTextures(const bimg::ImageContainer& image, uint viewId);

    void generateTextures(
        const bimg::ImageContainer& image,
        uint                        cubeSide,
        uint8_t                     cubeMips,
        uint                        viewId);

    static vcl::VertexBuffer fullScreenTriangle();
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_ENVIRONMENT_H
