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

#include <bx/bx.h>
#include <bx/file.h>
#include <bx/math.h>
#include <bx/readerwriter.h>

#include <bimg/bimg.h>
#include <bimg/decode.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/environment.h>

namespace vcl {

// needed for non power of two textures
constexpr uint ceilDiv(uint x, uint d)
{
    return x / d + (x % d != 0);
}

static const float VERTICES[9] {-3, -1, 1, 1, -1, 1, 1, 3, 1};

static bx::DefaultAllocator bxAllocator;

Environment::Environment(const std::string& imagePath, uint viewId)
{
    bimg::ImageContainer* image = loadImage(imagePath);
    if (image) {
        setAndGenerateTextures(*image, viewId);
        fullScreenTriangle();
        bimg::imageFree(image);
    }
}

/**
 * @brief Draws the environment in the background.
 * @param[in] viewId: The view ID to draw the background in.
 * @param[in] settings: The tone mapping operator to use.
 * @param[in] exposure: The exposure factor.
 */
void Environment::drawBackground(uint viewId, const PBRViewerSettings& settings)
    const
{
    if (!canDraw())
        return;

    using enum VertFragProgram;
    ProgramManager& pm = Context::instance().programManager();

    using enum TextureType;
    bindTexture(RAW_CUBE, VCL_MRB_CUBEMAP0);

    bindDataUniform(float(settings.toneMapping), settings.exposure);

    mVertexBuffer.bindVertex(0);

    bgfx::setState(BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LEQUAL);

    bgfx::submit(viewId, pm.getProgram<DRAWABLE_BACKGROUND_PBR>());
}

/**
 * @brief Binds the specified environment texture to the given texture stage.
 * @param[in] type: The type of texture to bind (RAW_CUBE, IRRADIANCE, SPECULAR,
 * BRDF_LUT).
 * @param[in] stage: The texture stage to bind the texture to.
 * @param[in] samplerFlags: The sampler flags to use when binding the texture.
 */
void Environment::bindTexture(TextureType type, uint stage, uint samplerFlags)
    const
{
    using enum TextureType;
    switch (type) {
    case RAW_CUBE:
        mCubeMapTexture.bind(
            stage, mEnvCubeSamplerUniform.handle(), samplerFlags);
        break;
    case IRRADIANCE:
        mIrradianceTexture.bind(
            stage, mIrradianceCubeSamplerUniform.handle(), samplerFlags);
        break;
    case SPECULAR:
        mSpecularTexture.bind(
            stage, mSpecularCubeSamplerUniform.handle(), samplerFlags);
        break;
    case BRDF_LUT:
        mBrdfLuTexture.bind(
            stage, mBrdfLutSamplerUniform.handle(), samplerFlags);
        break;
    }
}

/**
 * @brief Binds the provided data to the helper uniform (a vec4) handled by the
 * Environment class.
 *
 * @param[in] d0: The first float data to bind. Default is 0.0f.
 * @param[in] d1: The second float data to bind. Default is 0.0f.
 * @param[in] d2: The third float data to bind. Default is 0.0f.
 * @param[in] d3: The fourth float data to bind. Default is 0.0f.
 */
void Environment::bindDataUniform(
    const float d0,
    const float d1,
    const float d2,
    const float d3) const
{
    std::array<float, 4> data = {d0, d1, d2, d3};
    mDataUniform.bind(&data);
}

/**
 * @brief Determines the file format of the given image based on its extension.
 *
 * Recognized formats are HDR, EXR, KTX, DDS otherwise the format is marked as
 * UNKNOWN.
 *
 * @param[in] imagePath: The path to the image file.
 * @return The determined file format.
 */
Environment::FileFormat Environment::getFileFormat(const std::string& imagePath)
{
    using enum Environment::FileFormat;
    if (imagePath.find(".hdr", imagePath.length() - 4) != std::string::npos)
        return HDR;
    if (imagePath.find(".exr", imagePath.length() - 4) != std::string::npos)
        return EXR;
    if (imagePath.find(".ktx", imagePath.length() - 4) != std::string::npos)
        return KTX;
    if (imagePath.find(".dds", imagePath.length() - 4) != std::string::npos)
        return DDS;
    return UNKNOWN;
}

/**
 * @brief Loads the image from the specified file path.
 * @param[in] imagePath: The path to the image file.
 * @return A pointer to the loaded ImageContainer, can be nullptr.
 */
bimg::ImageContainer* Environment::loadImage(std::string imagePath)
{
    /* Code from bimg texturec */

    using enum Environment::FileFormat;
    FileFormat sourceFormat = getFileFormat(imagePath);

    if (sourceFormat == UNKNOWN)
        return nullptr;

    bx::Error      err;
    bx::FileReader reader;

    // open the file

    if (!bx::open(&reader, imagePath.c_str(), &err))
        return nullptr;

    // read file size and allocate memory

    uint32_t inputSize = (uint32_t) bx::getSize(&reader);

    if (inputSize == 0)
        return nullptr;

    uint8_t* inputData = (uint8_t*) bx::alloc(&bxAllocator, inputSize);

    // read the file and put it raw in inputData

    bx::read(&reader, inputData, inputSize, &err);
    bx::close(&reader);

    // copy the data in the final container reading its characteristics

    using enum bimg::TextureFormat::Enum;

    bimg::ImageContainer* output =
        bimg::imageParse(&bxAllocator, inputData, inputSize, RGBA32F, &err);

    bx::free(&bxAllocator, inputData);

    if (!err.isOk() ||
        (!output->m_cubeMap && sourceFormat != HDR &&
         sourceFormat !=
             EXR)) // file is neither a cubemap nor an equirectangular map
    {
        return nullptr;
    }

    return output;
}

/**
 * @brief Sets up the environment textures based on the given image.
 *
 * This function is called in the constructor after loading the image.
 *
 * @param[in] image: The image container holding the environment map data.
 */
void Environment::setAndGenerateTextures(
    const bimg::ImageContainer& image,
    uint                        viewId)
{
    // if it's not a cubemap it's equirectangular
    uint cubeSide = image.m_cubeMap ? image.m_width : ceilDiv(image.m_width, 4);

    uint8_t cubeMips =
        bimg::imageGetNumMips(bimg::TextureFormat::RGBA32F, cubeSide, cubeSide);

    // cube side for irradiance and specular
    uint irrSpecCubeSide = ceilDiv(cubeSide, 4);

    mSpecularMips = bimg::imageGetNumMips(
                        bimg::TextureFormat::RGBA32F,
                        irrSpecCubeSide,
                        irrSpecCubeSide) /
                    2; // ignore too low mips

    if (!image.m_cubeMap) { // equirect
        mHdrTexture.set(
            image,
            false, // has mips
            BGFX_TEXTURE_NONE);
    }

    const bool cubemapHasAlreadyMips = image.m_cubeMap && image.m_numMips > 1;

    const uint64_t cubemapTextureFlags =
        cubemapHasAlreadyMips ?
            // has already mips, no work to do
            BGFX_TEXTURE_NONE :
            // otherwise generate mips
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT;

    if (image.m_cubeMap) {
        mCubeMapTexture.set(
            image,
            true, // has mips
            cubemapTextureFlags);
    }
    else {
        mCubeMapTexture.set(
            nullptr,
            Point2i(cubeSide, cubeSide),
            true, // has mips
            cubemapTextureFlags,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
    }

    mIrradianceTexture.set(
        nullptr,
        Point2i(irrSpecCubeSide, irrSpecCubeSide),
        false, // has mips
        BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
        bgfx::TextureFormat::RGBA32F,
        true // is cubemap
    );

    mSpecularTexture.set(
        nullptr,
        Point2i(irrSpecCubeSide, irrSpecCubeSide),
        true, // has mips
        BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
        bgfx::TextureFormat::RGBA32F,
        true // is cubemap
    );

    mBrdfLuTexture.set(
        nullptr,
        Point2i(BRDF_LU_TEXTURE_SIZE, BRDF_LU_TEXTURE_SIZE),
        false,
        BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
        bgfx::TextureFormat::RGBA32F);

    generateTextures(image, cubeSide, cubeMips, viewId);
}

/**
 * @brief Generates the necessary environment textures (cubemap, irradiance
 * map, specular map, BRDF LUT).
 *  * This function is called by setAndGenerateTextures after setting up the
 * initial textures.
 */
void Environment::generateTextures(
    const bimg::ImageContainer& image,
    uint                        cubeSide,
    uint8_t                     cubeMips,
    uint                        viewId)
{
    using enum ComputeProgram;
    ProgramManager& pm = Context::instance().programManager();

    // if it's not a cubemap it's equirectangular

    bool shouldReleaseViewId = false;
    if (viewId == UINT_NULL) {
        viewId              = Context::instance().requestViewId();
        shouldReleaseViewId = true;
    }

    if (!image.m_cubeMap) {
        // convert hdr equirectangular to cubemap

        mHdrTexture.bind(0, mHdrSamplerUniform.handle());

        mCubeMapTexture.bindForCompute(
            1, 0, bgfx::Access::Write, bgfx::TextureFormat::RGBA32F);

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<HDR_EQUIRECT_TO_CUBEMAP>(),
            ceilDiv(cubeSide, 8),
            ceilDiv(cubeSide, 8),
            6);
    }

    const bool generateCubeMips =
        !image.m_cubeMap || (image.m_cubeMap && image.m_numMips <= 1);

    if (generateCubeMips) {
        // generate mipmaps for cubemap

        for (uint8_t mip = 1; mip < cubeMips; mip++) {
            const uint32_t mipSize = ceilDiv(cubeSide, 1 << mip);

            // ensure at least 1 threadgroup is dispatched for small mips
            // assuming the compute shader uses 8x8 threads per group
            // and checks for out-of-bounds internally
            const uint32_t threadGroups =
                (mipSize < 8) ? 1 : ceilDiv(mipSize, 8);

            mCubeMapTexture.bindForCompute(
                0,
                mip - 1,
                bgfx::Access::ReadWrite,
                bgfx::TextureFormat::RGBA32F);

            mCubeMapTexture.bindForCompute(
                1, mip, bgfx::Access::ReadWrite, bgfx::TextureFormat::RGBA32F);

            bgfx::dispatch(
                viewId,
                pm.getComputeProgram<CUBEMAP_MIPMAP_GEN>(),
                threadGroups,
                threadGroups,
                6);
        }
    }

    // create irradiance map from cubemap

    mCubeMapTexture.bind(
        0, mEnvCubeSamplerUniform.handle(), BGFX_SAMPLER_UVW_CLAMP);

    mIrradianceTexture.bindForCompute(
        1, 0, bgfx::Access::Write, bgfx::TextureFormat::RGBA32F);

    bindDataUniform(float(cubeSide));

    // cube side for irradiance and specular
    uint irrSpecCubeSide = ceilDiv(cubeSide, 4);

    bgfx::dispatch(
        viewId,
        pm.getComputeProgram<CUBEMAP_TO_IRRADIANCE>(),
        ceilDiv(irrSpecCubeSide, 8),
        ceilDiv(irrSpecCubeSide, 8),
        6);

    // create specular map from cubemap

    for (uint8_t mip = 0; mip < mSpecularMips; ++mip) {
        const uint32_t mipSize = ceilDiv(irrSpecCubeSide, 1 << mip);
        const float    roughness =
            static_cast<float>(mip) / static_cast<float>(mSpecularMips - 1);

        // ensure at least 1 threadgroup is dispatched for small mips
        // assuming the compute shader uses 8x8 threads per group
        // and checks for out-of-bounds internally
        const uint32_t threadGroups = (mipSize < 8) ? 1 : ceilDiv(mipSize, 8);

        mCubeMapTexture.bind(
            0, mEnvCubeSamplerUniform.handle(), BGFX_SAMPLER_UVW_CLAMP);

        mSpecularTexture.bindForCompute(
            1, mip, bgfx::Access::Write, bgfx::TextureFormat::RGBA32F);

        bindDataUniform(roughness, float(cubeSide));

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<CUBEMAP_TO_SPECULAR>(),
            threadGroups,
            threadGroups,
            6);
    }

    // generate BRDF lookup texture

    mBrdfLuTexture.bindForCompute(
        0, 0, bgfx::Access::Write, bgfx::TextureFormat::RGBA32F);

    bgfx::dispatch(
        viewId,
        pm.getComputeProgram<IBL_LOOKUP_TEXTURE_GEN>(),
        ceilDiv(BRDF_LU_TEXTURE_SIZE, 8),
        ceilDiv(BRDF_LU_TEXTURE_SIZE, 8));

    if (shouldReleaseViewId)
        Context::instance().releaseViewId(viewId);
}

/**
 * @brief Sets and returns the buffer for the full-screen triangle for
 * background drawing.
 */
vcl::VertexBuffer Environment::fullScreenTriangle()
{
    vcl::VertexBuffer vb;
    vb.create(
        VERTICES,
        3,
        bgfx::Attrib::Enum::Position,
        3,
        vcl::PrimitiveType::FLOAT,
        false);
    return std::move(vb);
}

} // namespace vcl
