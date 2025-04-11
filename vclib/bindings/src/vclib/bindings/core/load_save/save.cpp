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

#include <vclib/bindings/core/load_save/save.h>
#include <vclib/bindings/utils.h>

#include <vclib/io/mesh/save.h>
#include <vclib/meshes.h>

namespace vcl::bind {

void initSave(pybind11::module& m)
{
    auto f =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            namespace py = pybind11;

            m.def(
                "save",
                [](const MeshType& m, const std::string& filename) {
                    vcl::save(m, filename);
                },
                py::arg("m"),
                py::arg("filename"));
        };

    defForAllMeshTypes(m, f);
}

} // namespace vcl::bind
