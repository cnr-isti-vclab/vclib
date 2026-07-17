// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

vec3 a_position  : POSITION;
vec3 a_normal    : NORMAL;
vec4 a_color0    : COLOR0;

vec3  v_position  : TANGENT;
vec3  v_normal    : NORMAL;
vec4  v_color     : COLOR0;
float v_selected  : TEXCOORD0;

vec3  v_worldPos    : TEXCOORD1 = vec3(0.0, 0.0, 0.0);
float v_discardFlag : TEXCOORD2 = 0;
