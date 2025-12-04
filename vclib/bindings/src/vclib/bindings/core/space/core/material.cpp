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

    using enum Material::AlphaMode;
    py::enum_<Material::AlphaMode> am(c, "AlphaMode");
    am.value("ALPHA_OPAQUE", ALPHA_OPAQUE);
    am.value("ALPHA_MASK", ALPHA_MASK);
    am.value("ALPHA_BLEND", ALPHA_BLEND);
    am.export_values();

    using enum Material::TextureType;
    py::enum_<Material::TextureType> tt(c, "TextureType");
    tt.value("BASE_COLOR", BASE_COLOR);
    tt.value("METALLIC_ROUGHNESS", METALLIC_ROUGHNESS);
    tt.value("NORMAL", NORMAL);
    tt.value("OCCLUSION", OCCLUSION);
    tt.value("EMISSIVE", EMISSIVE);
    tt.value("COUNT", COUNT);
    tt.export_values();

    c.def(py::init<>());

    c.def("base_color", py::overload_cast<>(&Material::baseColor, py::const_));
    c.def(
        "set_base_color",
        [](Material& m, const Color& c) {
            m.baseColor() = c;
        },
        "base_color"_a);
    c.def("metallic", py::overload_cast<>(&Material::metallic, py::const_));
    c.def(
        "set_metallic",
        [](Material& m, float v) {
            m.metallic() = v;
        },
        "metallic"_a);
    c.def("roughness", py::overload_cast<>(&Material::roughness, py::const_));
    c.def(
        "set_roughness",
        [](Material& m, float v) {
            m.roughness() = v;
        },
        "roughness"_a);
    c.def(
        "emissive_color",
        py::overload_cast<>(&Material::emissiveColor, py::const_));
    c.def(
        "set_emissive_color",
        [](Material& m, const Color& c) {
            m.emissiveColor() = c;
        },
        "emissive_color"_a);
    c.def("alpha_mode", py::overload_cast<>(&Material::alphaMode, py::const_));
    c.def(
        "set_alpha_mode",
        [](Material& m, Material::AlphaMode v) {
            m.alphaMode() = v;
        },
        "alpha_mode"_a);
    c.def(
        "alpha_cutoff",
        py::overload_cast<>(&Material::alphaCutoff, py::const_));
    c.def(
        "set_alpha_cutoff",
        [](Material& m, float v) {
            m.alphaCutoff() = v;
        },
        "alpha_cutoff"_a);
    c.def(
        "double_sided",
        py::overload_cast<>(&Material::doubleSided, py::const_));
    c.def(
        "set_double_sided",
        [](Material& m, bool v) {
            m.doubleSided() = v;
        },
        "double_sided"_a);
    c.def(
        "normal_scale",
        py::overload_cast<>(&Material::normalScale, py::const_));
    c.def(
        "set_normal_scale",
        [](Material& m, float v) {
            m.normalScale() = v;
        },
        "normal_scale"_a);
    c.def(
        "occlusion_strength",
        py::overload_cast<>(&Material::occlusionStrength, py::const_));
    c.def(
        "set_occlusion_strength",
        [](Material& m, float v) {
            m.occlusionStrength() = v;
        },
        "occlusion_strength"_a);
    c.def(
        "base_color_texture_descriptor",
        py::overload_cast<>(&Material::baseColorTextureDescriptor, py::const_));
    c.def(
        "set_base_color_texture_descriptor",
        [](Material& m, const TextureDescriptor& t) {
            m.baseColorTextureDescriptor() = t;
        },
        "base_color_texture_descriptor"_a);

    c.def(
        "texture_descriptor",
        py::overload_cast<uint>(&Material::textureDescriptor, py::const_));
    c.def(
        "set_texture_descriptor",
        [](Material& m, uint type, const TextureDescriptor& t) {
            m.textureDescriptor(type) = t;
        },
        "type"_a,
        "texture_descriptor"_a);
    c.def(
        "texture_descriptor",
        py::overload_cast<Material::TextureType>(&Material::textureDescriptor, py::const_));
    c.def(
        "set_texture_descriptor",
        [](Material&                m,
           Material::TextureType    type,
           const TextureDescriptor& t) {
            m.textureDescriptor(type) = t;
        },
        "type"_a,
        "texture_descriptor"_a);

    c.def_static(
        "texture_type_to_color_space",
        &Material::textureTypeToColorSpace,
        "texture_type"_a);
}

} // namespace vcl::bind
