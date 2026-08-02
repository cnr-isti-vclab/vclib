// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/shaders_common.sh>

#include <vclib/bgfx/drawable/drawable_mesh/mesh_data_uniforms.sh>

void main()
{
    // Target 0: Object ID (16 bit) + Element Type (16 bit) (già combinati in u_meshId)
    vec4 color = uintABGRToVec4Color(u_meshId);
    gl_FragData[0] = color;
    
    // Target 1: Element ID (32 bit)
    uint elementId = gl_PrimitiveID + u_firstChunkPrimitiveID;
    gl_FragData[1] = uintABGRToVec4Color(elementId);
}
