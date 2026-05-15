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
