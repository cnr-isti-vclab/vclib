// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
#define VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H

#include "uniforms/viewer_drawer_uniforms.h"

#include <vclib/render/drawers/abstract_viewer_drawer.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/drawable_directional_light_bgfx.h>
#include <vclib/bgfx/drawable/drawable_environment.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>

#include <array>

namespace vcl {

template<typename DerivedRenderApp>
class ViewerDrawerBGFX : public AbstractViewerDrawer<DerivedRenderApp>
{
    inline static const uint N_ADDITIONAL_VIEWS =
        DrawObjectSettings::N_ADDITIONAL_VIEWS;

    using ParentViewer = AbstractViewerDrawer<DerivedRenderApp>;
    using DRA          = DerivedRenderApp;

    std::array<uint, N_ADDITIONAL_VIEWS> mAdditionalViewIds;



    // drawable directional light
    DrawableDirectionalLightBGFX mDrawableDirectionalLight;

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

    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        mDrawableDirectionalLight.init();
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

    void onDraw(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.viewId = viewId;

        ParentViewer::onDraw(viewId);

        setDirectionalLightVisibility(
            ParentViewer::currentMotion() ==
            ParentViewer::TrackBallType::DIR_LIGHT_ARC);

        if (mDrawableDirectionalLight.isVisible()) {
            mDrawableDirectionalLight.draw(settings);
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


        updateDrawableDirectionalLight();
    }

    void onDrawId(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.objectId = ParentViewer::id();
        settings.viewId   = viewId;

        setViewTransform(viewId);

        ParentViewer::drawableObjectVector().drawId(settings);
    }

    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        bool block = ParentViewer::onKeyPress(key, modifiers);

        if (!block) {
            switch(key) {
            case Key::F1:
                if (modifiers[KeyModifier::NO_MODIFIER]) {
                    mStatsEnabled = !mStatsEnabled;
                    bgfx::setDebug(
                        mStatsEnabled ? BGFX_DEBUG_STATS : BGFX_DEBUG_NONE);
                }
                break;

            default: break;
            }
        }

        return block;
    }

    bool onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        bool block = ParentViewer::onMouseDoubleClick(button, x, y, modifiers);

        if (!block && button == MouseButton::LEFT) {
            const bool homogeneousNDC =
                Context::instance().capabilites().homogeneousDepth;

            ParentViewer::readDepthRequest(x, y, homogeneousNDC);
        }
        return block;
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

    bool isDirectionalLightVisible() const
    {
        return mDrawableDirectionalLight.isVisible();
    }

    void setDirectionalLightVisibility(bool b)
    {
        mDrawableDirectionalLight.setVisibility(b);
    }

    void updateDrawableDirectionalLight()
    {
        auto v = ParentViewer::lightGizmoMatrix();
        mDrawableDirectionalLight.updateRotation(v);
    }

    auto* derived() { return static_cast<DRA*>(this); }

    const auto* derived() const { return static_cast<const DRA*>(this); }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_BGFX_H
