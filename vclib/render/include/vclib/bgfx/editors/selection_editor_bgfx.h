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

#ifndef VCL_BGFX_EDITORS_SELECTION_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_SELECTION_EDITOR_BGFX_H

#include <vclib/render/editors/editor.h>

#include <vclib/bgfx/buffers/index_buffer.h>
#include <vclib/bgfx/buffers/vertex_buffer.h>
#include <vclib/bgfx/context.h>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/selection/selection_parameters_bgfx.h>

#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/selection/selection_box.h>
#include <vclib/render/selection/selection_mode.h>

namespace vcl {

template<typename ViewerDrawer>
class SelectionEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    // ---- BGFX rendering resources ----
    bgfx::FrameBufferHandle     mVisibleSelectionFrameBuffer = BGFX_INVALID_HANDLE;
    bgfx::FrameBufferHandle     mUselessFB                   = BGFX_INVALID_HANDLE;
    std::array<bgfx::ViewId, 2> mVisibleSelectionViewIds     = {};
    bgfx::ViewId                mSelectionDrawingViewId      = 0;
    bgfx::VertexLayout          mVertexLayout;
    mutable VertexBuffer        mPosBuffer;
    IndexBuffer                 mTriIndexBuf;
    bool                        mInitialized = false;

    static const uint sVisibleFaceFramebufferSize = 4096u;

    // ---- Selection event state ----
    mutable SelectionBox  mSelectionBox;
    mutable SelectionMode mCurrentSelectionMode = SelectionMode::VERTEX_REGULAR;
    mutable bool          mSelectionCalcRequired = false;
    bool                  mLMBHeld               = false;
    bool                  mLMBPressPositionTaken = false;

    // ---- Rendering state ----
    mutable SelectionBox mBoxToDraw;

    // ---- Settings helpers ----

    /**
     * @brief Returns true if selection is currently active: the editor is
     * active and at least one of the 'selectVertices'/'selectFaces' settings
     * is enabled.
     */
    bool isSelectionActive() const
    {
        if (!Base::isActive())
            return false;
        const auto& cs = Base::settings().customSettings;
        bool sv = std::any_cast<bool>(cs.at("selectVertices"));
        bool sf = std::any_cast<bool>(cs.at("selectFaces"));
        return sv || sf;
    }

    /**
     * @brief Maps the current settings and drag modifier to the appropriate
     * SelectionMode.
     *
     * Priority: vertices over faces when both flags are set.
     * 'onlyVisible' is applied only for face selection.
     *
     * No modifier  → REGULAR (replace)
     * Ctrl         → ADD
     * Ctrl+Shift   → SUBTRACT
     */
    SelectionMode selectionModeForModifier(const KeyModifiers& mods) const
    {
        const auto& cs = Base::settings().customSettings;
        bool sv = std::any_cast<bool>(cs.at("selectVertices"));
        bool ov = std::any_cast<bool>(cs.at("onlyVisible"));

        bool isVertex  = sv; // vertex has priority when both flags are true
        bool isVisible = !isVertex && ov;

        bool ctrlShift =
            mods[KeyModifier::CONTROL] && mods[KeyModifier::SHIFT];
        bool ctrlOnly =
            mods[KeyModifier::CONTROL] && !mods[KeyModifier::SHIFT];

        if (ctrlShift) {
            if (isVertex) return SelectionMode::VERTEX_SUBTRACT;
            if (isVisible) return SelectionMode::FACE_VISIBLE_SUBTRACT;
            return SelectionMode::FACE_SUBTRACT;
        }
        if (ctrlOnly) {
            if (isVertex) return SelectionMode::VERTEX_ADD;
            if (isVisible) return SelectionMode::FACE_VISIBLE_ADD;
            return SelectionMode::FACE_ADD;
        }
        // No (relevant) modifier → REGULAR
        if (isVertex) return SelectionMode::VERTEX_REGULAR;
        if (isVisible) return SelectionMode::FACE_VISIBLE_REGULAR;
        return SelectionMode::FACE_REGULAR;
    }

    SelectionMode allModeForSettings() const
    {
        bool sv = std::any_cast<bool>(
            Base::settings().customSettings.at("selectVertices"));
        return sv ? SelectionMode::VERTEX_ALL : SelectionMode::FACE_ALL;
    }

    SelectionMode noneModeForSettings() const
    {
        bool sv = std::any_cast<bool>(
            Base::settings().customSettings.at("selectVertices"));
        return sv ? SelectionMode::VERTEX_NONE : SelectionMode::FACE_NONE;
    }

    SelectionMode invertModeForSettings() const
    {
        bool sv = std::any_cast<bool>(
            Base::settings().customSettings.at("selectVertices"));
        return sv ? SelectionMode::VERTEX_INVERT : SelectionMode::FACE_INVERT;
    }

public:
    SelectionEditorBGFX()
    {
        Base::settings().customSettings["selectVertices"] = false;
        Base::settings().customSettings["selectFaces"]    = false;
        Base::settings().customSettings["onlyVisible"]    = false;

        mVertexLayout.begin()
            .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
            .end();
    }

    ~SelectionEditorBGFX()
    {
        if (mInitialized) {
            if (bgfx::isValid(mVisibleSelectionFrameBuffer))
                bgfx::destroy(mVisibleSelectionFrameBuffer);
            if (bgfx::isValid(mUselessFB))
                bgfx::destroy(mUselessFB);
            Context::instance().releaseViewId(mVisibleSelectionViewIds[0]);
            Context::instance().releaseViewId(mVisibleSelectionViewIds[1]);
            Context::instance().releaseViewId(mSelectionDrawingViewId);
        }
    }

    void onInit(uint /*viewId*/) override
    {
        // Create initial vertex buffer with dummy positions
        float temp[8] = {0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f};
        mPosBuffer.create(bgfx::copy(temp, 8 * sizeof(float)), mVertexLayout);

        // Create index buffer for the two selection-box triangles
        mTriIndexBuf.create(bgfx::copy(
            SelectionBox::triangleIndices().data(), 6 * sizeof(uint)));

        // ---- Pass 1: render scene into visible-selection framebuffer ----
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

        // ---- Pass 2: compute pass (uses a small "useless" framebuffer) ----
        mVisibleSelectionViewIds[1] = Context::instance().requestViewId();
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

        // ---- Overlay view for drawing the selection box on top ----
        mSelectionDrawingViewId = Context::instance().requestViewId();
        Base::viewerSetupOverlayView(mSelectionDrawingViewId);

        mInitialized = true;
    }

    void draw(uint viewId) const override
    {
        if (!mInitialized)
            return;

        // Re-configure visible selection views every frame — required by Qt
        // (all other views follow this pattern; see context.cpp comment)
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

        bgfx::setViewFrameBuffer(mVisibleSelectionViewIds[1], mUselessFB);
        bgfx::setViewClear(mVisibleSelectionViewIds[1], BGFX_CLEAR_NONE);
        bgfx::setViewRect(mVisibleSelectionViewIds[1], 0, 0, 1, 1);
        bgfx::touch(mVisibleSelectionViewIds[1]);

        // Re-set up the overlay view each frame (handles canvas resize)
        Base::viewerSetupOverlayView(mSelectionDrawingViewId);

        // Keep the selection drawing view in sync with the main view transform
        {
            auto vm = Base::viewerViewMatrix();
            auto pm = Base::viewerProjectionMatrix();
            bgfx::setViewTransform(
                mSelectionDrawingViewId, vm.data(), pm.data());
        }

        if (isSelectionActive()) {
            calculateSelections(viewId);
            drawSelectionBox(mSelectionDrawingViewId, mBoxToDraw);
        }
        else {
            mBoxToDraw.nullAll();
            mSelectionBox.nullAll();
        }

        if (!mLMBHeld) {
            mBoxToDraw.nullAll();
        }
    }

    /**
     * @brief Handles Ctrl+A (select all), Ctrl+D (deselect all), Ctrl+I
     * (invert selection). Returns true only when one of those shortcuts is
     * consumed.
     */
    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        if (!isSelectionActive() || mLMBHeld)
            return false;

        if (modifiers[KeyModifier::CONTROL] && !modifiers[KeyModifier::ALT] &&
            !modifiers[KeyModifier::SHIFT]) {
            switch (key) {
            case Key::A:
                mCurrentSelectionMode  = allModeForSettings();
                mSelectionCalcRequired = true;
                return true;
            case Key::D:
                mCurrentSelectionMode  = noneModeForSettings();
                mSelectionCalcRequired = true;
                return true;
            case Key::I:
                mCurrentSelectionMode  = invertModeForSettings();
                mSelectionCalcRequired = true;
                return true;
            default: break;
            }
        }
        return false;
    }

    bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) override
    {
        return false;
    }

    bool onMouseMove(double x, double y, const KeyModifiers& modifiers) override
    {
        if (!isSelectionActive())
            return false;
        if (mLMBHeld) {
            mSelectionBox.set2({x, y});
            mCurrentSelectionMode  = selectionModeForModifier(modifiers);
            mSelectionCalcRequired = true;
        }
        return true; // Consume all mouse-move events while selection is active
    }

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        if (!isSelectionActive())
            return false;
        if (button == MouseButton::LEFT && !mLMBHeld) {
            mLMBHeld               = true;
            mLMBPressPositionTaken = true;
            mSelectionBox.set1({x, y});
            mSelectionBox.set2({x, y});
            mCurrentSelectionMode = selectionModeForModifier(modifiers);
        }
        return true; // Consume all mouse-press events while selection is active
    }

    bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        if (button == MouseButton::LEFT && mLMBHeld) {
            mLMBHeld               = false;
            mLMBPressPositionTaken = false;
            if (isSelectionActive()) {
                mSelectionCalcRequired = true;
            }
        }
        return isSelectionActive();
    }

    bool onMouseScroll(double dx, double dy, const KeyModifiers& modifiers)
        override
    {
        return isSelectionActive();
    }

private:
    /**
     * @brief Restricts the visible-selection projection to the sub-frustum
     * that corresponds to the given selection box.
     */
    bool setVisibleTrisSelectionProjViewMatrix(const SelectionBox& box) const
    {
        if (box.anyNull()) {
            return false;
        }
        auto s     = Base::viewerCanvasSize();
        uint win_w = s.x();
        uint win_h = s.y();
        Point4f minNDC(
            float(box.get1().value().x()) / float(win_w) * 2.f - 1.f,
            1.f - float(box.get2().value().y()) / float(win_h) * 2.f,
            0.f,
            1.f);
        Point4f maxNDC(
            float(box.get2().value().x()) / float(win_w) * 2.f - 1.f,
            1.f - float(box.get1().value().y()) / float(win_h) * 2.f,
            0.f,
            1.f);
        float     w = maxNDC.x() - minNDC.x();
        float     h = maxNDC.y() - minNDC.y();
        Matrix44f trns {
            {1.f, 0.f, 0.f, -(minNDC.x() + 0.5f * w)},
            {0.f, 1.f, 0.f, -(minNDC.y() + 0.5f * h)},
            {0.f, 0.f, 1.f, 0.f                      },
            {0.f, 0.f, 0.f, 1.f                      }
        };
        Matrix44f scl {
            {2.f / w, 0.f,     0.f, 0.f},
            {0.f,     2.f / h, 0.f, 0.f},
            {0.f,     0.f,     1.f, 0.f},
            {0.f,     0.f,     0.f, 1.f}
        };
        auto      vm      = Base::viewerViewMatrix();
        auto      pm      = Base::viewerProjectionMatrix();
        Matrix44f newProj = scl * trns * pm;
        bgfx::setViewTransform(
            mVisibleSelectionViewIds[0], vm.data(), newProj.data());
        return true;
    }

    void calculateSelections(uint viewId) const
    {
        if (!mSelectionCalcRequired) {
            return;
        }
        if (mSelectionBox.allValue()) {
            mBoxToDraw = mSelectionBox;
        }
        bool         skipSelection = false;
        SelectionBox minMaxBox     = mSelectionBox.toMinAndMax();
        if (mCurrentSelectionMode.isVisibleSelection()) {
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
            mCurrentSelectionMode,
            mLMBHeld,
            bgfx::getTexture(mVisibleSelectionFrameBuffer, 0),
            bgfx::getTexture(mVisibleSelectionFrameBuffer, 1),
            std::array<uint, 2> {
                sVisibleFaceFramebufferSize, sVisibleFaceFramebufferSize},
            0
        };
        auto dl = Base::drawList();
        for (size_t i = 0; i < dl->size(); i++) {
            params.meshId = uint(i + 1);
            auto el       = dl->at(i);
            if (auto p = dynamic_cast<AbstractDrawableMesh*>(el.get())) {
                p->computeSelection(params);
            }
        }
        selectionCalculated();
    }

    /**
     * @brief Returns the union of the screen-space bounding boxes of all
     * visible meshes that intersect the view frustum.
     */
    SelectionBox calculateWindowSpaceMeshBB() const
    {
        auto      vmf  = Base::viewerViewMatrix();
        auto      pmf  = Base::viewerProjectionMatrix();
        Matrix44d vMat = vmf.template cast<double>();
        Matrix44d pMat = pmf.template cast<double>();

        Box3d frustumNDC;
        frustumNDC.add(Point3d {-1.0, -1.0, 0.0});
        frustumNDC.add(Point3d {1.0, 1.0, 1.0});

        Box3d totalBB;
        auto  dl = Base::drawList();
        for (size_t i = 0; i < dl->size(); i++) {
            std::shared_ptr<DrawableObject> el = dl->at(i);
            if (!el->isVisible() ||
                !dynamic_cast<AbstractDrawableMesh*>(el.get())) {
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
            Box3d tempBox;
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
        auto    s      = Base::viewerCanvasSize();
        uint    width  = s.x();
        uint    height = s.y();
        Point3d boxPts[2] = {totalBB.min(), totalBB.max()};
        Point2d sSpace[2];
        for (size_t i = 0; i < 2; i++) {
            sSpace[i] = Point2d {
                (boxPts[i].x() + 1) / 2 * double(width),
                (1 - boxPts[i].y()) / 2 * double(height)};
        }
        box.set1(sSpace[0]);
        box.set2(sSpace[1]);
        return box.toMinAndMax();
    }

    void drawSelectionBox(uint viewId, const SelectionBox& box) const
    {
        if (box.anyNull()) {
            return;
        }
        std::array<float, 8> temp = box.vertexPositions();
        bgfx::setState(
            0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_ALWAYS |
            BGFX_STATE_BLEND_ALPHA);
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

    void selectionCalculated() const
    {
        mSelectionCalcRequired = false;
        if (!mLMBHeld) {
            mSelectionBox.nullAll();
        }
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_SELECTION_EDITOR_BGFX_H
