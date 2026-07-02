// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_color, v_texcoord1

#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/screenspace/primitives/uniforms/screenspace_points_uniforms.sh>

void main()
{
    vec4 color = v_color;

#ifdef POINTS_SHAPE_CIRCLE
    // Circle shape discards fragments outside the unit disk.
    vec2 uv = v_texcoord1 * 2.0 - vec2(1.0, 1.0);
    if (length(uv) > 1.0) {
        discard;
    }
#endif

    gl_FragColor = color;
}
