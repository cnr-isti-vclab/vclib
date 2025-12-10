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

    std::array<float, 4> mRoughnessData = {0.0f, 0.0f, 0.0f, 0.0f};

    vcl::Uniform 
        mHdrSamplerUniform            = Uniform("s_hdr", bgfx::UniformType::Sampler),
        mEnvCubeSamplerUniform        = Uniform("s_env0", bgfx::UniformType::Sampler),
        mIrradianceCubeSamplerUniform = Uniform("s_irradiance", bgfx::UniformType::Sampler),
        mSpecularCubeSamplerUniform   = Uniform("s_specular", bgfx::UniformType::Sampler),
        mRoughnessUniform             = Uniform("u_roughness", bgfx::UniformType::Vec4);

    std::unique_ptr<Texture> 
        mHdrTexture, 
        mCubeMapTexture, 
        mIrradianceTexture,
        mSpecularTexture;

    std::string mPanorama;

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

        if(!mPanorama.empty())
            mCubeMapTexture->bind(
                0,
                mEnvCubeSamplerUniform.handle(),
                BGFX_SAMPLER_UVW_CLAMP
            );

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
        loadEnvironmentTextures();
    }

    private:

    void loadEnvironmentTextures()
    {
        ProgramManager& pm = Context::instance().programManager();
        using enum ComputeProgram;

        // load hdr panorama

        bimg::ImageContainer *hdr = loadHdr(mPanorama);

        // convert hdr equirectangular to cubemap

        auto hdrTexture = std::make_unique<Texture>();
        hdrTexture->set(
            hdr->m_data,
            Point2i(hdr->m_width, hdr->m_height),
            false,
            BGFX_TEXTURE_NONE,
            bgfx::TextureFormat::RGBA32F
        );
        mHdrTexture = std::move(hdrTexture);

        const uint32_t cubeSide = hdr->m_width / 4;

        const bgfx::ViewId viewId = Context::instance().requestViewId();

        auto cubemapTexture = std::make_unique<Texture>();
        cubemapTexture->set(
            nullptr,
            Point2i(cubeSide, cubeSide),
            true,
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
        mCubeMapTexture = std::move(cubemapTexture); // FIXME? why?

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
            cubeSide,
            cubeSide,
            6
        );

        // generate mipmaps for cubemap

        const uint8_t cubeMips = bimg::imageGetNumMips(
            bimg::TextureFormat::RGBA32F,
            cubeSide,
            cubeSide
        );

        for(uint8_t mip = 1; mip < cubeMips; mip++)
        {
            const uint32_t mipSize = cubeSide >> mip;

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
                mipSize,
                mipSize,
                6
            );

        }

        // create irradiance map from cubemap

        const uint irradianceCubeSide = cubeSide / 4;

        auto irradianceTexture = std::make_unique<Texture>();
        irradianceTexture->set(
            nullptr,
            Point2i(irradianceCubeSide, irradianceCubeSide),
            false,
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
        mIrradianceTexture = std::move(irradianceTexture);

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

        bgfx::dispatch(
            viewId,
            pm.getComputeProgram<CUBEMAP_TO_IRRADIANCE>(),
            irradianceCubeSide,
            irradianceCubeSide,
            6
        );

        // create specular map from cubemap

        const uint specularCubeSide = cubeSide / 4;
        const uint8_t numMips = bimg::imageGetNumMips(
            bimg::TextureFormat::RGBA32F,
            specularCubeSide,
            specularCubeSide
        );

        auto specularTexture = std::make_unique<Texture>();
        specularTexture->set(
            nullptr,
            Point2i(specularCubeSide, specularCubeSide),
            true, // generate mips
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
        mSpecularTexture = std::move(specularTexture);

        for(uint8_t mip = 0; mip < numMips; ++mip) 
        {
            const uint32_t mipSize = specularCubeSide >> mip;
            const float roughness = static_cast<float>(mip) / static_cast<float>(numMips - 1);

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

            mRoughnessData[0] = roughness;
            mRoughnessUniform.bind(&mRoughnessData);

            bgfx::dispatch(
                viewId,
                pm.getComputeProgram<CUBEMAP_TO_SPECULAR>(),
                mipSize,
                mipSize,
                6
            );

        }

        Context::instance().releaseViewId(viewId);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
