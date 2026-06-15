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

#include <vclib/bgfx/buffers/frame_buffer.h>
#include <vclib/bgfx/screenspace/overlay/screenspace_box.h>
#include <vclib/bgfx/selection/selection_parameters_bgfx.h>

#include <vclib/render/drawable/abstract_drawable_mesh.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/selection/selection_mode.h>

#include <array>
#include <vector>

namespace vcl {

template<typename ViewerDrawer>
class SelectionEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    // ---- BGFX rendering resources ----
    FrameBuffer                 mVisibleSelectionFB;
    FrameBuffer                 mComputePassFB;
    std::array<bgfx::ViewId, 2> mVisibleSelectionViewIds = {};
    ScreenSpaceBox              mScreenSpaceBox;
    bool                        mInitialized = false;

    static const uint DEFAULT_VISIBLE_FACE_FB_SIZE = 4096u;
    uint mVisibleFaceFBSize = DEFAULT_VISIBLE_FACE_FB_SIZE;

    // ---- Selection event state ----
    std::optional<Box2d>       mSelectionBox;
    std::optional<Point2d>     mSelectionAnchor;
    std::vector<SelectionMode> mCurrentSelectionModes;
    bool                       mSelectionCalcRequired = false;
    bool                       mSelectionInProgress   = false;

    // GPU-to-CPU readback tracking for Qt (requires continuous frame updates)
    uint mPendingReadbackFrames = 0;

public:
    SelectionEditorBGFX()
    {
        Base::settings().customSettings["selectVertices"] = false;
        Base::settings().customSettings["selectFaces"]    = false;
        Base::settings().customSettings["onlyVisible"]    = false;
        Base::settings().customSettings["selectionBoxColor"] =
            vcl::Color(27, 120, 249, 64);
    }

    ~SelectionEditorBGFX()
    {
        if (mInitialized) {
            // FrameBuffers handle their own destruction
            Context::instance().releaseViewId(mVisibleSelectionViewIds[0]);
            Context::instance().releaseViewId(mVisibleSelectionViewIds[1]);
        }
    }

    void onInit(uint /*viewId*/) override
    {
        mVisibleFaceFBSize = std::min(
            DEFAULT_VISIBLE_FACE_FB_SIZE, Context::instance().capabilites().limits.maxTextureSize);

        // Initialize screen-space box for selection overlay
        mScreenSpaceBox.init();
        mScreenSpaceBox.setColor(
            std::any_cast<const vcl::Color&>(
                Base::settings().customSettings.at("selectionBoxColor")));

        // ---- Pass 1: render scene into visible-selection framebuffer ----
        mVisibleSelectionViewIds[0] = Context::instance().requestViewId();
        bgfx::TextureHandle texHandles[3];
        texHandles[0] = bgfx::createTexture2D(
            mVisibleFaceFBSize,
            mVisibleFaceFBSize,
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_RT | BGFX_TEXTURE_COMPUTE_WRITE |
                BGFX_SAMPLER_UVW_CLAMP);
        texHandles[1] = bgfx::createTexture2D(
            mVisibleFaceFBSize,
            mVisibleFaceFBSize,
            false,
            1,
            bgfx::TextureFormat::RGBA8,
            BGFX_TEXTURE_RT | BGFX_TEXTURE_COMPUTE_WRITE |
                BGFX_SAMPLER_UVW_CLAMP);
        texHandles[2] = bgfx::createTexture2D(
            mVisibleFaceFBSize,
            mVisibleFaceFBSize,
            false,
            1,
            Context::instance().DEFAULT_DEPTH_FORMAT,
            BGFX_TEXTURE_RT);
        mVisibleSelectionFB.create(texHandles, 3, true);
        bgfx::setViewFrameBuffer(
            mVisibleSelectionViewIds[0], mVisibleSelectionFB.handle());
        bgfx::setViewClear(
            mVisibleSelectionViewIds[0],
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            UINT_NULL);
        bgfx::setViewRect(
            mVisibleSelectionViewIds[0],
            0,
            0,
            mVisibleFaceFBSize,
            mVisibleFaceFBSize);
        bgfx::touch(mVisibleSelectionViewIds[0]);

        // ---- Pass 2: compute pass (uses a small "useless" framebuffer) ----
        mVisibleSelectionViewIds[1] = Context::instance().requestViewId();
        bgfx::TextureHandle computePassTexs[2];
        computePassTexs[0] = bgfx::createTexture2D(
            1,
            1,
            false,
            1,
            Context::instance().DEFAULT_COLOR_FORMAT,
            BGFX_TEXTURE_RT);
        computePassTexs[1] = bgfx::createTexture2D(
            1,
            1,
            false,
            1,
            Context::instance().DEFAULT_DEPTH_FORMAT,
            BGFX_TEXTURE_RT);
        mComputePassFB.create(computePassTexs, 2, true);
        bgfx::setViewFrameBuffer(
            mVisibleSelectionViewIds[1], mComputePassFB.handle());
        bgfx::setViewClear(mVisibleSelectionViewIds[1], BGFX_CLEAR_NONE);
        bgfx::setViewRect(mVisibleSelectionViewIds[1], 0, 0, 1, 1);
        bgfx::touch(mVisibleSelectionViewIds[1]);

        mInitialized = true;
    }

    void setSelectionBoxColor(const Color& color)
    {
        Base::settings().customSettings["selectionBoxColor"] = color;
        mScreenSpaceBox.setColor(color);
    }

    void draw(uint viewId) override
    {
        if (!mInitialized)
            return;

        if (!isSelectionActive()) {
            mSelectionBox.reset();
            mSelectionAnchor.reset();
            return;
        }

        // Re-configure visible selection views every frame — required by Qt
        // (all other views follow this pattern; see context.cpp comment)
        bgfx::setViewFrameBuffer(
            mVisibleSelectionViewIds[0], mVisibleSelectionFB.handle());
        bgfx::setViewClear(
            mVisibleSelectionViewIds[0],
            BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH,
            UINT_NULL);
        bgfx::setViewRect(
            mVisibleSelectionViewIds[0],
            0,
            0,
            mVisibleFaceFBSize,
            mVisibleFaceFBSize);
        bgfx::touch(mVisibleSelectionViewIds[0]);

        bgfx::setViewFrameBuffer(
            mVisibleSelectionViewIds[1], mComputePassFB.handle());
        bgfx::setViewClear(mVisibleSelectionViewIds[1], BGFX_CLEAR_NONE);
        bgfx::setViewRect(mVisibleSelectionViewIds[1], 0, 0, 1, 1);
        bgfx::touch(mVisibleSelectionViewIds[1]);

        // 1) Draw the selection box overlay (only visible while dragging)
        if (mSelectionInProgress) {
            mScreenSpaceBox.setBox(mSelectionBox.value_or(Box2d {}));
            mScreenSpaceBox.setVisible(true);
            mScreenSpaceBox.draw(viewId);
        }
        else {
            mScreenSpaceBox.setVisible(false);
        }

        // 2) Compute GPU selections with current parameters, if pending
        if (mSelectionCalcRequired) {
            computeSelections(viewId);

            // Trigger continuous updates for GPU-to-CPU selection readback
            // (Qt widgets only redraw on interaction, but readback needs 2
            // frames)
            mPendingReadbackFrames = 2;

            // Computation request done — reset flag
            mSelectionCalcRequired = false;
        }

        // Request continuous updates until GPU-to-CPU readback completes
        if (mPendingReadbackFrames > 0) {
            mPendingReadbackFrames--;
            Base::viewerUpdate();
        }
    }

    /**
     * @brief Handles Ctrl+A (select all), Ctrl+D (deselect all), Ctrl+I
     * (invert selection). Returns true only when one of those shortcuts is
     * consumed.
     */
    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        if (!isSelectionActive() || mSelectionInProgress)
            return false;

        if (modifiers.only(KeyModifier::CONTROL)) {
            using enum SelectionAction;
            switch (key) {
            case Key::A:
                mCurrentSelectionModes = actionModesForSettings<ALL>();
                mSelectionCalcRequired = true;
                return true;
            case Key::D:
                mCurrentSelectionModes = actionModesForSettings<NONE>();
                mSelectionCalcRequired = true;
                return true;
            case Key::I:
                mCurrentSelectionModes = actionModesForSettings<INVERT>();
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
        if (mSelectionInProgress) {
            mSelectionBox = Box2d(mSelectionAnchor.value());
            mSelectionBox->add(Point2d {x, y});
            mCurrentSelectionModes = selectionModesForModifier(modifiers);
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
        if (button == MouseButton::LEFT && !mSelectionInProgress) {
            mSelectionInProgress               = true;
            mSelectionAnchor       = Point2d {x, y};
            mSelectionBox          = Box2d({x, y});
            mCurrentSelectionModes = selectionModesForModifier(modifiers);
        }
        return true; // Consume all mouse-press events while selection is active
    }

    bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        if (button == MouseButton::LEFT && mSelectionInProgress) {
            mSelectionInProgress               = false;
            mSelectionCalcRequired = true;
            // Force a repaint so the selection box disappears immediately
            Base::viewerUpdate();
        }
        return isSelectionActive();
    }

    bool onMouseScroll(double dx, double dy, const KeyModifiers& modifiers)
        override
    {
        return isSelectionActive();
    }

private:
    // ---- Settings helpers ----

    /**
     * @brief Returns true if the draw-list element at the given index should
     * be processed, based on the current EditorSettings::editMode.
     *
     * CURRENT_OBJECT  → only the selected object
     * VISIBLE_OBJECTS → only visible objects
     */
    bool shouldProcessObject(const DrawableObjectVector& dl, uint index) const
    {
        switch (Base::settings().editMode) {
        case EditorSettings::EditMode::CURRENT_OBJECT:
            return index == dl.selectedObjectId();
        case EditorSettings::EditMode::VISIBLE_OBJECTS:
            return dl.at(index)->isVisible();
        default:
            // NONE and ALL_OBJECTS are not supported by this editor
            return false;
        }
    }

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
        bool        sv = std::any_cast<bool>(cs.at("selectVertices"));
        bool        sf = std::any_cast<bool>(cs.at("selectFaces"));
        return sv || sf;
    }

    /**
     * @brief Maps modifier keys to the corresponding SelectionAction.
     *
     * No modifier  → REGULAR (replace)
     * Ctrl         → ADD
     * Ctrl+Shift   → SUBTRACT
     */
    static SelectionAction actionFromModifier(const KeyModifiers& mods)
    {
        if (mods[KeyModifier::CONTROL] && mods[KeyModifier::SHIFT])
            return SelectionAction::SUBTRACT;
        if (mods[KeyModifier::CONTROL])
            return SelectionAction::ADD;
        return SelectionAction::REGULAR;
    }

    /**
     * @brief Maps the current settings and drag modifier to the appropriate
     * list of SelectionModes (one per active selection type).
     *
     * When both 'selectVertices' and 'selectFaces' are enabled both a vertex
     * mode and a face mode are returned. 'onlyVisible' is applied only to
     * face selection.
     */
    std::vector<SelectionMode> selectionModesForModifier(
        const KeyModifiers& mods) const
    {
        const auto& cs = Base::settings().customSettings;
        bool        sv = std::any_cast<bool>(cs.at("selectVertices"));
        bool        sf = std::any_cast<bool>(cs.at("selectFaces"));
        bool        ov = std::any_cast<bool>(cs.at("onlyVisible"));

        auto action = actionFromModifier(mods);

        std::vector<SelectionMode> modes;
        if (sv) {
            modes.push_back({SelectionPrimitive::VERTEX, action});
        }
        if (sf) {
            modes.push_back(
                {SelectionPrimitive::FACE, action, static_cast<bool>(ov)});
        }
        return modes;
    }

    template<SelectionAction ACTION>
    std::vector<SelectionMode> actionModesForSettings() const
    {
        const auto&                cs = Base::settings().customSettings;
        std::vector<SelectionMode> modes;
        if (std::any_cast<bool>(cs.at("selectVertices")))
            modes.push_back({SelectionPrimitive::VERTEX, ACTION});
        if (std::any_cast<bool>(cs.at("selectFaces")))
            modes.push_back({SelectionPrimitive::FACE, ACTION});
        return modes;
    }

    /**
     * @brief Restricts the visible-selection projection to the sub-frustum
     * that corresponds to the given selection box.
     * @return true if the projection was successfully updated, false if the box
     * is invalid (null or degenerate) and the projection was not changed.
     */
    bool setVisibleTrisSelectionProjViewMatrix(const Box2d& box)
    {
        if (box.isNull() || box.isDegenerate()) {
            return false;
        }
        auto    s     = Base::viewerCanvasSize();
        uint    win_w = s.x();
        uint    win_h = s.y();
        Point4f minNDC(
            float(box.min().x()) / float(win_w) * 2.f - 1.f,
            1.f - float(box.max().y()) / float(win_h) * 2.f,
            0.f,
            1.f);
        Point4f maxNDC(
            float(box.max().x()) / float(win_w) * 2.f - 1.f,
            1.f - float(box.min().y()) / float(win_h) * 2.f,
            0.f,
            1.f);
        float     w = maxNDC.x() - minNDC.x();
        float     h = maxNDC.y() - minNDC.y();
        assert(w > 0.f && h > 0.f);
        // set the matrix to translate and scale (scale * translate)
        // translate to the box center and scale to box size (relative to NDC)
        // (translate by -minNDC + 1/2 * dim then scale by
        // 2/dim to map the box to the whole viewport)
        const Matrix44f scaleTranslate {
            {2.f/w, 0.f, 0.f, -(2.f/w * minNDC.x() + 1.f)},
            {0.f, 2.f/h, 0.f, -(2.f/h * minNDC.y() + 1.f)},
            {0.f, 0.f, 1.f, 0.f },
            {0.f, 0.f, 0.f, 1.f }
        };
        auto      vm      = Base::viewerViewMatrix();
        auto      pm      = Base::viewerProjectionMatrix();
        Matrix44f newProj = scaleTranslate * pm;
        bgfx::setViewTransform(
            mVisibleSelectionViewIds[0], vm.data(), newProj.data());
        return true;
    }

    /**
     * @brief Performs GPU selection computation for all active modes.
     *
     * Only runs when `mSelectionCalcRequired` is true (set by mouse events
     * or keyboard shortcuts). After computation the flag is cleared and,
     * if the drag has ended, the selection box is nulled.
     */
    void computeSelections(uint viewId)
    {
        Box2d minMaxBox = mSelectionBox.value_or(Box2d {});

        // If any active mode is a visible-selection mode, set up the
        // restricted projection matrix once for all such modes.
        bool hasVisibleMode = false;
        for (const auto& mode : mCurrentSelectionModes) {
            if (mode.isVisibleSelection()) {
                hasVisibleMode = true;
                break;
            }
        }

        bool skipVisibleSelection = false;
        if (hasVisibleMode && !minMaxBox.isNull())
        {
            skipVisibleSelection = !setVisibleTrisSelectionProjViewMatrix(
                calculateWindowSpaceMeshBB().intersection(minMaxBox));
        }

        auto dl = Base::drawList();

        for (const auto& mode : mCurrentSelectionModes) {
            if (mode.isVisibleSelection() && skipVisibleSelection) {
                // Box is outside the mesh screen-space BB.
                // For REGULAR mode we must clear the current selection;
                // for ADD/SUBTRACT we simply skip (no change).
                if (mode.primitive == SelectionPrimitive::FACE &&
                    mode.action == SelectionAction::REGULAR && mode.visible) {
                    SelectionParameters clearParams = {
                        viewId,
                        mVisibleSelectionViewIds[0],
                        mVisibleSelectionViewIds[1],
                        Box2d(),
                        SelectionMode {
                                       SelectionPrimitive::FACE, SelectionAction::NONE},
                        mSelectionInProgress,
                        bgfx::getTexture(mVisibleSelectionFB, 0),
                        bgfx::getTexture(mVisibleSelectionFB, 1),
                        {mVisibleFaceFBSize,       mVisibleFaceFBSize   },
                        0
                    };
                    for (size_t i = 0; i < dl->size(); i++) {
                        if (!shouldProcessObject(*dl, i)) {
                            continue;
                        }
                        clearParams.meshId = i;
                        auto el            = dl->at(i);
                        if (auto p =
                                dynamic_cast<AbstractDrawableMesh*>(el.get())) {
                            p->computeSelection(clearParams);
                        }
                    }
                }
                continue; // skip this visible-selection mode
            }

            SelectionParameters params = {
                viewId,
                mVisibleSelectionViewIds[0],
                mVisibleSelectionViewIds[1],
                minMaxBox,
                mode,
                mSelectionInProgress,
                bgfx::getTexture(mVisibleSelectionFB, 0),
                bgfx::getTexture(mVisibleSelectionFB, 1),
                {mVisibleFaceFBSize, mVisibleFaceFBSize},
                0
            };
            for (size_t i = 0; i < dl->size(); i++) {
                if (!shouldProcessObject(*dl, i)) {
                    continue;
                }
                params.meshId = i;
                auto el       = dl->at(i);
                if (auto p = dynamic_cast<AbstractDrawableMesh*>(el.get())) {
                    p->computeSelection(params);
                }
            }
        }

        // Computation done — clear box if drag ended
        if (!mSelectionInProgress) {
            mSelectionBox.reset();
            mSelectionAnchor.reset();
        }
    }

    /**
     * @brief Returns the union of the screen-space bounding boxes of all
     * visible meshes that intersect the view frustum.
     */
    Box2d calculateWindowSpaceMeshBB()
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
            if (!shouldProcessObject(*dl, i)) {
                continue;
            }
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

        Box2d box;
        if (totalBB.isNull()) {
            return box;
        }
        auto    s         = Base::viewerCanvasSize();
        uint    width     = s.x();
        uint    height    = s.y();
        Point3d boxPts[2] = {totalBB.min(), totalBB.max()};
        Point2d sSpace[2];
        for (size_t i = 0; i < 2; i++) {
            sSpace[i] = Point2d {
                (boxPts[i].x() + 1) / 2 * double(width),
                (1 - boxPts[i].y()) / 2 * double(height)};
        }
        box.add(sSpace[0]);
        box.add(sSpace[1]);
        return box;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_SELECTION_EDITOR_BGFX_H
