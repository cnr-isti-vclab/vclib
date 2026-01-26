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

#ifndef VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_H
#define VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_H

#include <vclib/bgfx/uniform.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

namespace vcl {

class ViewerDrawerUniforms
{
    mutable std::array<float, 4> mData = {
        0.0, // exposure
        0.0, // tone mapping
        0.0, // specular mip levels
        0.0  // unused
    };

    Uniform mDataUniform =
        Uniform("u_viewerSettingsPack", bgfx::UniformType::Vec4);

public:
    ViewerDrawerUniforms() = default;

    void updateExposure(float exposure) const { mData[0] = exposure; }

    void updateToneMapping(PBRViewerSettings::ToneMapping tm) const
    {
        mData[1] = Uniform::uintBitsToFloat(toUnderlying(tm));
    }

    void updateSpecularMipsLevels(uint8_t specMips)
    {
        mData[2] = Uniform::uintBitsToFloat(uint(specMips));
    }

    void bind() const { mDataUniform.bind(mData.data()); }
};

} // namespace vcl

#endif // VCL_BGFX_DRAWERS_UNIFORMS_VIEWER_DRAWER_UNIFORMS_H
