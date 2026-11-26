// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/mesh.h>
#include <vclib/space/core.h>

BENCH(
    "polygon/geometry/face_normals",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("not a face mesh");
        }
        else {
            const Mesh& m = s.mesh();
            if (m.faceCount() == 0) {
                return s.skip("mesh has no faces");
            }

            using Point = vcl::PositionType<Mesh>;

            s.run([&]() {
                Point sum;
                sum.setZero();
                for (const auto& f : m.faces()) {
                    sum += vcl::Polygon<Point>::normal(
                        f.vertices() | vcl::views::positions);
                }
                return sum;
            });
        }
    });

BENCH(
    "polygon/triangulation/ear_cut",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if constexpr (!vcl::FaceMeshConcept<Mesh>) {
            return s.skip("not a face mesh");
        }
        else {
            const Mesh& m = s.mesh();
            if (m.faceCount() == 0) {
                return s.skip("mesh has no faces");
            }

            s.run([&]() {
                std::size_t totalIndices = 0;
                for (const auto& f : m.faces()) {
                    auto tris = vcl::earCut(f);
                    totalIndices += tris.size();
                }
                return totalIndices;
            });
        }
    });
