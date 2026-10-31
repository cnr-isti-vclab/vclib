// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

namespace {

void deleteQuarterOfFaces(bench::Mesh& m)
{
    for (vcl::uint i = 0; i < m.faceContainerSize(); i += 4) {
        m.deleteFace(i);
    }
}

} // namespace

BENCH("mesh/delete_faces_25_percent", [](bench::State& s) {
    const bench::Mesh& base = s.mesh();
    bench::Mesh        m    = s.copyMesh();

    s.run([&]() { m = base; }, [&]() { deleteQuarterOfFaces(m); });
});

BENCH("mesh/compact_after_deletion", [](bench::State& s) {
    const bench::Mesh& base = s.mesh();
    bench::Mesh        m    = s.copyMesh();

    s.run(
        [&]() {
            m = base;
            deleteQuarterOfFaces(m);
        },
        [&]() { m.compact(); });
});
