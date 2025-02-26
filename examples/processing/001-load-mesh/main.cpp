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

    auto suppFormatsTEM = ActionManager::loadMeshFormats<vcl::TriEdgeMesh>();

    std::cerr << "Supported load formats for TriEdgeMesh:" << std::endl;
    for (const auto& fmt : suppFormatsTEM) {
        std::cerr << fmt.description() << std::endl;
    }

    auto suppFormats = ActionManager::loadMeshFormats();

    std::cerr << "All the supported load formats:" << std::endl;
    for (const auto& fmt : suppFormats) {
        std::cerr << fmt.description() << std::endl;
    }

    vcl::TriEdgeMesh td =
        ActionManager::loadMeshAction<vcl::TriEdgeMesh>("obj")->load(
            VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.obj");

    vcl::PolyEdgeMesh ghp = ActionManager::loadMeshAction<vcl::PolyEdgeMesh>(
        "obj")->load(VCLIB_EXAMPLE_MESHES_PATH "/greek_helmet.obj");

    vcl::TriEdgeMesh ght = ActionManager::loadMeshAction<vcl::TriEdgeMesh>(
        "obj")->load(VCLIB_EXAMPLE_MESHES_PATH "/greek_helmet.obj");

    ActionManager::saveMeshAction<vcl::TriEdgeMesh>("obj")->save(
        VCLIB_RESULTS_PATH "/td1.obj", td);

    ActionManager::saveMeshAction<vcl::PolyEdgeMesh>("obj")->save(
        VCLIB_RESULTS_PATH "/greek_poly.obj", ghp);

    ActionManager::saveMeshAction<vcl::TriEdgeMesh>("obj")->save(
        VCLIB_RESULTS_PATH "/greek_tri.obj", ght);

    return 0;
}
