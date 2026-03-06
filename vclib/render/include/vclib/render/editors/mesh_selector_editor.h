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

    void draw(uint viewId) const override {}

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
