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

/**
 * @brief calls a function for all mesh types, if the function is callable for
 * the specific mesh type.
 *
 * The function should be defined in the following way:
 *
 * @code{.cpp}
 * pybind11::module m;
 *
 * auto fun = []<MeshConcept MeshType>(
 *                pybind11::module& m, MeshType = MeshType()) {
 *
 *      // add the function that uses MeshType to the module
 * };
 *
 * defForAllMeshTypes(m, fun);
 * @endcode
 *
 * The second MeshType argument is there to allow the compiler to deduce the
 * type of the mesh. It must not be used in the lambda body.
 * You can decide for which type of mesh the function should be defined by
 * using the MeshConcept concepts (e.g. if you want to define the function for
 * all mesh types that are FaceMeshConcept, you can use FaceMeshConcept as
 * template argument).
 *
 * @note If the function is defined in a different way, the compiler won't
 * give any error. The function will just not be defined for the specific mesh
 * type.
 *
 * @param pymod
 * @param function
 */
void defForAllMeshTypes(auto& pymod, auto&& function)
{
    using FType = decltype(function);

    if constexpr (std::invocable<FType, pybind11::module&, vcl::PointCloud>) {
        function.template operator()<vcl::PointCloud>(pymod);
    }
    if constexpr (std::invocable<FType, pybind11::module&, vcl::EdgeMesh>) {
        function.template operator()<vcl::EdgeMesh>(pymod);
    }
    if constexpr (std::invocable<FType, pybind11::module&, vcl::PolyMesh>) {
        function.template operator()<vcl::PolyMesh>(pymod);
    }
    if constexpr (std::invocable<FType, pybind11::module&, vcl::PolyEdgeMesh>) {
        function.template operator()<vcl::PolyEdgeMesh>(pymod);
    }
    if constexpr (std::invocable<FType, pybind11::module&, vcl::TriMesh>) {
        function.template operator()<vcl::TriMesh>(pymod);
    }
    if constexpr (std::invocable<FType, pybind11::module&, vcl::TriEdgeMesh>) {
        function.template operator()<vcl::TriEdgeMesh>(pymod);
    }
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_UTILS_H
