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

#include "mesh_curvature.h"

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
#include <QApplication>
#endif

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT_AND_BGFX
#include <vclib/ext/bgfx/drawable_mesh.h>
#include <vclib/ext/qt/bgfx/viewer_main_window.h>
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW_AND_BGFX
#include <vclib/ext/bgfx/drawable_mesh.h>
#include <vclib/ext/glfw/bgfx/minimal_viewer_window.h>
#elif VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
#include <vclib/ext/opengl2/drawable_mesh.h>
#include <vclib/ext/qglviewer/viewer_main_window.h>
#endif

int main(int argc, char** argv)
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT
    QApplication application(argc, argv);
#endif

    vcl::TriMesh m = vcl::loadObj<vcl::TriMesh>(VCLIB_ASSETS_PATH "/bimba.obj");

    updateCurvature(m);

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT_AND_BGFX
    vcl::qbgf::ViewerMainWindow          viewer;
    vcl::bgf::DrawableMesh<vcl::TriMesh> dm(m);
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW_AND_BGFX
    vcl::bglfwx::MinimalViewerWindow     viewer;
    vcl::bgf::DrawableMesh<vcl::TriMesh> dm(m);
#elif VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
    vcl::qgl::ViewerMainWindow           viewer;
    vcl::gl2::DrawableMesh<vcl::TriMesh> dm(m);
#endif

    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();
    vector->pushBack(dm);
    viewer.setDrawableObjectVector(vector);
#if VCLIB_RENDER_EXAMPLES_WITH_GLFW_AND_BGFX
    viewer.fitScene();
#endif

    viewer.show();

#if VCLIB_RENDER_EXAMPLES_WITH_QT
    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}
