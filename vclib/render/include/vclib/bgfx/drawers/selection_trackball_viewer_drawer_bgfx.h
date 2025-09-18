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

#ifndef VCL_BGFX_DRAWERS_SELECTION_TRACKBALL_VIEWER_DRAWER_BGFX
#define VCL_BGFX_DRAWERS_SELECTION_TRACKBALL_VIEWER_DRAWER_BGFX

#include "viewer_drawer_bgfx.h"

// #include "../../examples/render/979-selection/drawable_mesh_979.h"

#include <vclib/bgfx/drawable/drawable_axis.h>
#include <vclib/bgfx/drawable/drawable_directional_light.h>
#include <vclib/bgfx/drawable/drawable_mesh_979.h>
#include <vclib/bgfx/drawable/drawable_trackball.h>
#include <vclib/render/drawers/selection_trackball_event_drawer.h>
#include <vclib/render/selection/selectable.h>

namespace vcl {

template<typename DerivedRenderApp>
class SelectionTrackBallViewerDrawerBGFX :
        public ViewerDrawerBGFX<SelectionTrackBallEventDrawer<DerivedRenderApp>>
{
    using ParentViewer =
        ViewerDrawerBGFX<SelectionTrackBallEventDrawer<DerivedRenderApp>>;

    bgfx::VertexLayout       mVertexLayout;
    VertexBuffer             mPosBuffer;
    IndexBuffer              mTriIndexBuf;
    DrawableAxis             mAxis;
    DrawableTrackBall        mDrawTrackBall;
    DrawableDirectionalLight mDrawableDirectionalLight;

public:
    using ParentViewer::ParentViewer;

    SelectionTrackBallViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height)
    {
        mVertexLayout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .end();
        float temp[8] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
        mPosBuffer.create(bgfx::copy(temp, 8 * sizeof(float)), mVertexLayout);
        uint temp2[6] = {3, 2, 0, 1, 3, 2};
        mTriIndexBuf.create(bgfx::copy(temp2, 6 * sizeof(uint)));
    }

    SelectionTrackBallViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width = 1024,
        uint height = 768) : SelectionTrackBallViewerDrawerBGFX(width, height)
    {
        ParentViewer::setDrawableObjectVector(v);
    }

    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        mAxis.init();
        mDrawTrackBall.init();
        mDrawableDirectionalLight.init();
    }

    void onDraw(uint viewId) override
    {
        ParentViewer::onDraw(viewId);

        setDirectionalLightVisibility(
            ParentViewer::currentMotion() ==
            ParentViewer::TrackBallType::DIR_LIGHT_ARC);

        if (ParentViewer::selectionCalculationRequired()) {
            for (size_t i = 0; i < ParentViewer::mDrawList->size(); i++) {
                auto el = ParentViewer::mDrawList->at(i);
                if (auto p = dynamic_cast<Selectable*>(el.get())) {
                    p->calculateSelection(
                        viewId,
                        ParentViewer::selectionBox().toMinAndMax(),
                        ParentViewer::selectionMode());
                }
            }
            ParentViewer::selectionCalculated();
        }

        if (mAxis.isVisible()) {
            mAxis.draw(viewId);
        }

        if (mDrawTrackBall.isVisible()) {
            mDrawTrackBall.draw(viewId);
        }

        if (mDrawableDirectionalLight.isVisible()) {
            mDrawableDirectionalLight.draw(viewId);
        }

        if (ParentViewer::selectionBox().allValue()) {
            std::array<float, 8> temp =
                ParentViewer::selectionBox().vertexPositions();
            mTriIndexBuf.bind();
            mPosBuffer.create(bgfx::copy(&temp[0], 8 * sizeof(float)), mVertexLayout);
            mPosBuffer.bindVertex(VCL_MRB_VERTEX_POSITION_STREAM);
            bgfx::submit(
                viewId,
                Context::instance()
                    .programManager()
                    .getProgram<VertFragProgram::DRAWABLE_SELECTION_BOX>());
        }
    }

    void onDrawContent(uint viewId) override
    {
        ParentViewer::onDrawContent(viewId);

        updateDrawableTrackball();
        updateDrawableDirectionalLight();
    }

    // events
    void onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        ParentViewer::onKeyPress(key, modifiers);

        if (ParentViewer::getCurrentToolset() == ToolSets::SELECTION) {
            return;
        }

        switch (key) {
        case Key::A: toggleAxisVisibility(); break;

        case Key::T: toggleTrackBallVisibility(); break;

        default: break;
        }
    }

    void toggleAxisVisibility() { mAxis.setVisibility(!mAxis.isVisible()); }

    void toggleTrackBallVisibility()
    {
        mDrawTrackBall.setVisibility(!mDrawTrackBall.isVisible());
    }

private:
    void updateDrawableTrackball()
    {
        auto v = ParentViewer::gizmoMatrix();
        mDrawTrackBall.setTransform(v);
        mDrawTrackBall.updateDragging(ParentViewer::isDragging());
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
};

} // namespace vcl

#endif
