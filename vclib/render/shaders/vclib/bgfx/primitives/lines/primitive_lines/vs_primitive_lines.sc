// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position, a_normal, a_color0
$output v_color, v_normal

// cross section
$output v_worldPos, v_discardFlag

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#include <vclib/bgfx/primitives/lines/uniforms.sh>
#include <vclib/bgfx/shaders_common.sh> 

#define p                     a_position
#define color                 a_color0
#define normal                a_normal

void main() {
    v_color = color;
    v_normal = normalize(mul(u_normalMatrix, normal));
    v_worldPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;
    vec4 pos = mul(u_modelViewProj, vec4(p, 1.0));
    pos.z += -u_depthOffset * pos.w;
    gl_Position = pos;

    // discard flag - used to discard the whole vertex, but in fragment shader
    v_discardFlag = computeDiscardFlag(v_worldPos);
}
