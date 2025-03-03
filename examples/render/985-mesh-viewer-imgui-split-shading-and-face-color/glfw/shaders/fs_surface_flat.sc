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

$input v_position, v_normal, v_color, v_texcoord0, v_texcoord1

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

BUFFER_RO(primitiveColors, uint, VCL_MRB_PRIMITIVE_COLOR_BUFFER);    // color of each face / edge
BUFFER_RO(primitiveNormals, float, VCL_MRB_PRIMITIVE_NORMAL_BUFFER); // normal of each face / edge
BUFFER_RO(triTextureIds, uint, VCL_MRB_TRIANGLE_TEXTURE_ID_BUFFER);  // wedge ids of each face

// textures
SAMPLER2D(s_tex0, VCL_MRB_TEXTURE0);
SAMPLER2D(s_tex1, VCL_MRB_TEXTURE1);
SAMPLER2D(s_tex2, VCL_MRB_TEXTURE2);
SAMPLER2D(s_tex3, VCL_MRB_TEXTURE3);
SAMPLER2D(s_tex4, VCL_MRB_TEXTURE4);
SAMPLER2D(s_tex5, VCL_MRB_TEXTURE5);
SAMPLER2D(s_tex6, VCL_MRB_TEXTURE6);
SAMPLER2D(s_tex7, VCL_MRB_TEXTURE7);

vec4 getColorFromTexture(uint texId, vec2 uv) {
    switch (texId) {
        case 0: return texture2D(s_tex0, uv);
        case 1: return texture2D(s_tex1, uv);
        case 2: return texture2D(s_tex2, uv);
        case 3: return texture2D(s_tex3, uv);
        case 4: return texture2D(s_tex4, uv);
        case 5: return texture2D(s_tex5, uv);
        case 6: return texture2D(s_tex6, uv);
        case 7: return texture2D(s_tex7, uv);
        default: return vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void main()
{
    // depth offset - avoid z-fighting
    float depthOffset = 0.0;

    /***** compute light ******/
    vec3 normal = vec3(
            primitiveNormals[gl_PrimitiveID * 3],
            primitiveNormals[gl_PrimitiveID * 3 + 1],
            primitiveNormals[gl_PrimitiveID * 3 + 2]);
    normal = mul(u_modelView, vec4(normal, 0.0)).xyz;
    normal = normalize(normal);

    vec4 light = computeLight(u_lightDir, u_lightColor, normal);

    vec3 specular = computeSpecular(
                v_position,
                u_cameraEyePos,
                u_lightDir,
                u_lightColor,
                normal);


    /***** compute color ******/
    vec4 color = uintABGRToVec4Color(floatBitsToUint(u_userSurfaceColorFloat));

    switch(
        u_surfaceMode 
        & ( 
            posToBitFlag(VCL_MRS_SURF_COLOR_VERTEX)
            | posToBitFlag(VCL_MRS_SURF_COLOR_MESH)
            | posToBitFlag(VCL_MRS_SURF_COLOR_FACE)
            | posToBitFlag(VCL_MRS_SURF_TEX_VERTEX)
            | posToBitFlag(VCL_MRS_SURF_TEX_WEDGE)
        )
    ) {
        case posToBitFlag(VCL_MRS_SURF_COLOR_VERTEX):
            color = v_color;
            break;
        case posToBitFlag(VCL_MRS_SURF_COLOR_MESH):
            color = u_meshColor;
            break;
        case posToBitFlag(VCL_MRS_SURF_COLOR_FACE):
            color = uintABGRToVec4Color(primitiveColors[gl_PrimitiveID]);
            break;
        case posToBitFlag(VCL_MRS_SURF_TEX_VERTEX):
            color = getColorFromTexture(triTextureIds[gl_PrimitiveID], v_texcoord0);
            break;
        case posToBitFlag(VCL_MRS_SURF_TEX_WEDGE):
            color = getColorFromTexture(triTextureIds[gl_PrimitiveID], v_texcoord1);
            break;
    }

    gl_FragColor = light * color + vec4(specular, 0);
    gl_FragDepth = gl_FragCoord.z - depthOffset;
}
