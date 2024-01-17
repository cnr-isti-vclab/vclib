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

#ifndef VCL_RENDER_MESH_RENDER_SETTINGS_MACROS_H
#define VCL_RENDER_MESH_RENDER_SETTINGS_MACROS_H

/**
 * These macros are used both on the library and on the shader side.
 */

#define VCL_MRS_PRIMITIVE_POINTS    uint(1 << 0)
#define VCL_MRS_PRIMITIVE_LINES     uint(1 << 1)
#define VCL_MRS_PRIMITIVE_TRIANGLES uint(1 << 2)

#define VCL_MRS_DRAW_MESH uint(1 << 0) // mesh visibility

// points
#define VCL_MRS_DRAW_POINTS              uint(1 << 1) // point visibility
#define VCL_MRS_DRAW_POINTS_PIXEL        uint(1 << 2) // draw points as pixels
#define VCL_MRS_DRAW_POINTS_CIRCLE       uint(1 << 3) // draw points as circles
#define VCL_MRS_DRAW_POINTS_SHADING_NONE uint(1 << 4) // no shading
#define VCL_MRS_DRAW_POINTS_SHADING_VERT uint(1 << 5) // vertex normal shading
#define VCL_MRS_DRAW_POINTS_COLOR_VERTEX uint(1 << 6) // vert colors for points
#define VCL_MRS_DRAW_POINTS_COLOR_MESH   uint(1 << 7) // mesh color for points
#define VCL_MRS_DRAW_POINTS_COLOR_USER   uint(1 << 8) // user color for points

// surface
#define VCL_MRS_DRAW_SURF                uint(1 << 10) // surface visibility
#define VCL_MRS_DRAW_SURF_SHADING_NONE   uint(1 << 11) // no shading
#define VCL_MRS_DRAW_SURF_SHADING_FLAT   uint(1 << 12) // flat shading
#define VCL_MRS_DRAW_SURF_SHADING_SMOOTH uint(1 << 13) // smooth shading
#define VCL_MRS_DRAW_SURF_COLOR_FACE     uint(1 << 14) // face color for surface
#define VCL_MRS_DRAW_SURF_COLOR_VERTEX   uint(1 << 15) // vert color for surface
#define VCL_MRS_DRAW_SURF_COLOR_MESH     uint(1 << 16) // mesh color for surface
#define VCL_MRS_DRAW_SURF_COLOR_USER     uint(1 << 17) // user color for surface
#define VCL_MRS_DRAW_SURF_TEX_VERTEX     uint(1 << 18) // per vertex texcoords
#define VCL_MRS_DRAW_SURF_TEX_WEDGE      uint(1 << 19) // per wedge texcoords

// wireframe
#define VCL_MRS_DRAW_WIREFRAME            uint(1 << 20) // draw wireframe
#define VCL_MRS_DRAW_WIREFRAME_COLOR_USER uint(1 << 21) // user color for wrfrm
#define VCL_MRS_DRAW_WIREFRAME_COLOR_MESH uint(1 << 22) // mesh color for wrfrm

// bounding box - TODO: remove this
#define VCL_MRS_DRAW_BOUNDINGBOX uint(1 << 25) // bounding box visibility

#endif // VCL_RENDER_MESH_RENDER_SETTINGS_MACROS_H
