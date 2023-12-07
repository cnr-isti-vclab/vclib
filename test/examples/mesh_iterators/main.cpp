/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#include <iostream>

#include <vclib/load_save.h>
#include <vclib/meshes.h>

#include <vclib/algorithms/update.h>
#include <vclib/views/mesh.h>

int main()
{
    vcl::TriMesh m =
        vcl::load<vcl::TriMesh>(VCLIB_TEST_MODELS_PATH "/cube_tri.ply");

    m.enablePerFaceAdjacentFaces();
    vcl::updatePerFaceAdjacentFaces(m);

    // print

    const vcl::TriMesh& cm = m;

    std::cerr << "\n\nCoords per face:\n\n";

    for (const auto& f : cm | vcl::views::faces) {
        for (const auto& c : f | vcl::views::vertices | vcl::views::coords) {
            std::cerr << c << "\t";
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

    std::cerr << "\n\nTransform Coords per face:\n\n";

    for (auto& f : cm | vcl::views::faces) {
        for (auto& c : f | vcl::views::vertices | vcl::views::coords) {
            // c*=2;
            std::cerr << c << "\t";
        }
        std::cerr << "\n";
    }

    std::cerr << "\n\nPrint Coords per vertex:\n\n";

    for (const auto& c : cm.vertices() | vcl::views::coords) {
        std::cerr << c << "\n";
    }

    // transform
    std::cerr << "\n\nTransform coords per vertex:\n\n";

    for (vcl::Point3d& c : m.vertices() | vcl::views::coords) {
        c *= 2;
        std::cerr << c << "\n";
    }

    std::cerr << "\n\nTransform Selection:\n";

    uint i = 0;
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

    i = 0;
    for (auto& qual : m.vertices() | vcl::views::quality) {
        qual = ++i;
    }

    std::cerr << "\n\nPrint Vertex Quality:\n";

    for (const auto& qual : cm.vertices() | vcl::views::quality) {
        std::cerr << qual << "\n";
    }
    return 0;
}
