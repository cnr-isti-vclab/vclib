// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/algorithms/mesh/stat/geometry.h>

BENCH("stat/geometry/surface_area", [](bench::State& s) {
    const bench::Mesh& m = s.mesh();
    s.run([&]() { return vcl::surfaceArea(m); });
});
