// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position, a_normal, a_color0
$output v_position, v_normal, v_color, v_selected

// cross section
$output v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>

// is vertex selected? 1 bit per vertex (MSb first)
BUFFER_RO(vertex_selected, uint, 4);

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_position = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    v_normal = normalize(mul(u_normalMatrix, a_normal));
    v_worldPos = mul(u_model[0], vec4(a_position, 1.0)).xyz;

    // default case - color is taken from buffer
    v_color = a_color0;

    v_selected = float(getBoolFromBuffer(vertex_selected, gl_VertexID));

	// discard flag - used to discard the whole vertex, but in fragment shader
    v_discardFlag = computeDiscardFlag(v_worldPos);
}
