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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/drawable/mesh/mesh_render_settings.h>

#include <bgfx/bgfx.h>

namespace vcl {

/**
 * @brief The MeshRenderSettingsUniforms class is responsible for managing the
 * shader uniforms related to mesh render settings.
 *
 * It provides a static interface to set the uniform data based on the
 * current mesh render settings and to bind the uniforms to the shader programs.
 */
class MeshRenderSettingsUniforms
{
    // sDrawPack[0] -> draw mode0
    // sDrawPack[1] -> draw mode1
    // sDrawPack[2] -> unused
    // sDrawPack[3] -> unused
    inline static std::array<float, 4> sDrawPack = {0.0, 0.0, 0.0, 0.0};

    // sWidthPack[0] -> point width
    // sWidthPack[1] -> wireframe width
    // sWidthPack[2] -> edge width
    // sWidthPack[3] -> unused
    inline static std::array<float, 4> sWidthPack = {0.0, 0.0, 0.0, 0.0};

    // sColorPack[0] -> point user color
    // sColorPack[1] -> surface user color
    // sColorPack[2] -> wireframe user color
    // sColorPack[3] -> edge user color
    inline static std::array<float, 4> sColorPack = {0.0, 0.0, 0.0, 0.0};

    inline static Uniform sDrawModeUniform;
    inline static Uniform sWidthUniform;
    inline static Uniform sColorUniform;

public:
    MeshRenderSettingsUniforms() = delete;

    static void set(const vcl::MeshRenderSettings& settings)
    {
        auto mri = settings.drawMode();
        uint d0  = mri.points().underlying();
        d0 |= mri.surface().underlying() << 16;
        uint d1 = mri.wireframe().underlying();
        d1 |= mri.edges().underlying() << 16;

        sDrawPack[0] = std::bit_cast<float>(d0);
        sDrawPack[1] = std::bit_cast<float>(d1);

        sWidthPack[0] = settings.pointWidth();
        sWidthPack[1] = settings.wireframeWidth();
        sWidthPack[2] = settings.edgesWidth();

        sColorPack[0] = std::bit_cast<float>(settings.pointUserColor().abgr());
        sColorPack[1] =
            std::bit_cast<float>(settings.surfaceUserColor().abgr());
        sColorPack[2] =
            std::bit_cast<float>(settings.wireframeUserColor().abgr());
        sColorPack[3] = std::bit_cast<float>(settings.edgesUserColor().abgr());
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sDrawModeUniform.isValid())
            sDrawModeUniform =
                Uniform("u_mrsDrawPack", bgfx::UniformType::Vec4);
        if (!sWidthUniform.isValid())
            sWidthUniform = Uniform("u_mrsWidthPack", bgfx::UniformType::Vec4);
        if (!sColorUniform.isValid())
            sColorUniform = Uniform("u_mrsColorPack", bgfx::UniformType::Vec4);
        sDrawModeUniform.bind(sDrawPack.data());
        sWidthUniform.bind(sWidthPack.data());
        sColorUniform.bind(sColorPack.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_MESH_RENDER_SETTINGS_UNIFORMS_H
