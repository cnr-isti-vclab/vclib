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

#include <vclib/render/drawable/drawable_mesh.h>

#include <vclib/render/io/camera.h>

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

    enum GLTFExamples {
        CAMERAS = 0,
        CESIUM_MAN,
        CESIUM_MILK_TRUCK,
        DAMAGED_HELMET,
        DUCK,
        ORIENTATION_TEST,
        COUNT
    };

    static const std::string GLTFExampleFilenames[COUNT] = {
        "/gltf/Cameras/Cameras.gltf",
        "/gltf/CesiumMan/CesiumMan.gltf",
        "/gltf/CesiumMilkTruck/CesiumMilkTruck.gltf",
        "/gltf/DamagedHelmet/DamagedHelmet.gltf",
        "/gltf/Duck/Duck.gltf",
        "/gltf/OrientationTest/OrientationTest.gltf"};

    const bool LOAD_CUSTOM_CAMERA = false;

    const bool AS_SINGLE_MESH = true;

    uint selectedExample = DAMAGED_HELMET;

    if (AS_SINGLE_MESH) {
        vcl::DrawableMesh<vcl::TriMesh> drawable =
            getDrawableMesh<vcl::TriMesh>(
                VCLIB_EXAMPLE_MESHES_PATH +
                    GLTFExampleFilenames[selectedExample],
                false);
        showMeshesOnViewer(argc, argv, viewer, std::move(drawable));
    }
    else {
        std::vector<vcl::TriMesh> meshes = vcl::loadMeshes<vcl::TriMesh>(
            VCLIB_EXAMPLE_MESHES_PATH + GLTFExampleFilenames[selectedExample]);
        showMeshesOnViewer(argc, argv, viewer, std::move(meshes));
    }

    viewer.fitScene();

    if (LOAD_CUSTOM_CAMERA) {
        vcl::Camera<float> c = vcl::loadCameras<>(
            VCLIB_EXAMPLE_MESHES_PATH "/gltf/Cameras/Cameras.gltf")[1];

        std::cerr << "Camera loaded from gltf file:\n";
        std::cerr << "  Eye: " << c.eye().transpose() << "\n";
        std::cerr << "  Center: " << c.center().transpose() << "\n";
        std::cerr << "  Up: " << c.up().transpose() << "\n";
        std::cerr << "  FOV: " << c.fieldOfView() << "\n";
        std::cerr << "  Aspect: " << c.aspectRatio() << "\n";
        std::cerr << "  Near: " << c.nearPlane() << "\n";
        std::cerr << "  Far: " << c.farPlane() << "\n";
        viewer.setCamera(c);
    }

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
