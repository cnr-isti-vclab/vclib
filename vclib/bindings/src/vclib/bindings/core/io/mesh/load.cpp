// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/io/mesh/load.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/type_name.h>
#include <vclib/io/mesh/load_mesh.h>
#include <vclib/io/mesh/load_meshes.h>
#include <vclib/meshes.h>

#include <pybind11/stl.h>

namespace vcl::bind {

namespace {

LoadSettings makeLoadSettings(
    bool enableOptionalComponents,
    bool loadTextureImages)
{
    LoadSettings settings;
    settings.enableOptionalComponents = enableOptionalComponents;
    settings.loadTextureImages        = loadTextureImages;
    return settings;
}

} // namespace

void initLoadMesh(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    m.def("load_mesh_formats", []() {
        auto ff = vcl::loadMeshFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    m.def("load_meshes_formats", []() {
        auto ff = vcl::loadMeshesFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    auto fLoad = []<MeshConcept MeshType>(
                     pybind11::module& m, MeshType = MeshType()) {
        std::string meshName = camelCaseToSnakeCase(meshTypeName<MeshType>());

        m.def(
            "load_mesh",
            [](MeshType&          m,
               const std::string& filename,
               bool               enableOptionalComponents,
               bool               loadTextureImages,
               AbstractLogger&    log) {
                auto settings = makeLoadSettings(
                    enableOptionalComponents, loadTextureImages);
                vcl::loadMesh(m, filename, settings, log);
            },
            "m"_a,
            "filename"_a,
            "enable_optional_components"_a = true,
            "load_texture_images"_a        = false,
            "log"_a                        = py::cast(&vcl::nullLogger));

        m.def(
            "load_mesh_with_info",
            [](MeshType&          m,
               const std::string& filename,
               bool               enableOptionalComponents,
               bool               loadTextureImages,
               AbstractLogger&    log) {
                auto settings = makeLoadSettings(
                    enableOptionalComponents, loadTextureImages);
                return vcl::loadMeshWithInfo(m, filename, settings, log);
            },
            "m"_a,
            "filename"_a,
            "enable_optional_components"_a = true,
            "load_texture_images"_a        = false,
            "log"_a                        = py::cast(&vcl::nullLogger));

        std::string loadName = "load_" + meshName;
        m.def(
            loadName.c_str(),
            [](const std::string& filename,
               bool               enableOptionalComponents,
               bool               loadTextureImages,
               AbstractLogger&    log) {
                auto settings = makeLoadSettings(
                    enableOptionalComponents, loadTextureImages);
                return vcl::loadMesh<MeshType>(filename, settings, log);
            },
            "filename"_a,
            "enable_optional_components"_a = true,
            "load_texture_images"_a        = false,
            "log"_a                        = py::cast(&vcl::nullLogger));

        std::string loadWithInfoName = "load_" + meshName + "_with_info";
        m.def(
            loadWithInfoName.c_str(),
            [](const std::string& filename,
               bool               enableOptionalComponents,
               bool               loadTextureImages,
               AbstractLogger&    log) {
                auto settings = makeLoadSettings(
                    enableOptionalComponents, loadTextureImages);
                return vcl::loadMeshWithInfo<MeshType>(filename, settings, log);
            },
            "filename"_a,
            "enable_optional_components"_a = true,
            "load_texture_images"_a        = false,
            "log"_a                        = py::cast(&vcl::nullLogger));

        std::string loadMeshesName = "load_meshes_" + meshName;
        m.def(
            loadMeshesName.c_str(),
            [](const std::string& filename,
               bool               enableOptionalComponents,
               bool               loadTextureImages,
               AbstractLogger&    log) {
                auto settings = makeLoadSettings(
                    enableOptionalComponents, loadTextureImages);
                return vcl::loadMeshes<MeshType>(filename, settings, log);
            },
            "filename"_a,
            "enable_optional_components"_a = true,
            "load_texture_images"_a        = false,
            "log"_a                        = py::cast(&vcl::nullLogger));

        std::string loadMeshesInfoName = "load_meshes_with_info_" + meshName;
        m.def(
            loadMeshesInfoName.c_str(),
            [](const std::string& filename,
               bool               enableOptionalComponents,
               bool               loadTextureImages,
               AbstractLogger&    log) {
                auto settings = makeLoadSettings(
                    enableOptionalComponents, loadTextureImages);
                std::vector<MeshInfo> loadedInfo;
                auto                  meshes = vcl::loadMeshes<MeshType>(
                    filename, loadedInfo, settings, log);
                return std::make_pair(std::move(meshes), std::move(loadedInfo));
            },
            "filename"_a,
            "enable_optional_components"_a = true,
            "load_texture_images"_a        = false,
            "log"_a                        = py::cast(&vcl::nullLogger));
    };

    defForAllMeshTypes(m, fLoad);
}

} // namespace vcl::bind
