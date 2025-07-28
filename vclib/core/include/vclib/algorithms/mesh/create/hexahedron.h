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

#ifndef VCL_ALGORITHMS_MESH_CREATE_HEXAHEDRON_H
#define VCL_ALGORITHMS_MESH_CREATE_HEXAHEDRON_H

#include <vclib/mesh.h>

namespace vcl {

namespace detail {

template<FaceMeshConcept MeshType>
void fillHexahedronTriangles(MeshType& m)
{
    m.reserveFaces(12);
    m.addFace(0, 2, 1);
    m.addFace(3, 1, 2);
    m.addFace(0, 4, 2);
    m.addFace(6, 2, 4);
    m.addFace(0, 1, 4);
    m.addFace(5, 4, 1);
    m.addFace(7, 6, 5);
    m.addFace(4, 5, 6);
    m.addFace(7, 3, 6);
    m.addFace(2, 6, 3);
    m.addFace(7, 5, 3);
    m.addFace(1, 3, 5);
}

template<FaceMeshConcept MeshType>
void fillHexahedronQuads(MeshType& m)
{
    m.reserveFaces(6);
    m.addFace(2, 3, 1, 0);
    m.addFace(4, 6, 2, 0);
    m.addFace(1, 5, 4, 0);
    m.addFace(6, 4, 5, 7);
    m.addFace(3, 2, 6, 7);
    m.addFace(5, 1, 3, 7);
}

} // namespace detail

/**
 * @brief Creates and returns a hexahedron having as extremes the points ``(-1,
 * -1, -1)`` and ``(1, 1, 1)``.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is composed of quads or polygons, a mesh containing
 * 6 quads will be returned.
 *
 * @return A Mesh containing a ``[(-1, -1, -1), (1, 1, 1)]``Hexahedron .
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createHexahedron()
{
    using PositionType = MeshType::Vertex::PositionType;

    return createHexahedron<MeshType>(
        PositionType(-1, -1, -1), PositionType(1, 1, 1));
}

/**
 * @brief Creates and returns a Hexahedron having as extremes points min and max
 * given as arguments.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is composed of quads or polygons, a mesh containing
 * 6 quads will be returned.
 *
 * @return A Mesh containing a ``[min, max]``Hexahedron.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType, Point3Concept PositionType>
MeshType createHexahedron(const PositionType& min, const PositionType& max)
{
    MeshType m;

    // fill vertices...
    m.addVertices(
        PositionType(min(0), min(1), min(2)),
        PositionType(max(0), min(1), min(2)),
        PositionType(min(0), max(1), min(2)),
        PositionType(max(0), max(1), min(2)),
        PositionType(min(0), min(1), max(2)),
        PositionType(max(0), min(1), max(2)),
        PositionType(min(0), max(1), max(2)),
        PositionType(max(0), max(1), max(2)));

    // fill faces
    if constexpr (HasTriangles<MeshType>) {
        detail::fillHexahedronTriangles(m);
    }
    else {
        detail::fillHexahedronQuads(m);
    }
    return m;
}

/**
 * @brief Creates and returns a Cube having ``min`` as minimum extreme and the
 * given edge length.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is composed of quads or polygons, a mesh containing
 * 6 quads will be returned.
 *
 * @return A Mesh containing a ``[min, min+edgeLength]`` Cube.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType, Point3Concept PositionType>
MeshType createCube(const PositionType& min, double edgeLength)
{
    return createHexahedron<MeshType>(min, PositionType(min + edgeLength));
}

/**
 * @brief Creates and returns a Cube having ``(-0.5, -0.5, -0.5)`` as minimum
 * extreme and 1 length.
 *
 * If the mesh is composed of triangles, the the returned mesh is already
 * triangulated. If the mesh is composed of quads or polygons, a mesh containing
 * 6 quads will be returned.
 *
 * @return A Cube centered in ``(0, 0, 0)`` and with edge length 1.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createCube()
{
    using PositionType = MeshType::Vertex::PositionType;
    return createCube<MeshType>(PositionType(-0.5, -0.5, -0.5), 1);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_HEXAHEDRON_H
