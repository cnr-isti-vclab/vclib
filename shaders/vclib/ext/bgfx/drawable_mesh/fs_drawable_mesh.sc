/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <drawable_mesh/uniforms.sh>
#include <vclib/render/mesh_render_settings_macros.h>

BUFFER_RO(triangleColors, uint, 1);
BUFFER_RO(triangleNormals, float, 2);

void main()
{
    uint primitive = floatBitsToUint(u_primitiveFloat);
    uint drawMode = floatBitsToUint(u_drawModeFloat);

    // depth offset - avoid z-fighting
    float depthOffset = 0.0;

    // if not drawing mesh, discard
    if (!bool(drawMode & VCL_MRS_DRAW_MESH)) {
        discard;
    }

    // color
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    /***** compute light ******/
    // default values - no shading
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = v_normal;

    if (bool(primitive & VCL_MRS_PRIMITIVE_POINTS)) {
        // if per vert shading
        if (bool(drawMode & VCL_MRS_POINTS_SHADING_VERT)) {
            light = computeLight(u_lightDir, u_lightColor, normal);
        }

        color = uintToVec4Color(floatBitsToUint(u_userPointColorFloat));

        if (bool(drawMode & VCL_MRS_POINTS_COLOR_VERTEX)) {
            color = v_color;
        }
        else if (bool(drawMode & VCL_MRS_POINTS_COLOR_MESH)) {
            color = u_meshColor;
        }
        depthOffset = 0.0001;
    }
    else if (bool(primitive & VCL_MRS_PRIMITIVE_TRIANGLES)) {
        // if flat shading, compute normal of face
        if (bool(drawMode & VCL_MRS_SURF_SHADING_FLAT)) {
            normal = vec3(
                triangleNormals[gl_PrimitiveID * 3],
                triangleNormals[gl_PrimitiveID * 3 + 1],
                triangleNormals[gl_PrimitiveID * 3 + 2]);
            normal = mul(u_modelView, vec4(normal, 0.0)).xyz;
        }

        // if flat or smooth shading, compute light
        if (!bool(drawMode & VCL_MRS_SURF_SHADING_NONE)) {
            light = computeLight(u_lightDir, u_lightColor, normal);

            specular = computeSpecular(
                v_position,
                u_cameraEyePos,
                u_lightDir,
                u_lightColor,
                normal);
        }

        /***** compute color ******/
        color = uintToVec4Color(floatBitsToUint(u_userSurfaceColorFloat));

        if (bool(drawMode & VCL_MRS_SURF_COLOR_VERTEX)) {
            color = v_color;
        }
        if (bool(drawMode & VCL_MRS_SURF_COLOR_MESH)) {
            color = u_meshColor;
        }
        if (bool(drawMode & VCL_MRS_SURF_COLOR_FACE)) {
            color = uintToVec4Color(triangleColors[gl_PrimitiveID]);
        }
    }
    else { // wireframe
        // shading
        if (!bool(drawMode & VCL_MRS_WIREFRAME_SHADING_NONE)) {
            light = computeLight(u_lightDir, u_lightColor, normal);
        }

        color = uintToVec4Color(floatBitsToUint(u_userWireframeColorFloat));

        if (bool(drawMode & VCL_MRS_WIREFRAME_COLOR_VERT)) {
            color = v_color;
        }
        if (bool(drawMode & VCL_MRS_WIREFRAME_COLOR_MESH)) {
            color = u_meshColor;
        }
        depthOffset = 0.00005;
    }

    gl_FragColor = light * color + vec4(specular, 0);
    gl_FragDepth = gl_FragCoord.z - depthOffset;
}
