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

#include <iostream>

#include <vclib/meshes.h>

int main()
{
    vcl::EdgeMesh m;

    m.addVertices(3);

    m.addEdges(2);

    assert(m.number<vcl::ElemId::EDGE>() == 2);

    vcl::EdgeMesh::Edge::VertexIterator it = m.edge(0).vertexBegin();

    m.edge(0).setVertex(it, 0U);
    m.edge(0).setVertex(1U, 1U);
    m.edge(1).setVertex(0U, 1U);
    m.edge(1).setVertex(1U, 2U);

    (*it)->coord() = vcl::Point3d(1, 0, 0);

    assert(m.vertex(0).coord() == vcl::Point3d(1, 0, 0));

    assert(m.edge(0).vertex(0) == &m.vertex(0));
    assert(m.edge(0).vertex(1) == &m.vertex(1));
    assert(m.edge(1).vertex(0) == &m.vertex(1));
    assert(m.edge(1).vertex(1) == &m.vertex(2));

    return 0;
}
