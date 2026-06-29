// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/embree/scene.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

int main()
{
    using namespace vcl;

    PolyMesh m = loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/cube_poly.ply");

    std::vector<Point3d> origins = {
        {0, -0.1, 0  },
        {0, 0.1,  0  },
        {0, 2,    0.1}
    };
    std::vector<Point3d> directions = {
        {1,  0,  0},
        {-1, 0,  0},
        {0,  -1, 0}
    };

    embree::Scene scene(m);

    auto hits = scene.firstFaceIntersectedByRays(origins, directions);

    for (uint i = 0; const auto& [faceId, barCoords, tId, dist] : hits) {
        std::cout << "Ray " << i++ << ":\n";

        std::string fString =
            faceId == UINT_NULL ? "No face hit" : std::to_string(faceId);

        std::cout << "First face intersected by ray: " << fString << "\n";

        if (faceId != UINT_NULL) { // should never be UINT_NULL here
            std::cout << "Distance from ray origin to hit point: " << dist
                      << "\n";

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
    }

    return 0;
}
