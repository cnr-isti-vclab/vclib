/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/texture.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

#include <vclib/base.h>
#include <vclib/io.h>

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
    // Supported file formats for environment maps
    enum class FileFormat { UNKNOWN, HDR, EXR, KTX, DDS };

    // Size of the BRDF lookup texture
    static const uint BRDF_LU_TEXTURE_SIZE = 1024;

    // Sampler of mHdrTexture
    const Uniform mHdrSamplerUniform =
        Uniform("s_hdr", bgfx::UniformType::Sampler);
    // Sampler of mCubeMapTexture
    const Uniform mEnvCubeSamplerUniform =
        Uniform("s_env0", bgfx::UniformType::Sampler);
    // Sampler of mIrradianceTexture
    const Uniform mIrradianceCubeSamplerUniform =
        Uniform("s_irradiance", bgfx::UniformType::Sampler);
    // Sampler of mSpecularTexture
    const Uniform mSpecularCubeSamplerUniform =
        Uniform("s_specular", bgfx::UniformType::Sampler);
    // Sampler of mBrdfLuTexture
    const Uniform mBrdfLutSamplerUniform =
        Uniform("s_brdf_lut", bgfx::UniformType::Sampler);

    // The path of the environment image file provided as input.
    std::string mImagePath;

    // The number of mip levels in the specular cubemap,
    // this value is needed by the shader to correctly sample the specular map.
    uint8_t mSpecularMipLevels = 0;

    // The HDR environment map,
    // this is used if the input file is either in the HDR or EXR format
    // and it is an equirectangular image.
    Texture mHdrTexture;

    // The cubemap environment map,
    // this is either used directly if the input file is in the KTX or DDS
    // format or contains the cubemap generated from an equirectangular image.
    // It is used for display in the background and for caluclating the
    // irradiance and specular maps for the image-based lighting.
    Texture mCubeMapTexture;

    // The irradiance cubemap, generated from the environment cubemap.
    // It contains the diffuse part of the image-based lighting and
    // it is used in the shader to compute the diffuse contribution of the
    // environment lighting.
    Texture mIrradianceTexture;

    // The specular cubemap, generated from the environment cubemap.
    // It contains the specular part of the image-based lighting and
    // it is used in the shader to compute the specular contribution of the
    // environment lighting.
    Texture mSpecularTexture;

    // The BRDF lookup texture, generated independently from the environment
    // map. It is used in the shader to compute the BRDF part of the image-based
    // lighting.
    Texture mBrdfLuTexture;

    // Vertex buffer for rendering a full-screen triangle, used for drawing the
    // background.
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
        swap(mImagePath, other.mImagePath);
        swap(mSpecularMipLevels, other.mSpecularMipLevels);
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

    const std::string& imagePath() const { return mImagePath; }

    std::string imageFileName() const
    {
        return FileInfo::fileNameWithExtension(mImagePath);
    }

    uint8_t specularMipLevels() const { return mSpecularMipLevels; }

    void drawBackground(uint viewId, const PBRViewerSettings& settings) const;

    void bindTexture(
        TextureType type,
        uint        stage,
        uint        samplerFlags = BGFX_SAMPLER_UVW_CLAMP) const;

    /**
     * @brief Checks if the environment is ready to be drawn.
     * @return true if the environment can be drawn, false otherwise.
     */
    bool canDraw() const { return mCubeMapTexture.isValid(); }

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
