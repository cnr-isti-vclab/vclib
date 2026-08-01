// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_GIZMOS_UNIFORMS_DIRECTIONAL_LIGHT_GIZMO_UNIFORMS_H
#define VCL_BGFX_GIZMOS_UNIFORMS_DIRECTIONAL_LIGHT_GIZMO_UNIFORMS_H

#include <vclib/bgfx/uniform.h>

#include <vclib/space/core.h>

namespace vcl {

class DirectionalLightGizmoUniforms
{
    inline static std::array<float, 4> sLightColor = {1.0, 1.0, 0.0, 1.0};

    inline static Uniform sLightColorUniform;

public:
    DirectionalLightGizmoUniforms() = delete;

    static void setColor(const vcl::Color& color)
    {
        sLightColor[0] = color.redF();
        sLightColor[1] = color.greenF();
        sLightColor[2] = color.blueF();
        sLightColor[3] = color.alphaF();
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sLightColorUniform.isValid())
            sLightColorUniform = Uniform(
                "u_drawableDirectionalLightColor", bgfx::UniformType::Vec4);
        sLightColorUniform.bind(sLightColor.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_GIZMOS_UNIFORMS_DIRECTIONAL_LIGHT_GIZMO_UNIFORMS_H
