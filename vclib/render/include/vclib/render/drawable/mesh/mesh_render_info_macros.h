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

#ifndef VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_MACROS_H
#define VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_MACROS_H

/**
 * These macros are used both on the library and on the shader side.
 */

// points
#define VCL_MRS_DRAW_POINTS         0 // point visibility
#define VCL_MRS_POINTS_PIXEL        1 // draw points as pixels
#define VCL_MRS_POINTS_CIRCLE       2 // draw points as circles
#define VCL_MRS_POINTS_SHADING_NONE 3 // no shading
#define VCL_MRS_POINTS_SHADING_VERT 4 // vertex normal shading
#define VCL_MRS_POINTS_COLOR_VERTEX 5 // vert colors for points
#define VCL_MRS_POINTS_COLOR_MESH   6 // mesh color for points
#define VCL_MRS_POINTS_COLOR_USER   7 // user color for points

// surface
#define VCL_MRS_DRAW_SURF           0 // surface visibility
#define VCL_MRS_SURF_SHADING_NONE   1 // no shading
#define VCL_MRS_SURF_SHADING_FLAT   2 // flat shading
#define VCL_MRS_SURF_SHADING_SMOOTH 3 // smooth shading
#define VCL_MRS_SURF_COLOR_VERTEX   4 // vert color for surface
#define VCL_MRS_SURF_COLOR_FACE     5 // face color for surface
#define VCL_MRS_SURF_TEX_VERTEX     6 // per vertex texcoords
#define VCL_MRS_SURF_TEX_WEDGE      7 // per wedge texcoords
#define VCL_MRS_SURF_COLOR_MESH     8 // mesh color for surface
#define VCL_MRS_SURF_COLOR_USER     9 // user color for surface

// wireframe
#define VCL_MRS_DRAW_WIREFRAME         0 // draw wireframe
#define VCL_MRS_WIREFRAME_SHADING_NONE 1 // no shading
#define VCL_MRS_WIREFRAME_SHADING_VERT 2 // vertex normal shading
#define VCL_MRS_WIREFRAME_COLOR_VERT   3 // vert color for wireframe
#define VCL_MRS_WIREFRAME_COLOR_MESH   4 // mesh color for wireframe
#define VCL_MRS_WIREFRAME_COLOR_USER   5 // user color for wireframe

// edges
#define VCL_MRS_DRAW_EDGES           0 // draw edges
#define VCL_MRS_EDGES_SHADING_NONE   1 // no shading
#define VCL_MRS_EDGES_SHADING_FLAT   2 // edge normal shading
#define VCL_MRS_EDGES_SHADING_SMOOTH 3 // vertex normal shading
#define VCL_MRS_EDGES_COLOR_VERTEX   4 // vert color for edges
#define VCL_MRS_EDGES_COLOR_EDGE     5 // edge color for edges
#define VCL_MRS_EDGES_COLOR_MESH     6 // mesh color for edges
#define VCL_MRS_EDGES_COLOR_USER     7 // user color for edges

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_MACROS_H
