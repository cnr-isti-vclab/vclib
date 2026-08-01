// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_GIZMOS_UNIFORMS_TRACKBALL_GIZMO_UNIFORMS_H
#define VCL_BGFX_GIZMOS_UNIFORMS_TRACKBALL_GIZMO_UNIFORMS_H

#include <vclib/bgfx/uniform.h>

#include <vclib/space/core.h>

namespace vcl {

/**
 * @brief The TrackballGizmoUniforms class is responsible for managing the
 * shader uniforms related to a trackball gizmo.
 *
 * It provides a static interface to set the uniform data based on the
 * current dragging status and to bind the uniforms to the shader programs.
 */
class TrackballGizmoUniforms
{
    static constexpr float DRAGGING_ALPHA     = 0.9f;
    static constexpr float NOT_DRAGGING_ALPHA = 0.5f;

    // the only component is alpha
    inline static std::array<float, 4> sTrackBallSettings =
        {NOT_DRAGGING_ALPHA, 0, 0, 0};

    inline static Uniform sTrackballSettingsUniform;

public:
    TrackballGizmoUniforms() = delete;

    static void setDragging(bool dragging)
    {
        sTrackBallSettings[0] = dragging ? DRAGGING_ALPHA : NOT_DRAGGING_ALPHA;
    }

    static void bind()
    {
        // lazy initialization
        // to avoid creating uniforms before bgfx is initialized
        if (!sTrackballSettingsUniform.isValid())
            sTrackballSettingsUniform =
                Uniform("u_trackballSettingsPack", bgfx::UniformType::Vec4);
        sTrackballSettingsUniform.bind(sTrackBallSettings.data());
    }
};

} // namespace vcl

#endif // VCL_BGFX_GIZMOS_UNIFORMS_TRACKBALL_GIZMO_UNIFORMS_H
