// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SCREENSPACE_OVERLAY_UNIFORMS_SCREENSPACE_BOX_UNIFORMS_H
#define VCL_BGFX_SCREENSPACE_OVERLAY_UNIFORMS_SCREENSPACE_BOX_UNIFORMS_H

#include <vclib/bgfx/uniform.h>

#include <vclib/space/core.h>

#include <array>

namespace vcl {

class ScreenSpaceBoxUniforms
{
    static inline std::array<float, 4> sColor;

    static inline Uniform sColorUniform;

public:
    ScreenSpaceBoxUniforms() = delete;

    static void setColor(const Color& color)
    {
        sColor = {color.redF(), color.greenF(), color.blueF(), color.alphaF()};
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sColorUniform.isValid())
            sColorUniform =
                Uniform("u_selectionBoxColor", bgfx::UniformType::Vec4);
        sColorUniform.bind(sColor.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_SCREENSPACE_OVERLAY_UNIFORMS_SCREENSPACE_BOX_UNIFORMS_H
