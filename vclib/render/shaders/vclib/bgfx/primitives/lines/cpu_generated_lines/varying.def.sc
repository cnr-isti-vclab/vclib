// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

vec3 a_position  : POSITION;  // p0 
vec3 a_texcoord0 : TEXCOORD0; // p1
vec4 a_color0    : COLOR0;    // c0
vec4 a_color1    : COLOR1;    // c1
vec3 a_normal    : NORMAL;    // n0
vec3 a_texcoord1 : TEXCOORD1; // n1

vec4 v_color  : COLOR0;
vec3 v_normal : NORMAL;

vec3  v_worldPos0   : TEXCOORD2; // unused texcoord semantic
vec3  v_worldPos1   : TEXCOORD3; // unused texcoord semantic
float v_discardFlag : TEXCOORD4; // unused texcoord semantic
float v_t           : TEXCOORD5; // interpolation parameter along segment
