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
        mVisibleSelectionViewIds[0] = Context::instance().requestViewId();
        bgfx::TextureHandle texHandles[3];
        texHandles[0] = bgfx::createTexture2D(
            uint16_t(sVisibleFaceFramebufferSize),
            uint16_t(sVisibleFaceFramebufferSize),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_RT | BGFX_TEXTURE_COMPUTE_WRITE | BGFX_SAMPLER_UVW_CLAMP
        );
        texHandles[1] = bgfx::createTexture2D(
            uint16_t(sVisibleFaceFramebufferSize),
            uint16_t(sVisibleFaceFramebufferSize),
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_RT | BGFX_TEXTURE_COMPUTE_WRITE | BGFX_SAMPLER_UVW_CLAMP
        );
        texHandles[2] = bgfx::createTexture2D(
            uint16_t(sVisibleFaceFramebufferSize),
            uint16_t(sVisibleFaceFramebufferSize),
            false,
            1,
            Context::instance().DEFAULT_DEPTH_FORMAT,
            BGFX_TEXTURE_RT
        );
        mVisibleSelectionFrameBuffer = bgfx::createFrameBuffer(3, texHandles, true);
        bgfx::setViewFrameBuffer(mVisibleSelectionViewIds[0], mVisibleSelectionFrameBuffer);
        bgfx::setViewClear(mVisibleSelectionViewIds[0], BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0u);
        bgfx::setViewRect(mVisibleSelectionViewIds[0], 0, 0, sVisibleFaceFramebufferSize, sVisibleFaceFramebufferSize);
        bgfx::touch(mVisibleSelectionViewIds[0]);

        mVisibleSelectionViewIds[1] = Context::instance().requestViewId();
        bgfx::TextureHandle uselessTexs[2];
        uselessTexs[0] = bgfx::createTexture2D(
            uint16_t(1),
            uint16_t(1),
            false,
            1,
            Context::instance().DEFAULT_COLOR_FORMAT,
            BGFX_TEXTURE_RT
        );
        uselessTexs[1] = bgfx::createTexture2D(
            uint16_t(1),
            uint16_t(1),
            false,
            1,
            Context::instance().DEFAULT_DEPTH_FORMAT,
            BGFX_TEXTURE_RT
        );
        mUselessFB = bgfx::createFrameBuffer(2, uselessTexs, true);
        bgfx::setViewFrameBuffer(mVisibleSelectionViewIds[1], mUselessFB);
        bgfx::setViewClear(mVisibleSelectionViewIds[1], BGFX_CLEAR_NONE);
        bgfx::setViewRect(mVisibleSelectionViewIds[1], 0, 0, 1, 1);
        bgfx::touch(mVisibleSelectionViewIds[1]);
        mAxis.init();
        mDrawTrackBall.init();
        mDrawableDirectionalLight.init();
    }

    // Box is a parameter so that if we want to subdivide the selection
    // and do multiple passes for it we can do so
    // More options: 
    // - Move the near to the farthest possible z? (i.e. move the near such that you don't have a bunch of
    //   empty space before actually encountering some object)
    // - Attempt to intersect selection box with window space box generated by the object's BB and if there is no intersection
    //   don't even bother drawing the object (not sure about how useful this would be given that clipping is already a thing)
    // - Might be worth to calculate the window space box that contains all of the valid objects and intersect it with the selection box, to further
    //   reduce the "rendering area" (and therefore have a more accurate result even at greater distances)
    bool setVisibleTrisSelectionProjViewMatrix(const SelectionBox& box)
    {
        if (box.anyNull()) {
            return false;
        }
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
        float w = maxNDC.x() - minNDC.x();
        float h = maxNDC.y() - minNDC.y();
        Matrix44f trns
            {
                {1.f, 0.f, 0.f, -(minNDC.x() + 0.5f * w)},
                {0.f, 1.f, 0.f, -(minNDC.y() + 0.5f * h)},
                {0.f, 0.f, 1.f, 0.f},
                {0.f, 0.f, 0.f, 1.f}
            };
        Matrix44f scl
            {
                {2.f/w, 0.f, 0.f, 0.f},
                {0.f, 2.f/h, 0.f, 0.f},
                {0.f, 0.f, 1.f, 0.f},
                {0.f, 0.f, 0.f, 1.f}
            };
        Matrix44f newProj = scl * trns * TED::projectionMatrix();
        bgfx::setViewTransform(mVisibleSelectionViewIds[0], TED::viewMatrix().data(), newProj.data());
        return true;
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
            bool skipSelection = false;
            SelectionBox minMaxBox = ParentViewer::selectionBox().toMinAndMax();
            if (ParentViewer::selectionMode().isVisibleSelection()) {
                skipSelection = !setVisibleTrisSelectionProjViewMatrix(calculateWindowSpaceMeshBB().intersect(mBoxToDraw.toMinAndMax()));
            }
            if (!skipSelection) {
                SelectionParameters params = SelectionParameters{
                    viewId,
                    mVisibleSelectionViewIds[0],
                    mVisibleSelectionViewIds[1],
                    minMaxBox,
                    ParentViewer::selectionMode(),
                    ParentViewer::isSelectionTemporary(),
                    bgfx::getTexture(mVisibleSelectionFrameBuffer, 0),
                    bgfx::getTexture(mVisibleSelectionFrameBuffer, 1),
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
    SelectionBox calculateWindowSpaceMeshBB() {
        Matrix44d vMat = TED::viewMatrix().cast <double>();
        Matrix44d pMat = TED::projectionMatrix().cast <double>();
        Box3d totalBB;
        for (size_t i = 0; i < ParentViewer::mDrawList->size(); i++) {
            std::shared_ptr<DrawableObject> el = ParentViewer::mDrawList->at(i);
            if (!dynamic_cast<Selectable*>(el.get())) {
                continue;
            }
            Box3d bb = el->boundingBox();
            if (bb.isNull()) {
                continue;
            }
            Point4d center{0.0, 0.0, 0.0, 1.0};
            {
                Point3d temp = bb.center();
                center.x() = temp.x();
                center.y() = temp.y();
                center.z() = temp.z();
            }
            center = pMat * vMat * center;
            center /= center.w();
            if (
                abs(center.x()) > 1
                || abs(center.y()) > 1
                || abs(center.z()) > 1
            ) {
                continue;
            }
            totalBB.add(bb);
        }
        SelectionBox box;
        if (totalBB.isNull()) {
            return box;
        }
        Point4d center4;
        center4 << totalBB.center(), 1.0;
        //camSpaceCenter = vMat * camSpaceCenter;
        double radius = totalBB.diagonal();
        Matrix44d viewFrame = vMat.inverse();
        Point4d xAx = viewFrame.col(0);
        Point4d yAx = viewFrame.col(1);
        Point4d zAx = viewFrame.col(2);
        // We find the points of the front face of the sphere's bounding box aligned with the camera's axis
        // Since the camera "looks" towards -Z, higher Z = closer to the camera (which is why we do +zAx)
        Point4d boxPts[2] = {
            pMat * vMat * (center4 + (xAx -yAx +zAx).normalized() * radius),
            pMat * vMat * (center4 + (-xAx +yAx +zAx).normalized() * radius)
        };
        for (Point4d& p: boxPts) {
            p /= p.w();
        }
        uint width = ((DerivedRenderApp*) this)->width();
        uint height = ((DerivedRenderApp*) this)->height();
        Point2d sSpace[2];
        for (size_t i = 0; i < 2; i++) {
            sSpace[i] =  Point2d{(boxPts[i].x() + 1)/2 * double(width), (1 - boxPts[i].y())/2 * double(height)};
        }
        box.set1(sSpace[0]);
        box.set2(sSpace[1]);
        return box.toMinAndMax();
    }

    void ___temp() {
        // We calculate the near plane
        Point4d camCenter;
        camCenter << TED::camera().center().cast <double>(), 1.0;
        Point4d camEye;
        camEye << TED::camera().eye().cast <double>(), 1.0;
        Point4d nearNormal = (camEye - camCenter).normalized();
        Point4d camCenterOnNear = camCenter + nearNormal * double(TED::camera().nearPlane());

        Point4d sphereCenter;
        sphereCenter << totalBB.center(), 1.0;
        
        Point4d dVec = sphereCenter - camCenter;
        double d = dVec.norm();
        dVec.normalize();

        double denom = nearNormal.dot(dVec);
        double t;
        if (abs(denom) > 0.00001f) {
            t = (sphereCenter - camCenter).dot(nearNormal) / denom;
        } else {
            return box;
        }
        Point4d sphereCenterOnNear = camCenter * (1.0 - t) + sphereCenter * t;
        double radius = totalBB.diagonal();
        double sinTeta = radius / d;
        double delta = sinTeta * (sphereCenterOnNear - sphereCenter).norm();
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
