// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_SETTINGS_RENDER_MODE_H
#define VCL_RENDER_SETTINGS_RENDER_MODE_H

namespace vcl {

/**
 * @brief Specifies the render mode of the viewer.
 */
enum class RenderMode {
    /** @brief Classic shading model (e.g., Phong, per-vertex colors). */
    CLASSIC,
    /** @brief Physically Based Rendering (PBR) shading model. */
    PBR
};

} // namespace vcl

#endif // VCL_RENDER_SETTINGS_RENDER_MODE_H
