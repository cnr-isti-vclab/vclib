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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_VERTEX_SELECTION_UTILS_SH
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_VERTEX_SELECTION_UTILS_SH

// Bit-packed vertex selection buffer: bit 1 = selected, bit 0 = not selected.
// Each uint holds 32 vertex selection bits (MSB first: bit 31 = vertex 32*i).
BUFFER_RO(vertex_selected, uint, 4);

bool isVertexSelected(uint vertId)
{
    uint idx  = vertId / 32u;
    uint mask = 0x1u << (31u - (vertId % 32u));
    return (vertex_selected[idx] & mask) != 0u;
}

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_VERTEX_SELECTION_UTILS_SH
