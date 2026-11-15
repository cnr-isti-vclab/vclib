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

#include <stdexcept>

namespace vcl::bind {

namespace detail {

template<typename ElementType>
void checkVertexRefs(const ElementType& e)
{
    if (!e.isValid()) {
        throw std::runtime_error(
            "Cannot access vertex references on an invalid or null element "
            "proxy.");
    }
}

} // namespace detail

template<ElementConcept ElementType, typename... Options>
void initVertexReferences(pybind11::class_<ElementType, Options...>& c)
{
    using VertexType = vcl::VertexType<ElementType>;

    namespace py = pybind11;

    static const int N = ElementType::VERTEX_COUNT;

    c.def("vertex_count", [](const ElementType& e) {
        detail::checkVertexRefs(e);
        return e.vertexCount();
    });

    c.def(
        "vertex",
        [](ElementType& e, uint i) -> VertexType {
            detail::checkVertexRefs(e);
            if (i >= e.vertexCount()) {
                throw py::index_error();
            }
            return e.vertex(i);
        },
        py::keep_alive<0, 1>());

    c.def(
        "vertex_mod",
        [](ElementType& e, int i) -> VertexType {
            detail::checkVertexRefs(e);
            if (e.vertexCount() == 0) {
                throw py::index_error("Element has no vertices");
            }
            return e.vertexMod(i);
        },
        py::keep_alive<0, 1>());

    c.def("vertex_index", [](const ElementType& e, uint i) {
        detail::checkVertexRefs(e);
        if (i >= e.vertexCount()) {
            throw py::index_error();
        }
        return e.vertexIndex(i);
    });
    c.def("vertex_index_mod", [](const ElementType& e, int i) {
        detail::checkVertexRefs(e);
        if (e.vertexCount() == 0) {
            throw py::index_error("Element has no vertices");
        }
        return e.vertexIndexMod(i);
    });

    c.def("set_vertex", [](ElementType& e, uint i, const VertexType& v) {
        detail::checkVertexRefs(e);
        if (i >= e.vertexCount()) {
            throw py::index_error();
        }
        e.setVertex(i, v);
    });
    c.def("set_vertex", [](ElementType& e, uint i, uint vi) {
        detail::checkVertexRefs(e);
        if (i >= e.vertexCount()) {
            throw py::index_error();
        }
        e.setVertex(i, vi);
    });
    c.def("set_vertex_mod", [](ElementType& e, int i, const VertexType& v) {
        detail::checkVertexRefs(e);
        if (e.vertexCount() == 0) {
            throw py::index_error("Element has no vertices");
        }
        e.setVertexMod(i, v);
    });
    c.def("set_vertex_mod", [](ElementType& e, int i, uint vi) {
        detail::checkVertexRefs(e);
        if (e.vertexCount() == 0) {
            throw py::index_error("Element has no vertices");
        }
        e.setVertexMod(i, vi);
    });

    c.def("set_vertices", [](ElementType& e, const std::vector<uint>& v) {
        detail::checkVertexRefs(e);
        e.setVertices(v);
    });

    c.def("set_vertices", [](ElementType& e, const std::vector<VertexType>& v) {
        detail::checkVertexRefs(e);
        e.setVertices(v);
    });

    c.def("contains_vertex", [](const ElementType& e, const VertexType& v) {
        detail::checkVertexRefs(e);
        return e.containsVertex(v);
    });
    c.def("contains_vertex", [](const ElementType& e, uint vi) {
        detail::checkVertexRefs(e);
        return e.containsVertex(vi);
    });
    c.def("index_of_vertex", [](const ElementType& e, const VertexType& v) {
        detail::checkVertexRefs(e);
        return e.indexOfVertex(v);
    });
    c.def("index_of_vertex", [](const ElementType& e, uint vi) {
        detail::checkVertexRefs(e);
        return e.indexOfVertex(vi);
    });
    c.def(
        "index_of_edge",
        [](const ElementType& e, const VertexType& v1, const VertexType& v2) {
            detail::checkVertexRefs(e);
            return e.indexOfEdge(v1, v2);
        });
    c.def("index_of_edge", [](const ElementType& e, uint vi1, uint vi2) {
        detail::checkVertexRefs(e);
        return e.indexOfEdge(vi1, vi2);
    });

    if constexpr (N < 0) {
        c.def("resize_vertices", [](ElementType& e, uint n) {
            detail::checkVertexRefs(e);
            e.resizeVertices(n);
        });
        c.def("push_vertex", [](ElementType& e, const VertexType& v) {
            detail::checkVertexRefs(e);
            e.pushVertex(v);
        });
        c.def("push_vertex", [](ElementType& e, uint vi) {
            detail::checkVertexRefs(e);
            e.pushVertex(vi);
        });
        c.def("insert_vertex", [](ElementType& e, uint i, const VertexType& v) {
            detail::checkVertexRefs(e);
            e.insertVertex(i, v);
        });
        c.def("insert_vertex", [](ElementType& e, uint i, uint vi) {
            detail::checkVertexRefs(e);
            e.insertVertex(i, vi);
        });
        c.def("erase_vertex", [](ElementType& e, uint i) {
            detail::checkVertexRefs(e);
            e.eraseVertex(i);
        });
        c.def("clear_vertices", [](ElementType& e) {
            detail::checkVertexRefs(e);
            e.clearVertices();
        });
    }

    using VertexView = decltype(std::declval<ElementType&>().vertices());

    if (!registeredTypes.contains(typeid(VertexView))) {
        // inner class that allows to iterate over vertices
        pybind11::class_<VertexView> v(c, "_VertexReferencesRange");
        v.def(
            "__iter__",
            [](VertexView& v) { return py::make_iterator(v.begin(), v.end()); },
            py::keep_alive<0, 1>());

        v.def("__len__", [](const VertexView& v) { return v.size(); });
        v.def(
            "__getitem__",
            [](VertexView& v, int i) {
                int s = static_cast<int>(v.size());
                if (i < 0) {
                    i += s;
                }
                if (i < 0 || i >= s) {
                    throw py::index_error();
                }
                return v[static_cast<uint>(i)];
            },
            py::keep_alive<0, 1>());
        v.def("__bool__", [](const VertexView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(VertexView));
    }

    c.def(
        "vertices",
        [](ElementType& e) {
            detail::checkVertexRefs(e);
            return e.vertices();
        },
        py::keep_alive<0, 1>());

    using VertexIndexView =
        decltype(std::declval<const ElementType&>().vertexIndices());

    if (!registeredTypes.contains(typeid(VertexIndexView))) {
        // inner class that allows to iterate over vertex indices
        pybind11::class_<VertexIndexView> vi(c, "_VertexReferencesIndexRange");
        vi.def(
            "__iter__",
            [](VertexIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        vi.def("__len__", [](const VertexIndexView& v) { return v.size(); });
        vi.def("__getitem__", [](VertexIndexView& v, int i) {
            int s = static_cast<int>(v.size());
            if (i < 0) {
                i += s;
            }
            if (i < 0 || i >= s) {
                throw py::index_error();
            }
            return v[static_cast<uint>(i)];
        });
        vi.def("__bool__", [](const VertexIndexView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(VertexIndexView));
    }

    c.def(
        "vertex_indices",
        [](const ElementType& e) {
            detail::checkVertexRefs(e);
            return e.vertexIndices();
        },
        py::keep_alive<0, 1>());
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_VERTEX_REFERENCES_H
