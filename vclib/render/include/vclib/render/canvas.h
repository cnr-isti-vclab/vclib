// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_CANVAS_H
#define VCL_RENDER_CANVAS_H

#include "config.h"

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/canvas.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/opengl2/canvas.h>
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
template<typename DerivedRenderApp>
using Canvas = CanvasBGFX<DerivedRenderApp>;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
template<typename DerivedRenderApp>
using Canvas = CanvasOpenGL2<DerivedRenderApp>;
#endif

} // namespace vcl

#endif // VCL_RENDER_CANVAS_H
