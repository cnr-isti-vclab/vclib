/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH

uniform vec4 u_mrsDrawPack;
uniform vec4 u_mrsColorPack;

#define u_primitiveFloat u_mrsDrawPack.x
#define u_drawModeFloat u_mrsDrawPack.y
#define u_pointWidth u_mrsDrawPack.z

#define u_userPointColorFloat u_mrsColorPack.x
#define u_userSurfaceColorFloat u_mrsColorPack.y
#define u_userWireframeColorFloat u_mrsColorPack.z

#endif // VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_SH
