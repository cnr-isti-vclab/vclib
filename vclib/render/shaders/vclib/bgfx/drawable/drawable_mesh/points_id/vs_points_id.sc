// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position

// cross section
$output v_worldPos, v_discardFlag

#include <vclib/bgfx/shaders_common.sh>

#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));

    v_worldPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;

    // discard flag - used to discard the whole vertex, but in fragment shader
    v_discardFlag = computeDiscardFlag(v_worldPos);
}
