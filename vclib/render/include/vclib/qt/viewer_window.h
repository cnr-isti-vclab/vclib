// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_VIEWER_WINDOW_H
#define VCL_QT_VIEWER_WINDOW_H

#include <vclib/qt/window_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/drawers/viewer_drawer.h>
#include <vclib/render/render_app.h>

namespace vcl::qt {

using ViewerWindow =
    vcl::RenderApp<vcl::qt::WindowManager, vcl::Canvas, vcl::ViewerDrawer>;

} // namespace vcl::qt

#endif // VCL_QT_VIEWER_WINDOW_H
