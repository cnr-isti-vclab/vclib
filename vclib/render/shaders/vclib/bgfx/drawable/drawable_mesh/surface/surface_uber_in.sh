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

$input v_position, v_normal, v_tangent, v_color, v_texcoord0, v_texcoord1

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

BUFFER_RO(primitiveColors, uint, VCL_MRB_PRIMITIVE_COLOR_BUFFER);    // color of each face / edge
BUFFER_RO(primitiveNormals, float, VCL_MRB_PRIMITIVE_NORMAL_BUFFER); // normal of each face / edge

// textures
SAMPLER2D(baseColorTex, VCL_MRB_TEXTURE0);

void main()
{
    // depth offset - avoid z-fighting
    float depthOffset = 0.0;

    // color
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    /***** compute light ******/
    // default values - no shading
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = normalize(v_normal);

    // if flat shading, compute normal of face
    if (SURF_SHADING_FLAT) {
        normal = vec3(
            primitiveNormals[primitiveID * 3],
            primitiveNormals[primitiveID * 3 + 1],
            primitiveNormals[primitiveID * 3 + 2]);
        normal = mul(u_modelView, vec4(normal, 0.0)).xyz;
        normal = normalize(normal);
    }

    // if flat or smooth shading, compute light
    if (!SURF_SHADING_NONE) {
        light = computeLight(u_lightDir, u_lightColor, normal);

        vec3 specular = computeSpecular(
            v_position,
            vec3(0.0, 0.0, 0.0),
            u_lightDir,
            u_lightColor,
            normal);
    }
    
    /***** compute color ******/
    color = uintABGRToVec4Color(floatBitsToUint(u_userSurfaceColorFloat));

    if (SURF_COLOR_VERTEX) {
        color = v_color;
    }
    if (SURF_COLOR_MESH) {
        color = u_meshColor;
    }
    if (SURF_COLOR_FACE) {
        color = uintABGRToVec4Color(primitiveColors[primitiveID]);
    }
    if (SURF_TEX_VERTEX) {
        color = texture2D(baseColorTex, v_texcoord0);
    }
    if (SURF_TEX_WEDGE) {
        color = texture2D(baseColorTex, v_texcoord1);
    }

    gl_FragColor = light * color + vec4(specular, 0);
    gl_FragDepth = gl_FragCoord.z - depthOffset;
}
