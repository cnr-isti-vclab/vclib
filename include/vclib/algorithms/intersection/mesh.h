/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_ALGORITHMS_INTERSECTION_MESH_H
#define VCL_ALGORITHMS_INTERSECTION_MESH_H

#include "element.h"

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
 */
template<
    EdgeMeshConcept EdgeMesh,
    FaceMeshConcept MeshType,
    PlaneConcept    PlaneType>
EdgeMesh meshPlaneIntersection(const MeshType& m, const PlaneType& pl)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;
    using CoordType  = VertexType::CoordType;

    EdgeMesh em;

    std::vector<double> qH(m.vertexContainerSize());

    for (const VertexType& v : m.vertices())
        qH[m.index(v)] = pl.dist(v.coord());

    for (const FaceType& f : m.faces()) {
        std::vector<CoordType> ptVec;
        std::vector<CoordType> nmVec;
        for (uint j = 0; j < f.vertexNumber(); ++j) {
            if (qH[m.index(f.vertex(j))] == 0) {
                ptVec.push_back(f.vertex(j)->coord());
                if constexpr (HasPerVertexNormal<MeshType>) {
                    if (isPerVertexNormalAvailable(m)) {
                        nmVec.push_back(f.vertex(j)->normal());
                    }
                }
            }
            else if (
                (qH[m.index(f.vertex(j))] * qH[m.index(f.vertexMod(j + 1))]) <
                0)
            {
                const CoordType& p0 = f.vertex(j)->coord();
                const CoordType& p1 = f.vertexMod(j + 1)->coord();

                float q0 = qH[m.index(f.vertex(j))];
                float q1 = qH[m.index(f.vertexMod(j + 1))];

                std::pair<CoordType, CoordType> seg(p0, p1);
                CoordType pp = pl.segmentIntersection(seg);
                ptVec.push_back(pp);
                if constexpr (HasPerVertexNormal<MeshType>) {
                    if (isPerVertexNormalAvailable(m)) {
                        using NormalType     = VertexType::NormalType;
                        const NormalType& n0 = f.vertex(j)->normal();
                        const NormalType& n1 = f.vertexMod(j + 1)->normal();
                        CoordType         nn =
                            (n0 * fabs(q1) + n1 * fabs(q0)) / fabs(q0 - q1);
                        nmVec.push_back(nn);
                    }
                }
            }
        }
        if (ptVec.size() >= 2) {
            uint eid               = em.addEdge();
            uint v0                = em.addVertices(2);
            uint v1                = v0 + 1;
            em.vertex(v0).coord()  = ptVec[0];
            em.vertex(v1).coord()  = ptVec[1];
            em.edge(eid).vertex(0) = &em.vertex(v0);
            em.edge(eid).vertex(1) = &em.vertex(v1);
            if constexpr (
                HasPerVertexNormal<MeshType> && HasPerVertexNormal<EdgeMesh>)
            {
                if (isPerVertexNormalAvailable(m) &&
                    isPerVertexNormalAvailable(em))
                {
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
 * given a mesh return a new mesh made by a copy of all the faces entirely
 * includeded in the ball plus new faces created by refining the ones
 * intersected by the ball border. It works by recursively splitting the
 * triangles that cross the border, as long as their area is greater than a
 * given value tol. NOTE: the returned mesh is a triangle soup
 *
 * @param m
 * @param sphere
 * @param tol
 * @return
 */
template<FaceMeshConcept MeshType, typename SScalar>
MeshType meshSphereIntersection(
    const MeshType&             m,
    const vcl::Sphere<SScalar>& sphere,
    double                      tol)
{
    using VertexType = MeshType::VertexType;
    using CoordType  = VertexType::CoordType;
    using ScalarType = CoordType::ScalarType;
    using FaceType   = MeshType::FaceType;

    auto ffilter = [&sphere](const FaceType& f) -> bool {
        return faceSphereItersect(f, sphere);
    };

    MeshType res = perFaceMeshFilter(m, ffilter);

    uint i = 0;
    while (i < res.faceContainerSize()) {
        FaceType& f = res.face(i);

        CoordType                         witness;
        std::pair<ScalarType, ScalarType> ires(0, 0);

        bool allIn = true;
        for (const auto* v : f.vertices()) {
            allIn = allIn && sphere.isInside(v->coord());
        }
        if (!allIn && faceSphereItersect(f, sphere, witness, ires)) {
            if (faceArea(f) > tol) {
                uint v0 = res.addVertices(3);
                uint v1 = v0 + 1;
                uint v2 = v0 + 2;
                uint fi = res.addFaces(4);

                FaceType& f = res.face(i);

                res.vertex(v0).importFrom(*f.vertex(0));
                res.vertex(v0).coord() =
                    (f.vertex(0)->coord() + f.vertex(1)->coord()) / 2;
                res.vertex(v1).importFrom(*f.vertex(1));
                res.vertex(v1).coord() =
                    (f.vertex(1)->coord() + f.vertex(2)->coord()) / 2;
                res.vertex(v2).importFrom(*f.vertex(2));
                res.vertex(v2).coord() =
                    (f.vertex(2)->coord() + f.vertex(0)->coord()) / 2;

                res.face(fi).importFrom(f);
                res.face(fi).setVertices(
                    f.vertex(0), &res.vertex(v0), &res.vertex(v2));

                ++fi;
                res.face(fi).importFrom(f);
                res.face(fi).setVertices(
                    f.vertex(1), &res.vertex(v1), &res.vertex(v0));

                ++fi;
                res.face(fi).importFrom(f);
                res.face(fi).setVertices(
                    &res.vertex(v0), &res.vertex(v1), &res.vertex(v2));

                ++fi;
                res.face(fi).importFrom(f);
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
 * @brief Same as meshSphereIntersection(MeshType, Sphere, double);
 *
 * The tolerance is set as 1/10^5*2*pi*radius.
 * @param m
 * @param sphere
 * @return
 */
template<FaceMeshConcept MeshType, typename SScalar>
MeshType meshSphereIntersection(
    const MeshType&             m,
    const vcl::Sphere<SScalar>& sphere)
{
    double tol = M_PI * sphere.radius() * sphere.radius() / 100000;
    return meshSphereIntersection(m, sphere, tol);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_INTERSECTION_MESH_H
