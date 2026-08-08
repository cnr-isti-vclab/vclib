// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <QApplication>
#include "main_window.h"

int main(int argc, char* argv[])
{
    // This example creates a Qt application that uses the VCLib
    // HeadlessMeshViewer to render a mesh and display it in a Qt window. The
    // MainWindow class sets up the UI and integrates the headless mesh viewer.

    // You can customize the setupViewer function to add different meshes,
    // editors, and actions to the viewer.
    // The setupViewer function is defined in setup_viewer.h and is called from
    // the MainWindow constructor.

    QApplication app(argc, argv);
    MainWindow w;
    w.resize(1200, 800);
    w.show();
    return app.exec();
}
