// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_SH
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_SH

uniform vec4 u_linesSettings;

#define u_linesWidth u_linesSettings.x
#define u_linesGeneralColor uintABGRToVec4Color(floatBitsToUint(u_linesSettings.y))

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_SH
