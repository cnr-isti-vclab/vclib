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

#include <vclib/bindings/core/space/core/camera.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initCamera(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar     = double;
    using CameraType = Camera<Scalar>;
    using PointType  = typename CameraType::PointType;
    using MatrixType = typename CameraType::MatrixType;

    py::class_<CameraType> c(m, "Camera");
    c.def(py::init<>());
    defCopy(c);

    // ProjectionMode enum
    py::enum_<CameraType::ProjectionMode> pmEnum(c, "ProjectionMode");
    pmEnum.value("ORTHO", CameraType::ProjectionMode::ORTHO);
    pmEnum.value("PERSPECTIVE", CameraType::ProjectionMode::PERSPECTIVE);
    pmEnum.export_values();

    c.def("center", py::overload_cast<>(&CameraType::center, py::const_));
    c.def(
        "set_center",
        [](CameraType& cam, const PointType& p) {
            cam.center() = p;
        },
        "center"_a);

    c.def("eye", py::overload_cast<>(&CameraType::eye, py::const_));
    c.def(
        "set_eye",
        [](CameraType& cam, const PointType& p) {
            cam.eye() = p;
        },
        "eye"_a);

    c.def("up", py::overload_cast<>(&CameraType::up, py::const_));
    c.def(
        "set_up",
        [](CameraType& cam, const PointType& p) {
            cam.up() = p;
        },
        "up"_a);

    c.def(
        "field_of_view",
        py::overload_cast<>(&CameraType::fieldOfView, py::const_));
    c.def("set_field_of_view", [](CameraType& cam, const Scalar& fovDeg) {
        cam.fieldOfView() = fovDeg;
    });

    c.def(
        "set_field_of_view_adapting_eye_distance",
        &CameraType::setFieldOfViewAdaptingEyeDistance,
        "fov_deg"_a);

    c.def(
        "projection_mode",
        py::overload_cast<>(&CameraType::projectionMode, py::const_));
    c.def(
        "set_projection_mode",
        [](CameraType& cam, CameraType::ProjectionMode pm) {
            cam.projectionMode() = pm;
        },
        "projection_mode"_a);

    c.def(
        "vertical_height",
        py::overload_cast<>(&CameraType::verticalHeight, py::const_));
    c.def(
        "set_vertical_height",
        [](CameraType& cam, const Scalar& h) {
            cam.verticalHeight() = h;
        },
        "vertical_height"_a);

    c.def(
        "aspect_ratio",
        py::overload_cast<>(&CameraType::aspectRatio, py::const_));
    c.def(
        "set_aspect_ratio",
        [](CameraType& cam, const Scalar& ar) {
            cam.aspectRatio() = ar;
        },
        "aspect_ratio"_a);

    c.def(
        "near_plane", py::overload_cast<>(&CameraType::nearPlane, py::const_));
    c.def(
        "set_near_plane",
        [](CameraType& cam, const Scalar& np) {
            cam.nearPlane() = np;
        },
        "near_plane"_a);

    c.def("far_plane", py::overload_cast<>(&CameraType::farPlane, py::const_));
    c.def(
        "set_far_plane",
        [](CameraType& cam, const Scalar& fp) {
            cam.farPlane() = fp;
        },
        "far_plane"_a);

    c.def("view_matrix", &CameraType::viewMatrix);

    c.def("projection_matrix", &CameraType::projectionMatrix);
}

} // namespace vcl::bind
