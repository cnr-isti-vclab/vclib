// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "best_heightfield_orientation.h"

int main()
{
    using namespace vcl;

    const std::string resultsPath = VCLIB_EXTERNAL_RESULTS_PATH;

    auto fnSave = [&](const auto& mesh) {
        saveMesh(mesh, resultsPath + "/011_" + mesh.name() + ".ply");
    };

    auto brainMeshes = bestHeightfieldOrientation("brain.ply", "Brain", 10);

    // for each mesh in the std::tuple
    std::apply(
        [&](const auto&... mesh) {
            (fnSave(mesh), ...);
        },
        brainMeshes);

    auto bunnyMeshes = bestHeightfieldOrientation("bunny.obj", "Bunny", 100);

    // for each mesh in the std::tuple
    std::apply(
        [&](const auto&... mesh) {
            (fnSave(mesh), ...);
        },
        bunnyMeshes);

    return 0;
}
