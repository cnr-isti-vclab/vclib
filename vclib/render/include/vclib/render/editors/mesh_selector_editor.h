// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_MESH_SELECTOR_EDITOR_H
#define VCL_RENDER_EDITORS_MESH_SELECTOR_EDITOR_H

#include "editor.h"

namespace vcl {

template<typename ViewerDrawer>
class MeshSelectorEditor : public Editor<ViewerDrawer>
{
public:
    enum class MeshSelectorAction { SELECT_MESH };
    using MouseMap = BindingMap<
        std::pair<MouseButton::Enum, KeyModifiers>,
        MeshSelectorAction>;

private:
    using Base = Editor<ViewerDrawer>;

    // a callback function called when an object is selected
    std::function<void(uint)> mOnObjectSelectedFunction = nullptr;

    EditorSettings mSettings;

    MouseMap mMouseBindings = {
        {{MouseButton::RIGHT, {KeyModifier::NO_MODIFIER}},
         MeshSelectorAction::SELECT_MESH}
    };

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
