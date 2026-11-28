// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/io/mesh/save.h>
#include <vclib/bindings/utils.h>

#include <vclib/io/mesh/save_mesh.h>
#include <vclib/io/mesh/save_meshes.h>
#include <vclib/meshes.h>

#include <pybind11/stl.h>

namespace vcl::bind {

namespace {

SaveSettings makeSaveSettings(
    bool            binary,
    bool            saveTextureImages,
    bool            magicsMode,
    const MeshInfo& info,
    bool            embedBuffers,
    bool            meshlabCompatibility)
{
    SaveSettings settings;
    settings.binary               = binary;
    settings.embedBuffers         = embedBuffers;
    settings.saveTextureImages    = saveTextureImages;
    settings.magicsMode           = magicsMode;
    settings.meshlabCompatibility = meshlabCompatibility;
    settings.info                 = info;
    return settings;
}

} // namespace

void initSaveMesh(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    m.def("save_mesh_formats", []() {
        auto ff = vcl::saveMeshFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    m.def("save_meshes_formats", []() {
        auto ff = vcl::saveMeshesFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    auto f =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            m.def(
                "save_mesh",
                [](const MeshType&    m,
                   const std::string& filename,
                   bool               binary,
                   bool               saveTextureImages,
                   bool               magicsMode,
                   const MeshInfo&    info,
                   bool               embedBuffers,
                   bool               meshlabCompatibility,
                   AbstractLogger&    log) {
                    auto settings = makeSaveSettings(
                        binary,
                        saveTextureImages,
                        magicsMode,
                        info,
                        embedBuffers,
                        meshlabCompatibility);
                    vcl::saveMesh(m, filename, settings, log);
                },
                "m"_a,
                "filename"_a,
                "binary"_a                = true,
                "save_texture_images"_a   = false,
                "magics_mode"_a           = false,
                "info"_a                  = MeshInfo(),
                "embed_buffers"_a         = false,
                "meshlab_compatibility"_a = true,
                "log"_a                   = py::cast(&vcl::nullLogger));

            m.def(
                "save_meshes",
                [](const std::vector<MeshType>& meshes,
                   const std::string&           filename,
                   bool                         binary,
                   bool                         embedBuffers,
                   bool                         saveTextureImages,
                   bool                         magicsMode,
                   bool                         meshlabCompatibility,
                   const MeshInfo&              info,
                   AbstractLogger&              log) {
                    auto settings = makeSaveSettings(
                        binary,
                        saveTextureImages,
                        magicsMode,
                        info,
                        embedBuffers,
                        meshlabCompatibility);
                    vcl::saveMeshes(meshes, filename, settings, log);
                },
                "meshes"_a,
                "filename"_a,
                "binary"_a                = true,
                "embed_buffers"_a         = false,
                "save_texture_images"_a   = false,
                "magics_mode"_a           = false,
                "meshlab_compatibility"_a = true,
                "info"_a                  = MeshInfo(),
                "log"_a                   = py::cast(&vcl::nullLogger));
        };

    defForAllMeshTypes(m, f);
}

} // namespace vcl::bind
