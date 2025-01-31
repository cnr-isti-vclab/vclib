/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <bgfx_compute.sh>

BUFFER_WO(segmentsIndirectBuffer, uvec4, 0);
BUFFER_WO(joinsIndirectBuffer,    uvec4, 1);

uniform vec4 u_IndirectData;

NUM_THREADS(1, 1, 1)
void main()
{
	drawIndexedIndirect(segmentsIndirectBuffer, 0, 6, u_IndirectData.x - 1, 0, 0, 0);
	drawIndexedIndirect(joinsIndirectBuffer,    0, 6, u_IndirectData.x - 2, 0, 0, 0);
}
