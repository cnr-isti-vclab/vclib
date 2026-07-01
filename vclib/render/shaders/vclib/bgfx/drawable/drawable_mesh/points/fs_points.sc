// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

// TODO: remove v_position
$input v_position, v_normal, v_color

// cross section
$input v_worldPos, v_discardFlag

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>

void main()
{
    discardIfCrossSectionClipped(v_discardFlag, v_worldPos);

    // color
    vec4 color = vec4(0.0, 0.0, 0.0, 1.0);

    /***** compute light ******/
    // default values - no shading
    vec3 specular = vec3(0.0, 0.0, 0.0);
    vec4 light = vec4(1, 1, 1, 1);

    vec3 normal = normalize(v_normal);

    // if per vert shading
    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SHADING_VERT))) {
        light = computeLight(u_lightDir, u_lightColor, normal);
    }

    color = uintABGRToVec4Color(floatBitsToUint(u_userPointColorFloat));

    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_VERTEX))) {
        color = v_color;
    }
    else if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_MESH))) {
        color = u_meshColor;
    }

    // depth offset - avoid z-fighting
    float depthOffset = 0.0001;

    gl_FragColor = light * color + vec4(specular, 0);
    gl_FragDepth = gl_FragCoord.z - depthOffset;
}
