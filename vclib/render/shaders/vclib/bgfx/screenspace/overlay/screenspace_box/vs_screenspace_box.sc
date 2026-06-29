// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input a_position

// a_position vec2 in screen space

#include <vclib/bgfx/shaders_common.sh>

void main() {
    gl_Position = vec4(
        (a_position[0] - u_viewRect.x) / u_viewRect.z * 2 -1,
        1 - (a_position[1] - u_viewRect.y) / u_viewRect.w * 2,
        0,
        1);
}
