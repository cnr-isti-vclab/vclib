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

#include <vclib/render/io/camera.h>
#include <vclib/render/viewer/camera.h>

#include <default_viewer.h>
#include <get_drawable_mesh.h>

#if VCLIB_RENDER_EXAMPLES_WITH_QT
#include <QApplication>
#endif

int main(int argc, char** argv)
{
#if VCLIB_RENDER_EXAMPLES_WITH_QT
    QApplication application(argc, argv);
#endif

    auto viewer = defaultViewer();

    const bool TEXCOORDS_PER_VERTEX = false;
    const bool USE_BUNNY            = true;

    vcl::DrawableMesh<vcl::TriMesh> drawable =
        getDrawableMesh<vcl::TriMesh>("gltf/Cameras/Cameras.gltf");
    showMeshesOnViewer(argc, argv, viewer, std::move(drawable));

    vcl::Camera<float> c = vcl::loadCameras<>(VCLIB_EXAMPLE_MESHES_PATH
                                              "/gltf/Cameras/Cameras.gltf")[1];

    std::cerr << "Camera loaded from gltf file:\n";
    std::cerr << "  Eye: " << c.eye().transpose() << "\n";
    std::cerr << "  Center: " << c.center().transpose() << "\n";
    std::cerr << "  Up: " << c.up().transpose() << "\n";
    std::cerr << "  FOV: " << c.fieldOfView() << "\n";
    std::cerr << "  Aspect: " << c.aspectRatio() << "\n";
    std::cerr << "  Near: " << c.nearPlane() << "\n";
    std::cerr << "  Far: " << c.farPlane() << "\n";

    viewer.fitScene();
    viewer.setCamera(c);

    // fit view to use the trackball decently
    viewer.fitView();

    // get and set the camera to confirm the operation is correct
    viewer.setCamera(viewer.camera());

    // fit view to use the trackball decently
    viewer.fitView();

#if VCLIB_RENDER_EXAMPLES_WITH_QT
    viewer.showMaximized();
    return application.exec();
#else
    (void) argc; // unused
    (void) argv;
    return 0;
#endif
}
