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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/viewer/lights/directional_light.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief The DirectionalLightUniforms class is responsible for managing the
 * uniforms which describe a directional light that can be used by bgfx shaders.
 *
 * It provides a static interface to set the uniform data based on the current
 * directional light data and to bind the uniforms to the shader programs.
 *
 * The uniforms of this class can be used by including the shader header
 * <vclib/bgfx/drawable/uniforms/directional_light_uniforms.sh> in the shader.
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
    // just first 3 components are used
    inline static std::array<float, 4> sDir = {0.0, 0.0, 1.0, 0.0};

    // just first 3 components are used
    inline static std::array<float, 4> sCol = {1.0, 1.0, 1.0, 1.0};

    inline static Uniform sLightDirUniform;
    inline static Uniform sLightColorUniform;

public:
    DirectionalLightUniforms() = delete;

    /**
     * @brief setLight
     * @param light
     */
    template<typename S>
    static void setLight(const vcl::DirectionalLight<S>& light)
    {
        sDir[0] = light.direction().x();
        sDir[1] = light.direction().y();
        sDir[2] = light.direction().z();

        sCol[0] = light.color().redF();
        sCol[1] = light.color().greenF();
        sCol[2] = light.color().blueF();
        // light color alpha is not used
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sLightDirUniform.isValid())
            sLightDirUniform =
                Uniform("u_lightDirPack", bgfx::UniformType::Vec4);
        if (!sLightColorUniform.isValid())
            sLightColorUniform =
                Uniform("u_lightColorPack", bgfx::UniformType::Vec4);
        sLightDirUniform.bind(sDir.data());
        sLightColorUniform.bind(sCol.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DIRECTIONAL_LIGHT_UNIFORMS_H
