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

        bimg::ImageContainer *hdr = loadHdr(mPanorama);

        Image img = loadImage("C:/vclib/assets/texture-cubemap-test.png");

        //auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint8_t>(size);

        //std::copy(img.data(), img.data() + size, buffer);

        auto testCubemapTexture = std::make_unique<Texture>();
                testCubemapTexture->set(
                nullptr,
                vcl::Point2i(img.height(), img.height()),
                false,
                BGFX_TEXTURE_NONE | BGFX_SAMPLER_NONE,
                bgfx::TextureFormat::RGBA8,
                true
            );
        mCubeMapTexture = std::move(testCubemapTexture);

        std::size_t size = img.sizeInBytes();
        int side = img.height();                  // keeps your meaning: one cubemap face
        int bpp  = 4;                              // RGBA8
        int faceSize = side * side * bpp;

        for (uint z = 0; z < 6; z++)
        {
            auto [buffer, releaseFn] = getAllocatedBufferAndReleaseFn<uint8_t>(faceSize);

            for (int y = 0; y < side; y++)
            {
                const uint8_t* src =
                    img.data() + (y * side * 6 + z * side) * bpp;

                uint8_t* dst = buffer + y * side * bpp;

                memcpy(dst, src, side * bpp);
            }

            mCubeMapTexture->update(
                BGFX_CUBE_MAP_POSITIVE_X + z,
                0,
                0,
                0,
                (uint16_t)side,
                (uint16_t)side,
                buffer,
                true,
                releaseFn
            );
        }



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

        // auto cubemapTexture = std::make_unique<Texture>();
        // cubemapTexture->set(
        //     nullptr,
        //     Point2i(cubeSide, cubeSide),
        //     false, // TODO: add mips when and WHERE needed
        //     BGFX_TEXTURE_COMPUTE_WRITE | BGFX_TEXTURE_RT,
        //     bgfx::TextureFormat::RGBA32F,
        //     true // is cubemap
        // );
        //mCubeMapTexture = std::move(cubemapTexture); // FIXME? why?

        // mHdrTexture->bindForCompute(
        //     0,
        //     0,
        //     bgfx::Access::Read,
        //     bgfx::TextureFormat::RGBA32F
        // );
        
        // mCubeMapTexture->bindForCompute(
        //     1,
        //     0,
        //     bgfx::Access::Read,
        //     bgfx::TextureFormat::RGBA32F
        // );

        // bgfx::dispatch(
        //     viewId,
        //     pm.getComputeProgram<HDR_EQUIRECT_TO_CUBEMAP>(),
        //     cubeSide,
        //     cubeSide,
        //     6
        // );

        Context::instance().releaseViewId(viewId);
        
    }

    template<typename T>
    std::pair<T*, bgfx::ReleaseFn> getAllocatedBufferAndReleaseFn(uint size)
    {
        T* buffer = new T[size];

        return std::make_pair(buffer, [](void* ptr, void*) {
            delete[] static_cast<T*>(ptr);
        });
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
