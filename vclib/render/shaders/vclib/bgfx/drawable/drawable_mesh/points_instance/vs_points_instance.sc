// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position, a_normal
$output v_normal, v_texcoord1, v_selected

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>

// is vertex selected? 1 bit per vertex (MSb first)
BUFFER_RO(vertex_selected, uint, 4);

void main()
{
    uint idx = uint(gl_VertexID) & 3u; // last 2 bits
    // (bit 0 = x axis, bit 1 = y axis)
    vec4 pos = mul(u_modelViewProj, vec4(a_position, 1.0));
    vec2 quadUv = vec2(idx & 1u, (idx >> 1) & 1u);
    vec4 offset = vec4(
        // {-1, +1} * width * texel
        (2.0 * quadUv.x - 1.0) * u_pointWidth * u_viewTexel.x * pos.w, // is divided by 2
        (2.0 * quadUv.y - 1.0) * u_pointWidth * u_viewTexel.y * pos.w, // is divided by 2
        0, 0);

    gl_Position = pos + offset;
    v_normal = normalize(mul(u_normalMatrix, a_normal));

    // quad parametrization
    v_texcoord1 = quadUv;

    // vertex selection: 4 vertices per point, so point index = gl_VertexID / 4
    v_selected = float(getBoolFromBuffer(vertex_selected, gl_VertexID / 4));
}
