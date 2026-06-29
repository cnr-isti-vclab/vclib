// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
