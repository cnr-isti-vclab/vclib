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
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_EXT_BGFX_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H
#define VCL_EXT_BGFX_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H

#include <vclib/render/lights/directional_light.h>

#include <bgfx/bgfx.h>

#include "shader_uniform.h"

namespace vcl::bgf {

class DirectionalLightUniforms
{
    float dir[4] = {0.0, 0.0, 1.0, 0.0}; // dir[3]
    float col[4] = {1.0, 1.0, 1.0, 1.0}; // col[3]

    ShaderUniform lightDirUH =
        ShaderUniform("u_lightDirPack", bgfx::UniformType::Vec4);
    ShaderUniform lightColorUH =
        ShaderUniform("u_lightColorPack", bgfx::UniformType::Vec4);

public:
    DirectionalLightUniforms() {}

    template<typename S>
    void updateLight(const vcl::DirectionalLight<S>& light)
    {
        dir[0] = light.direction().x();
        dir[1] = light.direction().y();
        dir[2] = light.direction().z();

        col[0] = light.color().redF();
        col[1] = light.color().greenF();
        col[2] = light.color().blueF();
        // light color alpha is not used
    }

    void bind() const
    {
        bgfx::setUniform(lightDirUH.handle(), dir);
        bgfx::setUniform(lightColorUH.handle(), col);
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H
