// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

BENCH("mesh/append", []<typename Mesh>(bench::State<Mesh>& s) {
    const Mesh& m2 = s.mesh();
    Mesh        m1 = s.copyMesh();

    s.run(
        [&]() { m1 = m2; },
        [&]() {
            m1.append(m2);
            return m1.vertexCount();
        });
});
