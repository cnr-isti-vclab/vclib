// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "../benchmark.h"

#include <vclib/io.h>

// The loading includes the time spent reading the file.
BENCH("io/load_mesh", [](bench::State& s) {
    if (s.meshPath().empty())
        return s.skip("the dataset is not a file");

    const std::string& path = s.meshPath();
    s.run([&]() { return vcl::loadMesh<bench::Mesh>(path); });
});
