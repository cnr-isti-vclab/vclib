// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BGFX_SETTINGS_DRAW_OBJECT_SETTINGS_BGFX_H
#define VCL_BGFX_SETTINGS_DRAW_OBJECT_SETTINGS_BGFX_H

#include <vclib/base.h>

#include <vclib/bgfx/drawable/drawable_environment.h>
#include <vclib/bgfx/drawers/uniforms/viewer_drawer_uniforms.h>
#include <vclib/render/settings/pbr_viewer_settings.h>

#include <array>

namespace vcl {

/**
 * @brief A simple struct containing the settings to draw a drawable object
 * in a bgfx canvas.
 */
struct DrawObjectSettingsBGFX
{
    inline static const uint N_ADDITIONAL_VIEWS = 4;

    /**< @brief The object ID to assign to the object. */
    uint objectId = 0;

    /**< @brief The view ID on which to draw the object. */
    uint viewId = 0;

    std::array<uint, N_ADDITIONAL_VIEWS> additionalViewIds =
        makeArray<uint, N_ADDITIONAL_VIEWS>(BGFX_INVALID_VIEW);

    PBRViewerSettings pbrSettings;

    const DrawableEnvironment* environment = nullptr;
};

} // namespace vcl

#endif // VCL_BGFX_SETTINGS_DRAW_OBJECT_SETTINGS_BGFX_H
