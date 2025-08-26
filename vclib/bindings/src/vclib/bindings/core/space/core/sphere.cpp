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
