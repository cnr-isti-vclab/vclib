/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

$input v_position, v_normal, v_color

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

void main()
{
    uint drawMode0 = floatBitsToUint(u_drawMode0Float);

    // color
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    /***** compute light ******/
    // default values - no shading
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = normalize(v_normal);

    // if per vert shading
    if (bool(drawMode0 & VCL_MRS_POINTS_SHADING_VERT)) {
        light = computeLight(u_lightDir, u_lightColor, normal);
    }

    color = uintABGRToVec4Color(floatBitsToUint(u_userPointColorFloat));

    if (bool(drawMode0 & VCL_MRS_POINTS_COLOR_VERTEX)) {
        color = v_color;
    }
    else if (bool(drawMode0 & VCL_MRS_POINTS_COLOR_MESH)) {
        color = u_meshColor;
    }

    // depth offset - avoid z-fighting
    float depthOffset = 0.0001;

    gl_FragColor = light * color + vec4(specular, 0);
    gl_FragDepth = gl_FragCoord.z - depthOffset;
}
