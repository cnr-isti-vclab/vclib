// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "best_heightfield_orientation.h"

#include <default_viewer.h>

int main(int argc, char** argv)
{
    auto brainMeshes = bestHeightfieldOrientation("brain.ply", "Brain", 10);

    auto bunnyMeshes = bestHeightfieldOrientation("bunny.obj", "Bunny", 100);

    // merge the meshes into a single tuple
    auto allMeshes = std::tuple_cat(brainMeshes, bunnyMeshes);

    auto closure = [&](auto&&... meshes) {
        return showMeshesOnDefaultViewer(argc, argv, meshes...);
    };

    std::apply(closure, allMeshes);
}
