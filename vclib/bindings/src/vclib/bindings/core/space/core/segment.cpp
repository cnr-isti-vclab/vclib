// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/segment.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

#include <pybind11/operators.h>

namespace vcl::bind {

template<uint DIM>
void populateSegment(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar    = double;
    using PointType = Point<Scalar, DIM>;
    using S         = Segment<PointType>;

    std::string   cName = "Segment" + std::to_string(DIM);
    py::class_<S> c(m, cName.c_str());
    c.def(py::init<>());
    c.def(py::init<const PointType&, const PointType&>(), "p0"_a, "p1"_a);

    defCopy(c);

    c.def_property_readonly_static(
        "DIM", [](py::object /* self */) { return S::DIM; });

    c.def("p0", [](const S& s) { return s.p0(); });
    c.def("set_p0", [](S& s, const PointType& p) { s.p0() = p; }, "p0"_a);

    c.def("p1", [](const S& s) { return s.p1(); });
    c.def("set_p1", [](S& s, const PointType& p) { s.p1() = p; }, "p1"_a);

    c.def("mid_point", &S::midPoint);
    c.def("direction", &S::direction);
    c.def("normalized_direction", [](const S& s) {
        return PointType(s.direction().normalized());
    });
    c.def("length", &S::length);
    c.def("squared_length", &S::squaredLength);
    c.def("flip", &S::flip);

    c.def(py::self == py::self);
    c.def(py::self != py::self);
    c.def(py::self + py::self);
    c.def(py::self - py::self);
    c.def(py::self * Scalar());
    c.def(py::self / Scalar());
}

void initSegment(pybind11::module& m)
{
    populateSegment<2>(m);
    populateSegment<3>(m);
}

} // namespace vcl::bind
