// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/plane.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

#include <pybind11/operators.h>

namespace vcl::bind {

void initPlane(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    py::class_<Planed> c(m, "Plane");
    c.def(py::init<>());
    c.def(py::init<Point3d, double>(), "direction"_a, "offset"_a);
    c.def(py::init<Point3d, Point3d>(), "p0"_a, "normal"_a);
    c.def(py::init<Point3d, Point3d, Point3d>(), "p0"_a, "p1"_a, "p2"_a);

    defCopy(c);

    c.def("direction", &Planed::direction);
    c.def("offset", &Planed::offset);
    c.def("project_point", &Planed::projectPoint, "p"_a);
    c.def("mirror_point", &Planed::mirrorPoint, "p"_a);

    c.def(py::self == py::self);
    c.def(py::self != py::self);
}

} // namespace vcl::bind
