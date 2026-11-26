// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/io/image.h>
#include <vclib/bindings/utils.h>

#include <vclib/io/image/load.h>
#include <vclib/io/image/save.h>
#include <vclib/meshes.h>

#include <pybind11/stl.h>

namespace vcl::bind {

namespace {

BitSet8 toTextureTypeBitSet(
    const std::optional<std::vector<Material::TextureType>>& textureTypes)
{
    if (!textureTypes.has_value()) {
        return BitSet8::ALL();
    }

    BitSet8 bs;
    for (Material::TextureType t : *textureTypes) {
        bs[vcl::toUnderlying(t)] = true;
    }
    return bs;
}

} // namespace

void initIOImage(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    m.def("load_image_formats", []() {
        auto ff = vcl::loadImageFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    m.def(
        "load_image",
        [](const std::string& filename) { return vcl::loadImage(filename); },
        "filename"_a);

    m.def("save_image_formats", []() {
        auto ff = vcl::saveImageFormats();
        return std::vector<FileFormat>(ff.begin(), ff.end());
    });

    m.def(
        "save_image",
        [](const Image& image, const std::string& filename) {
            vcl::saveImage(image, filename);
        },
        "image"_a,
        "filename"_a);

    auto fMaterials =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType())
            requires HasMaterials<MeshType>
    {
        m.def(
            "load_texture_images",
            [](MeshType&          mesh,
               const std::string& basePath,
               const std::optional<std::vector<Material::TextureType>>& types,
               AbstractLogger&                                          log) {
                vcl::loadTextureImages(
                    mesh, basePath, toTextureTypeBitSet(types), log);
            },
            "mesh"_a,
            "base_path"_a     = "",
            "texture_types"_a = py::none(),
            "log"_a           = py::cast(&vcl::nullLogger));

        m.def(
            "save_texture_images",
            [](const MeshType&    mesh,
               const std::string& basePath,
               const std::optional<std::vector<Material::TextureType>>& types,
               AbstractLogger&                                          log) {
                vcl::saveTextureImages(
                    mesh, basePath, toTextureTypeBitSet(types), log);
            },
            "mesh"_a,
            "base_path"_a,
            "texture_types"_a = py::none(),
            "log"_a           = py::cast(&vcl::nullLogger));
    };

    defForAllMeshTypes(m, fMaterials);
}

} // namespace vcl::bind
