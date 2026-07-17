// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_normal, v_texcoord0, v_color, v_selected

#include <vclib/bgfx/primitives/uniforms/points_uniforms.sh>
#include <vclib/bgfx/shaders_common.sh>

void main()
{
#if POINTS_ID_SHAPE_CIRCLE
    // circle mode (if outside of the circle, discard)
    vec2 uv = v_texcoord0 * 2.0 - vec2(1.0, 1.0);
    if (length(uv) > 1.0) {
        discard;
    }
#endif

    gl_FragColor = u_pointsId;
}
