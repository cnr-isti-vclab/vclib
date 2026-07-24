// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$output v_color, v_normal

#include <bgfx_shader.sh>
#include <vclib/bgfx/shaders_common.sh>
#include <vclib/bgfx/primitives/uniforms/lines_uniforms.sh>

BUFFER_RO(vertexPosBuffer, vec4, 0);
DECLARE_FETCH_VEC3(getVertexPos, vertexPosBuffer)

#if LINES_COLOR_PER_VERTEX
BUFFER_RO(vertexColBuffer, uint, 1);
#endif

#if LINES_INDICES_ON
BUFFER_RO(indexBuffer, uint, 2);
#endif

#if LINES_COLOR_PER_LINE
BUFFER_RO(lineColBuffer, uint, 3);
#endif

#if LINES_SHADING_PER_VERTEX
BUFFER_RO(vertexNorBuffer, vec4, 4);
DECLARE_FETCH_VEC3(getVertexNor, vertexNorBuffer)
#endif

#if LINES_SHADING_PER_LINE
BUFFER_RO(lineNorBuffer, vec4, 5);
DECLARE_FETCH_VEC3(getLineNor, lineNorBuffer)
#endif

uint getVind(uint vind) {
#if LINES_INDICES_ON
    return indexBuffer[vind];
#else
    return vind;
#endif
}

#define NEAR_EPSILON 0.001
#define LENGTH_EPSILON 0.0001

void main() {
    uint lineIndex = gl_VertexID / 6u;
    uint localVertex = gl_VertexID % 6u;

#if LINES_TOPO_LINES
    uint vertexIndex0 = getVind(lineIndex * 2u);
    uint vertexIndex1 = getVind(lineIndex * 2u + 1u);
#else
    uint vertexIndex0 = getVind(lineIndex);
    uint vertexIndex1 = getVind(lineIndex + 1u);
#endif

    vec3 p0 = getVertexPos(vertexIndex0);
    vec3 p1 = getVertexPos(vertexIndex1);

    vec4 p0_NDC = mul(u_modelViewProj, vec4(p0, 1.0));
    vec4 p1_NDC = mul(u_modelViewProj, vec4(p1, 1.0));

#if LINES_SHADING_PER_VERTEX
    vec3 n0 = getVertexNor(vertexIndex0);
    vec3 n1 = getVertexNor(vertexIndex1);
    vec3 n0_NDC = normalize(mul(u_normalMatrix, n0));
    vec3 n1_NDC = normalize(mul(u_normalMatrix, n1));
#elif LINES_SHADING_PER_LINE
    vec3 n = getLineNor(lineIndex);
    vec3 n_NDC = normalize(mul(u_normalMatrix, n));
    vec3 n0_NDC = n_NDC;
    vec3 n1_NDC = n_NDC;
#else
    vec3 n0_NDC = vec3_splat(0.0);
    vec3 n1_NDC = vec3_splat(0.0);
#endif

#if LINES_COLOR_PER_VERTEX
    vec4 c0 = uintABGRToVec4Color(vertexColBuffer[vertexIndex0]);
    vec4 c1 = uintABGRToVec4Color(vertexColBuffer[vertexIndex1]);
#elif LINES_COLOR_PER_LINE
    vec4 c_line = uintABGRToVec4Color(lineColBuffer[lineIndex]);
    vec4 c0 = c_line;
    vec4 c1 = c_line;
#else
    vec4 c0 = u_linesGeneralColor;
    vec4 c1 = u_linesGeneralColor;
#endif

    vec4 clippedP0 = p0_NDC;
    vec4 clippedP1 = p1_NDC;
    vec4 clippedColor0 = c0;
    vec4 clippedColor1 = c1;
    vec3 clippedNormal0 = n0_NDC;
    vec3 clippedNormal1 = n1_NDC;

    if (p0_NDC.w < NEAR_EPSILON) {
        float t = (NEAR_EPSILON - p0_NDC.w) / (p1_NDC.w - p0_NDC.w);
        clippedP0 = mix(p0_NDC, p1_NDC, t);
        clippedColor0 = mix(c0, c1, t);
        clippedNormal0 = mix(n0_NDC, n1_NDC, t);
    }
    if (p1_NDC.w < NEAR_EPSILON) {
        float t = (NEAR_EPSILON - p1_NDC.w) / (p0_NDC.w - p1_NDC.w);
        clippedP1 = mix(p1_NDC, p0_NDC, t);
        clippedColor1 = mix(c1, c0, t);
        clippedNormal1 = mix(n1_NDC, n0_NDC, t);
    }

    const vec2 offsets[6] = {
        vec2(-1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0, -1.0),
        vec2( 1.0, -1.0), vec2(-1.0,  1.0), vec2( 1.0,  1.0)
    };
    vec2 offset = offsets[localVertex];
    vec2 uv = offset * 0.5 + 0.5;

    vec4 p = mix(clippedP0, clippedP1, uv.x);
    vec4 color = mix(clippedColor0, clippedColor1, uv.x);
    vec3 normal = mix(clippedNormal0, clippedNormal1, uv.x);

    vec2 ndp0 = clippedP0.xy / clippedP0.w;
    vec2 ndp1 = clippedP1.xy / clippedP1.w;
    vec2 screenDir = ndp1 - ndp0;
    float screenDirLen = length(screenDir);

    if (screenDirLen > LENGTH_EPSILON) {
        vec2 T = screenDir / screenDirLen;
        vec2 N = vec2(-T.y, T.x);
        
        float pixelsToNDCX = 2.0 / u_viewRect.z;
        float pixelsToNDCY = 2.0 / u_viewRect.w;
        
        float side = 2.0 * uv.y - 1.0;
        vec2 offsetNDC = N * (0.5 * u_linesWidth) * side * vec2(pixelsToNDCX, pixelsToNDCY);
        
        p.xy = p.xy + (offsetNDC * p.w);
    }

    v_color = color;
    v_normal = normal;

    // Apply depth offset in clip space.
    // We scale the offset by w to maintain it consistently after the perspective divide.
    p.z += -u_depthOffset * p.w;
    gl_Position = p;
}
