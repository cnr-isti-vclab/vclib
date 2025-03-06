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

#ifndef VCL_BINDINGS_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_BINDINGS_MESH_COMPONENTS_ADJACENT_EDGES_H

#include <vclib/concepts/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType>
void initAdjacentEdges(pybind11::class_<ElementType>& c)
{
    using EdgeType = ElementType::AdjacentEdgeType;

    // the actual type of the AdjacentEdges component
    using CompType = RemoveRef<
        decltype(ElementType().template component<CompId::ADJACENT_EDGES>())>;

    namespace py = pybind11;

    static const int N = ElementType::ADJ_EDGE_NUMBER;
    static const bool TTVN = CompType::TIED_TO_VERTEX_NUMBER;

    c.def("adj_edge_number", &ElementType::adjEdgesNumber);

    c.def("adj_edge", [](ElementType& e, uint i) {
        return e.adjEdge(i);
    }, py::return_value_policy::reference);
    c.def("adj_edge_mod", [](ElementType& e, int i) {
        return e.adjEdgeMod(i);
    }, py::return_value_policy::reference);

    c.def(
        "set_adj_edge",
        py::overload_cast<uint, EdgeType*>(&ElementType::setAdjEdge));
    c.def(
        "set_adj_edge",
        py::overload_cast<uint, uint>(&ElementType::setAdjEdge));
    c.def(
        "set_adj_edge_mod",
        py::overload_cast<int, EdgeType*>(&ElementType::setAdjEdgeMod));
    c.def(
        "set_adj_edge_mod",
        py::overload_cast<int, uint>(&ElementType::setAdjEdgeMod));

    c.def("set_adj_edges", [](ElementType& e, const std::vector<uint>& v) {
        e.setAdjEdges(v);
    });

    c.def(
        "set_adj_edges", [](ElementType& e, const std::vector<EdgeType*>& v) {
            e.setAdjEdges(v);
        });

    c.def(
        "contains_adj_edge",
        py::overload_cast<const EdgeType*>(
            &ElementType::containsAdjEdge, py::const_));
    c.def(
        "contains_adj_edge",
        py::overload_cast<uint>(&ElementType::containsAdjEdge, py::const_));

    c.def(
        "index_of_adj_edge",
        py::overload_cast<const EdgeType*>(
            &ElementType::indexOfAdjEdge, py::const_));
    c.def(
        "index_of_adj_edge",
        py::overload_cast<uint>(&ElementType::indexOfAdjEdge, py::const_));

    if constexpr (N < 0 && !TTVN) {
        c.def("resize_adj_edges", &ElementType::resizeAdjEdges);
        c.def(
            "push_adj_edge",
            py::overload_cast<EdgeType*>(&ElementType::pushAdjEdge));
        c.def(
            "push_adj_edge",
            py::overload_cast<uint>(&ElementType::pushAdjEdge));
        c.def(
            "insert_adj_edge",
            py::overload_cast<uint, EdgeType*>(&ElementType::insertAdjEdge));
        c.def(
            "insert_adj_edge",
            py::overload_cast<uint, uint>(&ElementType::insertAdjEdge));
        c.def("erase_adj_edge", &ElementType::eraseAdjEdge);
        c.def("clear_adj_edges", &ElementType::clearAdjEdges);
    }

    using AdjEdgeView = View<decltype(ElementType().adjEdgeBegin())>;

    // inner class that allows to iterate over adj edges
    pybind11::class_<AdjEdgeView> v(c, "_AdjEdgeRange");
    v.def(
        "__iter__",
        [](AdjEdgeView& v) {
            return py::make_iterator(v.begin(), v.end());
        },
        py::keep_alive<0, 1>());

    c.def("adj_edges", py::overload_cast<>(&ElementType::adjEdges));

    using AdjEdgeIndexView = View<decltype(ElementType().adjEdgeIndexBegin())>;

    // inner class that allows to iterate over adj edge indices
    pybind11::class_<AdjEdgeIndexView> vi(c, "_AdjEdgeIndexRange");
    vi.def(
        "__iter__",
        [](AdjEdgeIndexView& v) {
            return py::make_iterator(v.begin(), v.end());
        },
        py::keep_alive<0, 1>());

    c.def(
        "adj_edge_indices",
        py::overload_cast<>(&ElementType::adjEdgeIndices, py::const_));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_COMPONENTS_ADJACENT_EDGES_H
