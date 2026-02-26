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

#ifndef VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
#define VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H

#include "uniforms/viewer_drawer_uniforms.h"

#include <vclib/render/drawers/abstract_viewer_drawer.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/drawable_environment.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>

#include <array>

namespace vcl {

template<typename ViewProjEventDrawer>
class ViewerDrawerBGFX : public AbstractViewerDrawer<ViewProjEventDrawer>
{
    inline static const uint N_ADDITIONAL_VIEWS =
        DrawObjectSettings::N_ADDITIONAL_VIEWS;

    using ParentViewer = AbstractViewerDrawer<ViewProjEventDrawer>;
    using DRA          = ViewProjEventDrawer::DRA;

    std::array<uint, N_ADDITIONAL_VIEWS> mAdditionalViewIds;

    // flags
    bool mStatsEnabled = false;

    PBRViewerSettings mPBRSettings;

    DrawableEnvironment mPanorama = DrawableEnvironment("");

public:
    ViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
        for (uint i = 0; i < N_ADDITIONAL_VIEWS; i++) {
            mAdditionalViewIds[i] = Context::instance().requestViewId();
        }
        this->onResize(width, height);
    }

    ViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width = 1024,
        uint height = 768) : ViewerDrawerBGFX(width, height)
    {
        ParentViewer::setDrawableObjectVector(v);
    }

    ~ViewerDrawerBGFX()
    {
        for (uint i = 0; i < N_ADDITIONAL_VIEWS; i++) {
            Context::instance().releaseViewId(mAdditionalViewIds[i]);
        }
    }

    const PBRViewerSettings& pbrSettings() const { return mPBRSettings; }

    void setPbrSettings(const PBRViewerSettings& settings)
    {
        mPBRSettings = settings;
    }

    std::string panoramaFileName() const { return mPanorama.imageFileName(); }

    void setPanorama(const std::string& panorama)
    {
        mPanorama = DrawableEnvironment(panorama, ParentViewer::canvasViewId());
    }

    void onResize(uint width, uint height) override
    {
        ParentViewer::onResize(width, height);
        for (uint i = 0; i < N_ADDITIONAL_VIEWS; ++i) {
            bgfx::setViewRect(mAdditionalViewIds[i], 0, 0, width, height);
            bgfx::setViewClear(mAdditionalViewIds[i], BGFX_CLEAR_NONE);
            bgfx::touch(mAdditionalViewIds[i]);
        }
    }

    void onDrawContent(uint viewId) override
    {
        auto fbh = DRA::DRW::canvasFrameBuffer(derived());
        for (uint i = 0; i < N_ADDITIONAL_VIEWS; ++i) {
            bgfx::setViewFrameBuffer(mAdditionalViewIds[i], fbh);
            bgfx::touch(mAdditionalViewIds[i]);
        }

        DrawObjectSettings settings;
        settings.viewId = viewId;

        settings.additionalViewIds = mAdditionalViewIds;

        settings.pbrSettings = mPBRSettings;

        settings.environment = &mPanorama;

        setViewTransform(viewId);

        DirectionalLightUniforms::setLight(ParentViewer::light());
        DirectionalLightUniforms::bind();

        ViewerDrawerUniforms::setExposure(mPBRSettings.exposure);
        ViewerDrawerUniforms::setToneMapping(mPBRSettings.toneMapping);
        ViewerDrawerUniforms::setSpecularMipsLevels(
            mPanorama.specularMipLevels());
        ViewerDrawerUniforms::bind();

        // background will be drawn only if settings allow it
        mPanorama.drawBackground(settings.viewId, settings.pbrSettings);

        ParentViewer::drawableObjectVector().draw(settings);
    }

    void onDrawId(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.objectId = ParentViewer::id();
        settings.viewId   = viewId;

        setViewTransform(viewId);

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
    void setViewTransform(uint viewId)
    {
        // need to store the matrices
        // parent viewer returns by value
        Matrix44f vm = ParentViewer::viewMatrix();
        Matrix44f pm = ParentViewer::projectionMatrix();

        bgfx::setViewTransform(viewId, vm.data(), pm.data());

        for (uint i = 0; i < N_ADDITIONAL_VIEWS; ++i) {
            bgfx::setViewTransform(mAdditionalViewIds[i], vm.data(), pm.data());
        }
    }

    auto* derived() { return static_cast<DRA*>(this); }

    const auto* derived() const { return static_cast<const DRA*>(this); }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
