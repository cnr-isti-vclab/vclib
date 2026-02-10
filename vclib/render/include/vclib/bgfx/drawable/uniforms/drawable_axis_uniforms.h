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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_AXIS_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_AXIS_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/space/core/color.h>

namespace vcl {

/**
 * @brief The DrawableAxisUniforms class is responsible for managing the shader
 * uniforms related to a drawable axis.
 *
 * It provides an interface to set the uniform data based on the current axis
 * data and to bind the uniforms to the shader programs.
 */
class DrawableAxisUniforms
{
    inline static std::array<float, 4> sAxisColor = {1.0, 0.0, 0.0, 1.0};

    inline static Uniform sAxisColorUniform;

public:
    DrawableAxisUniforms() = delete;

    static void setColor(const vcl::Color& color)
    {
        sAxisColor[0] = color.redF();
        sAxisColor[1] = color.greenF();
        sAxisColor[2] = color.blueF();
        sAxisColor[3] = color.alphaF();
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sAxisColorUniform.isValid())
            sAxisColorUniform = Uniform("u_axisColor", bgfx::UniformType::Vec4);
        sAxisColorUniform.bind(sAxisColor.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_AXIS_UNIFORMS_H
