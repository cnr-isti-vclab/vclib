// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_CAMERA_GLTF_SAVE_H
#define VCL_IO_CAMERA_GLTF_SAVE_H

#include <vclib/io/file_info.h>

#include <vclib/space/core.h>

#include <tiny_gltf.h>

#include <stdexcept>
#include <string>
#include <vector>

namespace vcl {

// Save all cameras to a glTF file.
template<CameraConcept CameraType = Camera<float>>
inline void saveCamerasGltf(
    const std::vector<CameraType>& cameras,
    const std::string&             filename)
{
    tinygltf::Model model;

    model.asset.version   = "2.0";
    model.asset.generator = "vclib-tinygltf-exporter";

    for (size_t i = 0; i < cameras.size(); ++i) {
        const auto& cam = cameras[i];

        tinygltf::Camera gltfCam;
        if (cam.projectionMode() == CameraType::ProjectionMode::PERSPECTIVE) {
            gltfCam.type                    = "perspective";
            gltfCam.perspective.yfov        = cam.fieldOfView() * M_PI / 180.0;
            gltfCam.perspective.aspectRatio = cam.aspectRatio();
            gltfCam.perspective.znear       = cam.nearPlane();
            gltfCam.perspective.zfar        = cam.farPlane();
        }
        else {
            gltfCam.type = "orthographic";
            // ymag is half the vertical height
            gltfCam.orthographic.ymag = cam.verticalHeight() / 2.0;
            gltfCam.orthographic.xmag =
                cam.verticalHeight() / 2.0 * cam.aspectRatio();
            gltfCam.orthographic.znear = cam.nearPlane();
            gltfCam.orthographic.zfar  = cam.farPlane();
        }
        model.cameras.push_back(gltfCam);

        tinygltf::Node node;
        node.camera = static_cast<int>(i);

        // Calculate transformation matrix
        // The camera view is defined by eye, forward, up.
        // For glTF, camera points to -Z, Y is up, X is right in local space.
        // So the node matrix is a transformation from the camera's local space
        // to the world space. Local X maps to Right vector Local Y maps to Up
        // vector Local Z maps to Backward (-Forward) vector Translation is the
        // Eye position.

        auto forward = (cam.center() - cam.eye()).normalized();
        auto up      = cam.up().normalized();
        auto right   = forward.cross(up).normalized();

        node.matrix.resize(16, 0.0);
        node.matrix[0]  = right.x();
        node.matrix[1]  = right.y();
        node.matrix[2]  = right.z();
        node.matrix[3]  = 0;
        node.matrix[4]  = up.x();
        node.matrix[5]  = up.y();
        node.matrix[6]  = up.z();
        node.matrix[7]  = 0;
        node.matrix[8]  = -forward.x();
        node.matrix[9]  = -forward.y();
        node.matrix[10] = -forward.z();
        node.matrix[11] = 0;
        node.matrix[12] = cam.eye().x();
        node.matrix[13] = cam.eye().y();
        node.matrix[14] = cam.eye().z();
        node.matrix[15] = 1;

        model.nodes.push_back(node);

        if (model.scenes.empty()) {
            model.scenes.push_back(tinygltf::Scene());
            model.defaultScene = 0;
        }
        model.scenes[0].nodes.push_back(static_cast<int>(i));
    }

    tinygltf::TinyGLTF writer;
    std::string        ext      = toLower(FileInfo::extension(filename));
    bool               isBinary = (ext == ".glb");

    // write to file
    bool ret = writer.WriteGltfSceneToFile(
        &model, filename, true, true, true, isBinary);
    if (!ret) {
        throw std::runtime_error("Failed to save glTF cameras to " + filename);
    }
}

template<CameraConcept CameraType = Camera<float>>
inline void saveCameraGltf(
    const CameraType&  camera,
    const std::string& filename)
{
    saveCamerasGltf<CameraType>({camera}, filename);
}

} // namespace vcl

#endif // VCL_IO_CAMERA_GLTF_SAVE_H
