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

    void draw(uint) const override {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_AXIS_EDITOR_H
