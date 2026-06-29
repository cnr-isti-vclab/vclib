// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_BACKGROUND_UNIFORMS_SH
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_BACKGROUND_UNIFORMS_SH

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/pbr_common.sh>

uniform vec4 u_environmentSettingsPack;

#define u_roughness u_environmentSettingsPack.x
#define u_cubeSideResolution u_environmentSettingsPack.y

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_BACKGROUND_UNIFORMS_SH
