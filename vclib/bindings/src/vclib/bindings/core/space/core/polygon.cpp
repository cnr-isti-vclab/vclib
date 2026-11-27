// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/polygon.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

#include <pybind11/stl.h>

namespace vcl::bind {

template<uint DIM>
void populatePolygon(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar    = double;
    using PointType = Point<Scalar, DIM>;
    using Poly      = Polygon<PointType>;

    std::string      cName = "Polygon" + std::to_string(DIM);
    py::class_<Poly> c(m, cName.c_str());
    c.def(py::init<>());
    c.def(
        py::init([](const std::vector<PointType>& pts) { return Poly(pts); }),
        "points"_a);

    defCopy(c);

    c.def_property_readonly_static(
        "DIM", [](py::object /* self */) { return Poly::DIM; });

    c.def("size", &Poly::size);
    c.def("__len__", &Poly::size);
    c.def("resize", &Poly::resize, "n"_a);
    c.def("reserve", &Poly::reserve, "n"_a);
    c.def("clear", &Poly::clear);
    c.def(
        "push_back",
        [](Poly& p, const PointType& pt) { p.pushBack(pt); },
        "point"_a);

    c.def("point", [](const Poly& p, uint i) { return p.point(i); }, "i"_a);
    c.def(
        "set_point",
        [](Poly& p, uint i, const PointType& pt) { p.point(i) = pt; },
        "i"_a,
        "point"_a);

    c.def("__getitem__", [](const Poly& p, uint i) { return p.point(i); });
    c.def("__setitem__", [](Poly& p, uint i, const PointType& pt) {
        p.point(i) = pt;
    });

    c.def(
        "__iter__",
        [](const Poly& p) { return py::make_iterator(p.begin(), p.end()); },
        py::keep_alive<0, 1>());

    c.def("side_length", &Poly::sideLength, "i"_a);

    if constexpr (DIM == 3) {
        c.def("normal", [](const Poly& p) { return p.normal(); });
    }

    c.def("barycenter", [](const Poly& p) { return p.barycenter(); });
    c.def(
        "weighted_barycenter",
        [](const Poly& p, const std::vector<Scalar>& weights) {
            if (weights.size() != p.size()) {
                throw WrongSizeException(
                    "Weights size must match polygon size (" +
                    std::to_string(p.size()) + ").");
            }
            return p.weightedBarycenter(weights.begin());
        },
        "weights"_a);

    c.def("perimeter", [](const Poly& p) { return p.perimeter(); });
    c.def("area", [](const Poly& p) { return p.area(); });
}

void initPolygon(pybind11::module& m)
{
    populatePolygon<2>(m);
    populatePolygon<3>(m);
}

} // namespace vcl::bind
