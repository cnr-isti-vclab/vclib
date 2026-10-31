// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef TEST_LOAD_H
#define TEST_LOAD_H

#include "types.h"

#include <benchmark_runner.h>

#include <vclib/io.h>

#include <string>

namespace bench {

/**
 * @brief Benchmarks the loading of a mesh from file, including the I/O time.
 *
 * The meshes loaded during the benchmark are discarded.
 */
inline void benchmarkLoad(
    BenchmarkRunner&   runner,
    const std::string& path,
    int                iterations = -1)
{
    runner.run(
        "mesh_load",
        [&]() {
            Mesh m = vcl::loadMesh<Mesh>(path);
            doNotOptimize(m);
        },
        iterations);
}

} // namespace bench

#endif // TEST_LOAD_H
