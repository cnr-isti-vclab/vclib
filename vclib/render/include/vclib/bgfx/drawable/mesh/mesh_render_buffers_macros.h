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

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_MACROS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_MACROS_H

/**
 * These macros are used both on the library and on the shader side.
 */
#define VCL_MRB_VERTEX_POSITION_STREAM 0
#define VCL_MRB_VERTEX_NORMAL_STREAM   1
#define VCL_MRB_VERTEX_COLOR_STREAM    2
#define VCL_MRB_VERTEX_TEXCOORD_STREAM 3

#define VCL_MRB_TEXTURE0 0
#define VCL_MRB_TEXTURE1 1
#define VCL_MRB_TEXTURE2 2
#define VCL_MRB_TEXTURE3 3
#define VCL_MRB_TEXTURE4 4
#define VCL_MRB_TEXTURE5 5
#define VCL_MRB_TEXTURE6 6
#define VCL_MRB_TEXTURE7 7

#define VCL_MRB_CUBEMAP0 11
#define VCL_MRB_CUBEMAP1 12
#define VCL_MRB_CUBEMAP2 13

#define VCL_MRB_PRIMITIVE_COLOR_BUFFER  14
#define VCL_MRB_PRIMITIVE_NORMAL_BUFFER 15

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_MACROS_H
