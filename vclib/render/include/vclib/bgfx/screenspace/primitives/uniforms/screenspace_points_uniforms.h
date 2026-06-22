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

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_POINTS_UNIFORMS_H
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_POINTS_UNIFORMS_H

#include <vclib/bgfx/uniform.h>

#include <vclib/space/core.h>

#include <array>
#include <bit>

namespace vcl {

class ScreenSpacePointsUniforms
{
    inline static std::array<float, 4> sPointsSettings;
    inline static Uniform              sPointsSettingsUniform;

public:
    ScreenSpacePointsUniforms() = delete;

    static void setColorSetting(uint c)
    {
        sPointsSettings[0] = std::bit_cast<float>(c);
    }

    static void setShape(uint s)
    {
        sPointsSettings[1] = std::bit_cast<float>(s);
    }

    static void setWidth(float w) { sPointsSettings[2] = w; }

    static void setGeneralColor(const vcl::Color& c)
    {
        sPointsSettings[3] = std::bit_cast<float>(c.abgr());
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sPointsSettingsUniform.isValid())
            sPointsSettingsUniform =
                Uniform("u_pointsSettings", bgfx::UniformType::Vec4);
        sPointsSettingsUniform.bind(sPointsSettings.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_POINTS_UNIFORMS_H
