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

#include <vclib/bindings/core/space/complex/point_sampler.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>
#include <vclib/mesh.h>
#include <vclib/space/complex.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initPointSampler(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    using PS = PointSampler<>;
    using PT = PS::PointType;

    py::class_<PS> c(m, "PointSampler");
    c.def(py::init<>());

    c.def("samples", &PS::samples, py::return_value_policy::reference);
    c.def("sample", &PointSampler<>::sample, "i"_a);

    c.def("size", &PS::size);

    auto toMeshFun = []<MeshConcept MeshType>(
                         pybind11::class_<PS>& c, MeshType = MeshType()) {
        std::string name =
            "to_" + camelCaseToSnakeCase(meshTypeName<MeshType>());

        c.def(name.c_str(), [](const PS& p) -> MeshType {
            return p.toMesh<MeshType>();
        });
    };
    defForAllMeshTypes(c, toMeshFun);

    c.def(
        "__iter__",
        [](const PS& s) {
            return py::make_iterator(s.begin(), s.end());
        },
        py::keep_alive<0, 1>());
}

} // namespace vcl::bind
