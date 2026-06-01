/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

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
