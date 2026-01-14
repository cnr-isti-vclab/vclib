/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_RENDER_VIEWER_H
#define VCL_RENDER_VIEWER_H

#ifdef VCLIB_WITH_QT
#include <vclib/qt/mesh_viewer.h>
#elif VCLIB_WITH_GLFW
#ifdef VCLIB_WITH_IMGUI
#include <vclib/imgui/imgui_drawer.h>
#include <vclib/imgui/mesh_viewer_imgui_drawer.h>
#endif // VCLIB_WITH_IMGUI
#include <vclib/glfw/viewer_window.h> // leave this after the ImGui includes..
#endif // VCLIB_WITH_GLFW

namespace vcl {

/**
 * @brief The Viewer class is an alias to the default viewer class used in
 * VCLib.
 *
 * Depending on the available window managers, the Viewer class is aliased to
 * one of the following classes:
 * - If Qt is available, it is aliased to `vcl::qt::MeshViewer`.
 * - Else if GLFW is available and ImGui is enabled, it is aliased to
 *   a MeshViewer with ImGui mesh widgets.
 * - Else if GLFW is available, it is aliased to `vcl::glfw::ViewerWindow`.
 * - Else, a compilation error is triggered if no window manager is available
 *   and a Viewer instance is created.
 *
 * If you are using Qt, make sure to initialize a `QApplication` instance before
 * creating any Viewer instance. An example of always safe Viewer creation is
 * provided below:
 *
 * @code{.cpp}
 * int main(int argc, char** argv) {
 * #ifdef VCLIB_WITH_QT
 *     QApplication app(argc, argv);
 * #endif
 *     vcl::Viewer viewer;
 *     // use viewer...
 *     viewer.show();
 * #ifdef VCLIB_WITH_QT
 *     return app.exec();
 * #else
 *     return 0;
 * #endif
 * }
 * @endcode
 */

#ifdef VCLIB_WITH_QT

using Viewer = qt::MeshViewer;

#elif VCLIB_WITH_GLFW

#ifdef VCLIB_WITH_IMGUI
using Viewer = RenderApp<
    vcl::glfw::WindowManager,
    vcl::Canvas,
    vcl::imgui::ImGuiDrawer,
    vcl::imgui::MeshViewerDrawerImgui>;
#else // VCLIB_WITH_IMGUI
using Viewer = vcl::glfw::ViewerWindow;
#endif // VCLIB_WITH_IMGUI

#else // No Qt and no GLFW

// define an unusable Viewer to trigger a compilation error
struct Viewer
{
    Viewer()
    {
        static_assert(
            false,
            "No Window Manager available for Viewer type definition. "
            "Please allow usage of Qt or GLFW.");
    }
};

#endif // VCLIB_WITH_GLFW

} // namespace vcl

#endif // VCL_RENDER_VIEWER_H
