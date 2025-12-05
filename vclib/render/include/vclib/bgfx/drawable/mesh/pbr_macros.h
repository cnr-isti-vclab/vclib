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

#ifndef VCL_BGFX_DRAWABLE_MESH_PBR_MACROS_H
#define VCL_BGFX_DRAWABLE_MESH_PBR_MACROS_H

/**
 * These macros are used both on the library and on the shader side.
 */

#define VCL_PBR_VERTEX_COLOR       0
#define VCL_PBR_VERTEX_TANGENT     1
#define VCL_PBR_IS_ALPHA_MODE_MASK 2

#define VCL_PBR_TEXTURE_BASE_COLOR         0
#define VCL_PBR_TEXTURE_METALLIC_ROUGHNESS 1
#define VCL_PBR_TEXTURE_NORMAL             2
#define VCL_PBR_TEXTURE_OCCLUSION          3
#define VCL_PBR_TEXTURE_EMISSIVE           4

#endif // VCL_BGFX_DRAWABLE_MESH_PBR_MACROS_H
