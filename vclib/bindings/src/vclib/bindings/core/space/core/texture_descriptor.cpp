// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/space/core/texture_descriptor.h>

#include <vclib/space/core.h>

#include <pybind11/stl.h>

namespace vcl::bind {

void initTextureDescriptor(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    py::class_<TextureDescriptor> c(m, "TextureDescriptor");

    {
        using enum TextureDescriptor::MinificationFilter;
        py::enum_<TextureDescriptor::MinificationFilter> mif(
            c, "MinificationFilter");
        mif.value("NONE", NONE);
        mif.value("NEAREST", NEAREST);
        mif.value("LINEAR", LINEAR);
        mif.value("NEAREST_MIPMAP_NEAREST", NEAREST_MIPMAP_NEAREST);
        mif.value("LINEAR_MIPMAP_NEAREST", LINEAR_MIPMAP_NEAREST);
        mif.value("NEAREST_MIPMAP_LINEAR", NEAREST_MIPMAP_LINEAR);
        mif.value("LINEAR_MIPMAP_LINEAR", LINEAR_MIPMAP_LINEAR);
        mif.export_values();
    }

    using enum TextureDescriptor::MagnificationFilter;
    py::enum_<TextureDescriptor::MagnificationFilter> magf(
        c, "MagnificationFilter");
    magf.value("NONE", NONE);
    magf.value("NEAREST", NEAREST);
    magf.value("LINEAR", LINEAR);
    magf.export_values();

    using enum TextureDescriptor::WrapMode;
    py::enum_<TextureDescriptor::WrapMode> wm(c, "WrapMode");
    wm.value("REPEAT", REPEAT);
    wm.value("CLAMP_TO_EDGE", CLAMP_TO_EDGE);
    wm.value("MIRRORED_REPEAT", MIRRORED_REPEAT);
    wm.export_values();

    c.def(py::init<>());

    c.def("is_null", &TextureDescriptor::isNull);

    c.def("path", py::overload_cast<>(&TextureDescriptor::path, py::const_));
    c.def(
        "set_path",
        [](TextureDescriptor& t, const std::string& p) {
            t.path() = p;
        },
        "path"_a);
    c.def(
        "min_filter",
        py::overload_cast<>(&TextureDescriptor::minFilter, py::const_));
    c.def(
        "set_min_filter",
        [](TextureDescriptor& t, TextureDescriptor::MinificationFilter mf) {
            t.minFilter() = mf;
        },
        "min_filter"_a);
    c.def(
        "mag_filter",
        py::overload_cast<>(&TextureDescriptor::magFilter, py::const_));
    c.def(
        "set_mag_filter",
        [](TextureDescriptor& t, TextureDescriptor::MagnificationFilter mf) {
            t.magFilter() = mf;
        },
        "mag_filter"_a);
    c.def("wrap_u", py::overload_cast<>(&TextureDescriptor::wrapU, py::const_));
    c.def(
        "set_wrap_u",
        [](TextureDescriptor& t, TextureDescriptor::WrapMode wm) {
            t.wrapU() = wm;
        },
        "wrap_u"_a);
    c.def("wrap_v", py::overload_cast<>(&TextureDescriptor::wrapV, py::const_));
    c.def(
        "set_wrap_v",
        [](TextureDescriptor& t, TextureDescriptor::WrapMode wm) {
            t.wrapV() = wm;
        },
        "wrap_v"_a);
}

} // namespace vcl::bind
