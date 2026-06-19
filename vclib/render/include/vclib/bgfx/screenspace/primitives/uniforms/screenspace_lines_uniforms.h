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

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_H
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_H

#include <vclib/bgfx/uniform.h>

#include <vclib/space/core.h>

#include <array>
#include <bit>

namespace vcl {

class ScreenSpaceLinesUniforms
{
    inline static std::array<float, 4> sLinesSettings;
    inline static Uniform              sLinesSettingsUniform;

public:
    ScreenSpaceLinesUniforms() = delete;

    static void setWidth(float w) { sLinesSettings[0] = w; }

    static void setColorSetting(uint c)
    {
        sLinesSettings[2] = std::bit_cast<float>(c);
    }

    static void setGeneralColor(const vcl::Color& c)
    {
        sLinesSettings[3] = std::bit_cast<float>(c.abgr());
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sLinesSettingsUniform.isValid())
            sLinesSettingsUniform =
                Uniform("u_linesSettings", bgfx::UniformType::Vec4);
        sLinesSettingsUniform.bind(sLinesSettings.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_H
