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

#ifndef VCL_BGFX_DRAWABLE_DRAWABLE_MESH_FACE_SELECTION_UTILS_SH
#define VCL_BGFX_DRAWABLE_DRAWABLE_MESH_FACE_SELECTION_UTILS_SH

// Bit-packed face selection buffer: bit 1 = selected, bit 0 = not selected.
// Each uint holds 32 face selection bits (MSB first: bit 31 = face 32*i).
BUFFER_RO(face_selected, uint, 6);

bool isFaceSelected(uint faceId)
{
    uint idx  = faceId / 32u;
    uint mask = 0x1u << (31u - (faceId % 32u));
    return (face_selected[idx] & mask) != 0u;
}

#endif // VCL_BGFX_DRAWABLE_DRAWABLE_MESH_FACE_SELECTION_UTILS_SH
