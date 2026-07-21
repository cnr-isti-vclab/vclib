// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/viewer_settings.h>

namespace vcl {

/**
 * @brief The DrawableEnvironmentUniforms class is responsible for managing the
 * shader uniforms related to a drawable environment.
 *
 * It provides a static interface to set the uniform data based on the
 * current environment data and to bind the uniforms to the shader programs.
 */
class DrawableEnvironmentUniforms
{
    static inline std::array<float, 4> sData = {
        0.0, // roughness
        0.0, // cube side
        0.0,
        0.0};

    static inline Uniform sDataUniform;

public:
    DrawableEnvironmentUniforms() = delete;

    static void setRoughness(float roughness) { sData[0] = roughness; }

    static void setCubeSideResolution(float cubeSide) { sData[1] = cubeSide; }

    static void bind()
    {
        if (!sDataUniform.isValid())
            sDataUniform =
                Uniform("u_environmentSettingsPack", bgfx::UniformType::Vec4);
        sDataUniform.bind(sData.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H
