// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_RENDER_APPLICATION_H
#define VCL_RENDER_APPLICATION_H

#ifdef VCLIB_WITH_QT
#include <vclib/qt/utils/qapplication.h>

#include <QApplication>
#endif

namespace vcl {

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
 */

#ifdef VCLIB_WITH_QT

class Application
{
    QApplication mApp;

public:
    Application(int& argc, char** argv) : mApp(qt::qAppl(argc, argv)) {}

    int exec() { return mApp.exec(); }
};

#else // No Qt

class Application
{
public:
    Application(int& /*argc*/, char** /*argv*/) {}

    int exec() { return 0; }
};

#endif

} // namespace vcl

#endif // VCL_RENDER_APPLICATION_H
