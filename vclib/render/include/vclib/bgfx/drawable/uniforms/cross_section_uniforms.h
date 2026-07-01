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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_CROSS_SECTION_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_CROSS_SECTION_UNIFORMS_H

#include <vclib/bgfx/uniform.h>

#include <vclib/space/core.h>

namespace vcl {

class CrossSectionUniforms
{
    inline static std::array<float, 4> sCrossSectionMinData = {
        Point3f::min().x(), // x min
        Point3f::min().y(), // y min
        Point3f::min().z(), // z min
        0.0                 // per-fragment flag
                            // (0.0 = per-vertex, 1.0 = per-fragment)
    };

    inline static std::array<float, 4> sCrossSectionMaxData = {
        Point3f::max().x(), // x max
        Point3f::max().y(), // y max
        Point3f::max().z(), // z max
        0.0                 // unused
    };

    inline static Uniform sCrossSectionMinDataUniform;
    inline static Uniform sCrossSectionMaxDataUniform;

public:
    CrossSectionUniforms() = delete;

    static void set(
        const Point3f& minPoint    = Point3f::min(),
        const Point3f& maxPoint    = Point3f::max(),
        bool           perFragment = false)
    {
        sCrossSectionMinData[0] = minPoint.x();
        sCrossSectionMinData[1] = minPoint.y();
        sCrossSectionMinData[2] = minPoint.z();
        sCrossSectionMinData[3] = float(perFragment);

        sCrossSectionMaxData[0] = maxPoint.x();
        sCrossSectionMaxData[1] = maxPoint.y();
        sCrossSectionMaxData[2] = maxPoint.z();
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sCrossSectionMinDataUniform.isValid())
            sCrossSectionMinDataUniform =
                Uniform("u_crossSectionMinData", bgfx::UniformType::Vec4);
        if (!sCrossSectionMaxDataUniform.isValid())
            sCrossSectionMaxDataUniform =
                Uniform("u_crossSectionMaxData", bgfx::UniformType::Vec4);
        sCrossSectionMinDataUniform.bind(sCrossSectionMinData.data());
        sCrossSectionMaxDataUniform.bind(sCrossSectionMaxData.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_CROSS_SECTION_UNIFORMS_H
