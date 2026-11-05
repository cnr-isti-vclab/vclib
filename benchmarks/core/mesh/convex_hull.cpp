// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "benchmark.h"

#include <vclib/algorithms/mesh.h>

#include <vector>

BENCH_SLOW(
    "convex_hull/convex_hull",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("the mesh has no faces");
        }
        else {
            const Mesh& m = s.mesh();
            if (m.vertexCount() < 4) {
                return s.skip("not enough vertices (< 4)");
            }

            const auto pts = m.vertices() | vcl::views::positions;

            // Pre-flight check outside of measurement: verify points can form a
            // valid tetrahedron (not all coincident, collinear, or coplanar).
            try {
                using PointType = std::ranges::range_value_t<decltype(pts)>;
                std::vector<PointType> testPts;
                testPts.reserve(m.vertexCount());
                for (const auto& p : pts) {
                    testPts.push_back(p);
                }
                vcl::detail::firstTetOptimization(testPts);
            }
            catch (const std::exception& e) {
                return s.skip(e.what());
            }

            s.run([&]() {
                return vcl::convexHull<Mesh>(pts, s.seed());
            });
        }
    });

