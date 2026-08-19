// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_EDITOR_H
#define VCL_RENDER_EDITORS_EDITOR_H

#include <vclib/render/drawable/drawable_object_vector.h>
#include <vclib/render/input.h>
#include <vclib/render/settings/editor_settings.h>
#include <vclib/render/undo_redo/undo_redo_action.h>

#include <vclib/base.h>

#include <nlohmann/json.hpp>

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

    bool mIsActive = false;

    std::function<void()> mOnStateUpdatedCallback;

public:
    using ViewerDrawerType = ViewerDrawer;

    Editor()          = default;
    virtual ~Editor() = default;

    /**
     * @brief Returns the name of the editor, typically used in UI.
     * @return the name of the editor as a string.
     */
    virtual std::string name() const = 0;

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
    virtual void setActive(bool active)
    {
        mIsActive = active;
        notifyStateUpdated();
    }

    /**
     * @brief Sets the callback to be invoked when the editor state changes.
     *
     * This is typically used by UI components to stay synchronized with the
     * internal state of the editor.
     */
    void setOnStateUpdatedCallback(std::function<void()> cb)
    {
        mOnStateUpdatedCallback = std::move(cb);
    }

    /**
     * @brief Returns the editor settings.
     * @return a reference to the EditorSettings object.
     */
    virtual EditorSettings& settings() = 0;

    /**
     * @brief Returns the editor settings.
     * @return a const reference to the EditorSettings object.
     */
    virtual const EditorSettings& settings() const = 0;

    /**
     * @brief Loads the editor settings from a JSON object.
     * @param[in] j: the JSON object containing the settings.
     */
    virtual void loadSettings(const nlohmann::json& j) {}

    /**
     * @brief Saves the editor settings to a JSON object.
     * @param[out] j: the JSON object where the settings will be saved.
     */
    virtual void saveSettings(nlohmann::json& j) const {}

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
     * @brief Called by the viewer immediately after the viewer pointer is set.
     *
     * Subclasses may override this function to perform initialization that
     * requires access to the viewer, such as registering global actions.
     */
    virtual void onViewerSet() {}

    /**
     * @brief Draws the editor content for the given view.
     *
     * This function is called at every frame by the viewer when the editor is
     * active, during the content drawing pass. Subclasses may implement this
     * function to draw their content.
     *
     * @param[in] viewId: the identifier of the view to draw into.
     */
    virtual void drawContent(uint viewId) {}

    /**
     * @brief Draws the editor for the given view.
     *
     * This function is called at every frame by the viewer when the editor is
     * active.
     *
     * @param[in] viewId: the identifier of the view to draw into.
     */
    virtual void draw(uint viewId) {}

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
     * @brief Notifies listeners (e.g. GUI) that the internal state or settings
     * of the editor have changed and need visual synchronization.
     */
    void notifyStateUpdated() const
    {
        if (mOnStateUpdatedCallback)
            mOnStateUpdatedCallback();
    }

    /**
     * @brief Returns the shared drawable object vector of the viewer.
     * @return a shared pointer to the DrawableObjectVector.
     */
    std::shared_ptr<DrawableObjectVector> drawList() const { return mDrawList; }

    /**
     * @brief Returns the canvas size from the viewer.
     * @return the canvas size as a 2D point (width, height).
     */
    auto viewerCanvasSize() const
    {
        assert(mViewer);
        return mViewer->canvasSize();
    }

    /**
     * @brief Returns the DPI scale of the window from the viewer.
     * @return the DPI scale.
     */
    auto viewerDpiScale() const
    {
        assert(mViewer);
        return mViewer->dpiScale();
    }

    /**
     * @brief Returns the canvas frame buffer from the viewer.
     * @return the canvas frame buffer handle.
     */
    auto viewerCanvasFrameBuffer() const
    {
        assert(mViewer);
        return mViewer->canvasFrameBuffer();
    }

    /**
     * @brief Returns the current view matrix from the viewer.
     * @return the view matrix.
     */
    auto viewerViewMatrix() const
    {
        assert(mViewer);
        return mViewer->viewMatrix();
    }

    /**
     * @brief Returns the current projection matrix from the viewer.
     * @return the projection matrix.
     */
    auto viewerProjectionMatrix() const
    {
        assert(mViewer);
        return mViewer->projectionMatrix();
    }

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
     * @brief Requests the viewer to read the Object ID, Element Type, and Element ID
     * of the object at the given screen coordinates.
     *
     * The result is delivered asynchronously via the provided callback.
     *
     * @param[in] x: the x coordinate of the point, in window pixels.
     * @param[in] y: the y coordinate of the point, in window pixels.
     * @param[in] idCallback: callback invoked with the objectId, elementType, and elementId
     */
    void viewerReadElementIdRequest(
        double                                x,
        double                                y,
        std::function<void(ushort, ushort, uint)> idCallback)
    {
        assert(mViewer);
        mViewer->readElementIdRequest(x, y, std::move(idCallback));
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

    /**
     * @brief Requests the viewer to enable/disable continuous redraw.
     */
    void viewerSetContinuousRedraw(bool enabled) const
    {
        assert(mViewer);
        mViewer->setContinuousRedraw(enabled);
    }

    /**
     * @brief Pushes an undo/redo action to the viewer's undo/redo stack.
     * @param[in] action: the action to push.
     */
    void pushUndoRedoAction(std::unique_ptr<UndoRedoAction> action)
    {
        assert(mViewer);
        mViewer->pushUndoRedoAction(std::move(action));
    }

    /**
     * @brief Registers a global action in the viewer.
     *
     * @param[in] name: The unique name of the global action.
     * @param[in] defaultShortcut: The default keyboard shortcut to trigger the
     * action.
     * @param[in] callback: The function to execute when the shortcut is
     * pressed.
     */
    void viewerRegisterGlobalAction(
        const std::string&                 name,
        std::pair<Key::Enum, KeyModifiers> defaultShortcut,
        std::function<void()>              callback)
    {
        assert(mViewer);
        mViewer->registerGlobalAction(
            name, defaultShortcut, std::move(callback));
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
