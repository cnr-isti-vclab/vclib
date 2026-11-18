// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh.h>

BENCH("update/border/update_border", []<typename Mesh>(bench::State<Mesh>& s) {
    if constexpr (!vcl::FaceMeshConcept<Mesh>) {
        return s.skip("the mesh has no faces");
    }
    else {
        if (s.mesh().vertexCount() == 0) {
            return s.skip("the mesh has no vertices");
        }
        if (s.mesh().faceCount() == 0) {
            return s.skip("the mesh has no faces");
        }

        const Mesh& base = s.mesh();
        Mesh        m    = s.copyMesh();

        s.run([&]() { m = base; }, [&]() { vcl::updateBorder(m); });
    }
});
