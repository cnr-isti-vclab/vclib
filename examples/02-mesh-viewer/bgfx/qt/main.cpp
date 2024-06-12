/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <QApplication>

#include <vclib/ext/qt/mesh_viewer.h>

#include "common.h"


int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    vcl::qt::MeshViewer mv;

    // load and set up a drawable mesh
    auto drawable = getDrawableMesh<vcl::TriMesh>();

    auto v = std::make_shared<vcl::DrawableObjectVector>();
    v->pushBack(drawable);

    mv.setDrawableObjectVector(v);

    mv.show();
    mv.showMaximized();

    return app.exec();
}
