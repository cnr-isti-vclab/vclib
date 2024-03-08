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

#ifndef VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H
#define VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H

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

template<vcl::MeshConcept... MeshTypes>
int showMeshesOnDefaultViewer(int argc, char** argv, const MeshTypes&... meshes)
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT
    QApplication application(argc, argv);
#endif

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT_AND_BGFX
    vcl::qbgf::ViewerMainWindow viewer;
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW_AND_BGFX
    vcl::bglfwx::MinimalViewerWindow viewer;
#elif VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
    vcl::qgl::ViewerMainWindow viewer;
#endif

    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();
#ifdef VCLIB_RENDER_EXAMPLES_WITH_BGFX
    (vector->pushBack(vcl::bgf::DrawableMesh(meshes)), ...);
#elif VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
    (vector->pushBack(vcl::gl2::DrawableMesh(meshes)), ...);
#endif

    viewer.setDrawableObjectVector(vector);

#if VCLIB_RENDER_EXAMPLES_WITH_GLFW_AND_BGFX
    viewer.fitScene();
#endif

    viewer.show();

#if VCLIB_RENDER_EXAMPLES_WITH_QT
    viewer.showMaximized();
    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}

#endif // VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H
