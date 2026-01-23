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
#include <vclib/bgfx/pbr_common.sh>

#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

BUFFER_RO(primitiveColors, uint, VCL_MRB_PRIMITIVE_COLOR_BUFFER);    // color of each face / edge
BUFFER_RO(primitiveNormals, float, VCL_MRB_PRIMITIVE_NORMAL_BUFFER); // normal of each face / edge

// textures
SAMPLER2D(baseColorTex, VCL_MRB_TEXTURE0);
SAMPLER2D(metallicRoughnessTex, VCL_MRB_TEXTURE1);
SAMPLER2D(normalTex, VCL_MRB_TEXTURE2);
SAMPLER2D(occlusionTex, VCL_MRB_TEXTURE3);
SAMPLER2D(emissiveTex, VCL_MRB_TEXTURE4);
SAMPLER2D(s_brdf_lut, VCL_MRB_TEXTURE5);

SAMPLERCUBE(s_irradiance, VCL_MRB_CUBEMAP0);
SAMPLERCUBE(s_specular, VCL_MRB_CUBEMAP1);

void main()
{
    // texcoord to use
    bool useTexture =
        bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_TEX_VERTEX)) ||
        bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_TEX_WEDGE));

    vec2 texcoord = v_texcoord0; // per vertex
    if (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_TEX_WEDGE))) {
        texcoord = v_texcoord1; // per wedge
    }

    // base color
    vec4 vertexBaseColor  = vec4_splat(1.0);
    vec4 textureBaseColor = vec4_splat(1.0);

    // color to use per vertex
    // if the user selected per face, per mesh or per user, override
    if (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_COLOR_FACE))) {
        vertexBaseColor = uintABGRToVec4Color(primitiveColors[primitiveID]);
    }
    else if (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_COLOR_MESH))) {
        vertexBaseColor = u_meshColor;
    }
    else if (bool(u_surfaceMode & posToBitFlag(VCL_MRS_SURF_COLOR_USER))) {
        vertexBaseColor = uintABGRToVec4Color(floatBitsToUint(u_userSurfaceColorFloat));
    }
    else {
        if (isPerVertexColorAvailable(u_pbr_settings))
            vertexBaseColor = v_color; // per-vertex color available
    }

    if (useTexture && isBaseColorTextureAvailable(u_pbr_texture_settings)) {
        // base color texture available
        textureBaseColor = texture2D(baseColorTex, texcoord);
    }

    // multiply vertex color with material base color
    vec4 baseColor = u_baseColorFactor * textureBaseColor * vertexBaseColor;

    // alpha mode MASK
    if (isAlphaModeMask(u_pbr_settings))
        if (baseColor.a < u_alphaCutoff)
            discard; // discard fragment

    // metallic-roughness
    vec4 metallicRoughnessTexture = vec4_splat(1.0);

    if (useTexture && isMetallicRoughnessTextureAvailable(u_pbr_texture_settings)) {
        // metallic-roughness texture available
        metallicRoughnessTexture = texture2D(metallicRoughnessTex, texcoord);
    }

    float metallic = u_metallicFactor * metallicRoughnessTexture.b; // metallic is stored in B channel
    float roughness = u_roughnessFactor * metallicRoughnessTexture.g; // roughness is stored in G channel

    // normal
    vec3 normal;

    if (useTexture && isNormalTextureAvailable(u_pbr_texture_settings)) {
        vec3 normalTexture = texture2D(normalTex, texcoord).xyz;

        // remapping normals
        // from [0,1] to [-1,1] for x and y (red and green)
        // from (0.5,1] to (0,1] for z (blue)
        normalTexture *= 2.0;
        normalTexture -= 1.0;

        // scale normal's x and y as requested by gltf 2.0 specification
        normalTexture *= vec3(u_normalScale, u_normalScale, 1.0);

        mat3 tangentFrame;

        if (isPerVertexTangentAvailable(u_pbr_settings)) {
            vec3 bitangent = cross(normalize(v_normal), normalize(v_tangent.xyz)) * v_tangent.w;
            tangentFrame = tangentFrameFromGivenVectors(v_tangent.xyz, bitangent, v_normal, vcl_FrontFacing);
        }
        else {
            // construct tangent frame using vertex normals
            tangentFrame = tangentFrameFromNormal(v_normal, v_position, texcoord, vcl_FrontFacing);
        }

        // change the basis of the normal provided by the texture
        // from tangent space to the space used for computations
        normal = mul(normalTexture, tangentFrame);

        normal = normalize(normal);
    }
    else {
        normal = normalize(v_normal);
        if (!vcl_FrontFacing)
            normal *= -1.0;
    }

    // emissive
    vec3 emissiveTexture = vec3_splat(1.0);

    if (useTexture && isEmissiveTextureAvailable(u_pbr_texture_settings)) {
        // emissive texture available
        emissiveTexture = texture2D(emissiveTex, texcoord).rgb;
    }

    vec3 emissiveColor = u_emissiveFactor * emissiveTexture;

    if(useImageBasedLighting(u_pbr_settings))
    {
        // view direction
        vec3 V = normalize(-v_position); // camera is at the origin

        vec3 reflection = normalize(reflect(-V, normal));

        reflection = normalize(mul(u_invView, vec4(reflection, 0.0)).xyz);

        // convert from camera to world space
        normal = normalize(mul(u_invView, vec4(normal, 0.0)).xyz);
        V = normalize(mul(u_invView, vec4(V, 0.0)).xyz);

        float NoV = clampedDot(normal, V);

        vec3 f0_dielectric = vec3_splat(0.04);
        vec3 f90 = vec3_splat(1.0);

        // diffuse light
        vec3 diffuseLight = textureCube(s_irradiance, leftHand(normal)).rgb;

        // specular light
        float specularMipLevel = roughness * (u_specularMipLevels - 1.0);
        
        vec3 specularLight = textureCubeLod(s_specular, leftHand(reflection), specularMipLevel).rgb;

        // Fresnel
        vec2 brdf = texture2D(s_brdf_lut, vec2(NoV, roughness)).rg;
        vec3 metalFresnel = iblGgxFresnel(brdf, NoV, roughness, baseColor.rgb);
        vec3 dielectricFresnel = iblGgxFresnel(brdf, NoV, roughness, f0_dielectric);

        // occlusion
        float occlusion = 1.0;
        if(useTexture && isOcclusionTextureAvailable(u_pbr_texture_settings))
        {
            occlusion = texture2D(occlusionTex, texcoord).r;
        }
        occlusion = 1.0 + u_occlusionStrength * (occlusion - 1.0);

        gl_FragColor = pbrColorIbl(
            diffuseLight,
            baseColor,
            specularLight,
            metalFresnel,
            dielectricFresnel,
            metallic,
            occlusion,
            emissiveColor,
            u_exposure,
            u_toneMapping
        );
    }
    else
    {
        #ifndef VIEWER_LIGHTS

        // precomputed default light directions from https://github.com/KhronosGroup/glTF-Sample-Viewer
        vec3 lightDirections[2] = {LIGHT_KEY_DIR_VIEW, LIGHT_FILL_DIR_VIEW};
        vec3 lightColors[2] = {vec3_splat(1.0), vec3_splat(1.0)};
        float lightIntensities[2] = {1.0, 0.5};

        #else

        vec3 lightDirections[1] = {-u_lightDir};
        vec3 lightColors[1] = {u_lightColor};
        float lightIntensities[1] = {1.0};

        #endif // VIEWER_LIGHTS

        gl_FragColor = pbrColorLights(
            v_position.xyz,
            vec3_splat(0.0), // camera position
            lightDirections,
            lightColors,
            lightIntensities,
            baseColor,
            normal,
            metallic,
            roughness,
            emissiveColor,
            u_exposure,
            u_toneMapping
        );
    }
}
