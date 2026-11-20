// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_ALGORITHMS_MESH_INTERSECTION_H
#define VCL_ALGORITHMS_MESH_INTERSECTION_H

#include <vclib/algorithms/core.h>
#include <vclib/mesh.h>

/**
 * @defgroup intersection_mesh Mesh Intersection Algorithms
 *
 * @ingroup algorithms_mesh
 *
 * @brief List of mesh intersection algorithms.
 *
 * You can access these algorithms by including
 * `#include <vclib/algorithms/mesh.h>`
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
    using PositionType = PositionType<MeshType>;
    using IndexType    = EdgeMesh::IndexType;

    EdgeMesh em;

    std::vector<double> qH(m.vertexContainerSize());

    for (auto&& v : m.vertices())
        qH[v.index()] = pl.dist(v.position());

    for (auto&& f : m.faces()) {
        std::vector<PositionType> ptVec;
        std::vector<PositionType> nmVec;
        for (uint j = 0; j < f.vertexCount(); ++j) {
            if (qH[f.vertexIndex(j)] == 0) {
                ptVec.push_back(f.vertex(j).position());
                if constexpr (HasVertexNormal<MeshType>) {
                    if (m.isVertexNormalAvailable()) {
                        nmVec.push_back(f.vertex(j).normal());
                    }
                }
            }
            else if ((qH[f.vertexIndex(j)] * qH[f.vertexIndexMod(j + 1)]) < 0) {
                const PositionType& p0 = f.vertex(j).position();
                const PositionType& p1 = f.vertexMod(j + 1).position();

                double q0 = qH[f.vertexIndex(j)];
                double q1 = qH[f.vertexIndexMod(j + 1)];

                std::pair<PositionType, PositionType> seg(p0, p1);
                PositionType pp = pl.segmentIntersection(seg);
                ptVec.push_back(pp);
                if constexpr (HasVertexNormal<MeshType>) {
                    if (m.isVertexNormalAvailable()) {
                        const auto&  n0 = f.vertex(j).normal();
                        const auto&  n1 = f.vertexMod(j + 1).normal();
                        PositionType nn =
                            (n0 * std::abs(q1) + n1 * std::abs(q0)) /
                            std::abs(q0 - q1);
                        nmVec.push_back(nn);
                    }
                }
            }
        }
        if (ptVec.size() >= 2) {
            IndexType v0             = em.addVertices(2);
            IndexType v1             = v0 + 1;
            IndexType eid            = em.addEdge(v0, v1);
            em.vertex(v0).position() = ptVec[0];
            em.vertex(v1).position() = ptVec[1];
            if constexpr (
                HasVertexNormal<MeshType> && HasVertexNormal<EdgeMesh>) {
                if (m.isVertexNormalAvailable() &&
                    em.isVertexNormalAvailable()) {
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
    using IndexType    = MeshType::IndexType;
    using PositionType = PositionType<MeshType>;
    using ScalarType   = ScalarType<MeshType>;
    using FaceType     = ConstFaceType<MeshType>;

    auto faceSphereIntersectionFilter = [&sphere](FaceType f) -> bool {
        return intersect(f, sphere);
    };

    MeshType res = faceMeshFilter(m, faceSphereIntersectionFilter);

    IndexType i = 0;
    while (i < res.faceContainerSize()) {
        auto f = res.face(i);
        if (f.deleted()) {
            ++i;
            continue;
        }

        PositionType                      witness;
        std::pair<ScalarType, ScalarType> ires(0, 0);

        bool allIn = true;
        for (auto&& v : f.vertices()) {
            allIn = allIn && sphere.isInside(v.position());
        }
        if (!allIn && intersect(f, sphere, witness, ires)) {
            if (faceArea(f) > tol) {
                IndexType v0 = res.addVertices(3);
                IndexType v1 = v0 + 1;
                IndexType v2 = v0 + 2;
                IndexType fi = res.addFaces(4);

                auto currF = res.face(i);

                res.vertex(v0).importFrom(currF.vertex(0), false);
                res.vertex(v0).position() =
                    (currF.vertex(0).position() + currF.vertex(1).position()) /
                    ScalarType(2);
                res.vertex(v1).importFrom(currF.vertex(1), false);
                res.vertex(v1).position() =
                    (currF.vertex(1).position() + currF.vertex(2).position()) /
                    ScalarType(2);
                res.vertex(v2).importFrom(currF.vertex(2), false);
                res.vertex(v2).position() =
                    (currF.vertex(2).position() + currF.vertex(0).position()) /
                    ScalarType(2);

                res.face(fi).importFrom(currF, false);
                res.face(fi).setVertices(currF.vertexIndex(0), v0, v2);

                ++fi;
                res.face(fi).importFrom(currF, false);
                res.face(fi).setVertices(currF.vertexIndex(1), v1, v0);

                ++fi;
                res.face(fi).importFrom(currF, false);
                res.face(fi).setVertices(v0, v1, v2);

                ++fi;
                res.face(fi).importFrom(currF, false);
                res.face(fi).setVertices(v2, v1, currF.vertexIndex(2));

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
 * @brief Compute the intersection between a mesh and a ball.
 *
 * Same as intersection(const MeshType&, const Sphere<SScalar>&, double), where
 * the tolerance is set as 1/10^5*2*pi*radius.
 *
 * @note The returned mesh is a triangle soup
 *
 * @param m
 * @param sphere
 * @return
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
