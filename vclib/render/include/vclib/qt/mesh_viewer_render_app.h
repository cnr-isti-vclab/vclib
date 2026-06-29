// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_MESH_VIEWER_RENDER_APP_H
#define VCL_QT_MESH_VIEWER_RENDER_APP_H

// This file defines the RenderApp used by the qt MeshViewer application.
// By default, the vcl::qt::ViewerWidget is used as the RenderApp.
// it can be replaced with a custom RenderApp by defining an header file
// called custom_mesh_viewer_render_app.h that defines a RenderApp named
// MeshViewerRenderApp (defined inside the vcl::qt namespace).
// note: the MeshViewerRenderApp class must be a QWidget.

#if __has_include(<custom_mesh_viewer_render_app.h>)
#include "utils.h"
#include <custom_mesh_viewer_render_app.h>
#else

#include "viewer_widget.h"

namespace vcl::qt {

using MeshViewerRenderApp = ViewerWidget;

} // namespace vcl::qt
#endif

#endif // VCL_QT_MESH_VIEWER_RENDER_APP_H
