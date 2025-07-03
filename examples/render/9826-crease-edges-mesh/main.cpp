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

#include "crease_edges_mesh.h"

#include <default_viewer.h>

int main(int argc, char** argv)
{
    vcl::TriMesh c = vcl::createCylinder<vcl::TriMesh>(1, 1, 36);
    vcl::updatePerVertexAndFaceNormals(c);
    c.enablePerVertexColor();
    vcl::setPerVertexColor(c, vcl::Color::Black);
    c.name() = "cylinder";

    vcl::EdgeMesh e = creaseEdgesMesh<vcl::EdgeMesh>(c);

    vcl::setPerVertexColor(c, vcl::Color::Gray);

    e.name() = "crease edges";
    e.enablePerEdgeColor();

    for (auto& edge : e.edges()) {
        if (edge.vertex(0)->position().y() > 0)
            edge.color() = vcl::Color::Red;
        else
            edge.color() = vcl::Color::Blue;
    }

    return showMeshesOnDefaultViewer(argc, argv, std::move(c), std::move(e));
}
