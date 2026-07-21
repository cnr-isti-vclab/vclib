// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_SH
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_SH

uniform vec4 u_meshColor;
uniform vec4 u_meshData; // x: meshId; y: first chunk index

#define u_meshId floatBitsToUint(u_meshData.x)
#define u_firstChunkPrimitiveID floatBitsToUint(u_meshData.y)

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_MESH_UNIFORMS_SH
