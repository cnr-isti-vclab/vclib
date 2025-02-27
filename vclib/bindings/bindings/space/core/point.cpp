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

#include "point.h"

#include <vclib/space/core.h>

namespace vcl::bind {

template<uint DIM>
void populatePoint(pybind11::module& m)
{
    namespace py = pybind11;

    using P = Point<double, DIM>;

    std::string cName = "Point" + std::to_string(DIM);
    py::class_<P> c(m, cName.c_str());
    c.def(py::init<>());
    if constexpr (DIM == 1) {
        c.def(py::init<double>(), py::arg("x"));
    }
    if constexpr (DIM == 2) {
        c.def(py::init<double, double>(), py::arg("x"), py::arg("y"));
    }
    if constexpr (DIM == 3) {
        c.def(
            py::init<double, double, double>(),
            py::arg("x"),
            py::arg("y"),
            py::arg("z"));
    }
    if constexpr (DIM == 4) {
        c.def(
            py::init<double, double, double, double>(),
            py::arg("x"),
            py::arg("y"),
            py::arg("z"),
            py::arg("w"));
    }

    if constexpr (DIM >= 1) {
        c.def("x", py::overload_cast<>(&P::x, py::const_));
        c.def("set_x", [](P& p, double v) { p.x() = v; });
    }
    if constexpr (DIM >= 2) {
        c.def("y", py::overload_cast<>(&P::y, py::const_));
        c.def("set_y", [](P& p, double v) { p.y() = v; });
    }
    if constexpr (DIM >= 3) {
        c.def("z", py::overload_cast<>(&P::z, py::const_));
        c.def("set_z", [](P& p, double v) { p.z() = v; });
    }
    if constexpr (DIM >= 4) {
        c.def("w", py::overload_cast<>(&P::w, py::const_));
        c.def("set_w", [](P& p, double v) { p.w() = v; });
    }
}

void initPoint(pybind11::module& m)
{
    namespace py = pybind11;

    populatePoint<2>(m);
    populatePoint<3>(m);
    populatePoint<4>(m);
}

} // namespace vcl::bind
