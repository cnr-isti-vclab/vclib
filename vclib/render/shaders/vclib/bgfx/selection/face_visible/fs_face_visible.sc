// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bgfx/selection/uniforms.sh>

#define V4RGBA(x) uintABGRToVec4Color(x).wzyx

void main() {
    // NOTE: u_meshID == UINT_MAX is reserved to indicate that no data is available
    if(u_meshID == uint(0xFFFFFFFF)) {
        discard;
    }
    gl_FragData[0] = V4RGBA(gl_PrimitiveID);
    gl_FragData[1] = V4RGBA(u_meshID);
}
