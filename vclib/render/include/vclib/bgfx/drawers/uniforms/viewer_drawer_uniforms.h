// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_H
#define VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

namespace vcl {

/**
 * @brief The ViewerDrawerUniforms class is responsible for managing the
 * shader uniforms related to a viewer drawer.
 *
 * It provides a static interface to set the uniform data based on the
 * current viewer settings and to bind the uniforms to the shader programs.
 */
class ViewerDrawerUniforms
{
    using enum PBRViewerSettings::ToneMapping;

    static inline std::array<float, 4> sData = {
        1.0,                             // exposure
        std::bit_cast<float>(ACES_HILL), // tone mapping
        0.0,                             // specular mip levels
        0.0                              // unused
    };

    static inline Uniform sDataUniform;

public:
    ViewerDrawerUniforms() = delete;

    static void setExposure(float exposure) { sData[0] = exposure; }

    static void setToneMapping(PBRViewerSettings::ToneMapping tm)
    {
        sData[1] = std::bit_cast<float>(tm);
    }

    static void setSpecularMipsLevels(uint8_t specMips)
    {
        sData[2] = std::bit_cast<float>(uint(specMips));
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sDataUniform.isValid())
            sDataUniform =
                Uniform("u_viewerSettingsPack", bgfx::UniformType::Vec4);
        sDataUniform.bind(sData.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_H
