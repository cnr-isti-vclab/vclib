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

#include <vclib/bindings/core/io/mesh/load.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/io/mesh/load_mesh.h>
#include <vclib/meshes.h>

namespace vcl::bind {

void initLoadMesh(pybind11::module& m)
{
    namespace py = pybind11;

    auto fLoad = []<MeshConcept MeshType>(
                     pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "load_mesh",
            [](MeshType&          m,
               const std::string& filename,
               MeshInfo&          loadedInfo,
               bool               enableOptionalComponents,
               bool               loadTextureImages) {
                LoadSettings settings;
                settings.enableOptionalComponents = enableOptionalComponents;
                settings.loadTextureImages        = loadTextureImages;
                vcl::loadMesh(m, filename, loadedInfo, settings);
            },
            py::arg("m"),
            py::arg("filename"),
            py::arg("loaded_info")                = MeshInfo(),
            py::arg("enable_optional_components") = true,
            py::arg("load_texture_images")        = false);
    };

    defForAllMeshTypes(m, fLoad);

    auto fNameLoad =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            std::string name =
                "load_" + camelCaseToSnakeCase(meshTypeName<MeshType>());
            m.def(
                name.c_str(),
                [](const std::string& filename,
                   MeshInfo&          loadedInfo,
                   bool               enableOptionalComponents,
                   bool               loadTextureImages) {
                    return vcl::loadMesh<MeshType>(filename);
                },
                py::arg("filename"),
                py::arg("loaded_info")                = MeshInfo(),
                py::arg("enable_optional_components") = true,
                py::arg("load_texture_images")        = false);
        };

    defForAllMeshTypes(m, fNameLoad);
}

} // namespace vcl::bind
