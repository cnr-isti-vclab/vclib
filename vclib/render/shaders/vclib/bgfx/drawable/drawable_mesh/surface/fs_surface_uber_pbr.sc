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

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

BUFFER_RO(primitiveColors, uint, VCL_MRB_PRIMITIVE_COLOR_BUFFER);    // color of each face / edge
BUFFER_RO(primitiveNormals, float, VCL_MRB_PRIMITIVE_NORMAL_BUFFER); // normal of each face / edge

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
        case 0u: return texture2D(s_tex0, uv);
        case 1u: return texture2D(s_tex1, uv);
        case 2u: return texture2D(s_tex2, uv);
        case 3u: return texture2D(s_tex3, uv);
        case 4u: return texture2D(s_tex4, uv);
        case 5u: return texture2D(s_tex5, uv);
        case 6u: return texture2D(s_tex6, uv);
        case 7u: return texture2D(s_tex7, uv);
        default: return vec4(0.0, 0.0, 0.0, 1.0);
    }
}

void main()
{
    // precomputed default light directions from https://github.com/KhronosGroup/glTF-Sample-Viewer
    vec3 lightDirections[2] = {LIGHT_KEY_DIR, LIGHT_FILL_DIR};
    vec3 lightColors[2] = {vec3_splat(1.0), vec3_splat(1.0)};
    float lightIntensities[2] = {1.0, 0.5};

    // texcoord to use
    vec2 texcoord = v_texcoord0; // per vertex
    if (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_TEX_WEDGE))) {
        texcoord = v_texcoord1; // per wedge
    }

    // base color
    vec4 vertexBaseColor, textureBaseColor, baseColor;

    if(isPerVertexColorAvailable(u_settings.x))
        vertexBaseColor = v_color; // per-vertex color available
    else
        vertexBaseColor = vec4_splat(1.0); // no per-vertex color available, use white

    if(isBaseColorTextureAvailable(u_settings.x)) {
        // base color texture available
        textureBaseColor = getColorFromTexture(0u, texcoord);
    }
    else {
        // no base color texture available, use white
        textureBaseColor = vec4_splat(1.0);
    }

    baseColor = u_baseColorFactor * textureBaseColor * vertexBaseColor; // multiply vertex color with material base color

    // alpha mode MASK
    if(isAlphaModeMask(u_settings.x))
        if(baseColor.a < u_alphaCutoff)
            discard; // discard fragment

    // metallic-roughness
    vec4 metallicRoughnessTexture;
    float metallic, roughness;

    if(isMetallicRoughnessTextureAvailable(u_settings.x))
        metallicRoughnessTexture = getColorFromTexture(1u, texcoord); // metallic-roughness texture available
    else
        metallicRoughnessTexture = vec4_splat(1.0); // no metallic-roughness texture available, use default value

    metallic = u_metallicFactor * metallicRoughnessTexture.b; // metallic is stored in B channel
    roughness = u_roughnessFactor * metallicRoughnessTexture.g; // roughness is stored in G channel

    // normal
    vec3 normal;

    if(isNormalTextureAvailable(u_settings.x))
    {
        vec3 normalTexture = getColorFromTexture(2u, texcoord).xyz;

        // remapping normals
        // from [0,1] to [-1,1] for x and y (red and green)
        // from (0.5,1] to (0,1] for z (blue)
        normalTexture *= 2.0;
        normalTexture -= 1.0;

        // scale normal's x and y as requested by gltf 2.0 specification
        normalTexture *= vec3(u_normalScale, u_normalScale, 1.0);

        // construct tangent frame using vertex normals
        mat3 tangentFrame = tangentFrameFromNormal(v_normal, v_position, texcoord, vcl_FrontFacing);

        // change the basis of the normal provided by the texture
        // from tangent space to the space used for computations
        normal = mul(normalTexture, tangentFrame);

        normal = normalize(normal);
    }
    else {
        normal = normalize(v_normal);
        if(!vcl_FrontFacing)
            normal *= -1.0;
    } 

    // emissive
    vec3 emissiveTexture, emissiveColor;

    if(isEmissiveTextureAvailable(u_settings.x))
        emissiveTexture = getColorFromTexture(4u, texcoord).rgb; // emissive texture available
    else
        emissiveTexture = vec3_splat(1.0); // no emissive texture available, use white

    emissiveColor = u_emissiveFactor * emissiveTexture;

    gl_FragColor = pbrColor(
        v_position.xyz,
        vec3_splat(0.0), // camera position
        lightDirections,
        lightColors,
        lightIntensities,
        baseColor,
        normal,
        metallic,
        roughness,
        emissiveColor
    );
}
