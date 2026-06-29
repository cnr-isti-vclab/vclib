// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/sphere.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initSphere(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    py::class_<Sphered> c(m, "Sphere");
    c.def(py::init<>());
    c.def(py::init<Point3d, double>(), "center"_a, "radius"_a);

    c.def("center", py::overload_cast<>(&Sphered::center, py::const_));
    c.def(
        "set_center",
        [](Sphered& s, const Point3d& c) {
            s.center() = c;
        },
        "center"_a);

    c.def("radius", py::overload_cast<>(&Sphered::radius, py::const_));
    c.def(
        "set_radius",
        [](Sphered& s, double r) {
            s.radius() = r;
        },
        "radius"_a);

    c.def("diameter", &Sphered::diameter);
    c.def("circumference", &Sphered::circumference);
    c.def("surface_area", &Sphered::surfaceArea);
    c.def("volume", &Sphered::volume);
    c.def("is_inside", &Sphered::isInside, "p"_a);
    c.def("intersects", &Sphered::intersects, "box"_a);
}

} // namespace vcl::bind
