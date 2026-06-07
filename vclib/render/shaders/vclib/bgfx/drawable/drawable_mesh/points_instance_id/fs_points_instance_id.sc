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

$input v_texcoord0

// cross section
$input v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>

void main()
{
    discardIfCrossSectionClipped(v_discardFlag, v_worldPos);

    // circle mode (if outside of the circle, discard)
    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_CIRCLE))) {
        if (length(2.0 * v_texcoord0 - vec2(1.0, 1.0)) > 1.0) {
            discard;
        }
    }
    
    /***** render ID to color ******/
    vec4 color = uintABGRToVec4Color(u_meshId);

    gl_FragColor = color;
}
