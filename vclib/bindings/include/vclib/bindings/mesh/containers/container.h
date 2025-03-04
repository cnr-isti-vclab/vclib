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

#ifndef VCL_BINDINGS_MESH_CONTAINERS_CONTAINER_H
#define VCL_BINDINGS_MESH_CONTAINERS_CONTAINER_H

#include <vclib/concepts/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

namespace detail {

template<uint ELEM_ID, uint COMP_ID, MeshConcept MeshType>
void addOptionalComponentFunctions(
    pybind11::class_<MeshType>& c,
    const std::string&          name,
    const std::string&          compName)
{
    if constexpr (mesh::HasPerElementOptionalComponent<
                      MeshType,
                      ELEM_ID,
                      COMP_ID>) {
        c.def(
            ("is_per_" + name + "_" + compName + "_enabled").c_str(),
            [](const MeshType& t) {
                return t.template isPerElementComponentEnabled<
                    ELEM_ID,
                    COMP_ID>();
            });
        c.def(
            ("enable_per_" + name + "_" + compName).c_str(),
            [](MeshType& t) {
                return t.template enablePerElementComponent<
                    ELEM_ID,
                    COMP_ID>();
            });
        c.def(
            ("disable_per_" + name + "_" + compName).c_str(),
            [](MeshType& t) {
                return t.template disablePerElementComponent<
                    ELEM_ID,
                    COMP_ID>();
            });
    }
}

} // namespace detail

template<ElementConcept Element, MeshConcept MeshType>
void initContainer(
    pybind11::class_<MeshType>& c,
    const std::string&          name,
    std::string                 namePlural = "")
{
    namespace py = pybind11;

    static const uint ELEM_ID = Element::ELEMENT_ID;

    std::string capitalName = name;
    capitalName[0] = std::toupper(capitalName[0]);

    if (namePlural.empty())
        namePlural = name + "s";

    c.def(name.c_str(), [](MeshType& t, uint i) -> Element& {
        return t.template element<ELEM_ID>(i);
    }, py::return_value_policy::reference);

    c.def((name + "_number").c_str(), &MeshType::template number<ELEM_ID>);
    c.def(
        (name + "_container_size").c_str(),
        &MeshType::template containerSize<ELEM_ID>);
    c.def(
        ("deleted_" + name + "_number").c_str(),
        &MeshType::template deletedNumber<ELEM_ID>);

    c.def(
        ("add_" + name).c_str(),
        py::overload_cast<>(&MeshType::template add<ELEM_ID>));

    c.def(
        ("add_" + namePlural).c_str(),
        py::overload_cast<uint>(&MeshType::template add<ELEM_ID>));

    c.def(
        ("clear_" + namePlural).c_str(),
        &MeshType::template clearElements<ELEM_ID>);
    c.def(
        ("resize_" + namePlural).c_str(),
        &MeshType::template resize<ELEM_ID>);
    c.def(
        ("reserve_" + namePlural).c_str(),
        &MeshType::template reserve<ELEM_ID>);

    c.def(
        ("compact_" + namePlural).c_str(),
        &MeshType::template compactElements<ELEM_ID>);

    c.def(
        ("delete_" + name).c_str(),
        py::overload_cast<uint>(&MeshType::template deleteElement<ELEM_ID>));

    using ElemView = View<decltype(MeshType().template begin<ELEM_ID>())>;

    // inner class that allows to iterate over elements
    pybind11::class_<ElemView> v(c, ("_" + capitalName + "Range").c_str());

    v.def(
        "__iter__",
        [](ElemView& t) {
            return py::make_iterator(
                t.begin(), t.end());
        },
        py::keep_alive<0, 1>());

    c.def(
        namePlural.c_str(),
        py::overload_cast<bool>(&MeshType::template elements<ELEM_ID>),
        py::arg("jump_deleted") = true);

    // optional components

    detail::addOptionalComponentFunctions<ELEM_ID, CompId::COLOR>(
        c, name, "color");
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_CONTAINERS_CONTAINER_H
