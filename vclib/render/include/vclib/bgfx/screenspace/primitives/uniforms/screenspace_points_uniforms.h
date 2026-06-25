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
    // .x = point width in pixels
    // .y = general color
    // .z = unused
    // .w = unused
    inline static std::array<float, 4> sPointsSettings =
        {1.0f, 0.0f, 0.0f, 0.0f};
    inline static Uniform sPointsSettingsUniform;

public:
    ScreenSpacePointsUniforms() = delete;

    /**
     * @brief Sets the width of the points.
     * @param width The point width in pixels.
     */
    static void setWidth(float width) { sPointsSettings[0] = width; }

    /**
     * @brief Sets the general color for points.
     * @param color The uniform color to apply when per-vertex colors are not
     * used.
     */
    static void setGeneralColor(const vcl::Color& color)
    {
        sPointsSettings[1] = std::bit_cast<float>(color.abgr());
    }

    /**
     * @brief Binds the uniform to the current bgfx context.
     *
     * Lazily initializes the bgfx uniform handle if it hasn't been created yet.
     */
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
