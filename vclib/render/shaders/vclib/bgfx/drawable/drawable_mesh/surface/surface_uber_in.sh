// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_position, v_normal, v_tangent, v_color, v_texcoord0, v_texcoord1

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/drawable/uniforms/drawable_mesh_texture_uniforms.sh>

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next lines with:

BUFFER_RO(primitiveColors, uint, VCL_MRB_PRIMITIVE_COLOR_BUFFER);   // color of each face / edge
BUFFER_RO(primitiveNormals, vec4, VCL_MRB_PRIMITIVE_NORMAL_BUFFER); // normal of each face / edge
*/

#ifdef SURF_COLOR_FACE
BUFFER_RO(primitiveColors, uint, 13);    // color of each face / edge
#endif

#ifdef SURF_SHADING_FLAT
BUFFER_RO(primitiveNormals, vec4, 14); // normal of each face / edge
DECLARE_FETCH_VEC3(fetchPrimitiveNormal, primitiveNormals);
#endif

void main()
{
    // color
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    /***** compute light ******/
    // default values - no shading
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = normalize(v_normal);

#ifdef SURF_SHADING_FLAT
    // if flat shading, compute normal of face
    normal = fetchPrimitiveNormal(primitiveID);
    normal = normalize(mul(u_normalMatrix, normal));
#endif

#ifdef SURF_SHADING_NORMAL_MAP
    if (isNormalTextureAvailable()) {
        vec2 texcoord = v_texcoord0;
#ifdef SURF_TEX_WEDGE
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


#ifndef SURF_SHADING_NONE
    // if flat or smooth shading, compute light
    light = computeLight(u_lightDir, u_lightColor, normal);

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

    /***** compute color ******/
    color = uintABGRToVec4Color(floatBitsToUint(u_userSurfaceColorFloat));

#ifdef SURF_COLOR_VERTEX
    color = v_color;
#endif
#ifdef SURF_COLOR_MESH
    color = u_meshColor;
#endif
#ifdef SURF_COLOR_FACE
    color = uintABGRToVec4Color(primitiveColors[primitiveID]);
#endif
#ifdef SURF_TEX_VERTEX
    if (isBaseColorTextureAvailable())
        color = baseColorTex(v_texcoord0);
    else
        color = vec4(0.0, 0.0, 0.0, 1.0);
#endif
#ifdef SURF_TEX_WEDGE
    if (isBaseColorTextureAvailable())
        color = baseColorTex(v_texcoord1);
    else
        color = vec4(0.0, 0.0, 0.0, 1.0);
#endif

    gl_FragColor = light * color + vec4(specular, 0);
}
