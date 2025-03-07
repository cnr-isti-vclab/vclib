/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_QT_MESH_VIEWER_RENDER_APP_H
#define VCL_QT_MESH_VIEWER_RENDER_APP_H

// This file defines the RenderApp used by the qt MeshViewer application.
// By default, the vcl::qt::ViewerWidget is used as the RenderApp.
// it can be replaced with a custom RenderApp by defining an header file
// called custom_mesh_viewer_render_app.h that defines a RenderApp named
// MeshViewerRenderApp (defined inside the vcl::qt namespace).
// note: the MeshViewerRenderApp class must be a QWidget.

#if __has_include(<custom_mesh_viewer_render_app.h>)
#include <custom_mesh_viewer_render_app.h>
#else

#include "viewer_widget.h"

namespace vcl::qt {

using MeshViewerRenderApp = ViewerWidget;

} // namespace vcl::qt
#endif

#endif // VCL_QT_MESH_VIEWER_RENDER_APP_H
