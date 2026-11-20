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
#include <pybind11/stl.h>

#include <optional>
#include <stdexcept>

namespace vcl::bind {

namespace detail {

template<typename ElementType>
void checkAdjVertices(const ElementType& e)
{
    if (!e.isValid()) {
        throw std::runtime_error(
            "Cannot access adjacent vertices on an invalid or null element "
            "proxy.");
    }
    if (!e.template isComponentAvailable<CompId::ADJACENT_VERTICES>()) {
        throw MissingComponentException(
            "Component 'ADJACENT_VERTICES' is not available on this element.");
    }
}

} // namespace detail

template<ElementConcept ElementType, typename... Options>
void initAdjacentVertices(pybind11::class_<ElementType, Options...>& c)
{
    using VertexType =
        std::remove_cvref_t<decltype(std::declval<ElementType&>().adjVertex(
            0))>;

    namespace py = pybind11;

    c.def("adj_vertex_count", [](const ElementType& e) {
        detail::checkAdjVertices(e);
        return e.adjVertexCount();
    });

    c.def(
        "adj_vertex",
        [](ElementType& e, uint i) -> std::optional<VertexType> {
            detail::checkAdjVertices(e);
            if (i >= e.adjVertexCount()) {
                throw py::index_error();
            }
            VertexType v = e.adjVertex(i);
            if (!v.isValid()) {
                return std::nullopt;
            }
            return v;
        },
        py::keep_alive<0, 1>());

    c.def(
        "adj_vertex_mod",
        [](ElementType& e, int i) -> std::optional<VertexType> {
            detail::checkAdjVertices(e);
            if (e.adjVertexCount() == 0) {
                throw py::index_error("Element has no adjacent vertices");
            }
            VertexType v = e.adjVertexMod(i);
            if (!v.isValid()) {
                return std::nullopt;
            }
            return v;
        },
        py::keep_alive<0, 1>());

    c.def("adj_vertex_index", [](const ElementType& e, uint i) {
        detail::checkAdjVertices(e);
        if (i >= e.adjVertexCount()) {
            throw py::index_error();
        }
        return e.adjVertexIndex(i);
    });
    c.def("adj_vertex_index_mod", [](const ElementType& e, int i) {
        detail::checkAdjVertices(e);
        if (e.adjVertexCount() == 0) {
            throw py::index_error("Element has no adjacent vertices");
        }
        return e.adjVertexIndexMod(i);
    });

    c.def("set_adj_vertex", [](ElementType& e, uint i, const VertexType& v) {
        detail::checkAdjVertices(e);
        if (i >= e.adjVertexCount()) {
            throw py::index_error();
        }
        e.setAdjVertex(i, v);
    });
    c.def("set_adj_vertex", [](ElementType& e, uint i, uint vi) {
        detail::checkAdjVertices(e);
        if (i >= e.adjVertexCount()) {
            throw py::index_error();
        }
        e.setAdjVertex(i, vi);
    });
    c.def("set_adj_vertex", [](ElementType& e, uint i, py::none) {
        detail::checkAdjVertices(e);
        if (i >= e.adjVertexCount()) {
            throw py::index_error();
        }
        e.setAdjVertex(i, VertexType::NULL_INDEX);
    });
    c.def("set_adj_vertex_mod", [](ElementType& e, int i, const VertexType& v) {
        detail::checkAdjVertices(e);
        if (e.adjVertexCount() == 0) {
            throw py::index_error("Element has no adjacent vertices");
        }
        e.setAdjVertexMod(i, v);
    });
    c.def("set_adj_vertex_mod", [](ElementType& e, int i, uint vi) {
        detail::checkAdjVertices(e);
        if (e.adjVertexCount() == 0) {
            throw py::index_error("Element has no adjacent vertices");
        }
        e.setAdjVertexMod(i, vi);
    });
    c.def("set_adj_vertex_mod", [](ElementType& e, int i, py::none) {
        detail::checkAdjVertices(e);
        if (e.adjVertexCount() == 0) {
            throw py::index_error("Element has no adjacent vertices");
        }
        e.setAdjVertexMod(i, VertexType::NULL_INDEX);
    });

    c.def("set_adj_vertices", [](ElementType& e, const std::vector<uint>& v) {
        detail::checkAdjVertices(e);
        e.setAdjVertices(v);
    });

    c.def(
        "set_adj_vertices",
        [](ElementType& e, const std::vector<VertexType>& v) {
            detail::checkAdjVertices(e);
            e.setAdjVertices(v);
        });

    c.def("contains_adj_vertex", [](const ElementType& e, const VertexType& v) {
        detail::checkAdjVertices(e);
        return e.containsAdjVertex(v);
    });
    c.def("contains_adj_vertex", [](const ElementType& e, uint vi) {
        detail::checkAdjVertices(e);
        return e.containsAdjVertex(vi);
    });

    c.def("index_of_adj_vertex", [](const ElementType& e, const VertexType& v) {
        detail::checkAdjVertices(e);
        return e.indexOfAdjVertex(v);
    });
    c.def("index_of_adj_vertex", [](const ElementType& e, uint vi) {
        detail::checkAdjVertices(e);
        return e.indexOfAdjVertex(vi);
    });

    c.def("resize_adj_vertices", [](ElementType& e, uint n) {
        detail::checkAdjVertices(e);
        e.resizeAdjVertices(n);
    });
    c.def("push_adj_vertex", [](ElementType& e, const VertexType& v) {
        detail::checkAdjVertices(e);
        e.pushAdjVertex(v);
    });
    c.def("push_adj_vertex", [](ElementType& e, uint vi) {
        detail::checkAdjVertices(e);
        e.pushAdjVertex(vi);
    });
    c.def("insert_adj_vertex", [](ElementType& e, uint i, const VertexType& v) {
        detail::checkAdjVertices(e);
        e.insertAdjVertex(i, v);
    });
    c.def("insert_adj_vertex", [](ElementType& e, uint i, uint vi) {
        detail::checkAdjVertices(e);
        e.insertAdjVertex(i, vi);
    });
    c.def("erase_adj_vertex", [](ElementType& e, uint i) {
        detail::checkAdjVertices(e);
        e.eraseAdjVertex(i);
    });
    c.def("clear_adj_vertices", [](ElementType& e) {
        detail::checkAdjVertices(e);
        e.clearAdjVertices();
    });

    using AdjVertexView = decltype(std::declval<ElementType&>().adjVertices());

    if (!registeredTypes.contains(typeid(AdjVertexView))) {
        // inner class that allows to iterate over adj vertices
        pybind11::class_<AdjVertexView> v(c, "_AdjVertexRange");
        v.def(
            "__iter__",
            [](AdjVertexView& v) {
                using It = detail::
                    NullableElementIterator<decltype(v.begin()), VertexType>;
                return py::make_iterator<py::return_value_policy::move>(
                    It {v.begin()}, It {v.end()});
            },
            py::keep_alive<0, 1>());

        v.def("__len__", [](const AdjVertexView& v) { return v.size(); });
        v.def(
            "__getitem__",
            [](AdjVertexView& v, int i) -> std::optional<VertexType> {
                int s = static_cast<int>(v.size());
                if (i < 0) {
                    i += s;
                }
                if (i < 0 || i >= s) {
                    throw py::index_error();
                }
                VertexType vert = v[static_cast<uint>(i)];
                if (!vert.isValid()) {
                    return std::nullopt;
                }
                return vert;
            },
            py::keep_alive<0, 1>());
        v.def("__bool__", [](const AdjVertexView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjVertexView));
    }

    c.def(
        "adj_vertices",
        [](ElementType& e) {
            detail::checkAdjVertices(e);
            return e.adjVertices();
        },
        py::keep_alive<0, 1>());

    using AdjVertexIndexView =
        decltype(std::declval<const ElementType&>().adjVertexIndices());

    if (!registeredTypes.contains(typeid(AdjVertexIndexView))) {
        // inner class that allows to iterate over adj vertex indices
        pybind11::class_<AdjVertexIndexView> vi(c, "_AdjVertexIndexRange");
        vi.def(
            "__iter__",
            [](AdjVertexIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        vi.def("__len__", [](const AdjVertexIndexView& v) { return v.size(); });
        vi.def("__getitem__", [](AdjVertexIndexView& v, int i) {
            int s = static_cast<int>(v.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return v[static_cast<uint>(i)];
        });
        vi.def(
            "__bool__", [](const AdjVertexIndexView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjVertexIndexView));
    }

    c.def(
        "adj_vertex_indices",
        [](const ElementType& e) {
            detail::checkAdjVertices(e);
            return e.adjVertexIndices();
        },
        py::keep_alive<0, 1>());
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_VERTICES_H
