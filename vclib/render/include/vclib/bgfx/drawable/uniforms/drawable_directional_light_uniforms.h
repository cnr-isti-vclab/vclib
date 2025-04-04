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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_DIRECTIONAL_LIGHT_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_DIRECTIONAL_LIGHT_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/space/core/color.h>

namespace vcl {

class DrawableDirectionalLightUniforms
{
    float mLightColor[4] = {1.0, 1.0, 0.0, 1.0};

    Uniform mLightColorUniform =
        Uniform("u_drawableDirectionalLightColor", bgfx::UniformType::Vec4);

public:
    DrawableDirectionalLightUniforms() = default;

    void setColor(const vcl::Color& color)
    {
        mLightColor[0] = color.redF();
        mLightColor[1] = color.greenF();
        mLightColor[2] = color.blueF();
        mLightColor[3] = color.alphaF();
    }

    void bind() const { mLightColorUniform.bind(mLightColor); }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_DIRECTIONAL_LIGHT_UNIFORMS_H
