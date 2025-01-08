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
    vcl::LoadSettings s;
    s.loadTextureImages = true;

    vcl::proc::TriMesh mesh = vcl::load<vcl::proc::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/TextureDouble.ply", s);

    vcl::proc::ActionManager manager;

    manager.add(vcl::proc::vclibActions());

    // saving obj
    manager.saveMeshAction("obj")->save("td.obj", mesh);

    // saving off
    manager.saveMeshAction("off")->save("td.off", mesh);

    // saving ply
    manager.saveMeshAction("ply")->save("td.ply", mesh);

    // saving stl
    manager.saveMeshAction("stl")->save("td.stl", mesh);

    return 0;
}
