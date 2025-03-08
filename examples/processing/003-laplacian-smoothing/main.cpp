/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include <vclib/processing.h>

#include <vclib/load_save.h>

int main()
{
    using namespace vcl::proc;

    vcl::TriEdgeMesh bunny =
        ActionManager::loadMeshActions("obj")->load<vcl::TriEdgeMesh>(
            VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    std::vector<vcl::TriEdgeMesh*> in_out;
    in_out.push_back(&bunny);

    auto action = ActionManager::filterActions("Laplacian Smoothing");

    action->execute(in_out);

    ActionManager::saveMeshActions("ply")->save(
        VCLIB_RESULTS_PATH "/smoothed_bunny.ply", bunny);

    return 0;
}
