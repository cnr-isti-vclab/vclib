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

$input v_normal, v_texcoord1, v_selected

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next line with:

BUFFER_RO(vertexColors, vec4, VCL_MRB_VERTEX_COLOR_STREAM); // colors (rgba as float bits)
*/
BUFFER_RO(vertexColors, vec4, 2); // colors (rgba as float bits)

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
        uint pointId = uint(gl_PrimitiveID) / 2u;
        color = uintABGRToVec4Color(floatBitsToUint(vertexColors[pointId / 4u][pointId % 4u]));
    }
    else if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_MESH))) {
        color = u_meshColor;
    }

    color = light * color; // + vec4(specular, 0);
    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_DRAW_SELECTION))) {
        if (v_selected > 0.5) {
            vec3 tmp =
                mix(color.rgb, u_selectionPointColor.rgb, u_selectionPointColor.a);
            color = vec4(tmp, color.a);
        }
    }
    // remember: NO depth writing (it kills performance)
    gl_FragColor = color;
}
