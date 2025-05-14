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

#ifndef VCL_ALGORITHMS_MESH_INTERSECTION_H
#define VCL_ALGORITHMS_MESH_INTERSECTION_H

#include <vclib/algorithms/core/intersection/element.h>
#include <vclib/mesh/requirements.h>

/**
 * @defgroup intersection_mesh Mesh Intersection Algorithms
 *
 * @ingroup algorithms_mesh
 *
 * @brief List of mesh intersection algorithms.
 *
 * You can access these algorithms by including `#include
 * <vclib/algorithms/mesh/intersection.h>`
 */

namespace vcl {

/**
 * @brief Takes a mesh and a plane as inputs and computes the intersection
 * between the mesh and the plane. It creates a new EdgeMesh to represent the
 * intersection edges.
 *
 * First, for each vertex in the original mesh, it computes its distance to the
 * plane and stores it in a vector. Then, for each face in the original mesh, it
 * checks each of its edges to see if they intersect the plane. If an edge
 * intersects the plane, the intersection point is computed and added to a
 * vector of intersection points. If the edge lies exactly on the plane, its
 * vertices are added to the intersection points.
 *
 * Once all intersection points have been computed, the function creates a new
 * edge mesh and adds an edge between each pair of consecutive intersection
 * points that forms a line segment on the plane. If the original mesh has
 * per-vertex normals, the function also computes and stores the normal at each
 * intersection point.
 *
 * Requirements:
 * - EdgeMesh:
 *   - Vertices
 *     - Normals (optional)
 *   - Edges
 *
 * - MeshType:
 *   - Vertices
 *     - Normals (optional)
 *   - Faces
 *
 * @param m
 * @param pl
 *
 * @return the intersection between the original mesh and the plane as a
 * collection of line segments with optional normal vectors.
 *
 * @ingroup intersection_mesh
 */
template<
    EdgeMeshConcept EdgeMesh,
    FaceMeshConcept MeshType,
    PlaneConcept    PlaneType>
EdgeMesh intersection(const MeshType& m, const PlaneType& pl)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;
    using PositionType  = VertexType::PositionType;

    EdgeMesh em;

    std::vector<double> qH(m.vertexContainerSize());

    for (const VertexType& v : m.vertices())
        qH[m.index(v)] = pl.dist(v.position());

    for (const FaceType& f : m.faces()) {
        std::vector<PositionType> ptVec;
        std::vector<PositionType> nmVec;
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (qH[m.index(f.vertex(j))] == 0) {
                ptVec.push_back(f.vertex(j)->position());
                if constexpr (HasPerVertexNormal<MeshType>) {
                    if (isPerVertexNormalAvailable(m)) {
                        nmVec.push_back(f.vertex(j)->normal());
                    }
                }
            }
            else if (
                (qH[m.index(f.vertex(j))] * qH[m.index(f.vertexMod(j + 1))]) <
                0) {
                const PositionType& p0 = f.vertex(j)->position();
                const PositionType& p1 = f.vertexMod(j + 1)->position();

                float q0 = qH[m.index(f.vertex(j))];
                float q1 = qH[m.index(f.vertexMod(j + 1))];

                std::pair<PositionType, PositionType> seg(p0, p1);
                PositionType pp = pl.segmentIntersection(seg);
                ptVec.push_back(pp);
                if constexpr (HasPerVertexNormal<MeshType>) {
                    if (isPerVertexNormalAvailable(m)) {
                        using NormalType     = VertexType::NormalType;
                        const NormalType& n0 = f.vertex(j)->normal();
                        const NormalType& n1 = f.vertexMod(j + 1)->normal();
                        PositionType         nn =
                            (n0 * fabs(q1) + n1 * fabs(q0)) / fabs(q0 - q1);
                        nmVec.push_back(nn);
                    }
                }
            }
        }
        if (ptVec.size() >= 2) {
            uint eid              = em.addEdge();
            uint v0               = em.addVertices(2);
            uint v1               = v0 + 1;
            em.vertex(v0).position() = ptVec[0];
            em.vertex(v1).position() = ptVec[1];
            em.edge(eid).setVertex(0, v0);
            em.edge(eid).setVertex(1, v1);
            if constexpr (
                HasPerVertexNormal<MeshType> && HasPerVertexNormal<EdgeMesh>) {
                if (isPerVertexNormalAvailable(m) &&
                    isPerVertexNormalAvailable(em)) {
                    em.vertex(v0).normal() = nmVec[0];
                    em.vertex(v1).normal() = nmVec[1];
                }
            }
        }
    }

    //    // Clean-up: Remove duplicate vertex
    //    tri::Clean<EdgeMeshType>::RemoveDuplicateVertex(em);

    //    // Clean-up: Sort edges ensuring orientation coherence
    //    for (size_t j = 1; j < em.edge.size(); j++) {
    //        auto& n = em.edge[j - 1].V(1);
    //        for (size_t i = j; i < em.edge.size(); i++) {
    //            auto& ei = em.edge[i];
    //            if (ei.V(1) == n)
    //                std::swap(ei.V(0), ei.V(1));
    //            if (ei.V(0) == n) {
    //                std::swap(em.edge[j], em.edge[i]);
    //                break;
    //            }
    //        }
    //    }

    return em;
}

/**
 * @brief Compute the intersection between a mesh and a ball.
 *
 * Given a mesh and a sphere, returns a new mesh made by a copy of all the faces
 * entirely included in the sphere, plus new faces created by refining the ones
 * intersected by the sphere border. It works by recursively splitting the
 * triangles that cross the border, as long as their area is greater than a
 * given value tol.
 *
 * @note The returned mesh is a triangle soup
 *
 * @param m
 * @param sphere
 * @param tol
 * @return
 *
 * @ingroup intersection_mesh
 */
template<FaceMeshConcept MeshType, typename SScalar>
MeshType intersection(
    const MeshType&        m,
    const Sphere<SScalar>& sphere,
    double                 tol)
{
    using VertexType = MeshType::VertexType;
    using PositionType  = VertexType::PositionType;
    using ScalarType = PositionType::ScalarType;
    using FaceType   = MeshType::FaceType;

    auto faceSphereIntersectionFilter = [&sphere](const FaceType& f) -> bool {
        return intersect(f, sphere);
    };

    MeshType res = perFaceMeshFilter(m, faceSphereIntersectionFilter);

    uint i = 0;
    while (i < res.faceContainerSize()) {
        FaceType& f = res.face(i);

        PositionType                         witness;
        std::pair<ScalarType, ScalarType> ires(0, 0);

        bool allIn = true;
        for (const auto* v : f.vertices()) {
            allIn = allIn && sphere.isInside(v->position());
        }
        if (!allIn && intersect(f, sphere, witness, ires)) {
            if (faceArea(f) > tol) {
                uint v0 = res.addVertices(3);
                uint v1 = v0 + 1;
                uint v2 = v0 + 2;
                uint fi = res.addFaces(4);

                FaceType& f = res.face(i);

                res.vertex(v0).importFrom(*f.vertex(0), false);
                res.vertex(v0).position() =
                    (f.vertex(0)->position() + f.vertex(1)->position()) / 2;
                res.vertex(v1).importFrom(*f.vertex(1), false);
                res.vertex(v1).position() =
                    (f.vertex(1)->position() + f.vertex(2)->position()) / 2;
                res.vertex(v2).importFrom(*f.vertex(2), false);
                res.vertex(v2).position() =
                    (f.vertex(2)->position() + f.vertex(0)->position()) / 2;

                res.face(fi).importFrom(f, false);
                res.face(fi).setVertices(
                    f.vertex(0), &res.vertex(v0), &res.vertex(v2));

                ++fi;
                res.face(fi).importFrom(f, false);
                res.face(fi).setVertices(
                    f.vertex(1), &res.vertex(v1), &res.vertex(v0));

                ++fi;
                res.face(fi).importFrom(f, false);
                res.face(fi).setVertices(
                    &res.vertex(v0), &res.vertex(v1), &res.vertex(v2));

                ++fi;
                res.face(fi).importFrom(f, false);
                res.face(fi).setVertices(
                    &res.vertex(v2), &res.vertex(v1), f.vertex(2));

                res.deleteFace(i);
            }
        }
        if (ires.first > 0.0) { // closest point - radius. If >0 is outside
            res.deleteFace(i);
        }

        ++i;
    }

    return res;
}

/**
 * @copydoc intersection(const MeshType&, const PlaneType&)
 *
 * The tolerance is set as 1/10^5*2*pi*radius.
 *
 * @ingroup intersection_mesh
 */
template<FaceMeshConcept MeshType, typename SScalar>
MeshType intersection(const MeshType& m, const Sphere<SScalar>& sphere)
{
    double tol = M_PI * sphere.radius() * sphere.radius() / 100000;
    return intersection(m, sphere, tol);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_INTERSECTION_H
