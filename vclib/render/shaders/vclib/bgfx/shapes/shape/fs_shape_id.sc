// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_position

#include <vclib/bgfx/shapes/shape/uniforms.sh>
#include <vclib/bgfx/shaders_common.sh>

void main()
{
    // Target 0: Object ID (16 bit) + Element Type (16 bit)
    // (already combined in u_shapeId)
    gl_FragData[0] = u_shapeId;
    
    // Target 1: Element ID (32 bit) - Always 0 for shapes
    gl_FragData[1] = uintABGRToVec4Color(0u);
}
