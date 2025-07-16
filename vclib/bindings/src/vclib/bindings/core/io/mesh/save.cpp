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

#include <vclib/bindings/core/io/mesh/save.h>
#include <vclib/bindings/utils.h>

#include <vclib/io/mesh/save.h>
#include <vclib/meshes.h>

namespace vcl::bind {

void initSaveMesh(pybind11::module& m)
{
    auto f =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            namespace py = pybind11;

            m.def(
                "save",
                [](const MeshType&    m,
                   const std::string& filename,
                   bool               binary,
                   bool               saveTextureImages,
                   bool               magicsMode,
                   const MeshInfo&    info) {
                    SaveSettings settings;
                    settings.binary            = binary;
                    settings.saveTextureImages = saveTextureImages;
                    settings.magicsMode        = magicsMode;
                    settings.info              = info;

                    vcl::saveMesh(m, filename, settings);
                },
                py::arg("m"),
                py::arg("filename"),
                py::arg("binary")              = true,
                py::arg("save_texture_images") = false,
                py::arg("magics_mode")         = false,
                py::arg("info")                = MeshInfo());
        };

    defForAllMeshTypes(m, f);
}

} // namespace vcl::bind
