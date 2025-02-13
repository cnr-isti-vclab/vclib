/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <bgfx/bgfx.h>

namespace vcl {

class MeshRenderSettingsUniforms
{
    // mDrawPack[0] -> draw mode0
    // mDrawPack[1] -> draw mode1
    // mDrawPack[2] -> unused
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

    Uniform mDrawModeUniform =
        Uniform("u_mrsDrawPack", bgfx::UniformType::Vec4);

    Uniform mWidthUniform = Uniform("u_mrsWidthPack", bgfx::UniformType::Vec4);

    Uniform mColorUniform = Uniform("u_mrsColorPack", bgfx::UniformType::Vec4);

public:
    MeshRenderSettingsUniforms() {}

    void updateSettings(const vcl::MeshRenderSettings& settings)
    {
        auto mri = settings.drawMode();
        uint d0 = mri.points.underlying();
        d0 |= mri.surface.underlying() << 16;
        uint d1 = mri.wireframe.underlying();
        d1 |= mri.edges.underlying() << 16;

        mDrawPack[0] = Uniform::uintBitsToFloat(d0);
        mDrawPack[1] = Uniform::uintBitsToFloat(d1);

        mWidthPack[0] = settings.pointWidth();
        mWidthPack[1] = settings.wireframeWidth();
        mWidthPack[2] = settings.edgesWidth();

        mColorPack[0] =
            Uniform::uintBitsToFloat(settings.pointUserColor().abgr());
        mColorPack[1] =
            Uniform::uintBitsToFloat(settings.surfaceUserColor().abgr());
        mColorPack[2] =
            Uniform::uintBitsToFloat(settings.wireframeUserColor().abgr());
        mColorPack[3] =
            Uniform::uintBitsToFloat(settings.edgesUserColor().abgr());
    }

    void bind() const
    {
        mDrawModeUniform.bind(mDrawPack);
        mWidthUniform.bind(mWidthPack);
        mColorUniform.bind(mColorPack);
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
