// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/mesh/views.h>

BENCH(
    "mesh/views_stream/raw_indices",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() {
            vcl::PositionType<Mesh> sum;
            sum.setZero();
            const std::size_t n = m.vertexContainerSize();
            for (std::size_t i = 0; i < n; ++i) {
                sum += m.vertex(i).position();
            }
            return sum;
        });
    });

BENCH(
    "mesh/views_stream/element_proxies",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() {
            vcl::PositionType<Mesh> sum;
            sum.setZero();
            for (auto v : m.vertices()) {
                sum += v.position();
            }
            return sum;
        });
    });

BENCH(
    "mesh/views_stream/component_view",
    []<typename Mesh>(bench::State<Mesh>& s) {
        const Mesh& m = s.mesh();
        s.run([&]() {
            vcl::PositionType<Mesh> sum;
            sum.setZero();
            for (const auto& p : m.vertices().positions()) {
                sum += p;
            }
            return sum;
        });
    });

BENCH("mesh/views_stream/pipe_views", []<typename Mesh>(bench::State<Mesh>& s) {
    const Mesh& m = s.mesh();
    s.run([&]() {
        vcl::PositionType<Mesh> sum;
        sum.setZero();
        for (const auto& p : m.vertices() | vcl::views::positions) {
            sum += p;
        }
        return sum;
    });
});
