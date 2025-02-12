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

/* uint mDrawMode0 */

#define VCL_MRS_DRAW_MESH uint(1 << 0) // mesh visibility

// points
#define VCL_MRS_DRAW_POINTS         uint(1 << 1) // point visibility
#define VCL_MRS_POINTS_PIXEL        uint(1 << 2) // draw points as pixels
#define VCL_MRS_POINTS_CIRCLE       uint(1 << 3) // draw points as circles
#define VCL_MRS_POINTS_SHADING_NONE uint(1 << 4) // no shading
#define VCL_MRS_POINTS_SHADING_VERT uint(1 << 5) // vertex normal shading
#define VCL_MRS_POINTS_COLOR_VERTEX uint(1 << 6) // vert colors for points
#define VCL_MRS_POINTS_COLOR_MESH   uint(1 << 7) // mesh color for points
#define VCL_MRS_POINTS_COLOR_USER   uint(1 << 8) // user color for points

// surface
#define VCL_MRS_DRAW_SURF           uint(1 << 10) // surface visibility
#define VCL_MRS_SURF_SHADING_NONE   uint(1 << 11) // no shading
#define VCL_MRS_SURF_SHADING_FLAT   uint(1 << 12) // flat shading
#define VCL_MRS_SURF_SHADING_SMOOTH uint(1 << 13) // smooth shading
#define VCL_MRS_SURF_COLOR_FACE     uint(1 << 14) // face color for surface
#define VCL_MRS_SURF_COLOR_VERTEX   uint(1 << 15) // vert color for surface
#define VCL_MRS_SURF_COLOR_MESH     uint(1 << 16) // mesh color for surface
#define VCL_MRS_SURF_COLOR_USER     uint(1 << 17) // user color for surface
#define VCL_MRS_SURF_TEX_VERTEX     uint(1 << 18) // per vertex texcoords
#define VCL_MRS_SURF_TEX_WEDGE      uint(1 << 19) // per wedge texcoords

// wireframe
#define VCL_MRS_DRAW_WIREFRAME         uint(1 << 20) // draw wireframe
#define VCL_MRS_WIREFRAME_SHADING_NONE uint(1 << 21) // no shading
#define VCL_MRS_WIREFRAME_SHADING_VERT uint(1 << 22) // vertex normal shading
#define VCL_MRS_WIREFRAME_COLOR_VERT   uint(1 << 23) // vert color for wireframe
#define VCL_MRS_WIREFRAME_COLOR_USER   uint(1 << 24) // user color for wireframe
#define VCL_MRS_WIREFRAME_COLOR_MESH   uint(1 << 25) // mesh color for wireframe

// bounding box - TODO: remove this
#define VCL_MRS_DRAW_BOUNDINGBOX uint(1 << 31) // bounding box visibility

/* uint mDrawMode1 */

// edges
#define VCL_MRS_DRAW_EDGES           uint(1 << 0) // draw edges
#define VCL_MRS_EDGES_SHADING_NONE   uint(1 << 1) // no shading
#define VCL_MRS_EDGES_SHADING_FLAT   uint(1 << 2) // edge normal shading
#define VCL_MRS_EDGES_SHADING_SMOOTH uint(1 << 3) // vertex normal shading
#define VCL_MRS_EDGES_COLOR_VERTEX   uint(1 << 4) // vert color for edges
#define VCL_MRS_EDGES_COLOR_EDGE     uint(1 << 5) // edge color for edges
#define VCL_MRS_EDGES_COLOR_USER     uint(1 << 6) // user color for edges
#define VCL_MRS_EDGES_COLOR_MESH     uint(1 << 7) // mesh color for edges

#endif // VCL_RENDER_DRAWABLE_MESH_MESH_RENDER_INFO_MACROS_H
