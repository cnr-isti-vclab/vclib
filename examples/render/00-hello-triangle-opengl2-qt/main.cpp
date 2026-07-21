// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../00-hello-triangle-opengl2-common/hello_triangle_drawer.h"

#include <vclib/qt/widget_manager.h>
#include <vclib/render/canvas.h>
#include <vclib/render/render_app.h>

int main(int argc, char** argv)
{
    auto app = vcl::qt::qAppl(argc, argv);

    using WidgetQt = vcl::
        RenderApp<vcl::qt::WidgetManager, vcl::Canvas, HelloTriangleDrawer>;

    WidgetQt tw("Hello Triangle with Qt");

    tw.show();

    return app.exec();
}
