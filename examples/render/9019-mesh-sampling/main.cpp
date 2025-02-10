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

#include <vclib/load_save.h>

#include <default_viewer.h>

#include "montecarlo_sampling.h"

int main(int argc, char** argv)
{
    vcl::TriMesh m = vcl::loadPly<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                                "/bunny_textured.ply");

    vcl::TriMesh samples;

    montecarloSampling(m, samples);

    vcl::updatePerFaceNormals(m);
    vcl::updatePerVertexNormals(m);
    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::LightBlue);
    m.enablePerFaceColor();
    vcl::setPerFaceColor(m, vcl::Color::LightBlue);

    for (const vcl::TriMesh::Vertex& v : samples.vertices()) {
        m.face(v.customComponent<vcl::uint>("birthFace")).color() =
            vcl::Color::LightRed;
    }

    return showMeshesOnDefaultViewer(
        argc, argv, std::move(m), std::move(samples));
}
