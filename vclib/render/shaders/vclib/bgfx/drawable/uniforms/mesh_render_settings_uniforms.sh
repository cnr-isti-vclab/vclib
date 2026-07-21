// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
#define VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH

#include <vclib/bgfx/shaders_common.sh>

uniform vec4 u_mrsDrawPack;
uniform vec4 u_mrsWidthPack;
uniform vec4 u_mrsColorPack[2];

#define u_pointsMode floatBitsToUint(u_mrsDrawPack.x)
#define u_surfaceMode (floatBitsToUint(u_mrsDrawPack.x) >> 16)
#define u_wireframeMode floatBitsToUint(u_mrsDrawPack.y)
#define u_edgesMode (floatBitsToUint(u_mrsDrawPack.y) >> 16)

#define u_pointWidth u_mrsWidthPack.x
#define u_wireframeWidth u_mrsWidthPack.y
#define u_edgesWidth u_mrsWidthPack.z

#define u_userPointColorFloat u_mrsColorPack[0].x
#define u_userSurfaceColorFloat u_mrsColorPack[0].y
#define u_userWireframeColorFloat u_mrsColorPack[0].z
#define u_userEdgesColorFloat u_mrsColorPack[0].w
#define u_selectionPointColorFloat u_mrsColorPack[1].x
#define u_selectionSurfaceColorFloat u_mrsColorPack[1].y

#define u_selectionPointColor \
    uintABGRToVec4Color(floatBitsToUint(u_selectionPointColorFloat))

#define u_selectionSurfaceColor \
    uintABGRToVec4Color(floatBitsToUint(u_selectionSurfaceColorFloat))

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
