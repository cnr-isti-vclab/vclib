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

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_TEXTURE_PATHS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_TEXTURE_PATHS_H

#include <vclib/bindings/utils.h>

#include <vclib/concepts/mesh.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace vcl::bind {

template<MeshConcept MeshType>
void initTexturePaths(pybind11::class_<MeshType>& c)
{
    namespace py = pybind11;

    c.def("texture_number", &MeshType::textureNumber);

    c.def(
        "texture_path",
        py::overload_cast<uint>(&MeshType::texturePath, py::const_));

    c.def("set_texture_path", [](MeshType& t, uint i, const std::string& p) {
        t.texturePath(i) = p;
    });

    c.def(
        "mesh_base_path",
        py::overload_cast<>(&MeshType::meshBasePath, py::const_));

    c.def("set_mesh_base_path", [](MeshType& t, const std::string& p) {
        t.meshBasePath() = p;
    });

    c.def("clear_texture_paths", &MeshType::clearTexturePaths);
    c.def("push_texture_path", &MeshType::pushTexturePath);

    using TexturePathView = decltype(MeshType().texturePaths());

    if (!registeredTypes.contains(typeid(TexturePathView))) {
        // inner class that allows to iterate over texturePaths
        pybind11::class_<TexturePathView> v(c, "_TexturePathRange");
        v.def(
            "__iter__",
            [](TexturePathView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(TexturePathView));
    }

    c.def("texture_paths", py::overload_cast<>(&MeshType::texturePaths));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_TEXTURE_PATHS_H
