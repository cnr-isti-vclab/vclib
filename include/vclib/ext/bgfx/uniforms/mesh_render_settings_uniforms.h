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
    // drawPack[0] -> primitive used
    // drawPack[1] -> draw mode
    float drawPack[4] = {0.0, 0.0, 0.0, 0.0};

    // colorPack[0] -> point user color
    // colorPack[1] -> surface user color
    // colorPack[2] -> wireframe user color
    float colorPack[4] = {0.0, 0.0, 0.0, 0.0};

    ShaderUniform drawModeUH =
        ShaderUniform("u_mrsDrawPack", bgfx::UniformType::Vec4);

    ShaderUniform colorUH =
        ShaderUniform("u_mrsColorPack", bgfx::UniformType::Vec4);

public:
    MeshRenderSettingsUniforms() {}

    void updatePrimitive(uint primitive)
    {
        drawPack[0] = uintBitsToFloat(primitive);
    }

    void updateSettings(const vcl::MeshRenderSettings& settings)
    {
        uint drawMode = settings.drawMode();

        drawPack[1] = uintBitsToFloat(drawMode);

        colorPack[1] = uintBitsToFloat(settings.surfaceUserColor().abgr());
    }

    void bind() const
    {
        bgfx::setUniform(drawModeUH.handle(), drawPack);
        bgfx::setUniform(colorUH.handle(), colorPack);
    }

private:
    static float uintBitsToFloat(uint bits)
    {
        union
        {
            uint   i;
            float f;
        } u;

        u.i = bits;
        return u.f;
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
