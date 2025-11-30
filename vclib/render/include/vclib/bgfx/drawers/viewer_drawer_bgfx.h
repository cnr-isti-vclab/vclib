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

    vcl::Uniform 
        mHdrSamplerUniform     = Uniform("s_hdr", bgfx::UniformType::Sampler),
        mEnvCubeSamplerUniform = Uniform("s_env0", bgfx::UniformType::Sampler);

    std::unique_ptr<Texture> mCubeMapTexture, mHdrTexture;

    // flags
    bool mStatsEnabled = false;

public:
    ViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
        mDirectionalLightUniforms.updateLight(ParentViewer::light());
        loadEnvironmentTextures();
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

        mCubeMapTexture->bind(0, mEnvCubeSamplerUniform.handle());

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

    private:

    void loadEnvironmentTextures()
    {
        //bimg::ImageContainer *cubemap = loadCubemapFromHdr(VCLIB_ASSETS_PATH "/pisa.hdr");
        bimg::ImageContainer *hdr = loadHdr(VCLIB_ASSETS_PATH "/pisa.hdr");

        auto hdrTexture = std::make_unique<Texture>();
        hdrTexture->set(
            hdr->m_data,
            Point2i(hdr->m_width, hdr->m_height),
            false,
            BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
            bgfx::TextureFormat::RGBA32F
        );
        mHdrTexture = std::move(hdrTexture);

        auto cubemapTexture = std::make_unique<Texture>();
        cubemapTexture->set(
            nullptr,
            Point2i(1024, 1024),
            false, // TODO: add mips when and WHERE needed
            BGFX_TEXTURE_COMPUTE_WRITE | BGFX_SAMPLER_UVW_CLAMP,
            bgfx::TextureFormat::RGBA32F,
            true // is cubemap
        );
        mCubeMapTexture = std::move(cubemapTexture); // FIXME? why?

        
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
