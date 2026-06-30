// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_GLFW_VIEWER_WINDOW_H
#define VCL_GLFW_VIEWER_WINDOW_H

// need this include before for X11 incompatibility with Qt...
#include <vclib/render/canvas.h>

#include <vclib/glfw/window_manager.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

namespace vcl::glfw {

using ViewerWindow = vcl::RenderApp<
    vcl::glfw::WindowManager,
    vcl::Canvas,
    vcl::ViewerDrawer>;

} // namespace vcl::glfw

#endif // VCL_GLFW_VIEWER_WINDOW_H
