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

#include <vclib/mesh/requirements.h>

#if VCLIB_RENDER_EXAMPLES_WITH_QT || VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
#include <QApplication>
#endif

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
#include <vclib/ext/qt/viewer_main_window.h>
#include <vclib/render/drawable/drawable_mesh.h>
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW
#include <vclib/ext/glfw/viewer_window.h>
#include <vclib/render/drawabledrawable_mesh.h>
#elif VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
#include <vclib/ext/opengl2/drawable_mesh.h>
#include <vclib/ext/qglviewer/viewer_main_window.h>
#endif

template<vcl::MeshConcept... MeshTypes>
int showMeshesOnDefaultViewer(int argc, char** argv, const MeshTypes&... meshes)
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT || VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
    QApplication application(argc, argv);
#endif

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
    vcl::qt::ViewerMainWindow viewer;
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW
    vcl::glfw::ViewerWindow viewer;
#elif VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
    vcl::qgl::ViewerMainWindow viewer;
#endif

    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();

#ifndef VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
    (vector->pushBack(vcl::DrawableMesh(meshes)), ...);
#else
    (vector->pushBack(vcl::gl2::DrawableMesh(meshes)), ...);
#endif

    viewer.setDrawableObjectVector(vector);

#if VCLIB_RENDER_EXAMPLES_WITH_GLFW
    viewer.fitScene();
#endif

    viewer.show();

#if VCLIB_RENDER_EXAMPLES_WITH_QT || VCLIB_RENDER_EXAMPLES_WITH_QGLVIEWER
    viewer.showMaximized();
    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}

#endif // VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H
