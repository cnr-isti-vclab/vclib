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

$input v_texcoord0

#include <vclib/bgfx/drawers/uniforms/viewer_drawer_uniforms.sh>
//#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

// textures
SAMPLERCUBE(s_env0, VCL_MRB_CUBEMAP0);

void main()
{
    vec3 color = textureCube(s_env0, normalize(v_texcoord0)).rgb;
    color *= u_exposure;
    color = toneMap(color, u_toneMapping);
    color = gammaCorrect(color);
    gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
