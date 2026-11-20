// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh.h>

#include <vector>

// Functions that need adjacencies use the mesh with the topology already
// updated (see State::meshWithTopology()).

BENCH(
    "stat/topology/face_vertex_references_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() { return vcl::faceVertexReferencesCount(m); });
    });

BENCH(
    "stat/topology/largest_face_size",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() { return vcl::largestFaceSize(m); });
    });

BENCH(
    "stat/topology/triangulated_face_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() { return vcl::triangulatedFaceCount(m); });
    });

BENCH(
    "stat/topology/largest_vertex_adjacent_vertices_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.meshWithTopology();
        s.run([&]() { return vcl::largestVertexAdjacentVerticesCount(m); });
    });

BENCH(
    "stat/topology/largest_vertex_adjacent_faces_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.meshWithTopology();
        s.run([&]() { return vcl::largestVertexAdjacentFacesCount(m); });
    });

BENCH(
    "stat/topology/vertices_to_duplicate_by_wedge_tex_coords_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        if (!bench::hasWedgeTexCoords(m))
            return s.skip("the mesh has no wedge texture coordinates");

        s.run(
            [&]() { return vcl::verticesToDuplicateByWedgeTexCoordsCount(m); });
    });

BENCH(
    "stat/topology/referenced_vertices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() {
            vcl::uint unreferenced = 0;
            return vcl::referencedVertices<std::vector<bool>>(m, unreferenced);
        });
    });

BENCH(
    "stat/topology/unreferenced_vertex_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() { return vcl::unreferencedVertexCount(m); });
    });

BENCH(
    "stat/topology/non_manifold_vertex_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.meshWithTopology();
        s.run([&]() { return vcl::nonManifoldVertexCount(m); });
    });

BENCH("stat/topology/is_water_tight", []<typename Mesh>(bench::State<Mesh>& s) {
    const Mesh& m = s.mesh();
    s.run([&]() { return vcl::isWaterTight(m); });
});

BENCH("stat/topology/hole_count", []<typename Mesh>(bench::State<Mesh>& s) {
    const Mesh& m = s.meshWithTopology();
    s.run([&]() { return vcl::holeCount(m); });
});

BENCH(
    "stat/topology/connected_components",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.meshWithTopology();
        s.run([&]() { return vcl::connectedComponents(m); });
    });

BENCH(
    "stat/topology/connected_component_count",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.meshWithTopology();
        s.run([&]() { return vcl::connectedComponentCount(m); });
    });

BENCH("stat/topology/face_flooding", []<typename Mesh>(bench::State<Mesh>& s) {
    if constexpr (!vcl::FaceMeshConcept<Mesh>) {
        return s.skip("not a face mesh");
    }
    else {
        Mesh m = s.copyMeshWithTopology();
        if (m.faceCount() == 0) {
            return s.skip("mesh has no faces");
        }

        using Face = vcl::FaceType<Mesh>;
        std::vector<Face> stack;
        stack.reserve(m.faceCount());

        s.run(
            [&]() {
                stack.clear();
                for (auto f : m.faces()) {
                    f.visited() = false;
                }
            },
            [&]() {
                Face start      = m.face(0);
                start.visited() = true;
                stack.push_back(start);

                std::size_t visitedCount = 0;

                while (!stack.empty()) {
                    Face curr = stack.back();
                    stack.pop_back();
                    ++visitedCount;

                    for (vcl::uint i = 0; i < curr.adjFaceCount(); ++i) {
                        Face adj = curr.adjFace(i);
                        if (adj && !adj.visited()) {
                            adj.visited() = true;
                            stack.push_back(adj);
                        }
                    }
                }
                return visitedCount;
            });
    }
});
