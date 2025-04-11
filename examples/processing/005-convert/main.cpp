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

#include <vclib/io.h>

int main()
{
    using namespace vcl::proc;

    vcl::PolyEdgeMesh bunny =
        ActionManager::loadMeshActions("obj")->load<vcl::PolyEdgeMesh>(
            VCLIB_EXAMPLE_MESHES_PATH "/greek_helmet.obj");

    std::shared_ptr<ConvertActions> action =
        ActionManager::convertActions("Convert to TriEdgeMesh");

    auto [id, anyMesh] = action->convert(bunny);

    ActionManager::saveMeshActions("ply")->save(
        VCLIB_RESULTS_PATH "/converted_greek_helmet.ply",
        std::any_cast<vcl::TriEdgeMesh>(anyMesh));

    return 0;
}
