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

    auto suppFormats = ActionManager::saveMeshFormats();

    std::cerr << "All the supported save formats:" << std::endl;
    for (const auto& fmt : suppFormats) {
        std::cerr << fmt.description() << std::endl;
    }

    vcl::LoadSettings s;
    s.loadTextureImages = true;

    vcl::TriEdgeMesh mesh = vcl::load<vcl::TriEdgeMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply", s);

    // saving obj
    ActionManager::saveMeshActions("obj")->save(
        VCLIB_RESULTS_PATH "/td.obj", mesh);

    // saving off
    ActionManager::saveMeshActions("off")->save(
        VCLIB_RESULTS_PATH "/td.off", mesh);

    // saving ply
    ActionManager::saveMeshActions("ply")->save(
        VCLIB_RESULTS_PATH "/td.ply", mesh);

    // saving stl
    ActionManager::saveMeshActions("stl")->save(
        VCLIB_RESULTS_PATH "/td.stl", mesh);

    return 0;
}
