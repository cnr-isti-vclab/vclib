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

#include <vclib/space/core.h>

#include <tiny_gltf.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace vcl {

// Internal helper to load the glTF model once (no duplication).
namespace detail {
inline tinygltf::Model loadTinyGltfCameraModel(const std::string& filename)
{
    tinygltf::TinyGLTF loader;
    tinygltf::Model    model;
    std::string        err, warn;

    std::string ext = toLower(FileInfo::extension(filename));
    bool        ret = false;
    if (ext == ".gltf")
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    else if (ext == ".glb")
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    else
        throw UnknownFileFormatException(ext);

    if (!ret)
        throw std::runtime_error(
            "Failed to load glTF cameras: " + err +
            (warn.empty() ? "" : " " + warn));
    return model;
}
} // namespace detail

// Load all cameras from a glTF file.
template<CameraConcept CameraType = Camera<float>>
inline std::vector<CameraType> loadCamerasGltf(const std::string& filename)
{
    using Scalar          = typename CameraType::ScalarType;
    tinygltf::Model model = detail::loadTinyGltfCameraModel(filename);

    std::vector<CameraType> cams;
    cams.reserve(model.cameras.size());

    for (size_t cameraIdx = 0; cameraIdx < model.cameras.size(); ++cameraIdx) {
        const tinygltf::Camera& gltfCamera = model.cameras[cameraIdx];
        CameraType              camera;

        if (gltfCamera.type == "perspective") {
            camera.projectionMode() = CameraType::ProjectionMode::PERSPECTIVE;
            camera.fieldOfView() = gltfCamera.perspective.yfov * 180.0 / M_PI;
            // TODO check what to do with the aspect ratio
            camera.aspectRatio() = gltfCamera.perspective.aspectRatio;
            camera.nearPlane()   = gltfCamera.perspective.znear;
            camera.farPlane()    = gltfCamera.perspective.zfar;
        }
        else if (gltfCamera.type == "orthographic") {
            camera.projectionMode() = CameraType::ProjectionMode::ORTHO;
            camera.verticalHeight() = gltfCamera.orthographic.ymag * 2.0;
            // TODO check what to do with the aspect ratio
            camera.aspectRatio() =
                gltfCamera.orthographic.xmag / gltfCamera.orthographic.ymag;
            camera.nearPlane() = gltfCamera.orthographic.znear;
            camera.farPlane()  = gltfCamera.orthographic.zfar;
        }

        bool foundNode = false;
        for (const auto& node : model.nodes) {
            if (node.camera == static_cast<int>(cameraIdx)) {
                // create a vcl matrix44 of double, not typededuced
                vcl::Matrix44d mat = vcl::Matrix44d::Identity();
                if (node.matrix.size() == 16) {
                    // copy node matrix to vcl matrix
                    mat = vcl::Matrix44d(node.matrix.data());
                }
                else {
                    // construct matrix from TRS
                    vcl::Point3d     translation(0, 0, 0);
                    vcl::Quaterniond rotation = vcl::Quaterniond::Identity();
                    vcl::Point3d     scale(1, 1, 1);
                    if (node.translation.size() == 3) {
                        translation = vcl::Point3d(
                            node.translation[0],
                            node.translation[1],
                            node.translation[2]);
                    }
                    if (node.rotation.size() == 4) {
                        rotation = vcl::Quaterniond(
                            node.rotation[0],
                            node.rotation[1],
                            node.rotation[2],
                            node.rotation[3]);
                    }
                    if (node.scale.size() == 3) {
                        scale = vcl::Point3d(
                            node.scale[0], node.scale[1], node.scale[2]);
                    }
                    // robut contruction from TRS use affine type then convert
                    // to matrix44
                    vcl::Affine3d affine = vcl::Affine3d::Identity();
                    affine.translate(translation);
                    affine.rotate(rotation);
                    affine.scale(scale);
                    mat = affine.matrix();
                }
                // Use mat instead of node.matrix
                typename CameraType::PointType eye(
                    mat(0, 3), mat(1, 3), mat(2, 3));
                camera.eye() = eye;

                typename CameraType::PointType forward(
                    -mat(0, 2), -mat(1, 2), -mat(2, 2));
                forward = forward.normalized();

                typename CameraType::PointType up(
                    mat(0, 1), mat(1, 1), mat(2, 1));
                camera.up() = up.normalized();

                Scalar distance = 1.0;
                camera.center() = eye + forward * distance;
                foundNode       = true;
                break;
            }
        }

        if (!foundNode) {
            // Invalid gltf camera: no node references it.
            throw std::runtime_error(
                "Invalid glTF camera: no node references camera index " +
                std::to_string(cameraIdx));
        }

        cams.push_back(camera);
    }

    assert(cams.size() == model.cameras.size());
    return cams;
}

// Single camera loader now delegates to the vector version.
template<CameraConcept CameraType = Camera<float>>
inline CameraType loadCameraGltf(
    const std::string& filename,
    uint               cameraIdx = 0)
{
    auto cams = loadCamerasGltf<CameraType>(filename);
    if (cameraIdx >= cams.size())
        throw std::out_of_range(
            "Camera index " + std::to_string(cameraIdx) +
            " is out of range. Total cameras: " + std::to_string(cams.size()));
    return cams[cameraIdx];
}

} // namespace vcl

#endif // VCL_RENDER_IO_CAMERA_GLTF_LOAD_H
