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

$input v_normal, v_color, v_texcoord1

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

void main()
{
    // defaul color and light
    vec4 light = vec4(1, 1, 1, 1);
    vec4 color = uintABGRToVec4Color(floatBitsToUint(u_userPointColorFloat));

    // circle mode (if outside of the circle, discard)
    bool isCircle = bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_CIRCLE));
    bool isSphere = bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SPHERE));
    if (isCircle || isSphere) {
        vec2 uv = v_texcoord1 * 2.0 - vec2(1.0, 1.0);
        if (length(uv) > 1.0) {
            discard;
        }
        if (isSphere) {
            // sphere mode
            v_normal = normalize(vec3(uv.x, uv.y, sqrt(1.0 - dot(uv, uv))));
        }
    }
    
    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SHADING_VERT))) {
        light = computeLight(u_lightDir, u_lightColor, v_normal);
    }

    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_VERTEX))) {
        color = v_color;
    }
    else if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_MESH))) {
        color = u_meshColor;
    }

    // NO depth writing (it kills performance)
    gl_FragColor = light * color; // + vec4(specular, 0);
}
