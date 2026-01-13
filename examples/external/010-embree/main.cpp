/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#include <vclib/embree/scene.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

int main()
{
    using namespace vcl;

    PolyMesh m = loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    Segment<Point3d> s({0, -0.1, 0}, {2, -0.1, 0});

    std::vector<Point3d> origins = { {0, -0.1, 0}, {0, 0.1, 0}, {0, 2, 0.1} };
    std::vector<Point3d> directions = { {1, 0, 0}, {-1, 0, 0}, {0, -1, 0} };

    embree::Scene scene(m);

    auto hits = scene.firstFaceIntersectedByRays(origins, directions);

    for (uint i = 0; const auto& [faceId, barCoords, tId] : hits) {
        std::cout << "Ray " << i++ << ":\n";

        std::cout << "First face intersected by segment: " << faceId << "\n";

        std::cout << "Barycentric coordinates of the intersection point: "
                  << barCoords << "\n";

        std::cout << "Triangle ID within the face: " << tId << "\n";

        std::vector<uint> triangulation = earCut(m.face(faceId));

        std::cout << "Vertices of the hit triangle:\n";

        uint        base = tId * 3;
        const auto& v0   = *m.face(faceId).vertex(triangulation[base + 0]);
        const auto& v1   = *m.face(faceId).vertex(triangulation[base + 1]);
        const auto& v2   = *m.face(faceId).vertex(triangulation[base + 2]);

        std::cout << " - Vertex " << v0.index() << ": " << v0.position()
                  << "\n";
        std::cout << " - Vertex " << v1.index() << ": " << v1.position()
                  << "\n";
        std::cout << " - Vertex " << v2.index() << ": " << v2.position()
                  << "\n";

        Point3d hitPos = v0.position() * barCoords.x() +
                         v1.position() * barCoords.y() +
                         v2.position() * barCoords.z();

        std::cout << "Hit point position: " << hitPos << "\n\n";
    }

    return 0;
}
