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

#ifndef VCL_EXT_BGFX_UNIFORMS_MATERIAL_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_MATERIAL_UNIFORMS_SH

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
#define u_pbr_texture_settings floatBitsToUint(u_settings.y)
#define u_tone_mapping floatBitsToUint(u_settings.z)
#define u_exposure u_settings.w

#endif // VCL_EXT_BGFX_UNIFORMS_MATERIAL_UNIFORMS_SH
