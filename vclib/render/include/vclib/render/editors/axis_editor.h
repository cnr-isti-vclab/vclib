// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_AXIS_EDITOR_H
#define VCL_RENDER_EDITORS_AXIS_EDITOR_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/editors/axis_editor_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/space/core.h>

#include "editor.h"
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename ViewerDrawer>
using AxisEditor = AxisEditorBGFX<ViewerDrawer>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// TODO: implement AxisEditorOpenGL2
template<typename ViewerDrawer>
class AxisEditor : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

public:
    AxisEditor()
    {
        // Initialize settings keys expected.
    }

    void draw(uint) override {}

    bool isVisible() const { return false; }

    void toggleVisibility() {}

    void setShortcutCallback(std::function<void(void)>) {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_AXIS_EDITOR_H
