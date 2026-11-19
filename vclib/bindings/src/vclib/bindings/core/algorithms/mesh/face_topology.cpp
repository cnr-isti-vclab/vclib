// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/face_topology.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh.h>

#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace vcl::bind {

void initFaceTopologyAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllFaces =
        []<FaceConcept FaceType>(pybind11::module& m, FaceType = FaceType()) {
            if constexpr (comp::HasAdjacentFaces<FaceType>) {
                m.def(
                    "is_face_manifold_on_edge",
                    [](const FaceType& f, uint edge) -> bool {
                        return isFaceManifoldOnEdge(f, edge);
                    },
                    "face"_a,
                    "edge"_a);

                m.def(
                    "is_face_edge_on_border",
                    [](const FaceType& f, uint edge) -> bool {
                        return isFaceEdgeOnBorder(f, edge);
                    },
                    "face"_a,
                    "edge"_a);

                m.def(
                    "check_flip_edge",
                    [](const FaceType& f, uint edge) -> bool {
                        return checkFlipEdge(f, edge);
                    },
                    "face"_a,
                    "edge"_a);

                m.def(
                    "edge_adjacent_face_count",
                    [](const FaceType& f, uint edge) -> uint {
                        return edgeAdjacentFaceCount(f, edge);
                    },
                    "face"_a,
                    "edge"_a);

                m.def(
                    "face_on_border_edge_count",
                    [](const FaceType& f) -> uint {
                        return faceOnBorderEdgeCount(f);
                    },
                    "face"_a);

                m.def(
                    "face_dihedral_angle_on_edge",
                    [](const FaceType& f, uint edge) {
                        return faceDihedralAngleOnEdge(f, edge);
                    },
                    "face"_a,
                    "edge"_a);

                m.def(
                    "detach_adjacent_faces_on_edge",
                    [](FaceType f, uint edge) {
                        detachAdjacentFacesOnEdge(f, edge);
                    },
                    "face"_a,
                    "edge"_a);

                m.def(
                    "detach_face",
                    [](FaceType f) {
                        detachFace(f);
                    },
                    "face"_a);

                m.def(
                    "flood_face_patch",
                    [](const FaceType& seed, pybind11::function selector) {
                        auto cppSelector = [selector](const FaceType& f) -> bool {
                            return selector(f).template cast<bool>();
                        };
                        return floodFacePatch(seed, cppSelector);
                    },
                    "seed"_a,
                    "face_selector"_a);
            }
        };

    defForAllFaceTypes(m, fAllFaces);
}

} // namespace vcl::bind
