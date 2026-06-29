// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position
$output v_texcoord0

#include <vclib/bgfx/drawable/uniforms/drawable_environment_uniforms.sh>

void main()
{
    vec4 position = vec4(a_position.x, a_position.y, a_position.z, 1.0);
    
    // get the outward view direction in world space back from clip space
    vec3 viewDir = mul(u_invProj, position).xyz;
    mat3 invViewRotOnly = mat3(
        normalize(u_invView[0].xyz),
        normalize(u_invView[1].xyz),
        normalize(u_invView[2].xyz)
    );
    vec3 worldViewDir = mul(invViewRotOnly, viewDir); // keep just the rotation part

    // cubemaps are left-handed, so invert the Z coordinate 
    // to adapt to our right-handed system
    v_texcoord0 = leftHand(worldViewDir);

    // draw the triangle in screen space
    gl_Position = position;
}
