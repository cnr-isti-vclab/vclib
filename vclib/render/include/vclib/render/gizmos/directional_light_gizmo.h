// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_H
#define VCL_RENDER_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_H

#include <vclib/render/config.h>

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/gizmos/directional_light_gizmo_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/opengl2/gizmos/directional_light_gizmo_opengl2.h>
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
using DirectionalLightGizmo = DirectionalLightGizmoBGFX;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
using DirectionalLightGizmo = DirectionalLightGizmoOpenGL2;
#endif

} // namespace vcl

#endif // VCL_RENDER_GIZMOS_DIRECTIONAL_LIGHT_GIZMO_H
