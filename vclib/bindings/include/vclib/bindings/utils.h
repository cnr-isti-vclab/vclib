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

#ifndef VCL_BINDINGS_UTILS_H
#define VCL_BINDINGS_UTILS_H

#include <vclib/meshes.h>

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <set>
#include <sstream>

namespace vcl::bind {

using RegisteredTypesSet = std::set<std::type_index>;

inline RegisteredTypesSet registeredTypes;

template<typename Class>
void defCopy(pybind11::class_<Class>& c)
{
    using namespace pybind11::literals;

    c.def("__copy__", [](const Class& self) {
        return Class(self);
    });
    c.def(
        "__deepcopy__",
        [](const Class& self, pybind11::dict) {
            return Class(self);
        },
        "memo"_a);
}

template<typename Class>
void defRepr(pybind11::class_<Class>& c)
{
    c.def("__repr__", [](const Class& self) {
        std::stringstream ss;
        ss << self;
        return ss.str();
    });
}

template<typename Class>
void defComparisonOperators(pybind11::class_<Class>& c)
{
    namespace py = pybind11;

    c.def(py::self == py::self);
    c.def(py::self != py::self);
    c.def(py::self < py::self);
    c.def(py::self <= py::self);
    c.def(py::self > py::self);
    c.def(py::self >= py::self);
}

void defForAllMeshTypes(auto& pymod, auto&& function)
{
    function.template operator()<vcl::PointCloud>(pymod);
    function.template operator()<vcl::EdgeMesh>(pymod);
    function.template operator()<vcl::PolyMesh>(pymod);
    function.template operator()<vcl::PolyEdgeMesh>(pymod);
    function.template operator()<vcl::TriMesh>(pymod);
    function.template operator()<vcl::TriEdgeMesh>(pymod);
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_UTILS_H
