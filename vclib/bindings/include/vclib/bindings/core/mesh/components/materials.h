// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_MATERIALS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_MATERIALS_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>
#include <vclib/space/core.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<MeshConcept MeshType, typename... Options>
void initMaterials(pybind11::class_<MeshType, Options...>& c)
{
    namespace py = pybind11;
    using namespace py::literals;

    c.def("material_count", &MeshType::materialCount);
    c.def("texture_image_count", &MeshType::textureImageCount);
    c.def("mesh_base_path", [](const MeshType& t) -> const std::string& {
        return t.meshBasePath();
    });
    c.def(
        "set_mesh_base_path",
        [](MeshType& t, const std::string& p) { t.meshBasePath() = p; },
        "path"_a);
    c.def(
        "material",
        [](const MeshType& t, ushort i) -> const Material& {
            if (i >= t.materialCount()) {
                throw py::index_error();
            }
            return t.material(i);
        },
        py::return_value_policy::reference_internal);
    c.def(
        "set_material",
        [](MeshType& t, ushort i, const Material& m) {
            if (i >= t.materialCount()) {
                throw py::index_error();
            }
            t.material(i) = m;
        },
        "index"_a,
        "material"_a);
    c.def(
        "texture_image",
        &MeshType::textureImage,
        py::return_value_policy::reference_internal);

    c.def("clear_materials", &MeshType::clearMaterials);
    c.def(
        "push_material",
        [](MeshType& t, const Material& m) { t.pushMaterial(m); },
        "material"_a);
    c.def(
        "push_texture_image",
        [](MeshType& t, const std::string& path, const Image& img) {
            t.pushTextureImage(path, img);
        },
        "texture_path"_a,
        "image"_a);

    using MaterialView = decltype(std::declval<MeshType&>().materials());

    if (!registeredTypes.contains(typeid(MaterialView))) {
        // inner class that allows to iterate over textures
        pybind11::class_<MaterialView> v(c, "_MaterialRange");
        v.def(
            "__iter__",
            [](MaterialView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        v.def("__len__", [](const MaterialView& v) { return v.size(); });
        v.def(
            "__getitem__",
            [](MaterialView& v, int i) -> const Material& {
                int s = static_cast<int>(v.size());
                if (i < 0) {
                    i += s;
                }
                if (i < 0 || i >= s) {
                    throw py::index_error();
                }
                return v[static_cast<uint>(i)];
            },
            py::return_value_policy::reference_internal);
        registeredTypes.insert(typeid(MaterialView));
    }

    c.def(
        "materials",
        [](MeshType& t) { return t.materials(); },
        py::keep_alive<0, 1>());

    using TextureImageView =
        decltype(std::declval<MeshType&>().textureImages());

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

    c.def(
        "texture_images",
        [](MeshType& t) { return t.textureImages(); },
        py::keep_alive<0, 1>());
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_MATERIALS_H
