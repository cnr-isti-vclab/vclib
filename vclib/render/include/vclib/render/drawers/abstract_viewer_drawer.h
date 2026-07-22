// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H
#define VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H

#include "trackball_event_drawer.h"

#include <vclib/render/drawable/drawable_axis.h>
#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/drawers/event_drawer.h>
#include <vclib/render/editors.h>
#include <vclib/render/read_buffer_types.h>
#include <vclib/render/settings/viewer_settings.h>
#include <vclib/space/core/color.h>

#include <memory>

namespace vcl {

/**
 * @brief The AbstractViewerDrawer class is a base class for all viewer drawers
 * implementations.
 *
 * It provides all the common functionalities of a viewer, but not the core
 * rendering functionalities. It is meant to be subclassed by a concrete viewer
 * drawer implementation.
 */
template<typename DerivedRenderApp>
class AbstractViewerDrawer : public TrackBallEventDrawer<DerivedRenderApp>
{
    using Base = TrackBallEventDrawer<DerivedRenderApp>;
    using DRA  = DerivedRenderApp;

    bool mReadRequested = false;

    // the default id for the viewer drawer is 0
    uint mId = 0;

    DrawableAxis mDrawAxis;

    std::function<void(void)> mCustomShortcutToggleAxisCallback = [this]() {
        toggleAxisVisibility();
    };

protected:
    ViewerSettings mViewerSettings;

    // the list of drawable objects
    // it could be owned by the viewer, or it could be shared with other
    // objects (e.g. the window that contains the viewer along with other
    // widgets)
    std::shared_ptr<DrawableObjectVector> mDrawList =
        std::make_shared<DrawableObjectVector>();

    std::vector<std::shared_ptr<Editor<AbstractViewerDrawer>>> mEditors;

    // the drawer id
    uint& id() { return mId; }

public:
    using EditorType = Editor<AbstractViewerDrawer>;
    using ViewerType = AbstractViewerDrawer;

    AbstractViewerDrawer(uint width = 1024, uint height = 768) :
            Base(width, height)
    {
    }

    ~AbstractViewerDrawer() = default;

    const DrawableObjectVector& drawableObjectVector() const
    {
        return *mDrawList;
    }

    DrawableObjectVector& drawableObjectVector() { return *mDrawList; }

    void setDrawableObjectVector(const std::shared_ptr<DrawableObjectVector>& v)
    {
        mDrawList = v;

        for (auto obj : *mDrawList) {
            obj->init();
        }

        for (std::shared_ptr<EditorType>& editor : mEditors) {
            if (editor) {
                editor->setDrawableObjectVector(mDrawList);
            }
        }

        fitScene();
    }

    const ViewerSettings& viewerSettings() const { return mViewerSettings; }

    void setViewerSettings(const ViewerSettings& settings)
    {
        mViewerSettings = settings;
    }

    // Default ViewerConcept placeholders. Can be shadowed by derived classes.

    std::string panoramaFileName() const { return ""; }

    void setPanorama(const std::string&) {}

    /**
     * @brief Pushes a new editor of the specified type into the viewer's editor
     * list.
     *
     * The editor is instantiated and initialized with the viewer and drawable
     * list.
     *
     * @tparam ET The template type of the Editor to push.
     * @param[in] active Whether the editor should be active upon creation.
     * @return A shared pointer to the newly created editor.
     */
    template<template<typename> typename ET>
    auto pushEditor(bool active = false)
    {
        auto editor = std::make_shared<ET<ViewerType>>();
        mEditors.push_back(editor);
        editor->setViewer(this);
        editor->setDrawableObjectVector(mDrawList);
        editor->setActive(active);
        return editor;
    }

    void refreshEditors()
    {
        for (std::shared_ptr<EditorType>& editor : mEditors) {
            editor->refresh();
        }
    }

    /**
     * @brief Helper function to add a DrawableObject to the scene.
     *
     * In addition to pushing the object to the underlying vector, this helper
     * safely calls `init()` on the newly added object (required to initialize
     * OpenGL/BGFX buffers) and calls `refreshEditors()` to update any GUI
     * components.
     *
     * If you choose to manually manipulate the vector via
     * `drawableObjectVector()`, you are responsible for calling `init()` on new
     * elements and `refreshEditors()`.
     */
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject> &&
             (!std::is_abstract_v<std::remove_cvref_t<U>>)
    uint pushDrawableObject(U&& obj)
    {
        mDrawList->pushBack(std::forward<U>(obj));
        mDrawList->back()->init();
        refreshEditors();
        return mDrawList->size() - 1;
    }

    /**
     * @brief Helper function to add a shared_ptr of DrawableObject to the
     * scene.
     *
     * In addition to pushing the object to the underlying vector, this helper
     * safely calls `init()` on the newly added object (required to initialize
     * OpenGL/BGFX buffers) and calls `refreshEditors()` to update any GUI
     * components.
     */
    uint pushDrawableObject(std::shared_ptr<DrawableObject> obj)
    {
        mDrawList->pushBack(std::move(obj));
        mDrawList->back()->init();
        refreshEditors();
        return mDrawList->size() - 1;
    }

    bool removeDrawableObject(uint id)
    {
        if (id >= mDrawList->size())
            return false;
        mDrawList->erase(id);
        refreshEditors();
        requestUpdate();
        return true;
    }

    /**
     * @brief Helper function to insert a DrawableObject at a specific position.
     *
     * Safely calls `init()` on the newly added object and calls
     * `refreshEditors()`.
     */
    template<typename U>
    requires std::derived_from<std::remove_cvref_t<U>, DrawableObject> &&
             (!std::is_abstract_v<std::remove_cvref_t<U>>)
    bool insertDrawableObject(uint pos, U&& obj)
    {
        if (pos > mDrawList->size())
            return false;
        mDrawList->insert(pos, std::forward<U>(obj));
        mDrawList->at(pos)->init();
        refreshEditors();
        return true;
    }

    /**
     * @brief Helper function to insert a shared_ptr of DrawableObject at a
     * specific position.
     *
     * Safely calls `init()` on the newly added object and calls
     * `refreshEditors()`.
     */
    bool insertDrawableObject(uint pos, std::shared_ptr<DrawableObject> obj)
    {
        if (pos > mDrawList->size())
            return false;
        mDrawList->insert(pos, std::move(obj));
        mDrawList->at(pos)->init();
        refreshEditors();
        return true;
    }

    /**
     * @brief Helper function to clear all objects from the scene.
     *
     * Clears the underlying vector and safely calls `refreshEditors()`.
     */
    void clearDrawableObjects()
    {
        mDrawList->clear();
        refreshEditors();
        requestUpdate();
    }

    /**
     * @brief Checks if the axis indicator is currently visible.
     *
     * @return true if the axis indicator is visible, false otherwise.
     */
    bool isAxisVisible() const { return mDrawAxis.isVisible(); }

    /**
     * @brief Toggles the visibility of the axis indicator.
     *
     * Flips the visibility state of the axis and requests a viewer update.
     */
    void toggleAxisVisibility()
    {
        mDrawAxis.setVisibility(!mDrawAxis.isVisible());
        requestUpdate();
    }

    /**
     * @brief Sets a custom callback for the axis visibility shortcut.
     *
     * By default, pressing 'A' toggles the axis visibility. This method allows
     * replacing the default behaviour with custom logic (e.g. triggering UI
     * buttons).
     *
     * @param[in] callback A callable invoked when the shortcut key is pressed.
     */
    void setShortcutToggleAxisCallback(std::function<void(void)> callback)
    {
        mCustomShortcutToggleAxisCallback = callback;
    }

    void fitScene()
    {
        Point3f sceneCenter;
        float   sceneRadius = 1;

        Box3d bb = mDrawList->boundingBox();

        if (!bb.isNull()) {
            sceneCenter = bb.center().cast<float>();
            sceneRadius = bb.diagonal();
        }

        Base::fitScene(sceneCenter, sceneRadius);
    }

    void fitView()
    {
        Point3f sceneCenter = mDrawList->center().cast<float>();

        Base::fitView(sceneCenter);
    }

    // events
    void onInit(uint) override
    {
        DRA::DRW::setCanvasDefaultClearColor(derived(), Color::DarkGray);
        mDrawList->init();
    }

    void onDraw(uint viewId) override
    {
        Base::onDraw(viewId);

        if (mDrawAxis.isVisible()) {
            DrawObjectSettings settings;
#ifdef VCLIB_RENDER_BACKEND_BGFX
            settings.viewId = viewId;
#endif // VCLIB_RENDER_BACKEND_BGFX
            mDrawAxis.draw(settings);
        }

        for (const auto& editor : mEditors) {
            if (editor->isActive())
                editor->draw(viewId);
        }
    }

    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        bool block = false;

        for (const auto& editor : mEditors) {
            if (!block && editor->isActive())
                block = editor->onKeyPress(key, modifiers);
        }

        if (!block)
            block = Base::onKeyPress(key, modifiers);

        if (!block) {
            switch (key) {
            case Key::R:
                if (modifiers[KeyModifier::NO_MODIFIER])
                    fitScene();
                break;
            case Key::S:
                if (modifiers[KeyModifier::CONTROL])
                    DRA::DRW::screenshot(derived(), "viewer_screenshot.png");
                break;
            case Key::A:
                if (modifiers[KeyModifier::NO_MODIFIER]) {
                    if (mCustomShortcutToggleAxisCallback)
                        mCustomShortcutToggleAxisCallback();
                }
                break;
            default: break;
            }
        }
        return block;
    }

    bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) override
    {
        bool block = false;

        for (const auto& editor : mEditors) {
            if (!block && editor->isActive())
                block = editor->onKeyRelease(key, modifiers);
        }

        if (!block)
            block = Base::onKeyRelease(key, modifiers);

        return block;
    }

    bool onMouseMove(double x, double y, const KeyModifiers& modifiers) override
    {
        bool block = false;

        for (const auto& editor : mEditors) {
            if (!block && editor->isActive())
                block = editor->onMouseMove(x, y, modifiers);
        }

        if (!block)
            block = Base::onMouseMove(x, y, modifiers);

        return block;
    }

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        bool block = false;

        for (const auto& editor : mEditors) {
            if (!block && editor->isActive())
                block = editor->onMousePress(button, x, y, modifiers);
        }

        if (!block)
            block = Base::onMousePress(button, x, y, modifiers);

        return block;
    }

    bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        bool block = false;

        for (const auto& editor : mEditors) {
            if (!block && editor->isActive())
                block = editor->onMouseRelease(button, x, y, modifiers);
        }

        if (!block)
            block = Base::onMouseRelease(button, x, y, modifiers);

        return block;
    }

    bool onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        bool block = false;

        for (const auto& editor : mEditors) {
            if (!block && editor->isActive())
                block = editor->onMouseDoubleClick(button, x, y, modifiers);
        }

        if (!block)
            block = Base::onMouseDoubleClick(button, x, y, modifiers);

        return block;
    }

    bool onMouseScroll(double x, double y, const KeyModifiers& modifiers)
        override
    {
        bool block = false;

        for (const auto& editor : mEditors) {
            if (!block && editor->isActive())
                block = editor->onMouseScroll(x, y, modifiers);
        }

        if (!block)
            block = Base::onMouseScroll(x, y, modifiers);

        return block;
    }

    uint canvasViewId() const { return DRA::DRW::canvasViewId(derived()); }

    auto canvasSize() const { return DRA::DRW::canvasSize(derived()); }

    void readDepthRequest(double x, double y, bool homogeneousNDC = true)
    {
        using ReadData   = ReadBufferTypes::ReadData;
        using FloatData  = ReadBufferTypes::FloatData;
        using MatrixType = Base::MatrixType;

        if (mReadRequested)
            return;

        // get point
        const Point2d p(x, y);

        // create the callback
        const auto proj = Base::projectionMatrix();
        const auto view = Base::viewMatrix();
        // viewport
        auto size = DRA::DRW::canvasSize(derived());

        const Point4f vp = {.0f, .0f, float(size.x()), float(size.y())};

        auto callback = [=, this](const ReadData& dt) {
            const auto& data = std::get<FloatData>(dt);
            assert(data.size() == 1);
            const float depth = data[0];

            mReadRequested = false;

            // if the depth is 1.0, the point is not in the scene
            if (depth == 1.0f) {
                return;
            }

            // unproject the point
            const Point3f p2d(p.x(), vp[3] - p.y(), depth);
            auto          unproj = unprojectScreenPosition(
                p2d, MatrixType(proj * view), vp, homogeneousNDC);

            this->focus(unproj);
            derived()->update();
        };

        mReadRequested =
            DRA::DRW::readDepth(derived(), Point2i(p.x(), p.y()), callback);
        if (mReadRequested)
            derived()->update();
    }

    void readIdRequest(double x, double y, std::function<void(uint)> idCallback)
    {
        using ReadData = ReadBufferTypes::ReadData;

        if (mReadRequested)
            return;

        // get point
        const Point2d p(x, y);

        // create the callback
        auto callback = [=, this](const ReadData& dt) {
            const auto& data = std::get<ReadBufferTypes::ByteData>(dt);
            assert(data.size() == 4);
            // TODO: check how to do this properly
            const uint id = *(uint32_t*) &data[0];

            mReadRequested = false;

            idCallback(id);
            derived()->update();
        };

        mReadRequested =
            DRA::DRW::readId(derived(), Point2i(p.x(), p.y()), callback);
        if (mReadRequested)
            derived()->update();
    }

    void requestUpdate() { derived()->update(); }

    void setContinuousRedraw(bool enabled)
    {
        DRA::DRW::setContinuousRedraw(derived(), enabled);
    }

private:
    auto* derived() { return static_cast<DRA*>(this); }

    const auto* derived() const { return static_cast<const DRA*>(this); }
};

} // namespace vcl

#endif // VCL_RENDER_DRAWERS_ABSTRACT_VIEWER_DRAWER_H
