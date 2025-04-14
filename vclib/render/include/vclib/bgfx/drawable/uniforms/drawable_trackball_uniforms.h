/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

class DrawableTrackballUniforms
{
    static constexpr float DRAGGING_ALPHA     = 0.9f;
    static constexpr float NOT_DRAGGING_ALPHA = 0.5f;

    // the only component is alpha
    std::array<float, 4> mTrackBallSettings;

    Uniform mTrackballSettingsUniform =
        Uniform("u_trackballSettingsPack", bgfx::UniformType::Vec4);

public:
    DrawableTrackballUniforms()
    {
        mTrackBallSettings[0] = NOT_DRAGGING_ALPHA; // default: not dragging
    }

    void setDragging(bool dragging)
    {
        mTrackBallSettings[0] = dragging ? DRAGGING_ALPHA : NOT_DRAGGING_ALPHA;
    }

    void bind() const
    {
        mTrackballSettingsUniform.bind(mTrackBallSettings.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_TRACKBALL_UNIFORMS_H
