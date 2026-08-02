// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace vcl::bind {

template<ElementConcept ElementType, typename... Options>
void initVertexReferences(pybind11::class_<ElementType, Options...>& c)
{
    using VertexType = ElementType::VertexType;

    namespace py = pybind11;

    static const int N = ElementType::VERTEX_COUNT;

    c.def("vertex_count", &ElementType::vertexCount);

    c.def(
        "vertex",
        [](ElementType& e, uint i) {
            return e.vertex(i);
        },
        py::return_value_policy::reference);
    c.def(
        "vertex_mod",
        [](ElementType& e, int i) {
            return e.vertexMod(i);
        },
        py::return_value_policy::reference);

    c.def(
        "set_vertex",
        py::overload_cast<uint, VertexType*>(&ElementType::setVertex));
    c.def("set_vertex", py::overload_cast<uint, uint>(&ElementType::setVertex));
    c.def(
        "set_vertex_mod",
        py::overload_cast<int, VertexType*>(&ElementType::setVertexMod));
    c.def(
        "set_vertex_mod",
        py::overload_cast<int, uint>(&ElementType::setVertexMod));

    c.def("set_vertices", [](ElementType& e, const std::vector<uint>& v) {
        e.setVertices(v);
    });

    c.def(
        "set_vertices", [](ElementType& e, const std::vector<VertexType*>& v) {
            e.setVertices(v);
        });

    c.def(
        "contains_vertex",
        py::overload_cast<const VertexType*>(
            &ElementType::containsVertex, py::const_));
    c.def(
        "contains_vertex",
        py::overload_cast<uint>(&ElementType::containsVertex, py::const_));
    c.def(
        "index_of_vertex",
        py::overload_cast<const VertexType*>(
            &ElementType::indexOfVertex, py::const_));
    c.def(
        "index_of_vertex",
        py::overload_cast<uint>(&ElementType::indexOfVertex, py::const_));
    c.def(
        "index_of_edge",
        py::overload_cast<const VertexType*, const VertexType*>(
            &ElementType::indexOfEdge, py::const_));
    c.def(
        "index_of_edge",
        py::overload_cast<uint, uint>(&ElementType::indexOfEdge, py::const_));

    if constexpr (N < 0) {
        c.def("resize_vertices", &ElementType::resizeVertices);
        c.def(
            "push_vertex",
            py::overload_cast<VertexType*>(&ElementType::pushVertex));
        c.def("push_vertex", py::overload_cast<uint>(&ElementType::pushVertex));
        c.def(
            "insert_vertex",
            py::overload_cast<uint, VertexType*>(&ElementType::insertVertex));
        c.def(
            "insert_vertex",
            py::overload_cast<uint, uint>(&ElementType::insertVertex));
        c.def("erase_vertex", &ElementType::eraseVertex);
        c.def("clear_vertices", &ElementType::clearVertices);
    }

    using VertexView = decltype(ElementType().vertices());

    if (!registeredTypes.contains(typeid(VertexView))) {
        // inner class that allows to iterate over vertices
        pybind11::class_<VertexView> v(c, "_VertexReferencesRange");
        v.def(
            "__iter__",
            [](VertexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(VertexView));
    }

    c.def("vertices", py::overload_cast<>(&ElementType::vertices));

    using VertexIndexView = decltype(ElementType().vertexIndices());

    if (!registeredTypes.contains(typeid(VertexIndexView))) {
        // inner class that allows to iterate over vertex indices
        pybind11::class_<VertexIndexView> vi(c, "_VertexReferencesIndexRange");
        vi.def(
            "__iter__",
            [](VertexIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(VertexIndexView));
    }

    c.def(
        "vertex_indices",
        py::overload_cast<>(&ElementType::vertexIndices, py::const_));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_VERTEX_REFERENCES_H
