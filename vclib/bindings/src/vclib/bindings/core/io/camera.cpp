// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/io/camera.h>

#include <vclib/io/camera/load.h>
#include <vclib/io/camera/save.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initIOCamera(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using CameraType = Camera<double>;

    m.def("load_camera_formats", []() {
        auto ff = vcl::loadCameraFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    m.def(
        "load_camera",
        [](const std::string& filename) {
            return vcl::loadCamera<CameraType>(filename);
        },
        "filename"_a);

    m.def(
        "load_cameras",
        [](const std::string& filename) {
            return vcl::loadCameras<CameraType>(filename);
        },
        "filename"_a);

    m.def("save_camera_formats", []() {
        auto ff = vcl::saveCameraFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    m.def(
        "save_camera",
        [](const CameraType& camera, const std::string& filename) {
            vcl::saveCamera<CameraType>(camera, filename);
        },
        "camera"_a,
        "filename"_a);

    m.def(
        "save_cameras",
        [](const std::vector<CameraType>& cameras,
           const std::string&             filename) {
            vcl::saveCameras<CameraType>(cameras, filename);
        },
        "cameras"_a,
        "filename"_a);
}

} // namespace vcl::bind
