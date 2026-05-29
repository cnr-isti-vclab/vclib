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

#ifndef VCL_BGFX_SELECTION_UNIFORMS_SH
#define VCL_BGFX_SELECTION_UNIFORMS_SH

#include <vclib/bgfx/shaders_common.sh>

uniform vec4 u_selectionBox; // screen space

uniform vec4 u_workgroupSizeAndCount;

uniform vec4 u_meshIdData;

#define u_workgroupSizeX floatBitsToUint(u_workgroupSizeAndCount.x)
#define u_workgroupSizeY floatBitsToUint(u_workgroupSizeAndCount.y)
#define u_workgroupSizeZ floatBitsToUint(u_workgroupSizeAndCount.z)
#define u_primitiveCount floatBitsToUint(u_workgroupSizeAndCount.w)

#define u_meshID floatBitsToUint(u_meshIdData.x)

uint getPrimitiveID(vec3 workGroupID)
{
    return workGroupID.x +
        u_workgroupSizeX * workGroupID.y +
        u_workgroupSizeX * u_workgroupSizeY * workGroupID.z;
}

#endif // VCL_BGFX_SELECTION_UNIFORMS_SH
