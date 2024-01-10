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

#ifndef VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
#define VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H

#include <vclib/render/mesh_render_settings.h>

#include <bgfx/bgfx.h>

#include "shader_uniform.h"

namespace vcl::bgf {

class MeshRenderSettingsUniforms
{
    float uniform[4] = {0.0, 0.0, 0.0, 0.0};

    ShaderUniform drawModeUH =
        ShaderUniform("drawMode", bgfx::UniformType::Vec4);

public:
    MeshRenderSettingsUniforms() {}

    void updateSettings(const vcl::MeshRenderSettings& settings)
    {
        int drawMode = settings.drawMode();

        uniform[0] = intBitsToFloat(drawMode);
    }

    void setUniforms() const { bgfx::setUniform(drawModeUH.handle(), uniform); }

private:
    static float intBitsToFloat(int bits)
    {
        union
        {
            int   i;
            float f;
        } u;

        u.i = bits;
        return u.f;
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
