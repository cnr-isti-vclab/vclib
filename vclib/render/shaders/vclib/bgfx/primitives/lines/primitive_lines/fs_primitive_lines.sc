// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_color, v_normal

// cross section
$input v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.sh>
#include <vclib/bgfx/primitives/lines/uniforms.sh>
#include <vclib/bgfx/shaders_common.sh> 

#include <bgfx_shader.sh>
#include <bgfx_compute.sh>

BUFFER_RO(edgesColors, uint, 0);

#define edgeColor    uintABGRToVec4Color(edgesColors[gl_PrimitiveID])
#define vertexColor  v_color

void main() {
    discardIfCrossSectionClipped(v_discardFlag, v_worldPos);

    vec4 color;
    if (colorToUse == 0)        color = vertexColor;
    else if (colorToUse == 1)   color = edgeColor;
    else                        color = generalColor;

    if (u_shadingPerVertex) {
        color *= computeLight(u_lightDir, u_lightColor, v_normal);
    }
    gl_FragColor = color;
}
