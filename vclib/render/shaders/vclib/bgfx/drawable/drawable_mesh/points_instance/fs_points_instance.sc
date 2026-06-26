// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

$input v_normal, v_texcoord1

#include <vclib/bgfx/drawable/drawable_mesh/uniforms.sh>
#include <vclib/bgfx/drawable/mesh/mesh_render_buffers_macros.h>

/*
TODO: when https://github.com/bkaradzic/bgfx/issues/3629 will be resolved,
restore next line with:

BUFFER_RO(vertexColors, uint, VCL_MRB_VERTEX_COLOR_STREAM); // colors (rgba as float bits)
*/
BUFFER_RO(vertexColors, uint, 2); // colors (rgba as float bits)

void main()
{
    // defaul color and light
    vec4 light = vec4(1, 1, 1, 1);
    vec4 color = uintABGRToVec4Color(floatBitsToUint(u_userPointColorFloat));

    // circle mode (if outside of the circle, discard)
    bool isCircle = bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_CIRCLE));
    bool isSphere = bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SPHERE));
    if (isCircle || isSphere) {
        vec2 uv = v_texcoord1 * 2.0 - vec2(1.0, 1.0);
        if (length(uv) > 1.0) {
            discard;
        }
        if (isSphere) {
            // sphere mode
            v_normal = normalize(vec3(uv.x, uv.y, sqrt(1.0 - dot(uv, uv))));
        }
    }
    
    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_SHADING_VERT))) {
        light = computeLight(u_lightDir, u_lightColor, v_normal);
    }

    if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_VERTEX))) {
        uint pointId = uint(gl_PrimitiveID) / 2u;
        color = uintABGRToVec4Color(vertexColors[pointId]);
    }
    else if (bool(u_pointsMode & posToBitFlag(VCL_MRS_POINTS_COLOR_MESH))) {
        color = u_meshColor;
    }

    // NO depth writing (it kills performance)
    gl_FragColor = light * color; // + vec4(specular, 0);
}
