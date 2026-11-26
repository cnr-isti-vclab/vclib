// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/ray.h>
#include <vclib/bindings/utils.h>

#include <vclib/space/core.h>

#include <pybind11/operators.h>

namespace vcl::bind {

template<uint DIM>
void populateRay(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using Scalar    = double;
    using PointType = Point<Scalar, DIM>;
    using R         = Ray<PointType>;

    std::string   cName = "Ray" + std::to_string(DIM);
    py::class_<R> c(m, cName.c_str());
    c.def(py::init<>());
    c.def(
        py::init<const PointType&, const PointType&>(),
        "origin"_a,
        "direction"_a);

    defCopy(c);

    c.def_property_readonly_static(
        "DIM", [](py::object /* self */) { return R::DIM; });

    c.def("origin", [](const R& r) { return r.origin(); });
    c.def(
        "set_origin",
        [](R& r, const PointType& p) { r.origin() = p; },
        "origin"_a);

    c.def("direction", [](const R& r) { return r.direction(); });
    c.def(
        "set_direction",
        [](R& r, const PointType& d) { r.direction() = d; },
        "direction"_a);

    c.def("normalized_direction", &R::normalizedDirection);

    c.def(py::self == py::self);
    c.def(py::self != py::self);
}

void initRay(pybind11::module& m)
{
    populateRay<2>(m);
    populateRay<3>(m);
}

} // namespace vcl::bind
