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

#include <vclib/algorithms.h>
#include <vclib/load_save.h>
#include <vclib/meshes.h>

int main()
{
    vcl::TriMesh m =
        vcl::loadPly<vcl::TriMesh>(VCLIB_TEST_MODELS_PATH "/bone.ply");

    m.addPerVertexCustomComponent<int>("flag");

    assert(m.hasPerVertexCustomComponent("flag"));

    for (vcl::TriMesh::Vertex& v : m.vertices()) {
        v.customComponent<int>("flag") = -4;
    }

    assert(m.vertex(10).customComponent<int>("flag") == -4);

    vcl::CustomComponentVectorHandle<int> v =
        m.perVertexCustomComponentVectorHandle<int>("flag");

    for (auto& m : v) {
        m = 8;
    }

    v.front() = 4;

    assert(m.vertex(0).customComponent<int>("flag") == 4);
    assert(m.vertex(9).customComponent<int>("flag") == 8);

    m.deletePerVertexCustomComponent("flag");

    assert(!m.hasPerVertexCustomComponent("flag"));

    m.addPerVertexCustomComponent<vcl::Point3f>("oldCoords");

    assert(m.hasPerVertexCustomComponent("oldCoords"));
    assert(m.isPerVertexCustomComponentOfType<vcl::Point3f>("oldCoords"));
    assert(!m.isPerVertexCustomComponentOfType<vcl::Point3d>("oldCoords"));

    for (vcl::TriMesh::Vertex& v : m.vertices()) {
        v.customComponent<vcl::Point3f>("oldCoords") = v.coord().cast<float>();
    }

    vcl::taubinSmoothing(m, 500, 0.7, -0.73);

    vcl::ConstCustomComponentVectorHandle<vcl::Point3f> oldCoords =
        m.perVertexCustomComponentVectorHandle<const vcl::Point3f>("oldCoords");

    double avgDist = 0;
    using CT       = vcl::TriMesh::Vertex::CoordType;
    using ST       = CT::ScalarType;
    for (vcl::TriMesh::Vertex& v : m.vertices()) {
        avgDist += v.coord().dist(oldCoords[m.index(v)].cast<ST>());
    }
    avgDist /= m.vertexNumber();

    std::cerr << "Avg distance after taubin smoothing: " << avgDist << "\n";

    m.addCustomComponent<CT>("barycenter", vcl::barycenter(m));

    std::cerr << "Mesh barycenter: " << m.customComponent<CT>("barycenter")
              << "\n";

    return 0;
}
