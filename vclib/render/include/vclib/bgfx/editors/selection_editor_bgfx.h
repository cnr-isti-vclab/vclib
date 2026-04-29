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

#ifndef VCL_BGFX_EDITORS_SELECTION_EDITOR_BGFX_H
#define VCL_BGFX_EDITORS_SELECTION_EDITOR_BGFX_H

#include <vclib/render/editors/editor.h>

namespace vcl {

template<typename ViewerDrawer>
class SelectionEditorBGFX : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

public:
    SelectionEditorBGFX()
    {
        Base::settings().customSettings["selectVertices"] = false;
        Base::settings().customSettings["selectFaces"] = false;
        Base::settings().customSettings["onlyVisible"] = false;
        // init other settings here...
    }

    void draw(uint viewId) const override
    {
        // ...
    }

    bool onKeyPress(Key::Enum key, const KeyModifiers& modifiers) override
    {
        return false;
    }

    bool onKeyRelease(Key::Enum key, const KeyModifiers& modifiers) override
    {
        return false;
    }

    bool onMouseMove(double x, double y, const KeyModifiers& modifiers) override
    {
        return false;
    }

    bool onMousePress(
        vcl::MouseButton::Enum   button,
        double                   x,
        double                   y,
        const vcl::KeyModifiers& modifiers) override
    {
        return false;
    }

    bool onMouseRelease(
        MouseButton::Enum   button,
        double              x,
        double              y,
        const KeyModifiers& modifiers) override
    {
        return false;
    }
};

} // namespace vcl

#endif // VCL_BGFX_EDITORS_SELECTION_EDITOR_BGFX_H
