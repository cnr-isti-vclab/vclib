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

#ifndef VCL_RENDER_EDITORS_SELECTION_EDITOR_H
#define VCL_RENDER_EDITORS_SELECTION_EDITOR_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/editors/selection_editor_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/space/core.h>

#include "editor.h"
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename ViewerDrawer>
using SelectionEditor = SelectionEditorBGFX<ViewerDrawer>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// No implementation of SelectionEditorOpenGL2
template<typename ViewerDrawer>
class SelectionEditor : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;
public:
    SelectionEditor()
    {
        Base::settings().customSettings["selectVertices"] = false;
        Base::settings().customSettings["selectFaces"] = false;
        Base::settings().customSettings["onlyVisible"] = false;
        Base::settings().customSettings["selectionBoxColor"] =
            vcl::Color(27, 120, 249, 64);
    }

    void draw(uint) override {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_SELECTION_EDITOR_H
