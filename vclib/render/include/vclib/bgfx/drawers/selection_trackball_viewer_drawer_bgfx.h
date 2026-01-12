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

    bgfx::FrameBufferHandle     mVisibleSelectionFrameBuffer;
    std::array<bgfx::ViewId, 2> mVisibleSelectionViewIds;
    bgfx::VertexLayout          mVertexLayout;
    VertexBuffer                mPosBuffer;
    IndexBuffer                 mTriIndexBuf;
    DrawableAxis                mAxis;
    DrawableTrackBall           mDrawTrackBall;
    DrawableDirectionalLight    mDrawableDirectionalLight;
    SelectionBox                mBoxToDraw;
    static const uint           sVisibleFaceFramebufferSize = 4096u;

public:
    using ParentViewer::ParentViewer;

    SelectionTrackBallViewerDrawerBGFX(uint width = 1024, uint height = 768) :
            ParentViewer(width, height), mVisibleSelectionViewIds()
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
    // And having the compute shader which has size equal to the framebuffer
    // execute quickly
    void onInit(uint viewId) override
    {
        ParentViewer::onInit(viewId);
        mVisibleSelectionViewIds[0] = Context::instance().requestViewId();
        mVisibleSelectionViewIds[1] = Context::instance().requestViewId();
        mVisibleSelectionFrameBuffer =
            Context::instance().createOffscreenFramebufferAndInitView(
                mVisibleSelectionViewIds[0],
                uint16_t(sVisibleFaceFramebufferSize),
                uint16_t(sVisibleFaceFramebufferSize),
                true,
                0u,
                1.0f,
                (uint8_t) 0U,
                bgfx::TextureFormat::Enum::RGBA8);
        bgfx::setViewFrameBuffer(mVisibleSelectionViewIds[1], mVisibleSelectionFrameBuffer);
        bgfx::setViewClear(
            mVisibleSelectionViewIds[1],
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL,
            0u,
            1.0f,
            0u);
        bgfx::setViewRect(mVisibleSelectionViewIds[0], 0, 0, 1024, 768);
        bgfx::setViewRect(mVisibleSelectionViewIds[1], 0, 0, 1024, 768);
        mAxis.init();
        mDrawTrackBall.init();
        mDrawableDirectionalLight.init();
    }

    // Box is a parameter so that if we want to subdivide the selection
    // and do multiple passes for it we can do so
    void setVisibleTrisSelectionProjViewMatrix(const SelectionBox& box)
    {
        using PM = Camera<float>::ProjectionMode;

        // We limit the projection to the selection box so that the pass itself
        // does the selection for us
        uint    win_w  = ((DerivedRenderApp*) this)->width();
        uint    win_h  = ((DerivedRenderApp*) this)->height();
        Point4f minNDC = Point4f(
            float(box.get1().value().x()) / float(win_w) * 2.f - 1.f,
            1.f - float(box.get2().value().y()) / float(win_h) * 2.f,
            0.f,
            1.f);
        Point4f maxNDC = Point4f(
            float(box.get2().value().x()) / float(win_w) * 2.f - 1.f,
            1.f - float(box.get1().value().y()) / float(win_h) * 2.f,
            0.f,
            1.f);
        Matrix44f invProj      = TED::projectionMatrix().inverse();
        Point4f   minViewSpace = invProj * minNDC;
        Point4f   maxViewSpace = invProj * maxNDC;
        minViewSpace /= minViewSpace.w();
        maxViewSpace /= maxViewSpace.w();
        float l = minViewSpace.x();
        float r = maxViewSpace.x();
        float b = minViewSpace.y();
        float t = maxViewSpace.y();
        float n = TED::camera().nearPlane();
        float f = TED::camera().farPlane();
        float proj[16];
        if (TED::camera().projectionMode() == PM::ORTHO) {
            bx::mtxOrtho(proj, l, r, b, t, n, f, 0.f, false);
        }
        else {
            bx::mtxProj(proj, t, b, l, r, n, f, false);
        }
        float* view = TED::viewMatrix().data();
        bgfx::setViewTransform(mVisibleSelectionViewIds[0], view, proj);
        bgfx::setViewTransform(mVisibleSelectionViewIds[1], view, proj);
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
            SelectionBox minMaxBox = ParentViewer::selectionBox().toMinAndMax();
            if (ParentViewer::selectionMode().isVisibleSelection()) {
                //setVisibleTrisSelectionProjViewMatrix(mBoxToDraw.toMinAndMax());
                //CHANGE TO REMOVE
                bgfx::setViewTransform(mVisibleSelectionViewIds[0], TED::viewMatrix().data(), TED::projectionMatrix().data());
                bgfx::setViewTransform(mVisibleSelectionViewIds[1], TED::viewMatrix().data(), TED::projectionMatrix().data());
                //END OF CHANGE TO REMOVE
            }
            SelectionParameters params = SelectionParameters{
                viewId,
                mVisibleSelectionViewIds[0],
                mVisibleSelectionViewIds[1],
                minMaxBox,
                ParentViewer::selectionMode(),
                ParentViewer::isSelectionTemporary(),
                bgfx::getTexture(mVisibleSelectionFrameBuffer, 0),
                std::array<uint, 2>{sVisibleFaceFramebufferSize, sVisibleFaceFramebufferSize},
                0
            };
            for (size_t i = 0; i < ParentViewer::mDrawList->size(); i++) {
                params.meshId = uint(i+1);
                auto el = ParentViewer::mDrawList->at(i);
                if (auto p = dynamic_cast<Selectable*>(el.get())) {
                    p->calculateSelection(params);
                }
            }
            ParentViewer::selectionCalculated();
        }

        {
            DrawObjectSettings settings;
            settings.objectId = 0;
            settings.viewId   = viewId;
            if (mAxis.isVisible()) {
                mAxis.draw(settings);
            }

            if (mDrawTrackBall.isVisible()) {
                mDrawTrackBall.draw(settings);
            }

            if (mDrawableDirectionalLight.isVisible()) {
                mDrawableDirectionalLight.draw(settings);
            }
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
