// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H

#include <vclib/render/input/input.h>
#include <vclib/render/input/input_action_map.h>
#include <vclib/render/selection/selection_mode.h>
#include <vclib/render/settings/editor_settings.h>

#include <vclib/space/core.h>

#include <nlohmann/json.hpp>

namespace vcl {

struct SelectionEditorSettings : public EditorSettings
{
    using KeyMap = InputActionMap<
        std::pair<Key::Enum, KeyModifiers>,
        SelectionAtomicAction>;
    using MouseMap = InputActionMap<MouseInput, SelectionDragAction>;

    bool       selectVertices    = false;
    bool       selectFaces       = false;
    bool       onlyVisible       = false;
    vcl::Color selectionBoxColor = vcl::Color(27, 120, 249, 64);

    KeyMap   keyBindings   = defaultKeyMap();
    MouseMap mouseBindings = defaultMouseMap();

    /**
     * @brief Resets the settings to their default values.
     */
    void resetDefaults()
    {
        onlyVisible       = false;
        selectionBoxColor = vcl::Color(27, 120, 249, 64);
        editMode          = EditMode::CURRENT_OBJECT;
        keyBindings.resetToDefaults();
        mouseBindings.resetToDefaults();
    }

    std::vector<std::reference_wrapper<AbstractInputActionMap>> actionMaps()
        override
    {
        return {keyBindings, mouseBindings};
    }

    /**
     * @brief Loads the settings from a JSON object.
     * @param[in] j: the JSON object to read from.
     */
    void loadSettings(const nlohmann::json& j)
    {
        if (j.contains("SelectionEditor")) {
            const auto& jSel = j["SelectionEditor"];
            onlyVisible      = jSel.value("onlyVisible", onlyVisible);
            selectionBoxColor =
                jSel.value("selectionBoxColor", selectionBoxColor);
            editMode = static_cast<EditMode>(
                jSel.value("editMode", static_cast<int>(editMode)));
        }
    }

    /**
     * @brief Saves the settings to a JSON object.
     * @param[out] j: the JSON object to write to.
     */
    void saveSettings(nlohmann::json& j) const
    {
        j["SelectionEditor"]["onlyVisible"]       = onlyVisible;
        j["SelectionEditor"]["selectionBoxColor"] = selectionBoxColor;
        j["SelectionEditor"]["editMode"]          = static_cast<int>(editMode);
    }

private:
    static KeyMap defaultKeyMap()
    {
        using enum Key::Enum;
        using enum KeyModifier::Enum;
        using Input = std::pair<Key::Enum, KeyModifiers>;

        KeyMap map("Selection Atomic Actions");
        map.registerActions({
            {SelectionAtomicAction::ALL,    "Select All",   Input {A, {CONTROL}}},
            {SelectionAtomicAction::NONE,   "Deselect All", Input {D, {CONTROL}}},
            {SelectionAtomicAction::INVERT,
             "Invert Selection",                            Input {I, {CONTROL}}}
        });
        return map;
    }

    static MouseMap defaultMouseMap()
    {
        using enum MouseButton::Enum;
        using enum KeyModifier::Enum;

        MouseMap map("Selection Drag Actions");
        map.registerActions({
            {SelectionDragAction::REGULAR,
             "Regular Selection",       MouseInput {LEFT, {NO_MODIFIER}, false}   },
            {SelectionDragAction::ADD,
             "Add to Selection",        MouseInput {LEFT, {CONTROL}, false}       },
            {SelectionDragAction::SUBTRACT,
             "Subtract from Selection", MouseInput {LEFT, {CONTROL, SHIFT}, false}}
        });
        return map;
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_SELECTION_EDITOR_SETTINGS_H
