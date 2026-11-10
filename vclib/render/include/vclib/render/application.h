// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_APPLICATION_H
#define VCL_RENDER_APPLICATION_H

#ifdef VCLIB_WITH_QT
#include <vclib/qt/application.h>
#endif
#if VCLIB_WITH_GLFW
#include <vclib/glfw/application.h>
#include <vclib/glfw/detail/window_manager_native.h>
#endif

#ifdef VCLIB_RENDER_BACKEND_BGFX
#include <vclib/bgfx/context.h>
#endif

#include <type_traits>

namespace vcl {

enum class ApplicationType { QT, GLFW, HEADLESS, DEFAULT };

namespace detail {

class HeadlessApplication
{
public:
    HeadlessApplication(int& /*argc*/, char** /*argv*/) {}

    ~HeadlessApplication()
    {
#ifdef VCLIB_RENDER_BACKEND_BGFX
        vcl::Context::shutdown();
#endif
    }

    int exec() { return 0; }
};

template<ApplicationType AppType>
consteval auto toApplicationType()
{
    if constexpr (AppType == ApplicationType::QT) {
#ifdef VCLIB_WITH_QT
        return std::type_identity<qt::Application> {};
#else
        static_assert(false, "Qt is not available. Please enable Qt support.");
#endif
    }
    else if constexpr (AppType == ApplicationType::GLFW) {
#ifdef VCLIB_WITH_GLFW
        return std::type_identity<glfw::Application> {};
#else
        static_assert(
            false, "GLFW is not available. Please enable GLFW support.");
#endif
    }
    else if constexpr (AppType == ApplicationType::HEADLESS) {
        return std::type_identity<HeadlessApplication> {};
    }
    else if constexpr (AppType == ApplicationType::DEFAULT) {
#ifdef VCLIB_WITH_QT
        return std::type_identity<qt::Application> {};
#elif VCLIB_WITH_GLFW
        return std::type_identity<glfw::Application> {};
#else
        return std::type_identity<HeadlessApplication> {};
#endif
    }
}

} // namespace detail

template<ApplicationType AppType = ApplicationType::DEFAULT>
using ApplicationT =
    typename decltype(detail::toApplicationType<AppType>())::type;

/**
 * @brief The Application class abstracts the initialization of the underlying
 * GUI framework (e.g. QApplication for Qt).
 *
 * This class is designed to provide a portable way to initialize and run the
 * main event loop across different backends. For instance, when using Qt, this
 * instantiates a QApplication (with appropriate platform flags on Linux) and
 * exec() will block until the application closes.
 * For GLFW, viewer.show() handles the event loop directly, and exec() simply
 * returns 0.
 *
 * Example usage:
 * @code{.cpp}
 * int main(int argc, char** argv) {
 *     vcl::Application app(argc, argv);
 *     vcl::MeshViewer viewer;
 *     // ...
 *     viewer.show();
 *     return app.exec();
 * }
 * @endcode
 *
 * @note You should use this Application class only if you are using the
 * default MeshViewer or other VCLib default GUI classes. If you are using a
 * specific GUI framework (like Qt or GLFW) directly, you should use the
 * corresponding application class (e.g., qt::Application or glfw::Application).
 */
using Application = ApplicationT<ApplicationType::DEFAULT>;

/**
 * @brief Utility function to retrieve the display ID of the current
 * Application instance. This function checks whether the system is using X11
 * or Wayland and retrieves the display ID accordingly.
 *
 * This function is specific for Linux systems and returns a pointer to the
 * display ID. If the system is not using X11 or Wayland, or if the display ID
 * cannot be retrieved, it returns nullptr.
 *
 * If this function is called before the vcl::Application instance is created,
 * it could return nullptr (depends whether you're using Qt or GLFW for window
 * managers). Make sure to call this function after the vcl::Application
 * instance is created.
 *
 * @return a pointer to the display ID, or nullptr if the display ID cannot be
 * retrieved.
 */
inline void* getDisplayId()
{
    void* did = nullptr;
#ifdef VCLIB_WITH_QT
    did = qt::getDisplayId();
#endif
#ifdef VCLIB_WITH_GLFW
    if (!did) {
        did = glfw::getDisplayId();
    }
#endif
    return did;
}

} // namespace vcl

#endif // VCL_RENDER_APPLICATION_H
