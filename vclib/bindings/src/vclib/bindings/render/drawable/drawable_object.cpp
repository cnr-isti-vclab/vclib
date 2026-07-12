// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/render/drawable/drawable_object.h>

#include <vclib/render/drawable/drawable_object.h>

namespace vcl::bind {

void initDrawableObject(pybind11::module& m)
{
    namespace py = pybind11;

    py::class_<vcl::DrawableObject, std::shared_ptr<vcl::DrawableObject>> c(
        m, "DrawableObject");

    c.def("is_visible", &vcl::DrawableObject::isVisible);
    c.def("set_visibility", &vcl::DrawableObject::setVisibility, py::arg("vis"));
    c.def_property("name",
        py::overload_cast<>(&vcl::DrawableObject::name, py::const_),
        py::overload_cast<>(&vcl::DrawableObject::name));
    c.def_property("info",
        py::overload_cast<>(&vcl::DrawableObject::info, py::const_),
        py::overload_cast<>(&vcl::DrawableObject::info));
    c.def("init", &vcl::DrawableObject::init);
}

} // namespace vcl::bind
