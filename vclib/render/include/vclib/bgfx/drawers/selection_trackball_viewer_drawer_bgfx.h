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

#include <bx/math.h>
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

    using TED = TrackBallEventDrawer<DerivedRenderApp>;

    bgfx::FrameBufferHandle  mVisibleSelectionFrameBuffer;
    bgfx::ViewId             mVisibleSelectionViewId;
    bgfx::VertexLayout       mVertexLayout;
    VertexBuffer             mPosBuffer;
    IndexBuffer              mTriIndexBuf;
    DrawableAxis             mAxis;
    DrawableTrackBall        mDrawTrackBall;
    DrawableDirectionalLight mDrawableDirectionalLight;
    SelectionBox             mBoxToDraw;

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
        mTriIndexBuf.create(
            bgfx::copy(
                SelectionBox::triangleIndices().data(), 6 * sizeof(uint)));
    }

    SelectionTrackBallViewerDrawerBGFX(
        const std::shared_ptr<DrawableObjectVector>& v,
        uint                                         width = 1024,
        uint height = 768) : SelectionTrackBallViewerDrawerBGFX(width, height)
    {
        ParentViewer::setDrawableObjectVector(v);
    }

    // IMPORTANT: Figure out how big the framebuffer should be
    // Same as SelectionBox?
    // Variable? Constant?
    // Same as visible view FrameBuffer?
    // You need to strike a balance between being accurate (big framebuffer)
    // And having the compute shader which has size equal to the framebuffer execute quickly
    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        mVisibleSelectionViewId = Context::instance().requestViewId();
        mVisibleSelectionFrameBuffer =
            Context::instance().createOffscreenFramebufferAndInitView(
                mVisibleSelectionViewId,
                4096,
                4096,
                false,
                0u,
                1.0f,
                (uint8_t) 0U,
                bgfx::TextureFormat::Enum::RGBA16F);
        mAxis.init();
        mDrawTrackBall.init();
        mDrawableDirectionalLight.init();
    }

    // Box is a parameter so that if we want to subdivide the selection
    // and do multiple passes for it we can do so
    void setVisibleTrisSelectionProjViewMatrix(SelectionBox box) {
        using PM         = Camera<float>::ProjectionMode;

        // We limit the projection to the selection box so that the pass itself
        // does the selection for us
        uint    win_w  = ((DerivedRenderApp*) this)->width();
        uint    win_h  = ((DerivedRenderApp*) this)->height();
        Point4f minNDC = Point4f(
            float(box.get1().value().x()) / float(win_w) * 2.f - 1.f,
            float(box.get1().value().y()) / float(win_h) * 2.f - 1.f,
            0.f,
            1.f);
        Point4f maxNDC = Point4f(
            float(box.get2().value().x()) / float(win_w) * 2.f - 1.f,
            float(box.get2().value().y()) / float(win_h) * 2.f - 1.f,
            1.f,
            1.f);
        Matrix44f invProj      = TED::projectionMatrix().inverse();
        Point4f   minViewSpace = invProj * minNDC;
        Point4f   maxViewSpace = invProj * maxNDC;
        minViewSpace           /= minViewSpace.w();
        maxViewSpace           /= maxViewSpace.w();
        float l                = min(minViewSpace.x(), maxViewSpace.x());
        float r                = max(minViewSpace.x(), maxViewSpace.x());
        float b                = max(minViewSpace.y(), maxViewSpace.y());
        float t                = min(minViewSpace.y(), maxViewSpace.y());
        float n                = min(minViewSpace.z(), maxViewSpace.z());
        float f                = max(minViewSpace.z(), maxViewSpace.z());
        float proj[16];
        if (TED::camera().projectionMode() == PM::ORTHO) {
            bx::mtxOrtho(proj, l, r, b, t, n, f, 0.f, false);
        }
        else {
            bx::mtxProj(proj, t, b, l, r, n, f, false);
        }
        float* view = TED::viewMatrix().data();
        bgfx::setViewTransform(mVisibleSelectionViewId, view, proj);
    }

    void onDraw(uint viewId) override
    {
        ParentViewer::onDraw(viewId);

        setDirectionalLightVisibility(
            ParentViewer::currentMotion() ==
            ParentViewer::TrackBallType::DIR_LIGHT_ARC);

        if (ParentViewer::selectionCalculationRequired()) {
            if (ParentViewer::selectionBox().allValue()) {
                mBoxToDraw = ParentViewer::selectionBox();
            }
            if (ParentViewer::selectionMode().isVisibleSelection() && ParentViewer::selectionMode().isFaceSelection()) {
                setVisibleTrisSelectionProjViewMatrix(mBoxToDraw.toMinAndMax());
                for (size_t i = 0; i < ParentViewer::mDrawList->size(); i++) {
                    uint oid = i+1;
                    auto el = ParentViewer::mDrawList->at(i);
                    if (auto p = dynamic_cast<Selectable*>(el.get())) {
                        DrawObjectSettings settings(mVisibleSelectionViewId, oid);
                        if (ParentViewer::selectionMode() == SelectionMode::FACE_VISIBLE_REGULAR) {
                            p->calculateSelection(settings, SelectionBox({std::nullopt, std::nullopt}), SelectionMode::FACE_NONE, true);
                        }
                        p->submitForVisibleFacesSelection(settings);
                    }
                }
            }
            for (size_t i = 0; i < ParentViewer::mDrawList->size(); i++) {
                auto el = ParentViewer::mDrawList->at(i);
                if (auto p = dynamic_cast<Selectable*>(el.get())) {
                    if (ParentViewer::selectionMode().isVisibleSelection() && ParentViewer::selectionMode().isFaceSelection()) {
                        bgfx::TextureHandle tex = bgfx::getTexture(mVisibleSelectionFrameBuffer, 0);
                        bgfx::setImage(4, tex, 0, bgfx::Access::Read, bgfx::TextureFormat::RGBA16F);
                    }
                    p->calculateSelection(
                        DrawObjectSettings(viewId, i+1),
                        ParentViewer::selectionBox().toMinAndMax(),
                        ParentViewer::selectionMode(),
                        ParentViewer::isSelectionTemporary());
                }
            }
            ParentViewer::selectionCalculated();
        }

        if (mAxis.isVisible()) {
            mAxis.draw(DrawObjectSettings(viewId, 0));
        }

        if (mDrawTrackBall.isVisible()) {
            mDrawTrackBall.draw(DrawObjectSettings(viewId, 0));
        }

        if (mDrawableDirectionalLight.isVisible()) {
            mDrawableDirectionalLight.draw(DrawObjectSettings(viewId, 0));
        }

        if (mBoxToDraw.allValue()) {
            std::array<float, 8> temp = mBoxToDraw.vertexPositions();
            bgfx::setState(
                0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_Z |
                BGFX_STATE_DEPTH_TEST_ALWAYS | BGFX_STATE_BLEND_ALPHA);
            mTriIndexBuf.bind();
            mPosBuffer.create(
                bgfx::copy(&temp[0], 8 * sizeof(float)), mVertexLayout);
            mPosBuffer.bindVertex(VCL_MRB_VERTEX_POSITION_STREAM);
            bgfx::submit(
                viewId,
                Context::instance()
                    .programManager()
                    .getProgram<VertFragProgram::DRAWABLE_SELECTION_BOX>());
        }

        if (!ParentViewer::isSelectionTemporary()) {
            mBoxToDraw.nullAll();
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
