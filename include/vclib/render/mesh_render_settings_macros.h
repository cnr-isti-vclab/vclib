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

#define VCL_MRS_DRAW_MESH 1 << 0 // mesh visibility

// points
#define VCL_MRS_DRAW_POINTS              1 << 1 // point visibility
#define VCL_MRS_DRAW_POINTS_PIXEL        1 << 2 // draw points as pixels
#define VCL_MRS_DRAW_POINTS_CIRCLE       1 << 3 // draw points as circles
#define VCL_MRS_DRAW_POINTS_COLOR_VERTEX 1 << 4 // use vertex colors for points
#define VCL_MRS_DRAW_POINTS_COLOR_MESH   1 << 5 // use mesh color for points
#define VCL_MRS_DRAW_POINTS_COLOR_USER   1 << 6 // use user color for points

// surface
#define VCL_MRS_DRAW_SURF              1 << 10 // surface visibility
#define VCL_MRS_DRAW_SURF_FLAT         1 << 11 // flat shading
#define VCL_MRS_DRAW_SURF_SMOOTH       1 << 12 // smooth shading
#define VCL_MRS_DRAW_SURF_COLOR_FACE   1 << 13 // use face color for surface
#define VCL_MRS_DRAW_SURF_COLOR_VERTEX 1 << 14 // use vertex color for surface
#define VCL_MRS_DRAW_SURF_COLOR_MESH   1 << 15 // use mesh color for surface
#define VCL_MRS_DRAW_SURF_COLOR_USER   1 << 16 // use user color for surface
#define VCL_MRS_DRAW_SURF_TEX_VERTEX   1 << 17 // use per vertex texcoords
#define VCL_MRS_DRAW_SURF_TEX_WEDGE    1 << 18 // use per wedge texcoords

// wireframe
#define VCL_MRS_DRAW_WIREFRAME            1 << 20 // draw wireframe
#define VCL_MRS_DRAW_WIREFRAME_COLOR_USER 1 << 21 // user color for wireframe
#define VCL_MRS_DRAW_WIREFRAME_COLOR_MESH 1 << 22 // mesh color for wireframe

// bounding box
#define VCL_MRS_DRAW_BOUNDINGBOX 1 << 25

#endif // VCL_RENDER_MESH_RENDER_SETTINGS_MACROS_H
