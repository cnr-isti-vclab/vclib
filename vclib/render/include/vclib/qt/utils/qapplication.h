// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_QT_UTILS_QAPPLICATION_H
#define VCL_QT_UTILS_QAPPLICATION_H

#include <QApplication>

namespace vcl::qt {

/**
 * @brief Set the QT_QPA_PLATFORM environment variable to "xcb" to ensure that
 * the XCB platform plugin is used on Linux. This is necessary to ensure that
 * the display ID can be retrieved correctly using
 * QNativeInterface::QX11Application.
 *
 * Call this function before creating the QApplication instance in the main
 * function to ensure that the XCB platform plugin is used on Linux.
 *
 * @code{.cpp}
 * int main(int argc, char** argv)
 * {
 *     vcl::qt::initXcb();
 *     QApplication app(argc, argv);
 *     // ...
 *     return app.exec();
 * }
 * @endcode
 */
inline void initXcb()
{
#ifdef Q_OS_LINUX
#ifndef VCLIB_RENDER_WITH_WAYLAND
    qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
#endif // VCLIB_RENDER_WITH_WAYLAND
#endif // Q_OS_LINUX
}

/**
 * @brief Utility function to create a QApplication instance with the XCB
 * platform plugin initialized. This function calls initXcb() before creating
 * the QApplication instance to ensure that the XCB platform plugin is used on
 * Linux.
 *
 * This function can be used as a drop-in replacement for the QApplication
 * constructor in the main function.
 *
 * Usage:
 *
 * @code{.cpp}
 * int main(int argc, char** argv)
 * {
 *     QApplication app = vcl::qt::qAppl(argc, argv);
 *     // ...
 *     return app.exec();
 * }
 * @endcode
 *
 * @param argc
 * @param argv
 * @return a QApplication instance.
 */
inline QApplication qAppl(int& argc, char** argv)
{
    initXcb();
    return QApplication(argc, argv);
}

/**
 * @brief Utility function to retrieve the display ID of the current
 * QApplication instance. This function checks whether the system is using X11
 * or Wayland and retrieves the display ID accordingly.
 *
 * This function is specific for Linux systems and returns a pointer to the
 * display ID. If the system is not using X11 or Wayland, or if the display ID
 * cannot be retrieved, it returns nullptr.
 *
 * If this function is called before the QApplication instance is created, it
 * will return nullptr. Make sure to call this function after the QApplication
 * instance is created.
 *
 * @return a pointer to the display ID, or nullptr if the display ID cannot be
 * retrieved.
 */
inline void* getDisplayId()
{
    void* displayId = nullptr;
#ifdef Q_OS_LINUX
    // Get the display ID depending on whether the system is using X11 or
    // Wayland
    if (auto x11App =
            qApp->nativeInterface<QNativeInterface::QX11Application>()) {
        displayId = x11App->display();
    }
    else if (
        auto wayApp =
            qApp->nativeInterface<QNativeInterface::QWaylandApplication>()) {
        displayId = wayApp->display();
    }
#endif
    return displayId;
}

} // namespace vcl::qt

#endif // VCL_QT_UTILS_QAPPLICATION_H
