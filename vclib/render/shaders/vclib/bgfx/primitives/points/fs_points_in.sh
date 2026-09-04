// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_normal, v_texcoord0, v_color, v_selected
$input v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/directional_light_uniforms.sh>
#include <vclib/bgfx/primitives/uniforms/points_uniforms.sh>
#include <vclib/bgfx/shaders_common.sh>

void main()
{
#if POINTS_SECTION_ON
    discardIfCrossSectionClipped(v_discardFlag, v_worldPos);
#endif

    vec4 color = v_color;

#if POINTS_SHAPE_CIRCLE
    // circle mode (if outside of the circle, discard)
    vec2 uv = v_texcoord0 * 2.0 - vec2(1.0, 1.0);
    if (length(uv) > 1.0) {
        discard;
    }
#endif

    vec4 light = vec4(1.0, 1.0, 1.0, 1.0);
#if POINTS_SHADING_PER_VERTEX
    light = computeLight(u_lightDir, u_lightColor, v_normal);
#endif

    vec4 baseColor = light * color;

#if POINTS_SELECTION_ON
    float selWeight = u_pointsSelectionColor.a * v_selected;
    vec3 tmp = mix(baseColor.rgb, u_pointsSelectionColor.rgb, selWeight);
    gl_FragColor = vec4(tmp, baseColor.a);
#else
    gl_FragColor = baseColor;
#endif
}
