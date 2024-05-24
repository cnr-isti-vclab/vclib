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

#ifndef VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
#define VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H

#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <bgfx/bgfx.h>

#include "shader_uniform.h"

namespace vcl {

class MeshRenderSettingsUniforms
{
    // mDrawPack[0] -> primitive used
    // mDrawPack[1] -> draw mode0
    // mDrawPack[2] -> draw mode1
    // mDrawPack[3] -> unused
    float mDrawPack[4] = {0.0, 0.0, 0.0, 0.0};

    // mWidthPack[0] -> point width
    // mWidthPack[1] -> wireframe width
    // mWidthPack[2] -> edge width
    // mWidthPack[3] -> unused
    float mWidthPack[4] = {0.0, 0.0, 0.0, 0.0};

    // mColorPack[0] -> point user color
    // mColorPack[1] -> surface user color
    // mColorPack[2] -> wireframe user color
    // mColorPack[3] -> edge user color
    float mColorPack[4] = {0.0, 0.0, 0.0, 0.0};

    ShaderUniform mDrawModeUniform =
        ShaderUniform("u_mrsDrawPack", bgfx::UniformType::Vec4);

    ShaderUniform mWidthUniform =
        ShaderUniform("u_mrsWidthPack", bgfx::UniformType::Vec4);

    ShaderUniform mColorUniform =
        ShaderUniform("u_mrsColorPack", bgfx::UniformType::Vec4);

public:
    MeshRenderSettingsUniforms() {}

    void updatePrimitive(uint primitive)
    {
        mDrawPack[0] = ShaderUniform::uintBitsToFloat(primitive);
    }

    void updateSettings(const vcl::MeshRenderSettings& settings)
    {
        mDrawPack[1] = ShaderUniform::uintBitsToFloat(settings.drawMode0());
        mDrawPack[2] = ShaderUniform::uintBitsToFloat(settings.drawMode1());

        mWidthPack[0] = settings.pointWidth();
        mWidthPack[1] = settings.wireframeWidth();
        mWidthPack[2] = settings.edgesWidth();

        mColorPack[0] = ShaderUniform::uintBitsToFloat(
            settings.pointCloudUserColor().abgr());
        mColorPack[1] =
            ShaderUniform::uintBitsToFloat(settings.surfaceUserColor().abgr());
        mColorPack[2] = ShaderUniform::uintBitsToFloat(
            settings.wireframeUserColor().abgr());
        mColorPack[3] =
            ShaderUniform::uintBitsToFloat(settings.edgesUserColor().abgr());
    }

    void bind() const
    {
        mDrawModeUniform.bind(mDrawPack);
        mWidthUniform.bind(mWidthPack);
        mColorUniform.bind(mColorPack);
    }
};

} // namespace vcl

#endif // VCL_BGFX_RENDER_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
