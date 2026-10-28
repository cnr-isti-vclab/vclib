// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_COMPACT_H
#define TEST_COMPACT_H

#include "types.h"

#include <benchmark_runner.h>

namespace bench {

inline void benchmarkDeleteAndCompact(
    BenchmarkRunner& runner,
    const Mesh&      baseM,
    int              iterations = -1)
{
    runner.run(
        "delete_faces_25_percent",
        [&]() {
            Mesh m = baseM;
            for (std::size_t i = 0; i < m.faceCount(); i += 4) {
                m.deleteFace(i);
            }
            doNotOptimize(m);
        },
        iterations);

    runner.run(
        "compact_after_deletion",
        [&]() {
            Mesh m = baseM;
            for (std::size_t i = 0; i < m.faceCount(); i += 4) {
                m.deleteFace(i);
            }
            m.compact();
            doNotOptimize(m);
        },
        iterations);
}

} // namespace bench

#endif // TEST_COMPACT_H
