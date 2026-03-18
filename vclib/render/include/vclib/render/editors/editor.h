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

#ifndef VCL_RENDER_EDITORS_EDITOR_H
#define VCL_RENDER_EDITORS_EDITOR_H

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/input.h>
#include <vclib/render/settings/editor_settings.h>

#include <vclib/base.h>

namespace vcl {

/**
 * @brief The Editor abstract class is a base class for all the editors of the
 * VCLib Render module.
 *
 * An editor is an object contained in an AbstractViewerDrawer that can "edit"
 * the behavior of the viewer itself. For example, it can be a gizmo that allows
 * to edit the position of a drawable object, or it can be a widget that allows
 * draw additional elements on top of the viewer (e.g. a grid, or a bounding
 * box).
 */
template<typename ViewerDrawer>
class Editor
{
    friend ViewerDrawer;

    ViewerDrawer* mViewer = nullptr;

    std::shared_ptr<DrawableObjectVector> mDrawList;

    EditorSettings mSettings;

    bool mIsActive = false;

public:
    using ViewerDrawerType = ViewerDrawer;

    Editor() = default;

    /**
     * @brief Returns whether the editor is currently active.
     *
     * Only active editors receive input events and are drawn by the viewer.
     *
     * @return true if the editor is active, false otherwise.
     */
    bool isActive() const { return mIsActive; }

    /**
     * @brief Sets the editor active state.
     *
     * When active, the editor receives input events and its `draw()` function
     * is called at every frame.
     *
     * @param[in] active: the new active state.
     */
    virtual void setActive(bool active) { mIsActive = active; }

    /**
     * @brief Returns the editor settings.
     * @return a reference to the EditorSettings object.
     */
    EditorSettings& settings() { return mSettings; }

    /**
     * @brief Returns the editor settings.
     * @return a const reference to the EditorSettings object.
     */
    const EditorSettings& settings() const { return mSettings; }

    /**
     * @brief Called by the viewer when the drawable object vector changes.
     *
     * Subclasses may override this function to update their internal state
     * when the set of drawable objects changes (e.g. when a new object is
     * pushed into the viewer).
     */
    virtual void refresh() {}

    /**
     * @brief Called by the viewer when the editor settings change.
     *
     * Subclasses may override this function to react to settings updates.
     */
    virtual void refreshSettings() {}

    /**
     * @brief Draws the editor content for the given view.
     *
     * This function is called at every frame by the viewer when the editor is
     * active. Subclasses must implement this function to draw their content.
     *
     * @param[in] viewId: the identifier of the view to draw into.
     */
    virtual void draw(uint viewId) const = 0;

    /**
     * @brief Called when a keyboard key is pressed.
     *
     * Subclasses may override this function to handle key press events.
     * If the event is consumed by the editor, the function should return
     * `true` to prevent further propagation to other editors or the viewer.
     *
     * @param[in] key: the key that was pressed.
     * @param[in] modifiers: the currently active key modifiers.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers)
    {
        return false;
    }

    /**
     * @brief Called when a keyboard key is released.
     *
     * Subclasses may override this function to handle key release events.
     * If the event is consumed by the editor, the function should return
     * `true` to prevent further propagation to other editors or the viewer.
     *
     * @param[in] key: the key that was released.
     * @param[in] modifiers: the currently active key modifiers.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers)
    {
        return false;
    }

    /**
     * @brief Called when the mouse cursor is moved.
     *
     * Subclasses may override this function to handle mouse move events.
     * If the event is consumed by the editor, the function should return
     * `true` to prevent further propagation to other editors or the viewer.
     *
     * @param[in] x: the x coordinate of the cursor, in window pixels.
     * @param[in] y: the y coordinate of the cursor, in window pixels.
     * @param[in] modifiers: the currently active key modifiers.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool onMouseMove(double x, double y, const KeyModifiers& modifiers)
    {
        return false;
    }

    /**
     * @brief Called when a mouse button is pressed.
     *
     * Subclasses may override this function to handle mouse press events.
     * If the event is consumed by the editor, the function should return
     * `true` to prevent further propagation to other editors or the viewer.
     *
     * @param[in] button: the mouse button that was pressed.
     * @param[in] x: the x coordinate of the cursor, in window pixels.
     * @param[in] y: the y coordinate of the cursor, in window pixels.
     * @param[in] modifiers: the currently active key modifiers.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers)
    {
        return false;
    }

    /**
     * @brief Called when a mouse button is released.
     *
     * Subclasses may override this function to handle mouse release events.
     * If the event is consumed by the editor, the function should return
     * `true` to prevent further propagation to other editors or the viewer.
     *
     * @param[in] button: the mouse button that was released.
     * @param[in] x: the x coordinate of the cursor, in window pixels.
     * @param[in] y: the y coordinate of the cursor, in window pixels.
     * @param[in] modifiers: the currently active key modifiers.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

    /**
     * @brief Called when a mouse button is double-clicked.
     *
     * Subclasses may override this function to handle mouse double-click
     * events. If the event is consumed by the editor, the function should
     * return `true` to prevent further propagation to other editors or the
     * viewer.
     *
     * @param[in] button: the mouse button that was double-clicked.
     * @param[in] x: the x coordinate of the cursor, in window pixels.
     * @param[in] y: the y coordinate of the cursor, in window pixels.
     * @param[in] modifiers: the currently active key modifiers.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool onMouseDoubleClick(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

    /**
     * @brief Called when the mouse wheel is scrolled.
     *
     * Subclasses may override this function to handle mouse scroll events.
     * If the event is consumed by the editor, the function should return
     * `true` to prevent further propagation to other editors or the viewer.
     *
     * @param[in] x: the horizontal scroll delta.
     * @param[in] y: the vertical scroll delta.
     * @param[in] modifiers: the currently active key modifiers.
     * @return true if the event is consumed, false otherwise.
     */
    virtual bool onMouseScroll(
        double              x,
        double              y,
        const KeyModifiers& modifiers)
    {
        return false;
    }

protected:
    /**
     * @brief Returns the shared drawable object vector of the viewer.
     * @return a shared pointer to the DrawableObjectVector.
     */
    std::shared_ptr<DrawableObjectVector> drawList() const { return mDrawList; }

    /**
     * @brief Requests the viewer to read the ID of the object at the given
     * screen coordinates.
     *
     * The result is delivered asynchronously via the provided callback.
     *
     * @param[in] x: the x coordinate of the point, in window pixels.
     * @param[in] y: the y coordinate of the point, in window pixels.
     * @param[in] idCallback: callback invoked with the ID of the object at the
     * given coordinates once the read is complete.
     */
    void viewerReadIdRequest(
        double                    x,
        double                    y,
        std::function<void(uint)> idCallback)
    {
        assert(mViewer);
        mViewer->readIdRequest(x, y, std::move(idCallback));
    }

    /**
     * @brief Requests the viewer to redraw the frame.
     *
     * Editors should call this function after modifying any state that affects
     * the visual output, so that the viewer schedules a new frame.
     */
    void viewerUpdate() const
    {
        assert(mViewer);
        mViewer->requestUpdate();
    }

private:
    // functions called by the (friend) viewer to set up the Editor
    void setViewer(ViewerDrawer* viewer) { mViewer = viewer; }

    void setDrawableObjectVector(const std::shared_ptr<DrawableObjectVector>& v)
    {
        mDrawList = v;
        refresh();
    }
};

} // namespace vcl

#endif // VCL_RENDER_EDITORS_EDITOR_H
