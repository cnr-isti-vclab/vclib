// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/io/mesh/save.h>
#include <vclib/bindings/utils.h>

#include <vclib/io/mesh/save_mesh.h>
#include <vclib/meshes.h>

namespace vcl::bind {

void initSaveMesh(pybind11::module& m)
{
    auto f =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            namespace py = pybind11;

            m.def(
                "save_mesh",
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
