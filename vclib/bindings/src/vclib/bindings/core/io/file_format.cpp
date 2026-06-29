// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

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
