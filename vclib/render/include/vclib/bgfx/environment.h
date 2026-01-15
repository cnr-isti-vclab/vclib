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
    std::string mImagePath;

    vcl::VertexBuffer mVertexBuffer;

    bool
        mBackgroundReady = false,
        mCanDraw         = false;

    uint32_t
        mCubeSide           = 0,
        mIrradianceCubeSide = 0,
        mSpecularCubeSide   = 0,
        mBrdfLutSize        = 1024;

    uint8_t
        mCubeMips     = 0,
        mSpecularMips = 0;

    vcl::Uniform
        mHdrSamplerUniform            = Uniform("s_hdr", bgfx::UniformType::Sampler),
        mEnvCubeSamplerUniform        = Uniform("s_env0", bgfx::UniformType::Sampler),
        mIrradianceCubeSamplerUniform = Uniform("s_irradiance", bgfx::UniformType::Sampler),
        mSpecularCubeSamplerUniform   = Uniform("s_specular", bgfx::UniformType::Sampler),
        mBrdfLutSamplerUniform        = Uniform("s_brdf_lut", bgfx::UniformType::Sampler),
        mDataUniform                  = Uniform("u_dataPack", bgfx::UniformType::Vec4);

    std::unique_ptr<Texture>
        mHdrTexture,
        mCubeMapTexture,
        mIrradianceTexture,
        mSpecularTexture,
        mBrdfLuTexture;

    enum class FileFormat
    {
        UNKNOWN,
        HDR,
        EXR,
        KTX,
        DDS
    };

    FileFormat mSourceFormat = FileFormat::UNKNOWN;

    bimg::ImageContainer* mImage = nullptr;

    public:

    Environment() = default;

    Environment(const std::string& imagePath):
        mImagePath(imagePath)
    {}

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
        swap(mBackgroundReady, other.mBackgroundReady);
        swap(mCanDraw, other.mCanDraw);
        swap(mCubeSide, other.mCubeSide);
        swap(mIrradianceCubeSide, other.mIrradianceCubeSide);
        swap(mSpecularCubeSide, other.mSpecularCubeSide);
        swap(mBrdfLutSize, other.mBrdfLutSize);
        swap(mCubeMips, other.mCubeMips);
        swap(mSpecularMips, other.mSpecularMips);
        swap(mSourceFormat, other.mSourceFormat);
        swap(mImage, other.mImage);
        swap(mHdrTexture, other.mHdrTexture);
        swap(mCubeMapTexture, other.mCubeMapTexture);
        swap(mIrradianceTexture, other.mIrradianceTexture);
        swap(mSpecularTexture, other.mSpecularTexture);
        swap(mBrdfLuTexture, other.mBrdfLuTexture);
        mImagePath.swap(other.mImagePath);
        mVertexBuffer.swap(other.mVertexBuffer);
    }

    friend void swap(Environment& first, Environment& second)
    {
        first.swap(second);
    }

    /** @brief Types of environment textures managed by the Environment class. */
    enum class TextureType
    {
        RAW_CUBE,
        IRRADIANCE,
        SPECULAR,
        BRDF_LUT
    };

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
    bool canDraw() const { return mCanDraw; }

    /** @brief Gets the number of mipmap levels in the specular environment map.
     * @return The number of mipmap levels in the specular environment map.
    */
    uint8_t specularMips() const { return mSpecularMips; }

    private:

    /** @brief Prepares the background environment by loading the panorama image and generating necessary textures.
     * @param[in] viewId: The view ID to use for texture generation.
    */
    void prepareBackground(const uint viewId);

    /** @brief Determines the file format of the given image based on its extension.
     * @param[in] imagePath: The path to the image file.
     * @return The determined file format.
     * Recognized formats are HDR, EXR, KTX, DDS otherwise the format is marked as UNKNOWN.
    */
    FileFormat getFileFormat(const std::string& imagePath);

    /** @brief Loads the image from the specified file path.
     * @param[in] imagePath: The path to the image file.
     * @return A pointer to the loaded ImageContainer, can be nullptr.
    */
    bimg::ImageContainer* loadImage(std::string imagePath);

    /** @brief Sets up the environment textures based on the loaded image.*/
    void setTextures();

    /** @brief Renders a full-screen triangle for background drawing.*/
    void fullScreenTriangle();

    /** @brief Generates the necessary environment textures (cubemap, irradiance map, specular map, BRDF LUT).
     * @param[in] viewId: The view ID to use for texture generation.
    */
    void generateTextures(const uint viewId);
};

} // namespace vcl

#endif // VCL_BGFX_ENVIRONMENT_H
