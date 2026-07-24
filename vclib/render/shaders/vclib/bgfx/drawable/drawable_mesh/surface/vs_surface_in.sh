// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position, a_normal, a_tangent, a_color0, a_texcoord0, a_texcoord1
$output v_position, v_normal, v_tangent, v_color, v_texcoord0, v_texcoord1

// cross section
$output v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_position = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    v_normal = normalize(mul(u_normalMatrix, a_normal));
    v_texcoord0 = a_texcoord0;
    v_texcoord1 = a_texcoord1;
    vec3 tangent = normalize(mul(u_normalMatrix, a_tangent.xyz));
    v_tangent = vec4(tangent.x, tangent.y, tangent.z, a_tangent.w);

    // default case - color is taken from buffer
    v_color = a_color0;

#ifdef SURFACE_SECTION_ON
    v_worldPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    // discard flag - used to discard the whole vertex, but in fragment shader
    v_discardFlag = computeDiscardFlag(v_worldPos);
#else
    v_discardFlag = 1;
#endif
}
