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

#include <vclib/bindings/core/space/core/texture.h>

#include <vclib/space/core.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initTexture(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<Texture> c(m, "Texture");
    c.def(py::init<>());
    //c.def(py::init<std::string>());

    c.def("path", py::overload_cast<>(&Texture::path, py::const_));
    c.def("set_path", [](Texture& t, const std::string& p) {
        t.path() = p;
    });
    c.def("image", py::overload_cast<>(&Texture::image, py::const_));
    c.def("set_image", [](Texture& t, const Image& i) {
        t.image() = i;
    });
}

} // namespace vcl::bind
