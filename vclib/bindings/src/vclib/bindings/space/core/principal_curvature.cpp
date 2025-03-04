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

#include <vclib/bindings/space/core/principal_curvature.h>

#include <vclib/bindings/utils.h>
#include <vclib/space/core.h>

namespace vcl::bind {

void initPrincipalCurvature(pybind11::module& m)
{
    namespace py = pybind11;
    using enum py::return_value_policy;

    using Scalar = double;
    using P = PrincipalCurvature<Scalar>;

    py::class_<P> c(m, "PrincipalCurvature");

    c.def("max_dir", py::overload_cast<>(&P::maxDir), reference);
    c.def("set_max_dir", [](P& p, const Point3<Scalar>& d) { p.maxDir() = d; });
    c.def("min_dir", py::overload_cast<>(&P::minDir), reference);
    c.def("set_min_dir", [](P& p, const Point3<Scalar>& d) { p.minDir() = d; });
    c.def("max_value", py::overload_cast<>(&P::maxValue, py::const_));
    c.def("set_max_value", [](P& p, Scalar v) { p.maxValue() = v; });
    c.def("min_value", py::overload_cast<>(&P::minValue, py::const_));
    c.def("set_min_value", [](P& p, Scalar v) { p.minValue() = v; });
}

} // namespace vcl::bind
