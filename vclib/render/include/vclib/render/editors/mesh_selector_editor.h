// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_MESH_SELECTOR_EDITOR_H
#define VCL_RENDER_EDITORS_MESH_SELECTOR_EDITOR_H

#include "editor.h"

#include <vclib/render/input.h>

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

template<typename ViewerDrawer>
class MeshSelectorEditor : public Editor<ViewerDrawer>
{
public:
    using MouseMap = InputActionMap<
        std::pair<MouseButton::Enum, KeyModifiers>,
        MeshSelectorAction>;

private:
    using Base = Editor<ViewerDrawer>;

    // a callback function called when an object is selected
    std::function<void(uint)> mOnObjectSelectedFunction = nullptr;

    EditorSettings mSettings;

    MouseMap mMouseBindings = defaultMouseMap();

    static MouseMap defaultMouseMap()
    {
        using enum MouseButton::Enum;
        using enum KeyModifier::Enum;
        using Input = std::pair<MouseButton::Enum, KeyModifiers>;

        MouseMap map("Mesh Selector Mouse Actions");
        map.registerActions({
            {MeshSelectorAction::SELECT_MESH, "Select Mesh", Input{RIGHT, {NO_MODIFIER}}}
        });
        return map;
    }

public:
    void setOnObjectSelectedFunction(const std::function<void(uint)>& f)
    {
        mOnObjectSelectedFunction = f;
    }

    MouseMap& mouseBindings() { return mMouseBindings; }

    const MouseMap& mouseBindings() const { return mMouseBindings; }

    // Editor implementation

    void onViewerSet() override
    {
        Base::viewerRegisterGlobalAction(
            "Toggle Mesh Selector",
            {Key::M, {KeyModifier::NO_MODIFIER}},
            [this]() {
                Base::setActive(!Base::isActive());
                Base::viewerUpdate();
            });
    }

    std::string name() const override { return "Mesh Selector"; }

    EditorSettings& settings() override { return mSettings; }

    const EditorSettings& settings() const override { return mSettings; }

    void draw(uint viewId) override {}

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        bool block = Base::onMousePress(button, x, y, modifiers);
        if (block)
            return true;

        auto action = mMouseBindings.action({button, modifiers});
        if (action.has_value() &&
            action.value() == MeshSelectorAction::SELECT_MESH) {
            auto callback = [&](uint id) {
                if (id == vcl::UINT_NULL)
                    return;

                if (mOnObjectSelectedFunction)
                    mOnObjectSelectedFunction(id);
                else
                    Base::drawList()->setSelectedObjectId(id);
            };

            Base::viewerReadIdRequest(x, y, callback);
            return true; // Smart blocking: consumed event
        }

        return false;
    }
};

} // namespace vcl

#endif // VCL_RENDER_EDITORS_MESH_SELECTOR_EDITOR_H
