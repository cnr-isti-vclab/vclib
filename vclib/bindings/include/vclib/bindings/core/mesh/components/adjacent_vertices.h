// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType>
void initAdjacentVertices(pybind11::class_<ElementType>& c)
{
    using VertexType = ElementType::AdjacentVertexType;

    // the actual type of the AdjacentVertices component
    using CompType = RemoveRef<
        decltype(ElementType()
                     .template component<CompId::ADJACENT_VERTICES>())>;

    namespace py = pybind11;

    c.def("adj_vertex_count", &ElementType::adjVertexCount);

    c.def(
        "adj_vertex",
        [](ElementType& e, uint i) {
            return e.adjVertex(i);
        },
        py::return_value_policy::reference);
    c.def(
        "adj_vertex_mod",
        [](ElementType& e, int i) {
            return e.adjVertexMod(i);
        },
        py::return_value_policy::reference);

    c.def(
        "set_adj_vertex",
        py::overload_cast<uint, VertexType*>(&ElementType::setAdjVertex));
    c.def(
        "set_adj_vertex",
        py::overload_cast<uint, uint>(&ElementType::setAdjVertex));
    c.def(
        "set_adj_vertex_mod",
        py::overload_cast<int, VertexType*>(&ElementType::setAdjVertexMod));
    c.def(
        "set_adj_vertex_mod",
        py::overload_cast<int, uint>(&ElementType::setAdjVertexMod));

    c.def("set_adj_vertices", [](ElementType& e, const std::vector<uint>& v) {
        e.setAdjVertices(v);
    });

    c.def(
        "set_adj_vertices",
        [](ElementType& e, const std::vector<VertexType*>& v) {
            e.setAdjVertices(v);
        });

    c.def(
        "contains_adj_vertex",
        py::overload_cast<const VertexType*>(
            &ElementType::containsAdjVertex, py::const_));
    c.def(
        "contains_adj_vertex",
        py::overload_cast<uint>(&ElementType::containsAdjVertex, py::const_));

    c.def(
        "index_of_adj_vertex",
        py::overload_cast<const VertexType*>(
            &ElementType::indexOfAdjVertex, py::const_));
    c.def(
        "index_of_adj_vertex",
        py::overload_cast<uint>(&ElementType::indexOfAdjVertex, py::const_));

    c.def("resize_adj_vertices", &ElementType::resizeAdjVertices);
    c.def(
        "push_adj_vertex",
        py::overload_cast<VertexType*>(&ElementType::pushAdjVertex));
    c.def(
        "push_adj_vertex",
        py::overload_cast<uint>(&ElementType::pushAdjVertex));
    c.def(
        "insert_adj_vertex",
        py::overload_cast<uint, VertexType*>(&ElementType::insertAdjVertex));
    c.def(
        "insert_adj_vertex",
        py::overload_cast<uint, uint>(&ElementType::insertAdjVertex));
    c.def("erase_adj_vertex", &ElementType::eraseAdjVertex);
    c.def("clear_adj_vertices", &ElementType::clearAdjVertices);

    using AdjVertexView = decltype(ElementType().adjVertices());

    if (!registeredTypes.contains(typeid(AdjVertexView))) {
        // inner class that allows to iterate over adj vertices
        pybind11::class_<AdjVertexView> v(c, "_AdjVertexRange");
        v.def(
            "__iter__",
            [](AdjVertexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(AdjVertexView));
    }

    c.def("adj_vertices", py::overload_cast<>(&ElementType::adjVertices));

    using AdjVertexIndexView = decltype(ElementType().adjVertexIndices());

    if (!registeredTypes.contains(typeid(AdjVertexIndexView))) {
        // inner class that allows to iterate over adj vertex indices
        pybind11::class_<AdjVertexIndexView> vi(c, "_AdjVertexIndexRange");
        vi.def(
            "__iter__",
            [](AdjVertexIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        registeredTypes.insert(typeid(AdjVertexIndexView));
    }

    c.def(
        "adj_vertex_indices",
        py::overload_cast<>(&ElementType::adjVertexIndices, py::const_));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_VERTICES_H
