// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

vec4 v_color : COLOR0;
vec3 v_normal : NORMAL;

vec3  v_worldPos0   : TEXCOORD2 = vec3(0.0, 0.0, 0.0);
vec3  v_worldPos1   : TEXCOORD3 = vec3(0.0, 0.0, 0.0);
float v_discardFlag : TEXCOORD4 = 0;
float v_t           : TEXCOORD5 = 0; // interpolation parameter along segment
