// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_MESH_SELECTOR_EDITOR_SETTINGS_H
#define VCL_RENDER_SETTINGS_MESH_SELECTOR_EDITOR_SETTINGS_H

#include <vclib/render/input/input.h>
#include <vclib/render/input/input_action_map.h>
#include <vclib/render/settings/editor_settings.h>

#include <nlohmann/json.hpp>

namespace vcl {

enum class MeshSelectorAction { SELECT_MESH };

inline std::string toString(MeshSelectorAction action)
{
    switch (action) {
    case MeshSelectorAction::SELECT_MESH: return "Select Mesh";
    default: return "Unknown";
    }
}

inline void fromString(const std::string& str, MeshSelectorAction& out)
{
    if (str == "Select Mesh")
        out = MeshSelectorAction::SELECT_MESH;
    else
        throw std::invalid_argument(
            "Invalid MeshSelectorAction string: " + str);
}

struct MeshSelectorEditorSettings : public EditorSettings
{
    using MouseMap = InputActionMap<MouseInput, MeshSelectorAction>;

    MouseMap mouseBindings = defaultMouseMap();

    void resetDefaults() { mouseBindings.resetToDefaults(); }

    std::vector<std::reference_wrapper<AbstractInputActionMap>> actionMaps()
        override
    {
        return {mouseBindings};
    }

    std::vector<std::reference_wrapper<const AbstractInputActionMap>>
    actionMaps() const override
    {
        return {mouseBindings};
    }

    void loadSettings(const nlohmann::json& j) override
    {
        EditorSettings::loadSettings(j);
        mouseBindings.loadSettings(j);
    }

private:
    static MouseMap defaultMouseMap()
    {
        using enum MouseButton::Enum;
        using enum KeyModifier::Enum;
        MouseMap map("Mesh Selector Mouse Actions");
        map.registerActions({
            {MeshSelectorAction::SELECT_MESH,
             "Select Mesh", {MouseInput {RIGHT, {NO_MODIFIER}, false}}}
        });
        return map;
    }
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_MESH_SELECTOR_EDITOR_SETTINGS_H
