/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H
#define VCLIB_RENDER_EXAMPLES_COMMON_DEFAULT_VIEWER_H

#include <vclib/mesh/requirements.h>
#include <vclib/render/concepts/drawable_object.h>

#if VCLIB_RENDER_EXAMPLES_WITH_QT
#include <QApplication>
#endif

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
#include <vclib/qt/mesh_viewer.h>
#include <vclib/render/drawable/drawable_mesh.h>
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW
#include <vclib/glfw/viewer_window.h>
#include <vclib/render/drawable/drawable_mesh.h>
#endif

template<vcl::MeshConcept MeshType>
void pushMeshOnVector(
    std::shared_ptr<vcl::DrawableObjectVector>& vector,
    const MeshType&                             mesh)
{
    if constexpr (vcl::DrawableObjectConcept<MeshType>)
        vector->pushBack(mesh);
    else
        vector->pushBack(vcl::DrawableMesh<MeshType>(mesh));
}

auto defaultViewer()
{
#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
    return vcl::qt::MeshViewer();
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW
    return vcl::glfw::ViewerWindow ;
#endif
}

template<vcl::MeshConcept... MeshTypes>
void showMeshesOnViewer(
    int    argc,
    char** argv,
    auto&  viewer,
    const MeshTypes&... meshes)
{
    std::shared_ptr<vcl::DrawableObjectVector> vector =
        std::make_shared<vcl::DrawableObjectVector>();

    (pushMeshOnVector(vector, meshes), ...);

    viewer.setDrawableObjectVector(vector);

#if VCLIB_RENDER_EXAMPLES_WITH_GLFW
    viewer.fitScene();
#endif

    viewer.show();
}

template<vcl::MeshConcept... MeshTypes>
int showMeshesOnDefaultViewer(int argc, char** argv, const MeshTypes&... meshes)
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT
    QApplication application(argc, argv);
#endif

#ifdef VCLIB_RENDER_EXAMPLES_WITH_QT
    vcl::qt::MeshViewer viewer;
#elif VCLIB_RENDER_EXAMPLES_WITH_GLFW
    vcl::glfw::ViewerWindow viewer;
#endif

    showMeshesOnViewer(argc, argv, viewer, meshes...);

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
