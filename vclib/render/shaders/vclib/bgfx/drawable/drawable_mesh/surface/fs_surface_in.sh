// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_position, v_normal, v_tangent, v_color, v_texcoord0, v_texcoord1

// cross section
$input v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/drawable_mesh/mesh_data_uniforms.sh>
#include <vclib/bgfx/drawable/drawable_mesh/render_settings_uniforms.sh>
#include <vclib/bgfx/drawable/drawable_mesh/texture_uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.sh>

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

// is face selected? 1 bit per triangle (MSb first)
BUFFER_RO(faceSelected, uint, VCL_MRB_PRIMITIVE_SELECTION_BUFFER);

#ifdef SURFACE_COLOR_FACE
// color of each face / edge
BUFFER_RAW_RO(primitiveColors, VCL_MRB_PRIMITIVE_COLOR_BUFFER);
#endif

#ifdef SURFACE_SHADING_FLAT
// normal of each face / edge
BUFFER_RAW_RO(primitiveNormals, VCL_MRB_PRIMITIVE_NORMAL_BUFFER);
DECLARE_FETCH_VEC3_FROM_FLOAT(fetchPrimitiveNormal, primitiveNormals)
#endif

void main()
{
#ifdef SURFACE_SECTION_ON
    discardIfCrossSectionClipped(v_discardFlag, v_worldPos);
#endif

    // color
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    /***** compute light ******/
    // default values - no shading
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = normalize(v_normal);

#ifdef SURFACE_SHADING_FLAT
    // if flat shading, compute normal of face
    normal = fetchPrimitiveNormal(primitiveID);
    normal = normalize(mul(u_normalMatrix, normal));
#endif

#ifdef SURFACE_SHADING_NORMAL_MAP
    if (isNormalTextureAvailable()) {
        vec2 texcoord = v_texcoord0;
#ifdef SURFACE_COLOR_TEX_WEDGE
        texcoord = v_texcoord1;
#endif
        // Calculate the Tangent, Bitangent, Normal (TBN) matrix 
        // to transform from tangent space to view space.
        vec3 t = normalize(v_tangent.xyz);
        vec3 b = normalize(cross(normal, t)) * v_tangent.w;

        // Sample the normal map and remap from [0, 1] to [-1, 1].
        vec3 nMap = normalTex(texcoord).xyz * 2.0 - 1.0;

        // Transform the sampled normal to view space using the TBN matrix.
        normal = normalize(t * nMap.x + b * nMap.y + normal * nMap.z);
    }
#endif

#ifdef SURFACE_BACK_FACE_DOUBLE_ON
    if (!vcl_FrontFacing) {
        normal = -normal;
    }
#endif


#ifndef SURFACE_SHADING_NONE
    // if flat or smooth shading, compute light
    light = computeLight(u_lightDir, u_lightColor, normal);

#ifdef SURFACE_SPECULAR_ON
    // all computations are in view (camera) space
    // => the camera eye is at (0, 0, 0)
    // also, u_lightDir is provided in view space
    specular = computeSpecular(
        v_position,
        vec3(0.0, 0.0, 0.0),
        u_lightDir,
        u_lightColor,
        normal);
#endif
#endif

    /***** compute color ******/
    color = uintABGRToVec4Color(floatBitsToUint(u_userSurfaceColorFloat));

#ifdef SURFACE_COLOR_VERTEX
    color = v_color;
#endif
#ifdef SURFACE_COLOR_MESH
    color = u_meshColor;
#endif
#ifdef SURFACE_COLOR_FACE
    color = uintABGRToVec4Color(rawLoadUint(primitiveColors, primitiveID));
#endif
#ifdef SURFACE_COLOR_TEX_VERTEX
    if (isBaseColorTextureAvailable())
        color = baseColorTex(v_texcoord0);
    else
        color = vec4(0.0, 0.0, 0.0, 1.0);
#endif
#ifdef SURFACE_COLOR_TEX_WEDGE
    if (isBaseColorTextureAvailable())
        color = baseColorTex(v_texcoord1);
    else
        color = vec4(0.0, 0.0, 0.0, 1.0);
#endif

    color = light * color + vec4(specular, 0);
#ifdef SURFACE_SELECTION_ON
        float selWeight =
            u_selectionSurfaceColor.a * float(getBoolFromBuffer(faceSelected, primitiveID));
        vec3 tmp = mix(color.rgb, u_selectionSurfaceColor.rgb, selWeight);
        gl_FragColor = vec4(tmp, color.a);
#else
        gl_FragColor = color;
#endif
}
