// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position, a_normal
$output v_position, v_normal

#include <vclib/bgfx/drawable/drawable_axis/uniforms.sh>

void main()
{
    gl_Position = mul(u_modelViewProj, vec4(a_position, 1.0));
    v_position = mul(u_modelView, vec4(a_position, 1.0)).xyz;
    v_normal = normalize(mul(u_normalMatrix, a_normal));
}
