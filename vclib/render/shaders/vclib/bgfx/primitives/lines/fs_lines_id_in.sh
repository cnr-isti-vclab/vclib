// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_color, v_normal

#include <vclib/bgfx/primitives/uniforms/lines_uniforms.sh>
#include <vclib/bgfx/shaders_common.sh>

void main() {
    gl_FragColor = u_linesId;
}
