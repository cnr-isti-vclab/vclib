// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifdef SURFACE_SECTION_ON
// cross section
$input v_worldPos, v_discardFlag
#endif

#include <vclib/bgfx/shaders_common.sh>

#include <vclib/bgfx/drawable/drawable_mesh/mesh_data_uniforms.sh>

#ifdef SURFACE_SECTION_ON
#include <vclib/bgfx/drawable/uniforms/cross_section_uniforms.sh>
#endif

void main()
{
#ifdef SURFACE_SECTION_ON
    discardIfCrossSectionClipped(v_discardFlag, v_worldPos);
#endif

    /***** render ID to color ******/
    vec4 color = uintABGRToVec4Color(u_meshId);

    gl_FragColor = color;
}
