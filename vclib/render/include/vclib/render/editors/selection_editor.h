// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
        Base::settings().customSettings["selectFaces"]    = false;
        Base::settings().customSettings["onlyVisible"]    = false;
        Base::settings().customSettings["selectionBoxColor"] =
            vcl::Color(27, 120, 249, 64);
    }

    void draw(uint) override {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_SELECTION_EDITOR_H
