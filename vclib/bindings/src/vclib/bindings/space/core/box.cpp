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

#include <vclib/bindings/space/core/box.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

#include <pybind11/operators.h>

namespace vcl::bind {

template<uint DIM>
void populateBox(pybind11::module& m)
{
    namespace py = pybind11;
    using enum py::return_value_policy;

    using Scalar    = double;
    using PointType = Point<Scalar, DIM>;
    using B         = Box<PointType>;

    std::string   cName = "Box" + std::to_string(DIM);
    py::class_<B> c(m, cName.c_str());
    c.def(py::init<>());
    c.def(py::init<PointType>());
    c.def(py::init<PointType, PointType>());

    defCopy(c);

    c.def_property_readonly_static("DIM", [](py::object /* self */) {
        return B::DIM;
    });

    c.def("min", py::overload_cast<>(&B::min), reference);
    c.def("set_min", [](B& b, const PointType& p) {
        b.min() = p;
    });

    c.def("max", py::overload_cast<>(&B::max), reference);
    c.def("set_max", [](B& b, const PointType& p) {
        b.max() = p;
    });

    c.def("is_null", &B::isNull);
    c.def("is_empty", &B::isEmpty);
    c.def("is_inside", &B::isInside);
    c.def("is_inside_strict", &B::isInsideStrict);
    c.def("is_inside_open_box", &B::isInsideOpenBox);
    c.def("overlap", &B::overlap);
    c.def("collide", &B::collide);
    c.def("intersects", &B::intersects);
    c.def("diagonal", &B::diagonal);
    c.def("squared_diagonal", &B::squaredDiagonal);
    c.def("center", &B::center);
    c.def("size", &B::size);
    c.def("volume", &B::volume);
    c.def("dim", &B::dim);
    c.def("min_dim", &B::minDim);
    c.def("max_dim", &B::maxDim);
    c.def("intersection", &B::intersection);
    c.def("set_null", &B::setNull);
    c.def("add", [](B& b, const PointType& p) {
        b.add(p);
    });
    c.def("add", [](B& b, const PointType& p, Scalar s) {
        b.add(p, s);
    });
    c.def("add", [](B& b, const B& box) {
        b.add(box);
    });
    c.def("translate", &B::translate);

    c.def(py::self == py::self);
    c.def(py::self != py::self);
}

void initBox(pybind11::module& m)
{
    populateBox<2>(m);
    populateBox<3>(m);
    populateBox<4>(m);
}

} // namespace vcl::bind
