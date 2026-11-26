// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/triangle.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

namespace vcl::bind {

template<uint DIM>
void populateTriangle(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar    = double;
    using PointType = Point<Scalar, DIM>;
    using T         = Triangle<PointType>;

    std::string   cName = "Triangle" + std::to_string(DIM);
    py::class_<T> c(m, cName.c_str());
    c.def(py::init<>());
    c.def(
        py::init([](const PointType& p0,
                    const PointType& p1,
                    const PointType& p2) {
            T t;
            t.point0() = p0;
            t.point1() = p1;
            t.point2() = p2;
            return t;
        }),
        "p0"_a,
        "p1"_a,
        "p2"_a);

    defCopy(c);

    c.def_property_readonly_static(
        "DIM", [](py::object /* self */) { return T::DIM; });

    c.def("size", &T::size);
    c.def("__len__", &T::size);

    c.def("point", [](const T& t, uint i) { return t.point(i); }, "i"_a);
    c.def(
        "set_point",
        [](T& t, uint i, const PointType& p) { t.point(i) = p; },
        "i"_a,
        "point"_a);

    c.def("__getitem__", [](const T& t, uint i) { return t.point(i); });
    c.def("__setitem__", [](T& t, uint i, const PointType& p) {
        t.point(i) = p;
    });

    c.def("point0", [](const T& t) { return t.point0(); });
    c.def(
        "set_point0",
        [](T& t, const PointType& p) { t.point0() = p; },
        "p0"_a);

    c.def("point1", [](const T& t) { return t.point1(); });
    c.def(
        "set_point1",
        [](T& t, const PointType& p) { t.point1() = p; },
        "p1"_a);

    c.def("point2", [](const T& t) { return t.point2(); });
    c.def(
        "set_point2",
        [](T& t, const PointType& p) { t.point2() = p; },
        "p2"_a);

    c.def(
        "side_length",
        [](const T& t, uint i) { return t.sideLength(i); },
        "i"_a);
    c.def("side_length0", &T::sideLength0);
    c.def("side_length1", &T::sideLength1);
    c.def("side_length2", &T::sideLength2);

    if constexpr (DIM == 3) {
        c.def("normal", [](const T& t) { return t.normal(); });
    }

    c.def("barycenter", [](const T& t) { return t.barycenter(); });
    c.def(
        "weighted_barycenter",
        [](const T& t, Scalar w0, Scalar w1, Scalar w2) {
            return t.weightedBarycenter(w0, w1, w2);
        },
        "w0"_a,
        "w1"_a,
        "w2"_a);
    c.def(
        "weighted_barycenter",
        [](const T& t, const Point3d& w) { return t.weightedBarycenter(w); },
        "weights"_a);

    c.def(
        "barycentric_coordinate_point",
        [](const T& t, Scalar b0, Scalar b1, Scalar b2) {
            return t.barycentricCoordinatePoint(b0, b1, b2);
        },
        "b0"_a,
        "b1"_a,
        "b2"_a);
    c.def(
        "barycentric_coordinate_point",
        [](const T& t, const Point3d& b) {
            return t.barycentricCoordinatePoint(b);
        },
        "barycentric_coords"_a);

    c.def(
        "barycentric_coordinates",
        [](const T& t, const PointType& p) {
            return t.barycentricCoordinates(p);
        },
        "p"_a);

    c.def("circumcenter", [](const T& t) { return t.circumcenter(); });
    c.def("perimeter", [](const T& t) { return t.perimeter(); });
    c.def("area", [](const T& t) { return t.area(); });
    c.def("quality_radii", [](const T& t) { return t.qualityRadii(); });
    c.def(
        "quality_mean_ratio", [](const T& t) { return t.qualityMeanRatio(); });
}

void initTriangle(pybind11::module& m)
{
    populateTriangle<2>(m);
    populateTriangle<3>(m);
}

} // namespace vcl::bind
