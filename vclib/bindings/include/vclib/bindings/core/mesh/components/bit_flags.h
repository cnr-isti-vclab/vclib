// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_BIT_FLAGS_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_BIT_FLAGS_H

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

#include <stdexcept>

namespace vcl::bind {

namespace detail {

template<typename ElementType>
void checkBitFlags(const ElementType& e)
{
    if (!e.isValid()) {
        throw std::runtime_error(
            "Cannot access bit flags on an invalid or null element proxy.");
    }
}

template<ElementConcept ElementType, typename... Options>
void initCommonFlags(pybind11::class_<ElementType, Options...>& c)
{
    namespace py = pybind11;

    c.def("deleted", [](const ElementType& e) {
        checkBitFlags(e);
        return e.deleted();
    });
    c.def("selected", [](const ElementType& e) {
        checkBitFlags(e);
        return e.selected();
    });
    c.def("set_selected", [](ElementType& e, bool s) {
        checkBitFlags(e);
        e.selected() = s;
    });
    c.def("on_border", [](const ElementType& e) {
        checkBitFlags(e);
        return e.onBorder();
    });
    c.def("visited", [](const ElementType& e) {
        checkBitFlags(e);
        return e.visited();
    });
    c.def("set_visited", [](ElementType& e, bool v) {
        checkBitFlags(e);
        e.visited() = v;
    });
    c.def("user_bit", [](const ElementType& e, uint i) {
        checkBitFlags(e);
        return e.userBit(i);
    });
    c.def("set_user_bit", [](ElementType& e, uint i, bool b) {
        checkBitFlags(e);
        e.userBit(i) = b;
    });
    c.def("reset_bit_flags", [](ElementType& e) {
        checkBitFlags(e);
        e.resetBitFlags();
    });
}

} // namespace detail

template<ElementConcept ElementType, typename... Options>
void initBitFlags(pybind11::class_<ElementType, Options...>& c)
{
    namespace py = pybind11;

    detail::initCommonFlags(c);

    c.def("set_on_border", [](ElementType& e, bool b) {
        detail::checkBitFlags(e);
        e.onBorder() = b;
    });
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_BIT_FLAGS_H
