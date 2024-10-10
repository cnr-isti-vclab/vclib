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

#ifndef VCL_ALGORITHMS_MESH_STAT_BARYCENTER_H
#define VCL_ALGORITHMS_MESH_STAT_BARYCENTER_H

#include <vclib/concepts/mesh.h>
#include <vclib/mesh/requirements.h>

namespace vcl {

/**
 * @brief Returns the barycenter of the mesh, that is the simple average of all
 * the vertex coordintes of the mesh.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *
 * @param[in] m: input mesh on which compute the barycenter.
 * @return The barycenter of the input mesh.
 */
template<MeshConcept MeshType>
typename MeshType::VertexType::CoordType barycenter(const MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;

    CoordType bar;

    for (const VertexType& v : m.vertices()) {
        bar += v.coord();
    }

    return bar / m.vertexNumber();
}

/**
 * @brief Returns the barycenter of the mesh weighted on the per vertex quality
 * values.
 *
 * The output baryceter is computed as a weighted average of the vertices of the
 * mesh, using the per Vertex Quality values as weights.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices:
 *     - Quality
 *
 * @param[in] m: input mesh on which compute the barycenter.
 * @return The barycenter weighted on the per vertex quality.
 */
template<MeshConcept MeshType>
typename MeshType::VertexType::CoordType scalarWeightedBarycenter(
    const MeshType& m)
{
    vcl::requirePerVertexQuality(m);

    using VertexType  = MeshType::VertexType;
    using CoordType   = VertexType::CoordType;
    using QualityType = VertexType::QualityType;

    CoordType   bar;
    QualityType weightedSum = 0;

    for (const VertexType& v : m.vertices()) {
        bar += v.coord() * v.quality();
        weightedSum += v.quality();
    }

    return bar / weightedSum;
}

/**
 * @brief Computes the barycenter of the surface thin-shell.
 * E.g. it assume a 'empty' model where all the mass is located on the surface
 * and compute the barycenter of that thinshell. Works for any polygonal model
 * (no problem with open, nonmanifold selfintersecting models). Useful for
 * computing the barycenter of 2D planar figures.
 *
 * Requirements:
 * - Mesh:
 *   - Vertices
 *   - Faces
 *
 * @param m
 * @return
 */
template<FaceMeshConcept MeshType>
typename MeshType::VertexType::CoordType shellBarycenter(const MeshType& m)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;
    using CoordType  = VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;

    CoordType bar;
    bar.setZero();
    ScalarType areaSum = 0;

    for (const FaceType& f : m.faces()) {
        ScalarType area = faceArea(f);
        bar += faceBarycenter(f) * area;
        areaSum += area;
    }

    return bar / areaSum;
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_STAT_BARYCENTER_H
