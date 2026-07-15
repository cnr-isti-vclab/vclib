// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_UNIFORMS_POINTS_UNIFORMS_SH
#define VCL_BGFX_PRIMITIVES_UNIFORMS_POINTS_UNIFORMS_SH

#include <vclib/bgfx/shaders_common.sh>

uniform vec4 u_pointsSettings;

#define u_pointsWidth u_pointsSettings.x
#define u_pointsGeneralColor uintABGRToVec4Color(floatBitsToUint(u_pointsSettings.y))
#define u_depthOffset u_pointsSettings.z
#define u_pointsId uintABGRToVec4Color(floatBitsToUint(u_pointsSettings.w))

#endif // VCL_BGFX_PRIMITIVES_UNIFORMS_POINTS_UNIFORMS_SH
