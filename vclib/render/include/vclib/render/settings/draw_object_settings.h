// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_DRAW_OBJECT_SETTINGS_H
#define VCL_RENDER_SETTINGS_DRAW_OBJECT_SETTINGS_H

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/settings/draw_object_settings_bgfx.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
#include <vclib/opengl2/settings/draw_object_settings_opengl2.h>
#endif

namespace vcl {

#ifdef VCLIB_RENDER_BACKEND_BGFX
using DrawObjectSettings = DrawObjectSettingsBGFX;
#endif

#ifdef VCLIB_RENDER_BACKEND_OPENGL2
using DrawObjectSettings = DrawObjectSettingsOpenGL2;
#endif

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_DRAW_OBJECT_SETTINGS_H
