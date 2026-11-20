// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh.h>

namespace {

template<typename Mesh>
Mesh meshWithCurvatureAndTopology(bench::State<Mesh>& s)
{
    Mesh m = s.meshWithTopology();
    bench::enableCurvatureComponents(m);
    return m;
}

template<typename Mesh>
Mesh meshWithCurvature(const bench::State<Mesh>& s)
{
    Mesh m = s.copyMesh();
    bench::enableCurvatureComponents(m);
    return m;
}

} // namespace

BENCH(
    "update/curvature/update_principal_curvature_taubin95",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if (s.mesh().vertexCount() == 0)
            return s.skip("the mesh has no vertices");
        if (s.mesh().faceCount() == 0)
            return s.skip("the mesh has no faces");

        Mesh m = meshWithCurvatureAndTopology(s);
        s.run([&]() { vcl::updatePrincipalCurvatureTaubin95(m); });
    });

BENCH(
    "update/curvature/update_principal_curvature_pca_montecarlo",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if (s.mesh().vertexCount() == 0)
            return s.skip("the mesh has no vertices");
        if (s.mesh().faceCount() == 0)
            return s.skip("the mesh has no faces");

        Mesh m              = meshWithCurvature(s);
        using Scalar        = vcl::ScalarType<Mesh>;
        const Scalar radius = vcl::boundingBox(m).diagonal() * Scalar(0.05);
        if (radius <= 0)
            return s.skip("bounding box diagonal is zero");

        s.run([&]() { vcl::updatePrincipalCurvaturePCA(m, radius, true); });
    });

BENCH_SLOW(
    "update/curvature/update_principal_curvature_pca_analytic",
    []<typename Mesh>(bench::State<Mesh>& s) {
        if (s.mesh().vertexCount() == 0)
            return s.skip("the mesh has no vertices");
        if (s.mesh().faceCount() == 0)
            return s.skip("the mesh has no faces");

        Mesh m              = meshWithCurvature(s);
        using Scalar        = vcl::ScalarType<Mesh>;
        const Scalar radius = vcl::boundingBox(m).diagonal() * Scalar(0.05);
        if (radius <= 0)
            return s.skip("bounding box diagonal is zero");

        s.run([&]() { vcl::updatePrincipalCurvaturePCA(m, radius, false); });
    });
