// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_LINES_UNIFORMS_SH
#define VCL_BGFX_PRIMITIVES_LINES_UNIFORMS_SH

uniform vec4 u_linesSettings;

#define thickness          u_linesSettings.x
#define u_colorShadingPack floatBitsToUint(u_linesSettings.y)

#define colorToUse         uint(u_colorShadingPack & 0x00FFFFFF)
#define u_shadingPerVertex bool(u_colorShadingPack >> 24u)

#define generalColor uintABGRToVec4Color(floatBitsToUint(u_linesSettings.z))

#define u_depthOffset u_linesSettings.w

#endif // VCL_BGFX_PRIMITIVES_LINES_UNIFORMS_SH
