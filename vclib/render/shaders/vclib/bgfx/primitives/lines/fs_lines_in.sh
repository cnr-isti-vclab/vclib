// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_color, v_normal

// cross section
$input v_worldPos0, v_worldPos1, v_discardFlag, v_t

#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.sh>

#include <vclib/bgfx/shaders_common.sh> 

void main() {
#if LINES_SECTION_ON
    vec3 fragPos = mix(v_worldPos0, v_worldPos1, v_t);
    discardIfCrossSectionClipped(v_discardFlag, fragPos);
#endif

    vec4 color = v_color;
    
#if LINES_SHADING_PER_VERTEX || LINES_SHADING_PER_LINE
    color *= computeLight(u_lightDir, u_lightColor, v_normal);
#endif

    gl_FragColor = color;
}
