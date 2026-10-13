// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_VIEWER_H
#define VCL_RENDER_VIEWER_H

#include <vclib/render/headless_viewer.h>

#ifdef VCLIB_WITH_QT
#include <vclib/qt/viewer_widget.h>
#include <vclib/qt/viewer_window.h>
#endif
#ifdef VCLIB_WITH_GLFW
#include <vclib/glfw/viewer_window.h>
#endif

namespace vcl {

enum class ViewerType {QT_WIDGET, QT_WINDOW, GLFW_WINDOW, HEADLESS, DEFAULT};

namespace detail {

template<ViewerType VwrType>
consteval auto viewerType()
{
    if constexpr (VwrType == ViewerType::QT_WIDGET) {
#ifdef VCLIB_WITH_QT
        return std::type_identity<qt::ViewerWidget> {};
#else
        static_assert(
            false,
            "Qt is not available. Please enable Qt support to use "
            "ViewerWidget.");
#endif
    }
    else if constexpr (VwrType == ViewerType::QT_WINDOW) {
#ifdef VCLIB_WITH_QT
        return std::type_identity<qt::ViewerWindow> {};
#else
        static_assert(
            false,
            "Qt is not available. Please enable Qt support to use "
            "ViewerWindow.");
#endif
    }
    else if constexpr (VwrType == ViewerType::GLFW_WINDOW) {
#if VCLIB_WITH_GLFW
        return std::type_identity<glfw::ViewerWindow> {};
#else
        static_assert(
            false,
            "GLFW is not available. Please enable GLFW to support "
            "ViewerWindow.");
#endif
    }
    else if constexpr (VwrType == ViewerType::HEADLESS) {
        return std::type_identity<HeadlessViewer> {};
    }
    else if constexpr (VwrType == ViewerType::DEFAULT) {
#ifdef VCLIB_WITH_QT
        return std::type_identity<qt::ViewerWindow> {};
#elif VCLIB_WITH_GLFW
        return std::type_identity<glfw::ViewerWindow> {};
#else
        return std::type_identity<HeadlessViewer> {};
#endif
    }
}

} // namespace detail

template <ViewerType VwrType = ViewerType::DEFAULT>
using ViewerT = typename decltype(detail::viewerType<VwrType>())::type;

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
 * Make sure to initialize a `vcl::Application` instance before creating any
 * Viewer instance. An example of always safe Viewer creation is provided below:
 *
 * @code{.cpp}
 * int main(int argc, char** argv) {
 *     vcl::Application app(argc, argv);
 *     vcl::Viewer viewer;
 *     // use viewer...
 *     viewer.show();
 *     return app.exec();
 * }
 * @endcode
 */
using Viewer = ViewerT<ViewerType::DEFAULT>;

} // namespace vcl

#endif // VCL_RENDER_VIEWER_H
