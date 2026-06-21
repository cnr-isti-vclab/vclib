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

#ifndef VCL_BGFX_EDITORS_AXIS_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_AXIS_EDITOR_BGFX_H

#include <vclib/bgfx/drawable/drawable_axis.h>
#include <vclib/render/editors/editor.h>

namespace vcl {

/**
 * @brief An editor that renders a 3D coordinate axis indicator in the viewer.
 *
 * AxisEditorBGFX is a specialized Editor subclass that draws an oriented
 * reference axis (X=red, Y=green, Z=blue) as an overlay in the BGFX-based
 * 3D viewer. It inherits from Editor<ViewerDrawer> and integrates into the
 * viewer's editornetwork so that it receives input events and is drawn each
 * frame when active.
 *
 * The axis drawable (DrawableAxis) is initially invisible by default. Its
 * visibility can be toggled through the built-in keyboard shortcut or via
 * the public API. A custom callback can be supplied to replace the default
 * toggle behaviour for the keyboard shortcut.
 *
 * Keyboard shortcuts:
 *   - <b>A</b> (no modifiers): toggles axis visibility (customizable).
 */
template<typename ViewerDrawer>
class AxisEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

    DrawableAxis mAxis;

    std::function<void(void)> mCustomShortcutCallback = [this]() {
        toggleVisibility();
    };

public:
    /**
     * @brief Constructs an AxisEditorBGFX instance.
     *
     * The axis is invisible by default (delegated to DrawableAxis).
     */
    AxisEditorBGFX() {};

    /**
     * @brief Sets the editor active state.
     *
     * When the editor becomes active or inactive, this method notifies the
     * parent viewer so that the render pipeline can react accordingly.
     *
     * @param[in] active: the new active state.
     */
    void setActive(bool active) override
    {
        Base::setActive(active);
        Base::viewerUpdate();
    }

    /**
     * @brief Draws the axis indicator for the given view.
     *
     * This method is called every frame while the editor is active. It
     * forwards the draw call to the internal DrawableAxis with a minimal
     * DrawObjectSettings structure carrying only the view ID.
     *
     * @param[in] viewId: the BGFX view ID for this draw pass.
     */
    void draw(uint viewId) override
    {
        DrawObjectSettings settings;
        settings.viewId = viewId;

        mAxis.draw(settings);
    }

    /**
     * @brief Handles keyboard key-press events.
     *
     * Responds to the 'A' key with no modifiers by invoking the custom
     * shortcut callback (which toggles visibility by default). All other
     * keys are ignored.
     *
     * @param[in] key:      the key that was pressed.
     * @param[in] modifiers: the modifier keys held at the time of the event.
     * @return true if the event was handled; false otherwise.
     */
    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        switch (key) {
        case Key::A:
            if (modifiers[KeyModifier::NO_MODIFIER])
                mCustomShortcutCallback();
            break;
        default: break;
        }
        return false;
    }

    /**
     * @brief Sets a custom callback for the keyboard shortcut.
     *
     * By default, pressing 'A' toggles the axis visibility. Call this
     * method to replace the default behaviour with custom logic.
     *
     * @param[in] callback: a callable invoked when the shortcut key is pressed.
     */
    void setShortcutCallback(std::function<void(void)> callback)
    {
        mCustomShortcutCallback = callback;
    }

    /**
     * @brief Returns whether the axis is currently visible.
     *
     * @return true if the axis drawable is set to visible, false otherwise.
     */
    bool isVisible() const { return mAxis.isVisible(); }

    /**
     * @brief Toggles the visibility of the axis indicator.
     *
     * Switches the internal DrawableAxis between visible and hidden states,
     * then notifies the parent viewer to trigger a redraw.
     */
    void toggleVisibility()
    {
        mAxis.setVisibility(!mAxis.isVisible());
        Base::viewerUpdate();
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_AXIS_EDITOR_BGFX_H
