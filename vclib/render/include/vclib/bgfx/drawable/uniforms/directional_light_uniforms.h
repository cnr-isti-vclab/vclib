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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H

#include "shader_uniform.h"

#include <vclib/render/viewer/lights/directional_light.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief The DirectionalLightUniforms class manages the uniforms which describe
 * a directional light that can be used by bgfx shaders.
 *
 * The uniforms of this class can be used by including the shader header
 * "uniforms/uniforms/directional_light_uniforms.sh" in the shader.
 *
 * The uniforms are:
 * - u_lightDirPack (vec4): the light direction packed in a vec4
 * - u_lightColorPack (vec4): the light color packed in a vec4
 *
 * The shader header makes available the following uniforms:
 * - u_lightDir (vec3): the light direction
 * - u_lightColor (vec3): the light color
 */
class DirectionalLightUniforms
{
    float mDir[4] = {0.0, 0.0, 1.0, 0.0}; // just first 3 components are used
    float mCol[4] = {1.0, 1.0, 1.0, 1.0}; // just first 3 components are used

    ShaderUniform mLightDirUniform =
        ShaderUniform("u_lightDirPack", bgfx::UniformType::Vec4);
    ShaderUniform mLightColorUniform =
        ShaderUniform("u_lightColorPack", bgfx::UniformType::Vec4);

public:
    DirectionalLightUniforms() {}

    /**
     * @brief updateLight
     * @param light
     */
    template<typename S>
    void updateLight(const vcl::DirectionalLight<S>& light)
    {
        mDir[0] = light.direction().x();
        mDir[1] = light.direction().y();
        mDir[2] = light.direction().z();

        mCol[0] = light.color().redF();
        mCol[1] = light.color().greenF();
        mCol[2] = light.color().blueF();
        // light color alpha is not used
    }

    void bind() const
    {
        mLightDirUniform.bind(mDir);
        mLightColorUniform.bind(mCol);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H
