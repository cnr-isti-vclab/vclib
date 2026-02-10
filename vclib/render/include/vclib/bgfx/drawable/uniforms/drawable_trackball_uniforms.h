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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_TRACKBALL_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_TRACKBALL_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/space/core/color.h>

namespace vcl {

/**
 * @brief The DrawableTrackballUniforms class is responsible for managing the
 * shader uniforms related to a drawable trackball.
 *
 * It provides a static interface to set the uniform data based on the
 * current dragging status and to bind the uniforms to the shader programs.
 */
class DrawableTrackballUniforms
{
    static constexpr float DRAGGING_ALPHA     = 0.9f;
    static constexpr float NOT_DRAGGING_ALPHA = 0.5f;

    // the only component is alpha
    inline static std::array<float, 4> sTrackBallSettings =
        {NOT_DRAGGING_ALPHA, 0, 0, 0};

    inline static Uniform sTrackballSettingsUniform;

public:
    DrawableTrackballUniforms() = delete;

    static void setDragging(bool dragging)
    {
        sTrackBallSettings[0] = dragging ? DRAGGING_ALPHA : NOT_DRAGGING_ALPHA;
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sTrackballSettingsUniform.isValid())
            sTrackballSettingsUniform =
                Uniform("u_trackballSettingsPack", bgfx::UniformType::Vec4);
        sTrackballSettingsUniform.bind(sTrackBallSettings.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_TRACKBALL_UNIFORMS_H
