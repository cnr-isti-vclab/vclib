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
    bgfx::FrameBufferHandle     mUselessFB;
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

        // Set up the views for visible face selection
        mVisibleSelectionViewIds[0] = Context::instance().requestViewId();
        bgfx::TextureHandle texHandles[3];
        texHandles[0] = bgfx::createTexture2D(
            uint16_t(sVisibleFaceFramebufferSize),
            uint16_t(sVisibleFaceFramebufferSize),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_RT | BGFX_TEXTURE_COMPUTE_WRITE |
                BGFX_SAMPLER_UVW_CLAMP);
        texHandles[1] = bgfx::createTexture2D(
            uint16_t(sVisibleFaceFramebufferSize),
            uint16_t(sVisibleFaceFramebufferSize),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_RT | BGFX_TEXTURE_COMPUTE_WRITE |
                BGFX_SAMPLER_UVW_CLAMP);
        texHandles[2] = bgfx::createTexture2D(
            uint16_t(sVisibleFaceFramebufferSize),
            uint16_t(sVisibleFaceFramebufferSize),
            false,
            1,
            Context::instance().DEFAULT_DEPTH_FORMAT,
            BGFX_TEXTURE_RT);
        mVisibleSelectionFrameBuffer =
            bgfx::createFrameBuffer(3, texHandles, true);
        bgfx::setViewFrameBuffer(
            mVisibleSelectionViewIds[0], mVisibleSelectionFrameBuffer);
        bgfx::setViewClear(
            mVisibleSelectionViewIds[0],
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            0u);
        bgfx::setViewRect(
            mVisibleSelectionViewIds[0],
            0,
            0,
            sVisibleFaceFramebufferSize,
            sVisibleFaceFramebufferSize);
        bgfx::touch(mVisibleSelectionViewIds[0]);

        mVisibleSelectionViewIds[1] = Context::instance().requestViewId();
        // These other framebuffer is not actually necessary (i could assign to
        // the second view the same framebuffer as the first or keep the default
        // one) but i prefer to do it this way in case some draw is erroneously
        // called on this view (if that happens the window's framebuffer or the
        // first view's framebuffer would get modified otherwise)
        bgfx::TextureHandle uselessTexs[2];
        uselessTexs[0] = bgfx::createTexture2D(
            uint16_t(1),
            uint16_t(1),
            false,
            1,
            Context::instance().DEFAULT_COLOR_FORMAT,
            BGFX_TEXTURE_RT);
        uselessTexs[1] = bgfx::createTexture2D(
            uint16_t(1),
            uint16_t(1),
            false,
            1,
            Context::instance().DEFAULT_DEPTH_FORMAT,
            BGFX_TEXTURE_RT);
        mUselessFB = bgfx::createFrameBuffer(2, uselessTexs, true);
        bgfx::setViewFrameBuffer(mVisibleSelectionViewIds[1], mUselessFB);
        bgfx::setViewClear(mVisibleSelectionViewIds[1], BGFX_CLEAR_NONE);
        bgfx::setViewRect(mVisibleSelectionViewIds[1], 0, 0, 1, 1);
        bgfx::touch(mVisibleSelectionViewIds[1]);

        mAxis.init();
        mDrawTrackBall.init();
        mDrawableDirectionalLight.init();
    }

    /**
     * @brief Sets the frustum of the first pass of the visible
     * selection to a sub-frustum that corresponds to the portion of the
     * frustum obtained by unprojecting the selection box
     *
     * @important: Currently {(0,0),(window_w, window_h)} is used for
     * calculations, however what should be used is instead {(viewport_x,
     * viewport_y),(viewport_w, viewport_h)} to account for cases in which the
     * viewport is not the entire window
     *
     * @param[in] box: The selection box to derive the sub-frustum from
     * @return: true if the operation was successful, false otherwise
     */
    bool setVisibleTrisSelectionProjViewMatrix(const SelectionBox& box)
    {
        if (box.anyNull()) {
            return false;
        }
        // We limit the projection to the selection box so that the pass itself
        // does the selection for us
        // WARNING: here the viewport should be used instead of width and height
        // of the window but there is no way to obtain it currently (this is to
        // account for cases in which the viewport is NOT (0,0)->(width, height)
        // but something different)
        uint    win_w  = ((DerivedRenderApp*) this)->width();
        uint    win_h  = ((DerivedRenderApp*) this)->height();
        Point4f minNDC = Point4f(
            float(box.get1().value().x() - 0) / float(win_w) * 2.f - 1.f,
            1.f - float(box.get2().value().y() - 0) / float(win_h) * 2.f,
            0.f,
            1.f);
        Point4f maxNDC = Point4f(
            float(box.get2().value().x()) / float(win_w) * 2.f - 1.f,
            1.f - float(box.get1().value().y()) / float(win_h) * 2.f,
            0.f,
            1.f);
        float     w = maxNDC.x() - minNDC.x();
        float     h = maxNDC.y() - minNDC.y();
        Matrix44f trns {
            {1.f, 0.f, 0.f, -(minNDC.x() + 0.5f * w)},
            {0.f, 1.f, 0.f, -(minNDC.y() + 0.5f * h)},
            {0.f, 0.f, 1.f, 0.f                     },
            {0.f, 0.f, 0.f, 1.f                     }
        };
        Matrix44f scl {
            {2.f / w, 0.f,     0.f, 0.f},
            {0.f,     2.f / h, 0.f, 0.f},
            {0.f,     0.f,     1.f, 0.f},
            {0.f,     0.f,     0.f, 1.f}
        };
        Matrix44f newProj = scl * trns * TED::projectionMatrix();
        bgfx::setViewTransform(
            mVisibleSelectionViewIds[0],
            TED::viewMatrix().data(),
            newProj.data());
        return true;
    }

    void onDraw(uint viewId) override
    {
        ParentViewer::onDraw(viewId);

        setDirectionalLightVisibility(
            ParentViewer::currentMotion() ==
            ParentViewer::TrackBallType::DIR_LIGHT_ARC);

        calculateSelections(viewId);

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

        drawSelectionBox(viewId, calculateWindowSpaceMeshBB());
        drawSelectionBox(viewId, mBoxToDraw);

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
    /**
     * @brief "Enqueues" the selection request on all the valid DrawableObjects
     * in the drawList
     *
     * @param[in] viewId: The draw viewId
     */
    void calculateSelections(uint viewId)
    {
        if (!ParentViewer::selectionCalculationRequired()) {
            return;
        }
        if (ParentViewer::selectionBox().allValue()) {
            mBoxToDraw = ParentViewer::selectionBox();
        }
        bool         skipSelection = false;
        SelectionBox minMaxBox     = ParentViewer::selectionBox().toMinAndMax();
        if (ParentViewer::selectionMode().isVisibleSelection()) {
            skipSelection = !setVisibleTrisSelectionProjViewMatrix(
                calculateWindowSpaceMeshBB().intersect(
                    mBoxToDraw.toMinAndMax()));
        }
        if (skipSelection) {
            return;
        }
        SelectionParameters params = SelectionParameters {
            viewId,
            mVisibleSelectionViewIds[0],
            mVisibleSelectionViewIds[1],
            minMaxBox,
            ParentViewer::selectionMode(),
            ParentViewer::isSelectionTemporary(),
            bgfx::getTexture(mVisibleSelectionFrameBuffer, 0),
            bgfx::getTexture(mVisibleSelectionFrameBuffer, 1),
            std::array<uint, 2> {
                                 sVisibleFaceFramebufferSize, sVisibleFaceFramebufferSize},
            0
        };
        // Call calculateSelection method on all the DrawableObjects which
        // implement the Selectable interface REMINDER: in this context objectId
        // 0 is reserved to indicate a fragment which did NOT pass in face
        // visible selection
        for (size_t i = 0; i < ParentViewer::mDrawList->size(); i++) {
            params.meshId = uint(i + 1);
            auto el       = ParentViewer::mDrawList->at(i);
            if (auto p = dynamic_cast<Selectable*>(el.get())) {
                p->calculateSelection(params);
            }
        }
        ParentViewer::selectionCalculated();
    }

    /**
     * @brief Calculates the union of the bounding boxes of all meshes that are
     * eligible for selection and that are NOT entirely out of the frustum and
     * then returns its projection on the screen
     *
     * @important: Currently {(0,0),(window_w, window_h)} is used for
     * calculations, however what should be used is instead {(viewport_x,
     * viewport_y),(viewport_w, viewport_h)} to account for cases in which the
     * viewport is not the entire window
     * 
     * @return: The projection (in screen space) of the union of the bounding
     * boxes
     */
    SelectionBox calculateWindowSpaceMeshBB()
    {
        Matrix44d vMat = TED::viewMatrix().template cast<double>();
        Matrix44d pMat = TED::projectionMatrix().template cast<double>();
        Box3d     frustumNDC;
        frustumNDC.add(Point3d {-1.0, -1.0, -1.0});
        frustumNDC.add(Point3d {1.0, 1.0, 1.0});
        // In NDC space
        Box3d totalBB;
        for (size_t i = 0; i < ParentViewer::mDrawList->size(); i++) {
            std::shared_ptr<DrawableObject> el = ParentViewer::mDrawList->at(i);
            if (!el->isVisible() || !dynamic_cast<Selectable*>(el.get())) {
                continue;
            }
            Box3d bb = el->boundingBox();
            if (bb.isNull()) {
                continue;
            }
            Point3d maxBB        = bb.max();
            Point3d minBB        = bb.min();
            Point4d boxPoints[8] = {
                Point4d {minBB.x(), minBB.y(), minBB.z(), 1.0},
                Point4d {maxBB.x(), minBB.y(), minBB.z(), 1.0},
                Point4d {minBB.x(), maxBB.y(), minBB.z(), 1.0},
                Point4d {maxBB.x(), maxBB.y(), minBB.z(), 1.0},
                Point4d {minBB.x(), minBB.y(), maxBB.z(), 1.0},
                Point4d {minBB.x(), maxBB.y(), maxBB.z(), 1.0},
                Point4d {maxBB.x(), minBB.y(), maxBB.z(), 1.0},
                Point4d {maxBB.x(), maxBB.y(), maxBB.z(), 1.0}
            };
            Box3d   tempBox;
            Point4d center {0.0, 0.0, 0.0, 1.0};
            for (const Point4d& p : boxPoints) {
                Point4d pNDC = pMat * vMat * p;
                pNDC /= pNDC.w();
                tempBox.add(pNDC.head<3>());
            }
            Box3d inters = tempBox.intersection(frustumNDC);
            if (inters.isNull()) {
                continue;
            }
            totalBB.add(inters);
        }
        SelectionBox box;
        if (totalBB.isNull()) {
            return box;
        }
        uint    width     = ((DerivedRenderApp*) this)->width();
        uint    height    = ((DerivedRenderApp*) this)->height();
        Point3d boxPts[2] = {totalBB.min(), totalBB.max()};
        Point2d sSpace[2];
        for (size_t i = 0; i < 2; i++) {
            sSpace[i] = Point2d {
                (boxPts[i].x() - 0 + 1) / 2 * double(width),
                (1 - boxPts[i].y() - 0) / 2 * double(height)};
        }
        box.set1(sSpace[0]);
        box.set2(sSpace[1]);
        return box.toMinAndMax();
    }

    void drawSelectionBox(uint viewId, const SelectionBox& box)
    {
        if (box.anyNull()) {
            return;
        }
        std::array<float, 8> temp = box.vertexPositions();
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
