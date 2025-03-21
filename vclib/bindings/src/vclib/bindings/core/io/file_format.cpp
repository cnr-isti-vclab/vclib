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

#include <vclib/bindings/core/io/file_format.h>

#include <vclib/io/file_format.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initFileFormat(pybind11::module& m)
{
    namespace py = pybind11;

    pybind11::class_<vcl::FileFormat> c(m, "FileFormat");

    c.def(
        py::init<const std::string&, std::string>(),
        py::arg("extension"),
        py::arg("description") = "");
    c.def(
        py::init<std::vector<std::string>, std::string>(),
        py::arg("extensions"),
        py::arg("description") = "");

    c.def("description", &vcl::FileFormat::description);
    c.def("extensions", &vcl::FileFormat::extensions);
    c.def("match_extension", &vcl::FileFormat::matchExtension);
    c.def("__eq__", &vcl::FileFormat::operator==);
}

} // namespace vcl::bind
