// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/bindings/core/algorithms/mesh/update.h>
#include <vclib/bindings/utils.h>

#include <vclib/algorithms/mesh/update/border.h>
#include <vclib/algorithms/mesh/update/selection.h>
#include <vclib/algorithms/mesh/update/topology.h>

namespace vcl::bind::detail {

template<typename T>
concept FaceEdgeMeshConcept = FaceMeshConcept<T> && EdgeMeshConcept<T>;

void initUpdateSelectionTopologyBorderAlgorithms(pybind11::module& m)
{
    namespace py = pybind11;
    using namespace py::literals;

    auto fAllMeshes =
        []<MeshConcept MeshType>(pybind11::module& m, MeshType = MeshType()) {
            // selection.h

            m.def(
                "clear_vertex_selection",
                [](MeshType& m) { return clearVertexSelection(m); },
                "mesh"_a);

            // topology.h

            m.def(
                "clear_vertex_adjacent_vertices",
                [](MeshType& m) { return clearVertexAdjacentVertices(m); },
                "mesh"_a);

            m.def(
                "update_vertex_adjacent_vertices",
                [](MeshType& m) { return updateVertexAdjacentVertices(m); },
                "mesh"_a);
        };

    defForAllMeshTypes(m, fAllMeshes);

    auto fFaceMeshes = []<FaceMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // border.h

        m.def(
            "update_border",
            [](MeshType& m) { return vcl::updateBorder(m); },
            "mesh"_a);

        // selection.h

        m.def(
            "clear_face_selection",
            [](MeshType& m) { return clearFaceSelection(m); },
            "mesh"_a);

        m.def(
            "clear_face_edges_selection",
            [](MeshType& m) { return clearFaceEdgesSelection(m); },
            "mesh"_a);

        m.def(
            "select_non_manifold_vertices",
            [](MeshType& m, bool csf) {
                return selectNonManifoldVertices(m, csf);
            },
            "mesh"_a,
            "clear_selection_first"_a = true);

        m.def(
            "select_crease_face_edges",
            [](MeshType& m, double arn, double arp, double abe) {
                return selectCreaseFaceEdges(m, arn, arp, abe);
            },
            "mesh"_a,
            "angle_rad_neg"_a,
            "angle_rad_pos"_a,
            "also_border_edges"_a = false);

        // topology.h

        m.def(
            "clear_vertex_adjacent_faces",
            [](MeshType& m) { return clearVertexAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "update_vertex_adjacent_faces",
            [](MeshType& m) { return updateVertexAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "clear_face_adjacent_faces",
            [](MeshType& m) { return clearFaceAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "update_face_adjacent_faces",
            [](MeshType& m) { return updateFaceAdjacentFaces(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceMeshes);

    auto fEdgeMeshes = []<EdgeMeshConcept MeshType>(
                           pybind11::module& m, MeshType = MeshType()) {
        // selection.h

        m.def(
            "clear_edge_selection",
            [](MeshType& m) { return clearEdgeSelection(m); },
            "mesh"_a);

        // topology.h

        m.def(
            "clear_vertex_adjacent_edges",
            [](MeshType& m) { return clearVertexAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "update_vertex_adjacent_edges",
            [](MeshType& m) { return updateVertexAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "clear_edge_adjacent_edges",
            [](MeshType& m) { return clearEdgeAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "update_edge_adjacent_edges",
            [](MeshType& m) { return updateEdgeAdjacentEdges(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fEdgeMeshes);

    auto fFaceEdgeMeshes = []<FaceEdgeMeshConcept MeshType>(
                               pybind11::module& m, MeshType = MeshType()) {
        m.def(
            "clear_face_adjacent_edges",
            [](MeshType& m) { return clearFaceAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "update_face_adjacent_edges",
            [](MeshType& m) { return updateFaceAdjacentEdges(m); },
            "mesh"_a);

        m.def(
            "clear_edge_adjacent_faces",
            [](MeshType& m) { return clearEdgeAdjacentFaces(m); },
            "mesh"_a);

        m.def(
            "update_edge_adjacent_faces",
            [](MeshType& m) { return updateEdgeAdjacentFaces(m); },
            "mesh"_a);
    };

    defForAllMeshTypes(m, fFaceEdgeMeshes);
}

} // namespace vcl::bind::detail
