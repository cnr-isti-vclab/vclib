// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_MACROS_H
#define VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_MACROS_H

/**
 * These macros are used both on the library and on the shader side.
 */

/* Streams */

#define VCL_MRB_VERTEX_POSITION_STREAM 0
#define VCL_MRB_VERTEX_NORMAL_STREAM   1
#define VCL_MRB_VERTEX_COLOR_STREAM    2
#define VCL_MRB_VERTEX_TEXCOORD_STREAM 3

/* Stages */

// first 10 stages for 2d textures, then 3 for cubemaps, and 2 buffers
#define VCL_MRB_MAX_TEXTURES 10

#define VCL_MRB_CUBEMAP0 10
#define VCL_MRB_CUBEMAP1 11
#define VCL_MRB_CUBEMAP2 12

#define VCL_MRB_PRIMITIVE_COLOR_BUFFER  13
#define VCL_MRB_PRIMITIVE_NORMAL_BUFFER 14
// don't use stage 15 - it does not work on mac...

/* Flags - PBR */

// bit positions used to tell which features are enabled at shader level
#define VCL_PBR_VERTEX_COLOR         0
#define VCL_PBR_VERTEX_TANGENT       1
#define VCL_PBR_IS_ALPHA_MODE_MASK   2
#define VCL_PBR_IMAGE_BASED_LIGHTING 3

/* Constants - Compute (just for selection at the moment) */

#define VCL_COMPUTE_THREAD_COUNT_X 64
#define VCL_COMPUTE_THREAD_COUNT_Y 1
#define VCL_COMPUTE_THREAD_COUNT_Z 1

#endif // VCL_BGFX_DRAWABLE_MESH_MESH_RENDER_BUFFERS_MACROS_H
