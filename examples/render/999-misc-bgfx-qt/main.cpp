// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include <vclib/qt/application.h>
#include <vclib/qt/viewer_widget.h>

int main(int argc, char** argv)
{
    auto app = vcl::qt::Application(argc, argv);

    vcl::qt::ViewerWidget viewer;

    viewer.show();

    return app.exec();
}
