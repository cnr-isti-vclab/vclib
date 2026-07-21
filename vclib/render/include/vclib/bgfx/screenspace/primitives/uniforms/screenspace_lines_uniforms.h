// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_H
#define VCL_BGFX_SCREENSPACE_PRIMITIVES_UNIFORMS_SCREENSPACE_LINES_UNIFORMS_H

#include <vclib/bgfx/uniform.h>

#include <vclib/space/core.h>

#include <array>
#include <bit>

namespace vcl {

class ScreenSpaceLinesUniforms
{
    // .x = line width in pixels
    // .y = general color
    // .z = unused
    // .w = unused
    inline static std::array<float, 4> sLinesSettings = {0.f, 0.f, 0.f, 0.f};

    inline static Uniform sLinesSettingsUniform;

public:
    ScreenSpaceLinesUniforms() = delete;

    static void setWidth(float w) { sLinesSettings[0] = w; }

    static void setGeneralColor(const vcl::Color& c)
    {
        sLinesSettings[1] = std::bit_cast<float>(c.abgr());
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
