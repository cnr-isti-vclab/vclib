// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/image.h>

#include <vclib/space/core.h>

namespace vcl::bind {

void initImage(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<Image> c(m, "Image" /*, py::buffer_protocol()*/);

    using enum Image::ColorSpace;
    py::enum_<Image::ColorSpace> cs(c, "ColorSpace");
    cs.value("UNKNOWN", UNKNOWN);
    cs.value("LINEAR", LINEAR);
    cs.value("SRGB", SRGB);
    cs.export_values();

    c.def(py::init<>());

    c.def("is_null", &Image::isNull);
    c.def("height", &Image::height);
    c.def("width", &Image::width);
    c.def("size_in_bytes", &Image::sizeInBytes);
    c.def("pixel", &Image::pixel);
    c.def("mirror", &Image::mirror);
}

} // namespace vcl::bind
