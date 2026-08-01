// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_DRAWABLE_DRAWABLE_AXIS_H
#define VCL_RENDER_DRAWABLE_DRAWABLE_AXIS_H

#include <vclib/render/config.h>

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/drawable/drawable_axis_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// #include <vclib/opengl2/drawable/drawable_axis.h>
#endif

namespace vcl {

/**
 * @brief Type alias for the backend-specific DrawableAxis implementation.
 *
 * Depending on the active rendering backend, this alias resolves to
 * `DrawableAxisBGFX` (for BGFX) or `DrawableAxisOpenGL2` (for OpenGL 2.0).
 * It represents an axis indicator (X=red, Y=green, Z=blue) in the 3D scene.
 */
#ifdef VCLIB_RENDER_BACKEND_BGFX
using DrawableAxis = DrawableAxisBGFX;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
// TODO: implement DrawableAxisOpenGL2
class DrawableAxisOpenGL2
{
    bool mVisible = false;

public:
    DrawableAxisOpenGL2() = default;

    void draw(const auto&) {}

    bool isVisible() const { return mVisible; }

    void setVisibility(bool v) { mVisible = v; }
};

using DrawableAxis = DrawableAxisOpenGL2;
#endif

} // namespace vcl

#endif // VCL_RENDER_DRAWABLE_DRAWABLE_AXIS_H
