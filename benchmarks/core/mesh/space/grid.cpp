// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/base.h>
#include <vclib/space/complex.h>

#include <random>

BENCH("space/grid/build", []<typename Mesh>(bench::State<Mesh>& s) {
    const Mesh& m = s.mesh();
    if (m.vertexCount() == 0) {
        return s.skip("mesh has no vertices");
    }

    using Vertex = typename Mesh::VertexType;
    using Scalar = typename Vertex::PositionType::ScalarType;
    using Grid   = vcl::StaticGrid3<const Vertex*, Scalar>;

    s.run([&]() {
        Grid grid(m.vertices() | vcl::views::constAddrOf);
        return grid.totalCellCount();
    });
});

BENCH("space/grid/radius_search", []<typename Mesh>(bench::State<Mesh>& s) {
    const Mesh& m = s.mesh();
    if (m.vertexCount() == 0) {
        return s.skip("mesh has no vertices");
    }

    using Vertex = typename Mesh::VertexType;
    using Point  = typename Vertex::PositionType;
    using Scalar = typename Point::ScalarType;
    using Grid   = vcl::StaticGrid3<const Vertex*, Scalar>;
    using Sphere = vcl::Sphere<Scalar>;

    Grid grid(m.vertices() | vcl::views::constAddrOf);

    const auto   bbox   = vcl::boundingBox(m);
    const Scalar diag   = bbox.diagonal();
    const Scalar radius = diag * static_cast<Scalar>(0.02);

    const std::size_t   nQueries = 50000;
    std::vector<Sphere> queries;
    queries.reserve(nQueries);

    std::mt19937                           gen(s.seed());
    std::uniform_real_distribution<Scalar> disX(bbox.min().x(), bbox.max().x());
    std::uniform_real_distribution<Scalar> disY(bbox.min().y(), bbox.max().y());
    std::uniform_real_distribution<Scalar> disZ(bbox.min().z(), bbox.max().z());

    for (std::size_t i = 0; i < nQueries; ++i) {
        Point center(disX(gen), disY(gen), disZ(gen));
        queries.emplace_back(center, radius);
    }

    s.run([&]() {
        std::size_t totalFound = 0;
        for (const auto& sphere : queries) {
            totalFound += grid.countInSphere(sphere);
        }
        return totalFound;
    });
});
