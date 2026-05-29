/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#include <vclib/bgfx/selection/uniforms.sh>

#define V4RGBA(x) uintABGRToVec4Color(x).wzyx

void main() {
    // NOTE: u_meshID 0 is reserved to indicate that no data is available
    // (i.e. the fragment did NOT pass)
    if(u_meshID == uint(0)) {
        discard;
    }
    uint priId = uint(gl_PrimitiveID);
    gl_FragData[0] = V4RGBA(gl_PrimitiveID);
    gl_FragData[1] = V4RGBA(u_meshID);
}
