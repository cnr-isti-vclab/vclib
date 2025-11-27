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

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_MATERIALS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_MATERIALS_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>
#include <vclib/space/core/texture.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<MeshConcept MeshType>
void initMaterials(pybind11::class_<MeshType>& c)
{
    namespace py = pybind11;
    using namespace py::literals;

    c.def("materials_number", &MeshType::materialsNumber);
    c.def("texture_images_number", &MeshType::textureImagesNumber);
    c.def(
        "mesh_base_path",
        py::overload_cast<>(&MeshType::meshBasePath, py::const_));
    c.def(
        "set_mesh_base_path",
        [](MeshType& t, const std::string& p) {
            t.meshBasePath() = p;
        },
        "path"_a);
    c.def(
        "material",
        py::overload_cast<uint>(&MeshType::material, py::const_));
    c.def(
        "set_material",
        [](MeshType& t, uint i, const Material& m) {
            t.material(i) = m;
        },
        "index"_a,
        "material"_a);
    c.def("texture_image", &MeshType::textureImage);

    c.def("clear_materials", &MeshType::clearMaterials);
    c.def("push_material", &MeshType::pushMaterial);
    c.def(
        "push_texture_image",
        [](MeshType& t, const std::string& path, const Image& img) {
            t.pushTextureImage(path, img);
        },
        "texture_path"_a,
        "image"_a);

    using MaterialView = decltype(MeshType().materials());

    if (!registeredTypes.contains(typeid(MaterialView))) {
        // inner class that allows to iterate over textures
        pybind11::class_<MaterialView> v(c, "_MaterialRange");
        v.def(
            "__iter__",
            [](MaterialView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(MaterialView));
    }

    c.def("materials", py::overload_cast<>(&MeshType::materials));

    using TextureImageView = decltype(MeshType().textureImages());

    if (!registeredTypes.contains(typeid(TextureImageView))) {
        // inner class that allows to iterate over texture images
        pybind11::class_<TextureImageView> v(c, "_TextureImageRange");
        v.def(
            "__iter__",
            [](TextureImageView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(TextureImageView));
    }

    c.def("texture_images", py::overload_cast<>(&MeshType::textureImages));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_MATERIALS_H
