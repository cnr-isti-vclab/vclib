// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_SH
#define VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_SH

uniform vec4 u_baseColorFactor;
uniform vec4 u_FactorsPack;
uniform vec4 u_emissiveAlphaCutoffPack;
uniform vec4 u_settings;

#define u_occlusionStrength u_FactorsPack.r
#define u_roughnessFactor u_FactorsPack.g
#define u_metallicFactor u_FactorsPack.b
#define u_normalScale u_FactorsPack.a

#define u_emissiveFactor u_emissiveAlphaCutoffPack.rgb
#define u_alphaCutoff u_emissiveAlphaCutoffPack.a

#define u_pbr_settings floatBitsToUint(u_settings.x)

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MATERIAL_UNIFORMS_SH
