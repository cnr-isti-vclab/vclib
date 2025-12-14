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

#ifndef VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
#define VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H

#include <vclib/render/drawers/abstract_viewer_drawer.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

#include <vclib/bgfx/texture.h>
#include <vclib/bgfx/uniform.h>

#include <vclib/io.h>

#include <vclib/io/image/hdr/load.h> // TODO: add hdr load to io/image/load.h

namespace vcl {

template<typename ViewProjEventDrawer>
class ViewerDrawerBGFX : public AbstractViewerDrawer<ViewProjEventDrawer>
{
    using ParentViewer = AbstractViewerDrawer<ViewProjEventDrawer>;

    DirectionalLightUniforms mDirectionalLightUniforms;

    std::array<float, 4> mUniformData = {0.0f, 0.0f, 0.0f, 0.0f};
    uint8_t mSpecularMips = 0;

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

    std::string mPanorama;
    bool mComputeEnvironmentTextures = false;

    // flags
    bool mStatsEnabled = false;

public:
    ViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
        mDirectionalLightUniforms.updateLight(ParentViewer::light());
        //loadEnvironmentTextures();
    }

    ViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width = 1024,
        uint height = 768) : ViewerDrawerBGFX(width, height)
    {
        ParentViewer::setDrawableObjectVector(v);
    }

    void onDrawContent(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.viewId = viewId; 

        settings.pbrMode = ParentViewer::isPBREnabled();

        bgfx::setViewTransform(
            viewId,
            ParentViewer::viewMatrix().data(),
            ParentViewer::projectionMatrix().data());

        mDirectionalLightUniforms.updateLight(ParentViewer::light());
        mDirectionalLightUniforms.bind();

        if(mComputeEnvironmentTextures)
        {
            computeEnvironmentTextures(viewId);
            mComputeEnvironmentTextures = false;
        }

        // when panorama is set, bind environment textures - TODO: find a better way to define stages
        if(!mPanorama.empty())
        {
            mCubeMapTexture->bind(
                0,
                mEnvCubeSamplerUniform.handle(),
                BGFX_SAMPLER_UVW_CLAMP
            );

            mIrradianceTexture->bind(
                1,
                mIrradianceCubeSamplerUniform.handle(),
                BGFX_SAMPLER_UVW_CLAMP
            );

            mSpecularTexture->bind(
                2,
                mSpecularCubeSamplerUniform.handle(),
                BGFX_SAMPLER_UVW_CLAMP
            );

            mBrdfLuTexture->bind(
                15,         // not cubemap, just 2d texture
                mBrdfLutSamplerUniform.handle(),
                BGFX_SAMPLER_UVW_CLAMP
            );

            mUniformData[0] = float(mSpecularMips);
            mDataUniform.bind(&mUniformData);
        }

        ParentViewer::drawableObjectVector().draw(settings);
    }

    void onDrawId(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.objectId = ParentViewer::id();
        settings.viewId   = viewId;

        bgfx::setViewTransform(
            viewId,
            ParentViewer::viewMatrix().data(),
            ParentViewer::projectionMatrix().data());

        ParentViewer::drawableObjectVector().drawId(settings);
    }

    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        if (key == Key::F1) {
            if (mStatsEnabled) {
                mStatsEnabled = false;
                bgfx::setDebug(BGFX_DEBUG_NONE);
            }
            else {
                mStatsEnabled = true;
                bgfx::setDebug(BGFX_DEBUG_STATS);
            }
        }
        ParentViewer::onKeyPress(key, modifiers);
    }

    void onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        ParentViewer::onMouseDoubleClick(button, x, y, modifiers);

        if (button == MouseButton::LEFT) {
            const bool homogeneousNDC =
                Context::instance().capabilites().homogeneousDepth;

            ParentViewer::readDepthRequest(x, y, homogeneousNDC);
        }
    }

    void setPanorama(const std::string& panorama)
    {
        mPanorama = panorama;
        mComputeEnvironmentTextures = true;
    }

    private:

    void computeEnvironmentTextures(const uint viewId)
    {
        ProgramManager& pm = Context::instance().programManager();
        using enum ComputeProgram;

        // load hdr panorama - TODO: adapt to loading both hdr or cubemaps directly

        bimg::ImageContainer *hdr = loadHdr(mPanorama);

        const uint32_t cubeSide = hdr->m_width / 4;
        const uint8_t cubeMips = bimg::imageGetNumMips(
            bimg::TextureFormat::RGBA32F,
            cubeSide,
            cubeSide
        );

        const uint32_t irradianceCubeSide = cubeSide / 4;

        const uint32_t specularCubeSide = cubeSide / 4;
        const uint8_t specularMips = bimg::imageGetNumMips(
            bimg::TextureFormat::RGBA32F,
            specularCubeSide,
            specularCubeSide
        );

        // remember specular mips to sample them appropriately
        // when drawing meshes
        mSpecularMips = specularMips;

        const uint32_t brdfLutSize = 1024;

        // create textures

        auto hdrTexture = std::make_unique<Texture>();
        hdrTexture->set(
            hdr->m_data,
            Point2i(hdr->m_width, hdr->m_height),
            false, // has mips
            BGFX_TEXTURE_NONE,
            bgfx::TextureFormat::RGBA32F
        );
        mHdrTexture = std::move(hdrTexture);

        auto cubemapTexture = std::make_unique<Texture>();
        cubemapTexture->set(
            nullptr,
            Point2i(cubeSide, cubeSide),
            true, // has mips
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
        mCubeMapTexture = std::move(cubemapTexture); // FIXME? why?

        auto irradianceTexture = std::make_unique<Texture>();
        irradianceTexture->set(
            nullptr,
            Point2i(irradianceCubeSide, irradianceCubeSide),
            false, // has mips
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
        mIrradianceTexture = std::move(irradianceTexture);

        auto specularTexture = std::make_unique<Texture>();
        specularTexture->set(
            nullptr,
            Point2i(specularCubeSide, specularCubeSide),
            true, // has mips
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
        mSpecularTexture = std::move(specularTexture);

        auto brdfLuTexture = std::make_unique<Texture>();
        brdfLuTexture->set(
            nullptr,
            Point2i(brdfLutSize, brdfLutSize),
            false,
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
            bgfx::TextureFormat::RGBA32F
        );
        mBrdfLuTexture = std::move(brdfLuTexture);

        // convert hdr equirectangular to cubemap

        mHdrTexture->bind(
            0,
            mHdrSamplerUniform.handle()
        );
        
        mCubeMapTexture->bindForCompute(
            1,
            0,
            bgfx::Access::Write,
            bgfx::TextureFormat::RGBA32F
        );

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<HDR_EQUIRECT_TO_CUBEMAP>(),
            cubeSide / 8,
            cubeSide / 8,
            6
        );

        // generate mipmaps for cubemap

        for(uint8_t mip = 1; mip < cubeMips; mip++)
        {
            const uint32_t mipSize = cubeSide >> mip;

            // ensure at least 1 threadgroup is dispatched for small mips
            // assuming the compute shader uses 8x8 threads per group
            // and checks for out-of-bounds internally
            const uint32_t threadGroups = (mipSize < 8) ? 1 : (mipSize / 8); 

            mCubeMapTexture->bindForCompute(
                0,
                mip - 1,
                bgfx::Access::ReadWrite,
                bgfx::TextureFormat::RGBA32F
            );

            mCubeMapTexture->bindForCompute(
                1,
                mip,
                bgfx::Access::ReadWrite,
                bgfx::TextureFormat::RGBA32F
            );

            bgfx::dispatch(
                viewId,
                pm.getComputeProgram<CUBEMAP_MIPMAP_GEN>(),
                threadGroups,
                threadGroups,
                6
            );

        }

        // create irradiance map from cubemap

        mCubeMapTexture->bind(
            0,
            mEnvCubeSamplerUniform.handle(),
            BGFX_SAMPLER_UVW_CLAMP
        );

        mIrradianceTexture->bindForCompute(
            1,
            0,
            bgfx::Access::Write,
            bgfx::TextureFormat::RGBA32F
        );

        mUniformData[0] = float(cubeSide);
        mDataUniform.bind(&mUniformData);

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<CUBEMAP_TO_IRRADIANCE>(),
            irradianceCubeSide / 8,
            irradianceCubeSide / 8,
            6
        );

        // create specular map from cubemap

        for(uint8_t mip = 0; mip < specularMips; ++mip) 
        {
            const uint32_t mipSize = specularCubeSide >> mip;
            const float roughness = static_cast<float>(mip) / static_cast<float>(specularMips - 1);

            // ensure at least 1 threadgroup is dispatched for small mips
            // assuming the compute shader uses 8x8 threads per group
            // and checks for out-of-bounds internally
            const uint32_t threadGroups = (mipSize < 8) ? 1 : (mipSize / 8);

            mCubeMapTexture->bind(
                0,
                mEnvCubeSamplerUniform.handle(),
                BGFX_SAMPLER_UVW_CLAMP
            );

            mSpecularTexture->bindForCompute(
                1,
                mip,
                bgfx::Access::Write,
                bgfx::TextureFormat::RGBA32F
            );

            mUniformData[0] = roughness;
            mUniformData[1] = float(cubeSide);
            mDataUniform.bind(&mUniformData);

            bgfx::dispatch(
                viewId,
                pm.getComputeProgram<CUBEMAP_TO_SPECULAR>(),
                threadGroups,
                threadGroups,
                6
            );

        }

        // generate BRDF lookup texture

        mBrdfLuTexture->bindForCompute(
            0,
            0,
            bgfx::Access::Write,
            bgfx::TextureFormat::RGBA32F
        );

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<IBL_LOOKUP_TEXTURE_GEN>(),
            brdfLutSize / 8,
            brdfLutSize / 8
        );

    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
