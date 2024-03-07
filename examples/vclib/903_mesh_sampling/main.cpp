/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include <vclib/load_save.h>

#include <default_viewer.h>

#include "montecarlo_sampling.h"

int main(int argc, char** argv)
{
    vcl::TriMesh m =
        vcl::loadPly<vcl::TriMesh>(VCLIB_ASSETS_PATH "/bunny_textured.ply");

    vcl::TriMesh samples;

    montecarloSampling(m, samples);

    vcl::updatePerFaceNormals(m);
    vcl::updatePerVertexNormals(m);
    m.enablePerVertexColor();
    vcl::setPerVertexColor(m, vcl::Color::LightBlue);
    m.enablePerFaceColor();
    vcl::setPerFaceColor(m, vcl::Color::LightBlue);

    for (const vcl::TriMesh::Vertex& v : samples.vertices()) {
        m.face(v.customComponent<uint>("birthFace")).color() =
            vcl::Color::LightRed;
    }

    return showMeshesOnDefaultViewer(argc, argv, m, samples);
}
