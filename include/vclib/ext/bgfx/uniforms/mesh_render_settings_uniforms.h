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

#ifndef VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
#define VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H

#include <vclib/render/mesh_render_settings.h>

#include <bgfx/bgfx.h>

#include "shader_uniform.h"

namespace vcl::bgf {

class MeshRenderSettingsUniforms
{
    // mDrawPack[0] -> primitive used
    // mDrawPack[1] -> draw mode
    // mDrawPack[1] -> point width
    float mDrawPack[4] = {0.0, 0.0, 0.0, 0.0};

    // mColorPack[0] -> point user color
    // mColorPack[1] -> surface user color
    // mColorPack[2] -> wireframe user color
    float mColorPack[4] = {0.0, 0.0, 0.0, 0.0};

    ShaderUniform mDrawModeUniform =
        ShaderUniform("u_mrsDrawPack", bgfx::UniformType::Vec4);

    ShaderUniform mColorUniform =
        ShaderUniform("u_mrsColorPack", bgfx::UniformType::Vec4);

public:
    MeshRenderSettingsUniforms() {}

    void updatePrimitive(uint primitive)
    {
        mDrawPack[0] = uintBitsToFloat(primitive);
    }

    void updateSettings(const vcl::MeshRenderSettings& settings)
    {
        mDrawPack[1] = uintBitsToFloat(settings.drawMode());
        mDrawPack[2] = settings.pointWidth();

        mColorPack[0] = uintBitsToFloat(settings.pointCloudUserColor().abgr());
        mColorPack[1] = uintBitsToFloat(settings.surfaceUserColor().abgr());
        mColorPack[2] = uintBitsToFloat(settings.wireframeUserColor().abgr());
    }

    void bind() const
    {
        mDrawModeUniform.bind(mDrawPack);
        mColorUniform.bind(mColorPack);
    }

private:
    static float uintBitsToFloat(uint bits)
    {
        union
        {
            uint  i;
            float f;
        } u;

        u.i = bits;
        return u.f;
    }
};

} // namespace vcl::bgf

#endif // VCL_EXT_BGFX_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
