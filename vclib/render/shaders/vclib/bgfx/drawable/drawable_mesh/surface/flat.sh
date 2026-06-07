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

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next lines with:

BUFFER_RO(primitiveNormals, float, VCL_MRB_PRIMITIVE_NORMAL_BUFFER);
*/
BUFFER_RO(primitiveNormals, float, 14);

#define primitiveID (u_firstChunkPrimitiveID + gl_PrimitiveID)

#define SHADING float depthOffset = 0.0;\
vec3 normal = vec3(\
        primitiveNormals[primitiveID * 3],\
        primitiveNormals[primitiveID * 3 + 1],\
        primitiveNormals[primitiveID * 3 + 2]);\
normal = normalize(mul(u_normalMatrix, normal));\
vec4 light = computeLight(u_lightDir, u_lightColor, normal);\
vec3 specular = computeSpecular(\
        v_position,\
        vec3(0.0, 0.0, 0.0),\
        u_lightDir,\
        u_lightColor,\
        normal);
