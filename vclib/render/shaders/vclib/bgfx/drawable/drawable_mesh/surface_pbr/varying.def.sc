// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

vec3 a_position  : POSITION;
vec3 a_normal    : NORMAL;
vec4 a_tangent   : TANGENT;
vec4 a_color0    : COLOR0;
vec2 a_texcoord0 : TEXCOORD0;
vec2 a_texcoord1 : TEXCOORD1;

vec3 v_position  : TEXCOORD2; // unused texcoord semantic
vec3 v_normal    : NORMAL;
vec4 v_tangent   : TANGENT;
vec4 v_color     : COLOR0;
vec2 v_texcoord0 : TEXCOORD0;
vec2 v_texcoord1 : TEXCOORD1;

vec3  v_worldPos    : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
float v_discardFlag : TEXCOORD4 = 0;
