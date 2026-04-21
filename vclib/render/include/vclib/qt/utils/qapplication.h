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

} // namespace vcl::qt

#endif // VCL_QT_UTILS_QAPPLICATION_H
