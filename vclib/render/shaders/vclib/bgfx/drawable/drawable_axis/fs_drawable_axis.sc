// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_position
$input v_normal

#include <vclib/bgfx/drawable/drawable_axis/uniforms.sh>

void main()
{
    // color
    vec4 color = u_axisColor;

    /***** compute light ******/
    // default values - no shading
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    // compute normal
    vec3 normal = normalize(v_normal);

    // compute light
    light = computeLight(u_lightDir, u_lightColor, normal);

    // compute specular
    specular = computeSpecular(
        v_position,
        vec3(0.0, 0.0, 0.0),
        u_lightDir,
        u_lightColor,
        normal);

    gl_FragColor = light * u_axisColor + vec4(specular, 0);
}
