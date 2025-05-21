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

#ifndef VCL_ALGORITHMS_MESH_CREATE_ICOSAHEDRON_H
#define VCL_ALGORITHMS_MESH_CREATE_ICOSAHEDRON_H

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Creates and returns an icosahedron mesh.
 *
 * @tparam MeshType: The type of the mesh to create, it must satisfy the
 * FaceMeshConcept.
 * @param[in] normalizeVertices: If true, the vertices are normalized.
 *
 * @return The icosahedron mesh.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createIcosahedron(bool normalizeVertices = false)
{
    using VertexType   = MeshType::VertexType;
    using PositionType = VertexType::PositionType;

    MeshType mesh;

    const double t = (1.0 + std::sqrt(5.0)) / 2.0;

    // Vertices
    mesh.addVertex(PositionType(-1.0, t, 0.0));
    mesh.addVertex(PositionType(1.0, t, 0.0));
    mesh.addVertex(PositionType(-1.0, -t, 0.0));
    mesh.addVertex(PositionType(1.0, -t, 0.0));
    mesh.addVertex(PositionType(0.0, -1.0, t));
    mesh.addVertex(PositionType(0.0, 1.0, t));
    mesh.addVertex(PositionType(0.0, -1.0, -t));
    mesh.addVertex(PositionType(0.0, 1.0, -t));
    mesh.addVertex(PositionType(t, 0.0, -1.0));
    mesh.addVertex(PositionType(t, 0.0, 1.0));
    mesh.addVertex(PositionType(-t, 0.0, -1.0));
    mesh.addVertex(PositionType(-t, 0.0, 1.0));

    if (normalizeVertices) {
        for (VertexType& v : mesh.vertices())
            v.position().normalize();
    }

    // Faces
    mesh.addFace(0, 11, 5);
    mesh.addFace(0, 5, 1);
    mesh.addFace(0, 1, 7);
    mesh.addFace(0, 7, 10);
    mesh.addFace(0, 10, 11);
    mesh.addFace(1, 5, 9);
    mesh.addFace(5, 11, 4);
    mesh.addFace(11, 10, 2);
    mesh.addFace(10, 7, 6);
    mesh.addFace(7, 1, 8);
    mesh.addFace(3, 9, 4);
    mesh.addFace(3, 4, 2);
    mesh.addFace(3, 2, 6);
    mesh.addFace(3, 6, 8);
    mesh.addFace(3, 8, 9);
    mesh.addFace(4, 9, 5);
    mesh.addFace(2, 4, 11);
    mesh.addFace(6, 2, 10);
    mesh.addFace(8, 6, 7);
    mesh.addFace(9, 8, 1);

    return mesh;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_ICOSAHEDRON_H
