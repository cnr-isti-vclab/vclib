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

#include <vclib/algorithms/mesh/update.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/views/mesh.h>

#include <iostream>

int main()
{
    vcl::TriMesh m =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_tri.ply");

    m.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(m);

    // print

    const vcl::TriMesh& cm = m;

    std::cerr << "\n\nVert Positions per face:\n\n";

    for (const auto& f : cm | vcl::views::faces) {
        for (const auto& p : f | vcl::views::vertices | vcl::views::positions) {
            std::cerr << p << "\t";
        }
        std::cerr << "\n";
    }

    std::cerr << "\n\nAdjFaces per face:\n\n";

    for (const auto& f : cm | vcl::views::faces) {
        for (const auto& af : f | vcl::views::adjFaces | vcl::views::notNull) {
            std::cerr << m.index(af) << "\t";
        }
        std::cerr << "\n";
    }

    std::cerr << "\n\nTransform Positions per face:\n\n";

    for (auto& f : cm | vcl::views::faces) {
        for (auto& p : f | vcl::views::vertices | vcl::views::positions) {
            // c*=2;
            std::cerr << p << "\t";
        }
        std::cerr << "\n";
    }

    std::cerr << "\n\nPrint Positions per vertex:\n\n";

    for (const auto& p : cm.vertices() | vcl::views::positions) {
        std::cerr << p << "\n";
    }

    // transform
    std::cerr << "\n\nTransform Positions per vertex:\n\n";

    for (vcl::Point3d& p : m.vertices() | vcl::views::positions) {
        p *= 2;
        std::cerr << p << "\n";
    }

    std::cerr << "\n\nTransform Selection:\n";

    unsigned int i = 0;
    for (auto& v : m.vertices()) {
        v.selected() = i % 2 ? true : false;
        std::cerr << v.selected() << "\n";
        ++i;
    }

    std::cerr << "\n\nPrint Selection:\n";
    for (bool sel : m.vertices() | vcl::views::selection) {
        std::cerr << sel << "\n";
    }

    std::cerr << "\n\nPrint Selected:\n";
    for (const auto& v : m.vertices() | vcl::views::selected) {
        std::cerr << m.index(v) << "\n";
    }

    std::cerr << "\n\nPrint Selected for each face:\n";
    for (const auto& f : cm | vcl::views::faces) {
        std::cerr << "Face: " << m.index(f) << "\n";
        for (const auto* v : f.vertices() | vcl::views::selected) {
            std::cerr << m.index(v) << "\n";
        }
    }

    // quality
    m.enablePerVertexQuality();

    vcl::parallelFor(m.vertices() | vcl::views::quality, [&](auto& q) {
        q = 10;
    });

    std::cerr << "\n\nPrint Vertex Quality:\n";

    for (const auto& qual : cm.vertices() | vcl::views::quality) {
        std::cerr << qual << "\n";
    }
    return 0;
}
