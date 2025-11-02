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

#include <vclib/bindings/core/space/core/material.h>

#include <vclib/bindings/utils.h>
#include <vclib/space/core.h>

namespace vcl::bind {

void initMaterial(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    py::class_<Material> c(m, "Material");
    c.def(py::init<>());

    c.def("base_color", py::overload_cast<>(&Material::baseColor, py::const_));
    c.def("set_base_color", [](Material& m, const Color& c) {
        m.baseColor() = c;
    });
    c.def("metallic", py::overload_cast<>(&Material::metallic, py::const_));
    c.def("set_metallic", [](Material& m, float v) {
        m.metallic() = v;
    });
    c.def("roughness", py::overload_cast<>(&Material::roughness,
                                           py::const_));
    c.def("set_roughness", [](Material& m, float v) {
        m.roughness() = v;
    });
    c.def("emissive_color",
          py::overload_cast<>(&Material::emissiveColor, py::const_));
    c.def("set_emissive_color", [](Material& m, const Color& c) {
        m.emissiveColor() = c;
    });
    c.def("alpha_mode", py::overload_cast<>(&Material::alphaMode, py::const_));
    c.def("set_alpha_mode", [](Material& m, Material::AlphaMode v) {
        m.alphaMode() = v;
    });
    c.def("alpha_cutoff", py::overload_cast<>(&Material::alphaCutoff,
                                           py::const_));
    c.def("set_alpha_cutoff", [](Material& m, float v) {
        m.alphaCutoff() = v;
    });
    c.def("base_color_texture",
          py::overload_cast<>(&Material::baseColorTexture, py::const_));
    c.def("set_base_color_texture", [](Material& m, const Texture& t) {
        m.baseColorTexture() = t;
    });
    c.def("double_sided", py::overload_cast<>(&Material::doubleSided,
                                           py::const_));
    c.def("set_double_sided", [](Material& m, bool v) {
        m.doubleSided() = v;
    });
}

} // namespace vcl::bind
