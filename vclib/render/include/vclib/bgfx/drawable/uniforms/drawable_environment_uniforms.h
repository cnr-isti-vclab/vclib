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

#ifndef VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H
#define VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

namespace vcl {

class DrawableEnvironmentUniforms
{
    mutable std::array<float, 4> mData = {
        0.0, // roughness
        0.0, // cube side
        0.0,
        0.0
    };

    Uniform mDataUniform =
        Uniform("u_environmentSettingsPack", bgfx::UniformType::Vec4);

public:
    DrawableEnvironmentUniforms() = default;

    void updateRoughness(float roughness) { mData[0] = roughness; }

    void updateCubeSideResolution(float cubeSide) { mData[1] = cubeSide; }

    void bind() const { mDataUniform.bind(mData.data()); }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWABLE_UNIFORMS_DRAWABLE_ENVIRONMENT_UNIFORMS_H
