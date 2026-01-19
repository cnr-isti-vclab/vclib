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

#ifndef VCL_BGFX_ENVIRONMENT_H
#define VCL_BGFX_ENVIRONMENT_H

#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include <vclib/base.h>
#include <vclib/bgfx/buffers.h>
#include <vclib/bgfx/uniform.h>
#include <vclib/bgfx/texture.h>

namespace vcl {

/** @brief A class representing an environment for PBR rendering.
 * It manages the loading and setup of environment maps, including
 * HDR images, cubemaps, irradiance maps, specular maps, and BRDF LUTs.
 */
class Environment
{
    enum class FileFormat
    {
        UNKNOWN,
        HDR,
        EXR,
        KTX,
        DDS
    };

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
    const Uniform mDataUniform = Uniform("u_dataPack", bgfx::UniformType::Vec4);

    Texture mHdrTexture;
    Texture mCubeMapTexture;
    Texture mIrradianceTexture;
    Texture mSpecularTexture;
    Texture mBrdfLuTexture;

    const vcl::VertexBuffer mVertexBuffer = fullScreenTriangle();

public:
    /** @brief Types of environment textures managed by the Environment class. */
    enum class TextureType
    {
        RAW_CUBE,
        IRRADIANCE,
        SPECULAR,
        BRDF_LUT
    };

    Environment() = default;

    Environment(const std::string& imagePath);

    Environment(const Environment& other) = delete;

    Environment(Environment&& other) { swap(other); }

    ~Environment() = default;

    Environment& operator=(const Environment& other) = delete;

    Environment& operator=(Environment&& other)
    {
        swap(other);
        return *this;
    }

    void swap(Environment& other)
    {
        using std::swap;
        swap(mSpecularMips, other.mSpecularMips);
        swap(mHdrTexture, other.mHdrTexture);
        swap(mCubeMapTexture, other.mCubeMapTexture);
        swap(mIrradianceTexture, other.mIrradianceTexture);
        swap(mSpecularTexture, other.mSpecularTexture);
        swap(mBrdfLuTexture, other.mBrdfLuTexture);
    }

    friend void swap(Environment& first, Environment& second)
    {
        first.swap(second);
    }

    /** @brief Draws the environment in the background.
    * @param[in] viewId: The view ID to draw the background in.
    * @param[in] toneMapping: The tone mapping operator to use.
    * @param[in] exposure: The exposure factor.
    */
    void drawBackground(const uint viewId, const int toneMapping, const float exposure);

    /** @brief Binds the specified environment texture to the given texture stage.
    * @param[in] type: The type of texture to bind (RAW_CUBE, IRRADIANCE, SPECULAR, BRDF_LUT).
    * @param[in] stage: The texture stage to bind the texture to.
    * @param[in] samplerFlags: The sampler flags to use when binding the texture.
    */
    void bindTexture(TextureType type, uint stage, uint samplerFlags = BGFX_SAMPLER_UVW_CLAMP) const;

    /** @brief Binds the provided data to the helper uniform (a vec4) handled by the Environment class.
     * @param[in] d0: The first float data to bind. Default is 0.0f.
     * @param[in] d1: The second float data to bind. Default is 0.0f.
     * @param[in] d2: The third float data to bind. Default is 0.0f.
     * @param[in] d3: The fourth float data to bind. Default is 0.0f.
    */
    void bindDataUniform(const float d0 = 0.0f, const float d1 = 0.0f, const float d2 = 0.0f, const float d3 = 0.0f) const;

    /** @brief Checks if the environment is ready to be drawn.
     * @return true if the environment can be drawn, false otherwise.
    */
    bool canDraw() const { return mCubeMapTexture.isValid(); }

    /** @brief Gets the number of mipmap levels in the specular environment map.
     * @return The number of mipmap levels in the specular environment map.
    */
    uint8_t specularMips() const { return mSpecularMips; }

private:
    FileFormat getFileFormat(const std::string& imagePath);

    bimg::ImageContainer* loadImage(std::string imagePath);

    void setAndGenerateTextures(
        const bimg::ImageContainer& image);

    void generateTextures(
        const bimg::ImageContainer& image,
        uint                        cubeSide,
        uint8_t                     cubeMips);

    static vcl::VertexBuffer fullScreenTriangle();
};

} // namespace vcl

#endif // VCL_BGFX_ENVIRONMENT_H
