// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/drawable/drawable_environment.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.h>

#include <bimg/bimg.h>
#include <bimg/decode.h>

namespace vcl {

// needed for non power of two textures
constexpr uint ceilDiv(uint x, uint d)
{
    return x / d + (x % d != 0);
}

static const float VERTICES[9] {-3, -1, 1, 1, -1, 1, 1, 3, 1};

DrawableEnvironment::DrawableEnvironment(
    const Panorama& panorama,
    uint            viewId)
{
    if (panorama.isValid()) {
        setAndGenerateTextures(panorama, viewId);
    }
}

/**
 * @brief Draws the environment in the background.
 * @param[in] viewId: The view ID to draw the background in.
 * @param[in] settings: The tone mapping operator to use.
 */
void DrawableEnvironment::drawBackground(
    uint                     viewId,
    const PBRViewerSettings& settings) const
{
    if (settings.pbrMode && settings.renderBackgroundPanorama && canDraw()) {
        using enum VertFragProgram;
        ProgramManager& pm = Context::instance().programManager();

        using enum TextureType;
        bindTexture(RAW_CUBE, VCL_MRB_CUBEMAP0);

        mVertexBuffer.bind(0);

        bgfx::setState(BGFX_STATE_WRITE_MASK | BGFX_STATE_DEPTH_TEST_LEQUAL);

        bgfx::submit(viewId, pm.getProgram<DRAWABLE_ENVIRONMENT_PBR>());
    }
}

/**
 * @brief Binds the specified environment texture to the given texture stage.
 * @param[in] type: The type of texture to bind (RAW_CUBE, IRRADIANCE, SPECULAR,
 * BRDF_LUT).
 * @param[in] stage: The texture stage to bind the texture to.
 * @param[in] samplerFlags: The sampler flags to use when binding the texture.
 */
void DrawableEnvironment::bindTexture(
    TextureType type,
    uint        stage,
    uint        samplerFlags) const
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
 * @brief Sets up the environment textures based on the given panorama.
 *
 * This function is called in the constructor after verifying validity.
 *
 * @param[in] panorama: The panorama object containing image data.
 * @param[in] viewId: The view ID to use for dispatching computes.
 */
void DrawableEnvironment::setAndGenerateTextures(
    const Panorama& panorama,
    uint            viewId)
{
    const bimg::ImageContainer& image = *panorama.image();

    // if it's not a cubemap it's equirectangular
    uint cubeSide = image.m_cubeMap ? image.m_width : ceilDiv(image.m_width, 4);

    uint8_t cubeMips =
        bimg::imageGetNumMips(bimg::TextureFormat::RGBA32F, cubeSide, cubeSide);

    // cube side for irradiance and specular
    uint irrSpecCubeSide = ceilDiv(cubeSide, 4);

    mSpecularMipLevels = bimg::imageGetNumMips(
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
 *
 * This function is called by setAndGenerateTextures after setting up the
 * initial textures.
 */
void DrawableEnvironment::generateTextures(
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

    DrawableEnvironmentUniforms::setCubeSideResolution(cubeSide);
    DrawableEnvironmentUniforms::bind();

    // cube side for irradiance and specular
    uint irrSpecCubeSide = ceilDiv(cubeSide, 4);

    bgfx::dispatch(
        viewId,
        pm.getComputeProgram<CUBEMAP_TO_IRRADIANCE>(),
        ceilDiv(irrSpecCubeSide, 8),
        ceilDiv(irrSpecCubeSide, 8),
        6);

    // create specular map from cubemap

    for (uint8_t mip = 0; mip < mSpecularMipLevels; ++mip) {
        const uint32_t mipSize   = ceilDiv(irrSpecCubeSide, 1 << mip);
        const float    roughness = static_cast<float>(mip) /
                                static_cast<float>(mSpecularMipLevels - 1);

        // ensure at least 1 threadgroup is dispatched for small mips
        // assuming the compute shader uses 8x8 threads per group
        // and checks for out-of-bounds internally
        const uint32_t threadGroups = (mipSize < 8) ? 1 : ceilDiv(mipSize, 8);

        mCubeMapTexture.bind(
            0, mEnvCubeSamplerUniform.handle(), BGFX_SAMPLER_UVW_CLAMP);

        mSpecularTexture.bindForCompute(
            1, mip, bgfx::Access::Write, bgfx::TextureFormat::RGBA32F);

        DrawableEnvironmentUniforms::setRoughness(roughness);
        DrawableEnvironmentUniforms::setCubeSideResolution(cubeSide);
        DrawableEnvironmentUniforms::bind();

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
vcl::VertexBuffer DrawableEnvironment::fullScreenTriangle()
{
    vcl::VertexBuffer vb;
    vb.create(
        VERTICES,
        3,
        bgfx::Attrib::Enum::Position,
        3,
        vcl::PrimitiveType::FLOAT);
    return vb;
}

} // namespace vcl
