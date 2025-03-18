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

#ifndef VCL_BINDINGS_MESH_COMPONENTS_TEXTURE_IMAGES_H
#define VCL_BINDINGS_MESH_COMPONENTS_TEXTURE_IMAGES_H

#include "texture_paths.h"

#include <vclib/space/core/texture.h>

namespace vcl::bind {

template<MeshConcept MeshType>
void initTextureImages(pybind11::class_<MeshType>& c)
{
    namespace py = pybind11;

    initTexturePaths(c);

    c.def("texture", py::overload_cast<uint>(&MeshType::texture));
    c.def("set_texture", [](MeshType& t, uint i, const Texture& tex) {
        t.texture(i) = tex;
    });
    c.def("clear_textures", &MeshType::clearTextures);
    c.def("push_texture", &MeshType::pushTexture);

    using TextureView = decltype(MeshType().textures());

    if (!registeredTypes.contains(typeid(TextureView))) {
        // inner class that allows to iterate over textures
        pybind11::class_<TextureView> v(c, "_TextureRange");
        v.def(
            "__iter__",
            [](TextureView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(TextureView));
    }

    c.def("textures", py::overload_cast<>(&MeshType::textures));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_COMPONENTS_TEXTURE_IMAGES_H
