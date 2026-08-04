// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_UNIFORMS_LINES_UNIFORMS_H
#define VCL_BGFX_PRIMITIVES_UNIFORMS_LINES_UNIFORMS_H

#include <vclib/bgfx/static_uniform.h>

#include <vclib/space/core.h>

#include <array>
#include <bit>

namespace vcl {

/**
 * @brief Manages the uniform variables required by the Lines shaders.
 */
class LinesUniforms
{
    // .x = line width in pixels
    // .y = general color
    // .z = depth offset
    // .w = id
    inline static std::array<float, 4> sLinesSettings =
        {1.0f, 0.0f, 0.0f, 0.0f};

    inline static StaticUniform sLinesSettingsUniform {
        "u_linesSettings",
        bgfx::UniformType::Vec4};

public:
    LinesUniforms() = delete;

    /**
     * @brief Sets the width of the lines.
     * @param width The line width in pixels.
     */
    static void setWidth(float width) { sLinesSettings[0] = width; }

    /**
     * @brief Sets the general color for lines.
     * @param color The uniform color to apply when per-vertex colors are not
     * used.
     */
    static void setGeneralColor(const vcl::Color& color)
    {
        sLinesSettings[1] = std::bit_cast<float>(color.abgr());
    }

    /**
     * @brief Sets the depth offset for lines.
     * @param offset The depth offset value.
     */
    static void setDepthOffset(float offset) { sLinesSettings[2] = offset; }

    /**
     * @brief Sets the ID for picking.
     * @param id The ID.
     */
    static void setId(uint32_t id)
    {
        sLinesSettings[3] = std::bit_cast<float>(id);
    }

    /**
     * @brief Binds the uniform to the current bgfx context.
     *
     * Lazily initializes the bgfx uniform handle if it hasn't been created yet.
     */
    static void bind() { sLinesSettingsUniform.bind(sLinesSettings.data()); }
};

} // namespace vcl

#endif // VCL_BGFX_PRIMITIVES_UNIFORMS_LINES_UNIFORMS_H
