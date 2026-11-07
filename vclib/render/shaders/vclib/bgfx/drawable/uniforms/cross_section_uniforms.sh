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

#ifndef VCL_EXT_BGFX_UNIFORMS_CROSS_SECTION_UNIFORMS_SH
#define VCL_EXT_BGFX_UNIFORMS_CROSS_SECTION_UNIFORMS_SH

uniform vec4 u_crossSectionMinData;
uniform vec4 u_crossSectionMaxData;

#define u_crossSectionPerFragment bool(u_crossSectionMinData.w != 0.0)

#define u_crossSectionMinX u_crossSectionMinData.x
#define u_crossSectionMinY u_crossSectionMinData.y
#define u_crossSectionMinZ u_crossSectionMinData.z

#define u_crossSectionMaxX u_crossSectionMaxData.x
#define u_crossSectionMaxY u_crossSectionMaxData.y
#define u_crossSectionMaxZ u_crossSectionMaxData.z

/**
 * Given the world position of the vertex, compute and returns the discard flag
 * for the cross section test (1.0 if vertex should be discarded, 0.0 otherwise)
 *
 * This macro is meant to be used in the vertex shader. The result of this test
 * can be passed to the fragment shader as a varying, so that the fragment
 * shader can also discard fragments based on the same test
 *
 * pos: the world position of the vertex (vec3)
 * returns: the discard flag (float)
 */
#define computeDiscardFlag(pos) ( \
    (!u_crossSectionPerFragment && ( \
        (pos).x < u_crossSectionMinX || (pos).x > u_crossSectionMaxX || \
        (pos).y < u_crossSectionMinY || (pos).y > u_crossSectionMaxY || \
        (pos).z < u_crossSectionMinZ || (pos).z > u_crossSectionMaxZ \
    )) ? 1.0 : 0.0 \
)

/**
 * Given the discard flag computed by computeDiscardFlag and the world position
 * of the fragment, discard the fragment if the flag is greater than 0.0 or if
 * the fragment is outside the cross section bounds (if
 * u_crossSectionPerFragment is true).
 *
 * This macro is meant to be used in the fragment shader.
 *
 * flag: the discard flag computed by computeDiscardFlag and passed as a varying
 *       from the vertex shader (float)
 * pos: the world position of the fragment (vec3)
 */
#define discardIfCrossSectionClipped(flag, pos) \
    if (flag > 0.0) discard; \
    else if (u_crossSectionPerFragment && ( \
        (pos).x < u_crossSectionMinX || (pos).x > u_crossSectionMaxX || \
        (pos).y < u_crossSectionMinY || (pos).y > u_crossSectionMaxY || \
        (pos).z < u_crossSectionMinZ || (pos).z > u_crossSectionMaxZ \
    )) discard;

#endif // VCL_EXT_BGFX_UNIFORMS_CROSS_SECTION_UNIFORMS_SH
