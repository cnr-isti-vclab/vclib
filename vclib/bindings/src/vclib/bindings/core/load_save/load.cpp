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

#include <vclib/bindings/core/load_save/load.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/load_save/load.h>
#include <vclib/meshes.h>

namespace vcl::bind {

void initLoad(pybind11::module& m)
{
    namespace py = pybind11;

    auto fLoad = []<MeshConcept MeshType>(
                     pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "load",
            [](MeshType& m, const std::string& filename) {
                vcl::load(m, filename);
            },
            py::arg("m"),
            py::arg("filename"));
    };

    defForAllMeshTypes(m, fLoad);

    auto fNameLoad = []<MeshConcept MeshType>(
                         pybind11::module& m, MeshType = MeshType()) {
        std::string name =
            "load_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
        m.def(
            name.c_str(),
            [](const std::string& filename) {
                return vcl::load<MeshType>(filename);
            },
            py::arg("filename"));
    };

    defForAllMeshTypes(m, fNameLoad);
}

} // namespace vcl::bind
