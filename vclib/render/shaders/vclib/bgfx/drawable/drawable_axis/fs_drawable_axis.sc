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
