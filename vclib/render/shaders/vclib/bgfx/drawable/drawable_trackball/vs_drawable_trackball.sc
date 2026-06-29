// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position, a_color0
$output v_normal, v_color

#include <vclib/bgfx/drawable/drawable_trackball/uniforms.sh>

void main()
{
    v_normal = normalize(mul(u_model[0], vec4(a_position, 0.0)).xyz);
    v_color = vec4(a_color0.rgb, u_trackballAlpha);
    gl_Position = mul(u_proj, mul(u_model[0], vec4(a_position, 1.0)));
}
