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

#ifndef VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH

uniform vec4 u_mrsDrawPack;
uniform vec4 u_mrsWidthPack;
uniform vec4 u_mrsColorPack;

#define u_primitiveFloat u_mrsDrawPack.x
#define u_drawMode0Float u_mrsDrawPack.y
#define u_drawMode1Float u_mrsDrawPack.z

#define u_pointWidth u_mrsWidthPack.x
#define u_wireframeWidth u_mrsWidthPack.y
#define u_edgesWidth u_mrsWidthPack.z

#define u_userPointColorFloat u_mrsColorPack.x
#define u_userSurfaceColorFloat u_mrsColorPack.y
#define u_userWireframeColorFloat u_mrsColorPack.z
#define u_userEdgesColorFloat u_mrsColorPack.w

#endif // VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
