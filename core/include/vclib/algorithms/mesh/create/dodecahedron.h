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

#ifndef VCL_ALGORITHMS_MESH_CREATE_DODECAHEDRON_H
#define VCL_ALGORITHMS_MESH_CREATE_DODECAHEDRON_H

#include <vclib/algorithms/core/polygon.h>
#include <vclib/mesh/requirements.h>
#include <vclib/mesh/tmp_meshes.h>
#include <vclib/misc/logger.h>

namespace vcl {

/**
 * @brief Creates and returns a Polygon Mesh containing a Dodecahedron.
 *
 * The returned mesh will contain 12 pentagons.
 *
 * @return A Mesh containing a dodecahedron.
 *
 * @ingroup create
 */
template<PolygonMeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType createDodecahedron(LogType& log = nullLogger)
{
    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;

    MeshType mesh;

    const ScalarType fi = (1 + std::sqrt(5)) / 2;
    const ScalarType s0 = 0;
    const ScalarType s1 = fi;
    const ScalarType s2 = std::pow(fi, 2);
    const ScalarType s3 = 1;

    log.log(0, "Adding vertices to PolyMesh...");

    mesh.addVertices(
        CoordType(-s1, -s1, s1),
        CoordType(s2, s3, s0),
        CoordType(s2, -s3, s0),
        CoordType(-s2, s3, s0),
        CoordType(-s2, -s3, s0),
        CoordType(s0, s2, s3),
        CoordType(s0, s2, -s3),
        CoordType(s3, s0, -s2),
        CoordType(-s3, s0, -s2),
        CoordType(s0, -s2, -s3),
        CoordType(s0, -s2, s3),
        CoordType(s3, s0, s2),
        CoordType(-s3, s0, s2),
        CoordType(s1, s1, -s1),
        CoordType(s1, s1, s1),
        CoordType(-s1, s1, -s1),
        CoordType(-s1, s1, s1),
        CoordType(s1, -s1, -s1),
        CoordType(s1, -s1, s1),
        CoordType(-s1, -s1, -s1));

    log.log(50, "Vertices added to PolyMesh.");
    log.log(50, "Adding faces to PolyMesh...");

    mesh.reserveFaces(12);
    mesh.addFace(14, 11, 18, 2, 1);
    mesh.addFace(2, 17, 7, 13, 1);
    mesh.addFace(15, 8, 19, 4, 3);
    mesh.addFace(4, 0, 12, 16, 3);
    mesh.addFace(16, 5, 6, 15, 3);
    mesh.addFace(13, 6, 5, 14, 1);
    mesh.addFace(18, 10, 9, 17, 2);
    mesh.addFace(19, 9, 10, 0, 4);
    mesh.addFace(17, 9, 19, 8, 7);
    mesh.addFace(13, 7, 8, 15, 6);
    mesh.addFace(16, 12, 11, 14, 5);
    mesh.addFace(18, 11, 12, 0, 10);

    log.log(100, "Faces added to PolyMesh.");

    return mesh;
}

/**
 * @brief Creates and returns a Triangle Mesh containing a triangulated
 * Dodecahedron.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is polygonal, a mesh containing 12 pentagons will
 * be returned.
 *
 * @return A Mesh containing a dodecahedron.
 *
 * @ingroup create
 */
template<TriangleMeshConcept MeshType, LoggerConcept LogType = NullLogger>
MeshType createDodecahedron(LogType& log = nullLogger)
{
    log.startNewTask(0, 75, "Create Polygonal Dodecahedron.");

    detail::TMPSimplePolyMesh pmesh =
        createDodecahedron<detail::TMPSimplePolyMesh>(log);

    log.endTask("Create Polygonal Dodecahedron.");
    log.log(75, "Copying vertices into TriMesh...");

    MeshType mesh;
    using ST = MeshType::VertexType::CoordType::ScalarType;
    mesh.reserveVertices(pmesh.vertexNumber());
    for (const auto& v : pmesh.vertices()) {
        mesh.addVertex(v.coord().cast<ST>());
    }

    log.log(80, "Vertices copied into TriMesh.");
    log.log(80, "Triangularize and copy Faces into TriMesh...");

    for (const auto& f : pmesh.faces()) {
        std::vector<uint> ind = earCut(f);
        for (uint i = 0; i < ind.size(); i += 3) {
            mesh.addFace(
                pmesh.index(f.vertex(ind[i])),
                pmesh.index(f.vertex(ind[i + 1])),
                pmesh.index(f.vertex(ind[i + 2])));
        }
    }

    log.log(100, "Faces triangularized and copied into TriMesh.");

    return mesh;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_DODECAHEDRON_H
