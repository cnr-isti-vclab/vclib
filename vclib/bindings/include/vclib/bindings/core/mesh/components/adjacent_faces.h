// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_FACES_H

#include <vclib/bindings/utils.h>

#include <vclib/mesh.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <optional>
#include <stdexcept>

namespace vcl::bind {

namespace detail {

template<typename ElementType>
void checkAdjFaces(const ElementType& e)
{
    if (!e.isValid()) {
        throw std::runtime_error(
            "Cannot access adjacent faces on an invalid or null element "
            "proxy.");
    }
    if (!e.template isComponentAvailable<CompId::ADJACENT_FACES>()) {
        throw MissingComponentException(
            "Component 'ADJACENT_FACES' is not available on this element.");
    }
}

} // namespace detail

template<ElementConcept ElementType, typename... Options>
void initAdjacentFaces(pybind11::class_<ElementType, Options...>& c)
{
    using FaceType =
        std::remove_cvref_t<decltype(std::declval<ElementType&>().adjFace(0))>;

    namespace py = pybind11;

    static const int  N = ElementType::ADJ_FACE_COUNT;
    static const bool TTVC =
        comp::ComponentTypeT<CompId::ADJACENT_FACES, ElementType>::
            TIED_TO_VERTEX_COUNT;

    c.def("adj_face_count", [](const ElementType& e) {
        detail::checkAdjFaces(e);
        return e.adjFaceCount();
    });

    c.def(
        "adj_face",
        [](ElementType& e, uint i) -> std::optional<FaceType> {
            detail::checkAdjFaces(e);
            if (i >= e.adjFaceCount()) {
                throw py::index_error();
            }
            FaceType f = e.adjFace(i);
            if (!f.isValid()) {
                return std::nullopt;
            }
            return f;
        },
        py::keep_alive<0, 1>());

    c.def(
        "adj_face_mod",
        [](ElementType& e, int i) -> std::optional<FaceType> {
            detail::checkAdjFaces(e);
            if (e.adjFaceCount() == 0) {
                throw py::index_error("Element has no adjacent faces");
            }
            FaceType f = e.adjFaceMod(i);
            if (!f.isValid()) {
                return std::nullopt;
            }
            return f;
        },
        py::keep_alive<0, 1>());

    c.def("adj_face_index", [](const ElementType& e, uint i) {
        detail::checkAdjFaces(e);
        if (i >= e.adjFaceCount()) {
            throw py::index_error();
        }
        return e.adjFaceIndex(i);
    });
    c.def("adj_face_index_mod", [](const ElementType& e, int i) {
        detail::checkAdjFaces(e);
        if (e.adjFaceCount() == 0) {
            throw py::index_error("Element has no adjacent faces");
        }
        return e.adjFaceIndexMod(i);
    });

    c.def("set_adj_face", [](ElementType& e, uint i, const FaceType& f) {
        detail::checkAdjFaces(e);
        if (i >= e.adjFaceCount()) {
            throw py::index_error();
        }
        e.setAdjFace(i, f);
    });
    c.def("set_adj_face", [](ElementType& e, uint i, uint fi) {
        detail::checkAdjFaces(e);
        if (i >= e.adjFaceCount()) {
            throw py::index_error();
        }
        e.setAdjFace(i, fi);
    });
    c.def("set_adj_face", [](ElementType& e, uint i, py::none) {
        detail::checkAdjFaces(e);
        if (i >= e.adjFaceCount()) {
            throw py::index_error();
        }
        e.setAdjFace(i, FaceType::NULL_INDEX);
    });
    c.def("set_adj_face_mod", [](ElementType& e, int i, const FaceType& f) {
        detail::checkAdjFaces(e);
        if (e.adjFaceCount() == 0) {
            throw py::index_error("Element has no adjacent faces");
        }
        e.setAdjFaceMod(i, f);
    });
    c.def("set_adj_face_mod", [](ElementType& e, int i, uint fi) {
        detail::checkAdjFaces(e);
        if (e.adjFaceCount() == 0) {
            throw py::index_error("Element has no adjacent faces");
        }
        e.setAdjFaceMod(i, fi);
    });
    c.def("set_adj_face_mod", [](ElementType& e, int i, py::none) {
        detail::checkAdjFaces(e);
        if (e.adjFaceCount() == 0) {
            throw py::index_error("Element has no adjacent faces");
        }
        e.setAdjFaceMod(i, FaceType::NULL_INDEX);
    });

    c.def("set_adj_faces", [](ElementType& e, const std::vector<uint>& v) {
        detail::checkAdjFaces(e);
        e.setAdjFaces(v);
    });

    c.def("set_adj_faces", [](ElementType& e, const std::vector<FaceType>& v) {
        detail::checkAdjFaces(e);
        e.setAdjFaces(v);
    });

    c.def("contains_adj_face", [](const ElementType& e, const FaceType& f) {
        detail::checkAdjFaces(e);
        return e.containsAdjFace(f);
    });
    c.def("contains_adj_face", [](const ElementType& e, uint fi) {
        detail::checkAdjFaces(e);
        return e.containsAdjFace(fi);
    });

    c.def("index_of_adj_face", [](const ElementType& e, const FaceType& f) {
        detail::checkAdjFaces(e);
        return e.indexOfAdjFace(f);
    });
    c.def("index_of_adj_face", [](const ElementType& e, uint fi) {
        detail::checkAdjFaces(e);
        return e.indexOfAdjFace(fi);
    });

    if constexpr (N < 0 && !TTVC) {
        c.def("resize_adj_faces", [](ElementType& e, uint n) {
            detail::checkAdjFaces(e);
            e.resizeAdjFaces(n);
        });
        c.def("push_adj_face", [](ElementType& e, const FaceType& f) {
            detail::checkAdjFaces(e);
            e.pushAdjFace(f);
        });
        c.def("push_adj_face", [](ElementType& e, uint fi) {
            detail::checkAdjFaces(e);
            e.pushAdjFace(fi);
        });
        c.def("insert_adj_face", [](ElementType& e, uint i, const FaceType& f) {
            detail::checkAdjFaces(e);
            e.insertAdjFace(i, f);
        });
        c.def("insert_adj_face", [](ElementType& e, uint i, uint fi) {
            detail::checkAdjFaces(e);
            e.insertAdjFace(i, fi);
        });
        c.def("erase_adj_face", [](ElementType& e, uint i) {
            detail::checkAdjFaces(e);
            e.eraseAdjFace(i);
        });
        c.def("clear_adj_faces", [](ElementType& e) {
            detail::checkAdjFaces(e);
            e.clearAdjFaces();
        });
    }

    using AdjFaceView = decltype(std::declval<ElementType&>().adjFaces());

    if (!registeredTypes.contains(typeid(AdjFaceView))) {
        // inner class that allows to iterate over adj faces
        pybind11::class_<AdjFaceView> v(c, "_AdjFaceRange");
        v.def(
            "__iter__",
            [](AdjFaceView& v) {
                using It = detail::
                    NullableElementIterator<decltype(v.begin()), FaceType>;
                return py::make_iterator<py::return_value_policy::move>(
                    It {v.begin()}, It {v.end()});
            },
            py::keep_alive<0, 1>());

        v.def("__len__", [](const AdjFaceView& v) { return v.size(); });
        v.def(
            "__getitem__",
            [](AdjFaceView& v, int i) -> std::optional<FaceType> {
                int s = static_cast<int>(v.size());
                if (i < 0) {
                    i += s;
                }
                if (i < 0 || i >= s) {
                    throw py::index_error();
                }
                FaceType f = v[static_cast<uint>(i)];
                if (!f.isValid()) {
                    return std::nullopt;
                }
                return f;
            },
            py::keep_alive<0, 1>());
        v.def("__bool__", [](const AdjFaceView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjFaceView));
    }

    c.def(
        "adj_faces",
        [](ElementType& e) {
            detail::checkAdjFaces(e);
            return e.adjFaces();
        },
        py::keep_alive<0, 1>());

    using AdjFaceIndexView =
        decltype(std::declval<const ElementType&>().adjFaceIndices());

    if (!registeredTypes.contains(typeid(AdjFaceIndexView))) {
        // inner class that allows to iterate over adj face indices
        pybind11::class_<AdjFaceIndexView> vi(c, "_AdjFaceIndexRange");
        vi.def(
            "__iter__",
            [](AdjFaceIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());

        vi.def("__len__", [](const AdjFaceIndexView& v) { return v.size(); });
        vi.def("__getitem__", [](AdjFaceIndexView& v, int i) {
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
            "__bool__", [](const AdjFaceIndexView& v) { return !v.empty(); });

        registeredTypes.insert(typeid(AdjFaceIndexView));
    }

    c.def(
        "adj_face_indices",
        [](const ElementType& e) {
            detail::checkAdjFaces(e);
            return e.adjFaceIndices();
        },
        py::keep_alive<0, 1>());
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_CORE_MESH_COMPONENTS_ADJACENT_FACES_H
