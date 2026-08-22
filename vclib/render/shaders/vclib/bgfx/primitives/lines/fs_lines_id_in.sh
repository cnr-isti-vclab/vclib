// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_color, v_normal, v_selected, v_lineIndex

#include <vclib/bgfx/primitives/uniforms/lines_uniforms.sh>
#include <vclib/bgfx/shaders_common.sh>

void main() {
    // Target 0: Object ID (16 bit) + Element Type (16 bit) (combined in u_linesId)
    gl_FragData[0] = u_linesId;
    
    // Target 1: Element ID (32 bit)
    uint elementId = uint(v_lineIndex);
    gl_FragData[1] = uintABGRToVec4Color(elementId);
}
