/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
