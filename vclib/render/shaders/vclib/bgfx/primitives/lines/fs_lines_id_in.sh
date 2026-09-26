// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_color, v_normal, v_selected

// cross section
$input v_worldPos0, v_worldPos1, v_discardFlag, v_t

#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#include <vclib/bgfx/primitives/uniforms/lines_uniforms.sh>
#include <vclib/bgfx/shaders_common.sh>

void main() {
#if LINES_ID_SECTION_ON
    vec3 fragPos = mix(v_worldPos0, v_worldPos1, v_t);
    discardIfCrossSectionClipped(v_discardFlag, fragPos);
#endif
    gl_FragColor = u_linesId;
}
