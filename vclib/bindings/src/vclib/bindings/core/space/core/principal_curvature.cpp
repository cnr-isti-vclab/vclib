// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/principal_curvature.h>

#include <vclib/bindings/utils.h>
#include <vclib/space/core.h>

namespace vcl::bind {

void initPrincipalCurvature(pybind11::module& m)
{
    namespace py = pybind11;
    using enum py::return_value_policy;

    using Scalar = double;
    using P      = PrincipalCurvature<Scalar>;

    py::class_<P> c(m, "PrincipalCurvature");
    c.def(py::init<>());

    defCopy(c);

    c.def("max_dir", py::overload_cast<>(&P::maxDir), reference);
    c.def("set_max_dir", [](P& p, const Point3<Scalar>& d) {
        p.maxDir() = d;
    });
    c.def("min_dir", py::overload_cast<>(&P::minDir), reference);
    c.def("set_min_dir", [](P& p, const Point3<Scalar>& d) {
        p.minDir() = d;
    });
    c.def("max_value", py::overload_cast<>(&P::maxValue, py::const_));
    c.def("set_max_value", [](P& p, Scalar v) {
        p.maxValue() = v;
    });
    c.def("min_value", py::overload_cast<>(&P::minValue, py::const_));
    c.def("set_min_value", [](P& p, Scalar v) {
        p.minValue() = v;
    });
}

} // namespace vcl::bind
