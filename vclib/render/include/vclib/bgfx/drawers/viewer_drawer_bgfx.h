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

#include <vclib/render/drawers/abstract_viewer_drawer.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/environment.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>

namespace vcl {

template<typename ViewProjEventDrawer>
class ViewerDrawerBGFX : public AbstractViewerDrawer<ViewProjEventDrawer>
{
    using ParentViewer = AbstractViewerDrawer<ViewProjEventDrawer>;

    DirectionalLightUniforms mDirectionalLightUniforms;

    // flags
    bool mStatsEnabled = false;

    bool mPBRMode       = false;

    PBRSettings mPBRSettings;

    PBRSettings::ToneMapping mToneMapping = PBRSettings::ToneMapping::ACES_HILL;

    float mExposure = 1.0f;

    Environment mPanorama = Environment("");

public:
    ViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
        mDirectionalLightUniforms.updateLight(ParentViewer::light());
    }

    ViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width = 1024,
        uint height = 768) : ViewerDrawerBGFX(width, height)
    {
        ParentViewer::setDrawableObjectVector(v);
    }

    bool isPBREnabled() const { return mPBRMode; }

    void setPBR(bool enable) { mPBRMode = enable; }

    void enablePBR() { setPBR(true); }

    void disablePBR() { setPBR(false); }

    PBRSettings::ToneMapping getToneMapping() const { return mToneMapping; }

    void setToneMapping(PBRSettings::ToneMapping tm) { mToneMapping = tm; }

    float getExposure() const { return mExposure; }

    void setExposure(float exposure) { mExposure = exposure; }

    void setPanorama(const std::string& panorama)
    {
        mPanorama = Environment(panorama, ParentViewer::canvasViewId());
    }

    void onDrawContent(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.viewId = viewId;

        settings.pbrSettings.pbrMode = isPBREnabled();

        settings.pbrSettings.exposure = getExposure();

        settings.pbrSettings.toneMapping =
            toUnderlying(getToneMapping());

        settings.pbrSettings.environment = &mPanorama;

        setViewTransform(viewId);

        mDirectionalLightUniforms.updateLight(ParentViewer::light());
        mDirectionalLightUniforms.bind();

        if(settings.pbrSettings.pbrMode)
            mPanorama.drawBackground(
                settings.viewId,
                settings.pbrSettings.toneMapping,
                settings.pbrSettings.exposure);

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
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
