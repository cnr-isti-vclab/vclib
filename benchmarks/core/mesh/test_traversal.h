// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_TRAVERSAL_H
#define TEST_TRAVERSAL_H

#include "types.h"

#include <benchmark_runner.h>

#include <vclib/algorithms.h>

namespace bench {

inline void benchmarkTraversal(
    BenchmarkRunner& runner,
    const Mesh&      m,
    int              iterations = -1)
{
    runner.run(
        "surface_area",
        [&]() {
            double a = vcl::surfaceArea(m);
            doNotOptimize(a);
        },
        iterations);

    runner.run(
        "barycenter",
        [&]() {
            auto b = vcl::barycenter(m);
            doNotOptimize(b);
        },
        iterations);
}

} // namespace bench

#endif // TEST_TRAVERSAL_H
