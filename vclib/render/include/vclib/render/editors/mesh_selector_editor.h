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
    using Base = Editor<ViewerDrawer>;

    // a callback function called when an object is selected
    std::function<void(uint)> mOnObjectSelectedFunction = [](uint) {
    };

public:
    void setOnObjectSelectedFunction(const std::function<void(uint)>& f)
    {
        mOnObjectSelectedFunction = f;
    }

    // Editor implementation

    void draw(uint viewId) override {}

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        bool block = Base::onMousePress(button, x, y, modifiers);

        if (!block && button == vcl::MouseButton::RIGHT) {
            auto callback = [&](uint id) {
                if (id == vcl::UINT_NULL)
                    return;

                Base::drawList()->setSelectedObjectId(id);

                if (mOnObjectSelectedFunction)
                    mOnObjectSelectedFunction(id);
            };

            Base::viewerReadIdRequest(x, y, callback);
        }
        return block;
    }
};

} // namespace vcl

#endif // VCL_RENDER_EDITORS_MESH_SELECTOR_EDITOR_H
