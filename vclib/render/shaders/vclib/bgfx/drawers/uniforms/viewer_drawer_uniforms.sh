// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_SH
#define VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_SH

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/pbr_common.sh>

uniform vec4 u_viewerSettingsPack;

#define u_exposure u_viewerSettingsPack.x
#define u_toneMapping floatBitsToUint(u_viewerSettingsPack.y)
#define u_specularMipLevels floatBitsToUint(u_viewerSettingsPack.z)

#endif // VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_SH
