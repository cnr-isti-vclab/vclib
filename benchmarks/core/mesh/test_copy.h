// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_COPY_H
#define TEST_COPY_H

#include "types.h"

#include <benchmark_runner.h>

namespace bench {

inline void benchmarkCopy(
    BenchmarkRunner& runner,
    const Mesh&      m,
    int              iterations = -1)
{
    runner.run(
        "mesh_copy",
        [&]() {
            Mesh copy = m;
            doNotOptimize(copy);
        },
        iterations);
}

} // namespace bench

#endif // TEST_COPY_H
