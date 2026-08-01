// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_texcoord0

#include <vclib/bgfx/drawers/uniforms/viewer_drawer_uniforms.sh>

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
