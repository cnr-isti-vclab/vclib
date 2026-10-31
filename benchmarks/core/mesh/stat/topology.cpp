// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

// face_topology.h is needed by stat/topology.h but it is not included by it
#include <vclib/algorithms/mesh/face_topology.h>
#include <vclib/algorithms/mesh/stat/topology.h>

#include <vector>

// Functions that need adjacencies use the mesh with the topology already
// updated (see State::meshWithTopology()).

BENCH("stat/topology/face_vertex_references_count", [](bench::State& s) {
    const bench::Mesh& m = s.mesh();
    s.run([&]() { return vcl::faceVertexReferencesCount(m); });
});

BENCH("stat/topology/largest_face_size", [](bench::State& s) {
    const bench::Mesh& m = s.mesh();
    s.run([&]() { return vcl::largestFaceSize(m); });
});

BENCH("stat/topology/triangulated_face_count", [](bench::State& s) {
    const bench::Mesh& m = s.mesh();
    s.run([&]() { return vcl::triangulatedFaceCount(m); });
});

BENCH(
    "stat/topology/largest_per_vertex_adjacent_vertices_count",
    [](bench::State& s) {
        const bench::Mesh& m = s.meshWithTopology();
        s.run([&]() { return vcl::largestPerVertexAdjacentVerticesCount(m); });
    });

BENCH(
    "stat/topology/largest_per_vertex_adjacent_faces_count",
    [](bench::State& s) {
        const bench::Mesh& m = s.meshWithTopology();
        s.run([&]() { return vcl::largestPerVertexAdjacentFacesCount(m); });
    });

BENCH(
    "stat/topology/largest_per_element_adjacent_faces_count",
    [](bench::State& s) {
        const bench::Mesh& m = s.meshWithTopology();
        s.run([&]() {
            return vcl::largestPerElementAdjacentFacesCount<vcl::ElemId::FACE>(
                m);
        });
    });

BENCH(
    "stat/topology/vertices_to_duplicate_by_wedge_tex_coords_count",
    [](bench::State& s) {
        const bench::Mesh& m = s.mesh();
        if (!bench::hasWedgeTexCoords(m))
            return s.skip("the mesh has no wedge texture coordinates");

        s.run([&]() { return vcl::verticesToDuplicateByWedgeTexCoordsCount(m); });
    });

BENCH("stat/topology/referenced_vertices", [](bench::State& s) {
    const bench::Mesh& m = s.mesh();
    s.run([&]() {
        vcl::uint unreferenced = 0;
        return vcl::referencedVertices<std::vector<bool>>(m, unreferenced);
    });
});

BENCH("stat/topology/unreferenced_vertex_count", [](bench::State& s) {
    const bench::Mesh& m = s.mesh();
    s.run([&]() { return vcl::unreferencedVertexCount(m); });
});

BENCH("stat/topology/non_manifold_vertex_count", [](bench::State& s) {
    const bench::Mesh& m = s.meshWithTopology();
    s.run([&]() { return vcl::nonManifoldVertexCount(m); });
});

BENCH("stat/topology/is_water_tight", [](bench::State& s) {
    const bench::Mesh& m = s.mesh();
    s.run([&]() { return vcl::isWaterTight(m); });
});

BENCH("stat/topology/hole_count", [](bench::State& s) {
    const bench::Mesh& m = s.meshWithTopology();
    s.run([&]() { return vcl::holeCount(m); });
});

BENCH("stat/topology/connected_components", [](bench::State& s) {
    const bench::Mesh& m = s.meshWithTopology();
    s.run([&]() { return vcl::connectedComponents(m); });
});

BENCH("stat/topology/connected_component_count", [](bench::State& s) {
    const bench::Mesh& m = s.meshWithTopology();
    s.run([&]() { return vcl::connectedComponentCount(m); });
});
