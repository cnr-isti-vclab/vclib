// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_EDITORS_BOUNDING_BOX_EDITOR_H
#define VCL_RENDER_EDITORS_BOUNDING_BOX_EDITOR_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/editors/bounding_box_editor_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/space/core.h>

#include "editor.h"
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename ViewerDrawer>
using BoundingBoxEditor = BoundingBoxEditorBGFX<ViewerDrawer>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// TODO: implement BoundingBoxEditorOpenGL2
template<typename ViewerDrawer>
class BoundingBoxEditor : public Editor<ViewerDrawer>
{
    using Base = Editor<ViewerDrawer>;

public:
    BoundingBoxEditor()
    {
        // Initialize settings keys expected.
        Base::settings().customSettings["color"]     = vcl::Color();
        Base::settings().customSettings["thickness"] = 2.0f;
    }

    void draw(uint) override {}
};
#endif

} // namespace vcl

#endif // VCL_RENDER_EDITORS_BOUNDING_BOX_EDITOR_H
