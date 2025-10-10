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

#ifndef VCL_RENDER_IO_CAMERA_GLTF_LOAD_H
#define VCL_RENDER_IO_CAMERA_GLTF_LOAD_H

#include <vclib/io/file_info.h>

#include <vclib/render/concepts/camera.h>
#include <vclib/render/viewer/camera.h>

#include <tiny_gltf.h>

#include <string>

namespace vcl {

template<CameraConcept CameraType = Camera<float>>
inline CameraType loadCameraGltf(
    const std::string& filename,
    uint               cameraIdx = 0)
{
    CameraType camera;
    using Scalar = CameraType::ScalarType;

    tinygltf::TinyGLTF loader;
    tinygltf::Model    model;

    std::string err;
    std::string warn;

    // Determine the file format from the filename extension
    std::string ext = toLower(FileInfo::extension(filename));

    bool ret = false;

    if (ext == ".gltf") {
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    }
    else if (ext == ".glb") {
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    }
    else {
        throw UnknownFileFormatException(ext);
    }

    // Check if the camera index is valid
    if (cameraIdx >= model.cameras.size()) {
        throw std::out_of_range(
            "Camera index " + std::to_string(cameraIdx) +
            " is out of range. Total cameras: " +
            std::to_string(model.cameras.size()));
    }

    const tinygltf::Camera& gltfCamera = model.cameras[cameraIdx];

    // Set projection mode and intrinsic parameters
    if (gltfCamera.type == "perspective") {
        camera.projectionMode() = CameraType::ProjectionMode::PERSPECTIVE;
        camera.fieldOfView()    = gltfCamera.perspective.yfov * 180.0 / M_PI;
        camera.aspectRatio()    = gltfCamera.perspective.aspectRatio;
        camera.nearPlane()      = gltfCamera.perspective.znear;
        camera.farPlane()       = gltfCamera.perspective.zfar;
    }
    else if (gltfCamera.type == "orthographic") {
        camera.projectionMode() = CameraType::ProjectionMode::ORTHO;
        camera.verticalHeight() = gltfCamera.orthographic.ymag * 2.0;
        camera.aspectRatio() =
            gltfCamera.orthographic.xmag / gltfCamera.orthographic.ymag;
        camera.nearPlane() = gltfCamera.orthographic.znear;
        camera.farPlane()  = gltfCamera.orthographic.zfar;
    }

    // Find the node that uses this camera
    for (const auto& node : model.nodes) {
        if (node.camera == static_cast<int>(cameraIdx)) {
            // Extract transformation matrix
            std::vector<double> matrix = node.matrix;
            if (matrix.size() == 16) {
                // The matrix is in column-major order
                // Extract eye position (translation)
                typename CameraType::PointType eye(
                    matrix[12], matrix[13], matrix[14]);
                camera.eye() = eye;

                // Extract forward direction (negative Z axis in camera space)
                typename CameraType::PointType forward(
                    -matrix[8], -matrix[9], -matrix[10]);
                forward = forward.normalized();

                // Extract up direction (Y axis in camera space)
                typename CameraType::PointType up(
                    matrix[4], matrix[5], matrix[6]);
                camera.up() = up.normalized();

                // Calculate center (eye + forward * distance)
                // Use a default distance of 1.0 if not specified
                Scalar distance = 1.0;
                camera.center() = eye + forward * distance;
            }
            break;
        }
    }

    return camera;
}

} // namespace vcl

#endif // VCL_RENDER_IO_CAMERA_GLTF_LOAD_H
