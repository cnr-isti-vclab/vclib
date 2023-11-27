/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#include "load_bimba_and_bunny.h"

#ifdef VCLIB_WITH_QGLVIEWER
#include <QApplication>

#include <vclib/ext/opengl2/drawable_mesh.h>
#include <vclib/ext/qglviewer/viewer_main_window.h>
#endif

int main(int argc, char** argv)
{
    vcl::TriMesh m1, m2;

    loadBimbaAndBunnyMeshes(m1, m2);

#ifdef VCLIB_WITH_QGLVIEWER
    m1.enablePerVertexColor();
    vcl::updateBoundingBox(m1);
    vcl::updatePerVertexNormals(m1);
    vcl::setPerVertexColor(m1, vcl::Color::DarkMagenta);

    m2.enablePerVertexColor();
    vcl::updateBoundingBox(m2);
    vcl::updatePerVertexNormals(m2);
    vcl::setPerVertexColor(m2, vcl::Color::DarkMagenta);

    QApplication application(argc, argv);

    vcl::qgl::ViewerMainWindow           viewer;
    vcl::gl2::DrawableMesh<vcl::TriMesh> dm1(m1);
    vcl::gl2::DrawableMesh<vcl::TriMesh> dm2(m2);

    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();
    vector->pushBack(dm1);
    vector->pushBack(dm2);
    viewer.setDrawableObjectVector(vector);

    viewer.show();

    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}
