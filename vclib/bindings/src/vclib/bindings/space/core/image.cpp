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

#include <vclib/bindings/space/core/image.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initImage(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<Image> c(m, "Image", py::buffer_protocol());
    c.def(py::init<>());
    c.def(py::init<std::string>());

    c.def("is_null", &Image::isNull);
    c.def("height", &Image::height);
    c.def("width", &Image::width);
    c.def("size_in_bytes", &Image::sizeInBytes);
    c.def("pixel", &Image::pixel);
    c.def("load", &Image::load);
    c.def("save", &Image::save);
    c.def("mirror", &Image::mirror);
}

} // namespace vcl::bind
