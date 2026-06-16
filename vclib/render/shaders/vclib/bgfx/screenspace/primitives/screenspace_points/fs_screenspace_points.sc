/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

$input v_texcoord1

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.sh>

BUFFER_RO(pointColors, uint, 1); // colors (rgba as float bits)

void main()
{
    vec4 color = u_pointsGlobalColor;

    if (usePerPointColor()) {
        uint pointId = uint(gl_PrimitiveID) / 2u;
        color = uintABGRToVec4Color(pointColors[pointId]);
    }

    // Circle shape discards fragments outside the unit disk.
    if (!useQuadShape()) {
        vec2 uv = v_texcoord1 * 2.0 - vec2(1.0, 1.0);
        if (length(uv) > 1.0) {
            discard;
        }
    }

    gl_FragColor = color;
}
