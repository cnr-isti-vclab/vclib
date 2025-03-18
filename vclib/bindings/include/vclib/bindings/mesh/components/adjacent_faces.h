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

#ifndef VCL_BINDINGS_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_BINDINGS_MESH_COMPONENTS_ADJACENT_FACES_H

#include <vclib/bindings/utils.h>

#include <vclib/concepts/mesh.h>

#include <pybind11/pybind11.h>

namespace vcl::bind {

template<ElementConcept ElementType>
void initAdjacentFaces(pybind11::class_<ElementType>& c)
{
    using FaceType = ElementType::AdjacentFaceType;

    // the actual type of the AdjacentFaces component
    using CompType = RemoveRef<
        decltype(ElementType().template component<CompId::ADJACENT_FACES>())>;

    namespace py = pybind11;

    static const int  N    = ElementType::ADJ_FACE_NUMBER;
    static const bool TTVN = CompType::TIED_TO_VERTEX_NUMBER;

    c.def("adj_face_number", &ElementType::adjFacesNumber);

    c.def(
        "adj_face",
        [](ElementType& e, uint i) {
            return e.adjFace(i);
        },
        py::return_value_policy::reference);
    c.def(
        "adj_face_mod",
        [](ElementType& e, int i) {
            return e.adjFaceMod(i);
        },
        py::return_value_policy::reference);

    c.def(
        "set_adj_face",
        py::overload_cast<uint, FaceType*>(&ElementType::setAdjFace));
    c.def(
        "set_adj_face",
        py::overload_cast<uint, uint>(&ElementType::setAdjFace));
    c.def(
        "set_adj_face_mod",
        py::overload_cast<int, FaceType*>(&ElementType::setAdjFaceMod));
    c.def(
        "set_adj_face_mod",
        py::overload_cast<int, uint>(&ElementType::setAdjFaceMod));

    c.def("set_adj_faces", [](ElementType& e, const std::vector<uint>& v) {
        e.setAdjFaces(v);
    });

    c.def("set_adj_faces", [](ElementType& e, const std::vector<FaceType*>& v) {
        e.setAdjFaces(v);
    });

    c.def(
        "contains_adj_face",
        py::overload_cast<const FaceType*>(
            &ElementType::containsAdjFace, py::const_));
    c.def(
        "contains_adj_face",
        py::overload_cast<uint>(&ElementType::containsAdjFace, py::const_));

    c.def(
        "index_of_adj_face",
        py::overload_cast<const FaceType*>(
            &ElementType::indexOfAdjFace, py::const_));
    c.def(
        "index_of_adj_face",
        py::overload_cast<uint>(&ElementType::indexOfAdjFace, py::const_));

    if constexpr (N < 0 && !TTVN) {
        c.def("resize_adj_faces", &ElementType::resizeAdjFaces);
        c.def(
            "push_adj_face",
            py::overload_cast<FaceType*>(&ElementType::pushAdjFace));
        c.def(
            "push_adj_face",
            py::overload_cast<uint>(&ElementType::pushAdjFace));
        c.def(
            "insert_adj_face",
            py::overload_cast<uint, FaceType*>(&ElementType::insertAdjFace));
        c.def(
            "insert_adj_face",
            py::overload_cast<uint, uint>(&ElementType::insertAdjFace));
        c.def("erase_adj_face", &ElementType::eraseAdjFace);
        c.def("clear_adj_faces", &ElementType::clearAdjFaces);
    }

    using AdjFaceView = decltype(ElementType().adjFaces());

    if (!registeredTypes.contains(typeid(AdjFaceView))) {
        // inner class that allows to iterate over adj faces
        pybind11::class_<AdjFaceView> v(c, "_AdjFaceRange");
        v.def(
            "__iter__",
            [](AdjFaceView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(AdjFaceView));
    }

    c.def("adj_faces", py::overload_cast<>(&ElementType::adjFaces));

    using AdjFaceIndexView = decltype(ElementType().adjFaceIndices());

    if (!registeredTypes.contains(typeid(AdjFaceIndexView))) {
        // inner class that allows to iterate over adj face indices
        pybind11::class_<AdjFaceIndexView> vi(c, "_AdjFaceIndexRange");
        vi.def(
            "__iter__",
            [](AdjFaceIndexView& v) {
                return py::make_iterator(v.begin(), v.end());
            },
            py::keep_alive<0, 1>());
        registeredTypes.insert(typeid(AdjFaceIndexView));
    }

    c.def(
        "adj_face_indices",
        py::overload_cast<>(&ElementType::adjFaceIndices, py::const_));
}

} // namespace vcl::bind

#endif // VCL_BINDINGS_MESH_COMPONENTS_ADJACENT_FACES_H
