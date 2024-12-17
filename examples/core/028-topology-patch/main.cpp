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

#include <iostream>

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

int main()
{
    vcl::TriMesh m =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj");

    m.enablePerFaceAdjacentFaces();
    m.enablePerFaceColor();

    vcl::updatePerVertexAndFaceNormals(m);
    vcl::updatePerFaceAdjacentFaces(m);
    vcl::updateBoundingBox(m);

    vcl::setPerFaceColor(m, vcl::Color::Gray);

    auto min = m.boundingBox().min();

    auto func = [&](const vcl::TriMesh::Face& f) {
        return vcl::facePointVisibility(f, min);
    };

    bool         found    = false;
    unsigned int seedFace = vcl::UINT_NULL;
    auto         it       = m.faceBegin();
    while (!found) {
        if (func(*it)) {
            found    = true;
            seedFace = it->index();
        }
        ++it;
    }

    m.face(seedFace).color() = vcl::Color::Red;

    auto patchFaces = vcl::floodFacePatch(m.face(seedFace), func);

    for (auto fPtr : patchFaces) {
        if (fPtr->index() != seedFace) {
            auto& f   = m.face(fPtr->index());
            f.color() = vcl::Color::Green;
        }
    }

    vcl::savePly(m, VCLIB_RESULTS_PATH "/bimba_patch.ply");

    return 0;
}
