// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_VIEWER_H
#define VCL_RENDER_VIEWER_H

#ifdef VCLIB_WITH_QT
#include <vclib/qt/viewer_window.h>
#elif VCLIB_WITH_GLFW
#include <vclib/glfw/viewer_window.h>
#endif

namespace vcl {

/**
 * @brief The Viewer class is an alias to the default viewer class used in
 * VCLib.
 *
 * Depending on the available window managers, the Viewer class is aliased to
 * one of the following classes:
 * - If Qt is available, it is aliased to `vcl::qt::ViewerWindow`.
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

using Viewer = qt::ViewerWindow;

#elif VCLIB_WITH_GLFW

using Viewer = glfw::ViewerWindow;

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
