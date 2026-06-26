// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_normal, v_color

#include <vclib/bgfx/drawable/drawable_trackball/uniforms.sh>


void main()
{
    vec3 emission = vec3(0.5, 0.5, 0.5);
    vec3 normal = normalize(v_normal);
    float diffuse = max(dot(normal, u_lightDir), 0.0);
    gl_FragColor = vec4(emission + diffuse * v_color.rgb, v_color.a);
}
