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

#ifndef VCL_ALGORITHMS_MESH_CREATE_TETRAHEDRON_H
#define VCL_ALGORITHMS_MESH_CREATE_TETRAHEDRON_H

#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Creates a simple tetrahedron mesh with the following point
 * coordinates:
 *
 * ```
 * ( 1,  1,  1),
 * (-1,  1, -1),
 * (-1, -1,  1),
 * ( 1, -1, -1)
 * ```
 *
 * @tparam MeshType: the type of the mesh to be generated, it must satisfy the
 * FaceMeshConcept.
 *
 * @return a Mesh containing a Tetrahedron.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType>
MeshType createTetrahedron()
{
    using CoordType = MeshType::Vertex::CoordType;
    return createTetrahedron<MeshType>(
        CoordType(1, 1, 1),
        CoordType(-1, 1, -1),
        CoordType(-1, -1, 1),
        CoordType(1, -1, -1));
}

/**
 * @brief createTetrahedron createTetrahedron creates a simple tetrahedron mesh
 * with the given points. The function assumes that the points p0, p1 and p2 are
 * in counterclockwise order, and does not perform any sanity check about the
 * validity of the points.
 *
 * @tparam MeshType: the type of the mesh to be generated, it must satisfy the
 * FaceMeshConcept.
 * @tparam CoordType: the type of the coordinates of the points, it must satisfy
 * the Point3Concept.
 *
 * @param[in] p0: the first point of the tetrahedron.
 * @param[in] p1: the second point of the tetrahedron.
 * @param[in] p2: the third point of the tetrahedron.
 * @param[in] p3: the fourth point of the tetrahedron.
 *
 * @return a Mesh containing a Tetrahedron.
 *
 * @ingroup create
 */
template<FaceMeshConcept MeshType, Point3Concept CoordType>
MeshType createTetrahedron(
    const CoordType& p0,
    const CoordType& p1,
    const CoordType& p2,
    const CoordType& p3)
{
    MeshType m;

    m.addVertices(p0, p1, p2, p3);

    m.reserveFaces(4);
    m.addFace(0, 1, 2);
    m.addFace(0, 2, 3);
    m.addFace(0, 3, 1);
    m.addFace(3, 2, 1);

    return m;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CREATE_TETRAHEDRON_H
