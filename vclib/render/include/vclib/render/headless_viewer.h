// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_HEADLESS_VIEWER_H
#define VCL_RENDER_HEADLESS_VIEWER_H

#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/headless_manager.h>
#include <vclib/render/render_app.h>

namespace vcl {

/**
 * @brief An alias for a headless viewer application.
 *
 * This Viewer acts as a full rendering application but operates headlessly,
 * making it suitable for generating screenshots, off-screen rendering,
 * or testing in environments without a display server.
 */
using HeadlessViewer =
    vcl::RenderApp<vcl::HeadlessManager, vcl::Canvas, vcl::ViewerDrawer>;

} // namespace vcl

#endif // VCL_RENDER_HEADLESS_VIEWER_H
