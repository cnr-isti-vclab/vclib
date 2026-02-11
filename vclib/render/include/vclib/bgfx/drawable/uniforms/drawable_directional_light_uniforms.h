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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_DIRECTIONAL_LIGHT_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_DIRECTIONAL_LIGHT_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/space/core/color.h>

namespace vcl {

class DrawableDirectionalLightUniforms
{
    inline static std::array<float, 4> sLightColor = {1.0, 1.0, 0.0, 1.0};

    inline static Uniform sLightColorUniform;

public:
    DrawableDirectionalLightUniforms() = delete;

    static void setColor(const vcl::Color& color)
    {
        sLightColor[0] = color.redF();
        sLightColor[1] = color.greenF();
        sLightColor[2] = color.blueF();
        sLightColor[3] = color.alphaF();
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sLightColorUniform.isValid())
            sLightColorUniform = Uniform(
                "u_drawableDirectionalLightColor", bgfx::UniformType::Vec4);
        sLightColorUniform.bind(sLightColor.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_DIRECTIONAL_LIGHT_UNIFORMS_H
