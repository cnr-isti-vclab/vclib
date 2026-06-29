// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/shaders_common.sh>

#include <vclib/bgfx/drawable/uniforms/drawable_mesh_uniforms.sh>

void main()
{
    /***** render ID to color ******/
    vec4 color = uintABGRToVec4Color(u_meshId);

    gl_FragColor = color;
}
