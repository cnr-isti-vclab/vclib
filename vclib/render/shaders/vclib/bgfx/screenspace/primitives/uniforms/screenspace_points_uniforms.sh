/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_POINTS_UNIFORMS_SH
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_POINTS_UNIFORMS_SH

uniform vec4 u_pointsSettings;

#define u_pointsColorToUse floatBitsToUint(u_pointsSettings.x)
#define u_pointsShadeMode floatBitsToUint(u_pointsSettings.y)
#define u_pointsWidth u_pointsSettings.z
#define u_pointsGeneralColor uintABGRToVec4Color(floatBitsToUint(u_pointsSettings.w))

bool usePerPointColor() {
    return u_pointsColorToUse == 0u;
}

bool useQuadShape() {
    return u_pointsShadeMode == 0u;
}

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_POINTS_UNIFORMS_SH
