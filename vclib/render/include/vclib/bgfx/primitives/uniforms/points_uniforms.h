// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_PRIMITIVES_UNIFORMS_POINTS_UNIFORMS_H
#define VCL_BGFX_PRIMITIVES_UNIFORMS_POINTS_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/space/core.h>

#include <array>
#include <bit>

namespace vcl {

/**
 * @brief Manages the uniform variables required by the Points shaders.
 *
 * This utility class is responsible for packing points rendering settings
 * (such as width and general color) into a vec4 uniform and binding it
 * to the bgfx rendering context.
 */
class PointsUniforms
{
    // .x = point width in pixels
    // .y = general color
    // .z = depth offset
    // .w = unused
    inline static std::array<float, 4> sPointsSettings =
        {1.0f, 0.0f, 0.0f, 0.0f};
    inline static Uniform sPointsSettingsUniform;

    inline static std::array<float, 4> sSelectionSettings =
        {0.0f, 0.0f, 0.0f, 0.0f};
    inline static Uniform sSelectionSettingsUniform;

public:
    PointsUniforms() = delete;

    /**
     * @brief Sets the width of the points.
     * @param[in] width: The point width in pixels.
     */
    static void setWidth(float width) { sPointsSettings[0] = width; }

    /**
     * @brief Sets the general color for points.
     * @param[in] color: The uniform color to apply when per-vertex colors are
     * not used.
     */
    static void setGeneralColor(const vcl::Color& color)
    {
        sPointsSettings[1] = std::bit_cast<float>(color.abgr());
    }

    /**
     * @brief Sets the depth offset for points.
     * @param[in] depthOffset: The depth offset value.
     */
    static void setDepthOffset(float depthOffset)
    {
        sPointsSettings[2] = depthOffset;
    }

    /**
     * @brief Sets the object ID for points.
     * @param[in] id: The object ID.
     */
    static void setId(uint32_t id)
    {
        sPointsSettings[3] = std::bit_cast<float>(id);
    }

    /**
     * @brief Sets the selection color for points.
     * @param[in] color: The uniform selection color.
     */
    static void setSelectionColor(const vcl::Color& color)
    {
        sSelectionSettings[0] = std::bit_cast<float>(color.abgr());
    }

    /**
     * @brief Binds the uniform to the current bgfx context.
     *
     * Lazily initializes the bgfx uniform handle if it hasn't been created yet.
     */
    static void bind()
    {
        // Lazy initialization to avoid creating uniforms before bgfx is
        // initialized.
        if (!sPointsSettingsUniform.isValid())
            sPointsSettingsUniform =
                Uniform("u_pointsSettings", bgfx::UniformType::Vec4);
        if (!sSelectionSettingsUniform.isValid())
            sSelectionSettingsUniform =
                Uniform("u_pointsSelectionSettings", bgfx::UniformType::Vec4);

        sPointsSettingsUniform.bind(sPointsSettings.data());
        sSelectionSettingsUniform.bind(sSelectionSettings.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_PRIMITIVES_UNIFORMS_POINTS_UNIFORMS_H
