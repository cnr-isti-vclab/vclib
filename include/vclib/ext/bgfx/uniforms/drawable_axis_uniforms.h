/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#ifndef VCL_EXT_BGFX_UNIFORMS_DRAWABLE_AXIS_UNIFORMS_H
#define VCL_EXT_BGFX_UNIFORMS_DRAWABLE_AXIS_UNIFORMS_H

#include "shader_uniform.h"

#include <vclib/space/color.h>

namespace vcl::bgf {

class DrawableAxisUniforms
{
    float lightColor[4] = {1.0, 0.0, 0.0, 1.0};

    ShaderUniform lightColorUniform =
        ShaderUniform("u_axisColor", bgfx::UniformType::Vec4);

public:
    DrawableAxisUniforms() = default;

    void setColor(const vcl::Color& color)
    {
        lightColor[0] = color.redF();
        lightColor[1] = color.greenF();
        lightColor[2] = color.blueF();
        lightColor[3] = color.alphaF();
    }

    void bind() { lightColorUniform.bind(lightColor); }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_DRAWABLE_AXIS_UNIFORMS_H
