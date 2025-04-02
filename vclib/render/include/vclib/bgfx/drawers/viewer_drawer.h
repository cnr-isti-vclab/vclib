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

#ifndef VCL_BGFX_DRAWERS_VIEWER_DRAWER_H
#define VCL_BGFX_DRAWERS_VIEWER_DRAWER_H

#include <vclib/render/drawers/abstract_viewer_drawer.h>

#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/uniforms/camera_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.h>
#include <vclib/bgfx/drawable/uniforms/mesh_render_settings_uniforms.h>

namespace vcl {

template<
    template<typename DRA> typename ViewProjEventDrawer,
    typename DerivedRenderApp>
class ViewerDrawerBGFX : public AbstractViewerDrawer<ViewProjEventDrawer, DerivedRenderApp>
{
    using ParentViewer = AbstractViewerDrawer<ViewProjEventDrawer, DerivedRenderApp>;
    using DTB          = ParentViewer::DTB;

    CameraUniforms           mCameraUniforms;
    DirectionalLightUniforms mDirectionalLightUniforms;

    // flags
    bool mStatsEnabled = false;

public:
    ViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
        mCameraUniforms.updateCamera(DTB::camera());
        mDirectionalLightUniforms.updateLight(DTB::light());
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
        bgfx::setViewTransform(
            viewId, DTB::viewMatrix().data(), DTB::projectionMatrix().data());

        mCameraUniforms.updateCamera(DTB::camera());
        mCameraUniforms.bind();

        mDirectionalLightUniforms.updateLight(DTB::light());
        mDirectionalLightUniforms.bind();

        ParentViewer::drawableObjectVector().draw(viewId);
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
        const bool homogeneousNDC =
            Context::instance().capabilites().homogeneousDepth;

        ParentViewer::readRequest(button, x, y, modifiers, homogeneousNDC);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_VIEWER_DRAWER_H
