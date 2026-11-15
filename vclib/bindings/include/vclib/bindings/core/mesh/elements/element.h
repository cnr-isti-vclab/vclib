// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_ELEMENTS_ELEMENT_H
#define VCL_BINDINGS_CORE_MESH_ELEMENTS_ELEMENT_H

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initElement(pybind11::class_<ElementType, Options...>& c)
{
    namespace py = pybind11;

    c.def("index", [](const ElementType& e) { return e.index(); });

    c.def("is_valid", [](const ElementType& e) { return e.isValid(); });
    c.def("is_null", [](const ElementType& e) { return !e.isValid(); });
    c.def("__bool__", [](const ElementType& e) { return e.isValid(); });
    c.def("__eq__", [](const ElementType& e, py::object other) {
        if (other.is_none()) {
            return !e.isValid();
        }
        if (py::isinstance<ElementType>(other)) {
            return e == other.cast<const ElementType&>();
        }
        return false;
    });
    c.def("__ne__", [](const ElementType& e, py::object other) {
        if (other.is_none()) {
            return e.isValid();
        }
        if (py::isinstance<ElementType>(other)) {
            return e != other.cast<const ElementType&>();
        }
        return true;
    });
    c.def("__hash__", [](const ElementType& e) {
        return std::hash<ElementType> {}(e);
    });
    std::string className = py::str(c.attr("__name__"));
    c.def("__repr__", [className](const ElementType& e) {
        if (!e.isValid()) {
            return "<" + className + " (null)>";
        }
        return "<" + className + " " + std::to_string(e.index()) + ">";
    });

    c.def(
        "parent_mesh",
        [](ElementType& v) -> auto& {
            if (!v.isValid()) {
                throw std::runtime_error(
                    "Cannot get parent mesh of an invalid element proxy.");
            }
            return v.parentMesh();
        },
        py::return_value_policy::reference_internal);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_ELEMENTS_ELEMENT_H
