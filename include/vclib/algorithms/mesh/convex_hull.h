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

#ifndef VCL_ALGORITHMS_MESH_CONVEX_HULL_H
#define VCL_ALGORITHMS_MESH_CONVEX_HULL_H

#include <vclib/algorithms/core/visibility.h>
#include <vclib/algorithms/mesh/create/tetrahedron.h>
#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/concepts/mesh.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/shuffle.h>
#include <vclib/space/complex/graph/bipartite_graph.h>
#include <vclib/space/complex/mesh_pos.h>

namespace vcl {

namespace detail {

/**
 * @brief Shuffle the input range of points such that the first four points are
 * not coplanar.
 *
 * @tparam R
 * @param points
 * @param deterministic
 */
template<vcl::Range R>
void shufflePoints(R&& points, bool deterministic = false)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    vcl::shuffle(points, deterministic);

    auto itP0 = std::ranges::begin(points);
    auto itP1 = std::next(itP0);
    auto itP2 = std::next(itP1);
    auto itP3 = std::next(itP2);
    auto itP  = std::next(itP2);

    auto rEnd = std::ranges::end(points);

    while (itP != rEnd && vcl::arePointsCoplanar(*itP0, *itP1, *itP2, *itP)) {
        itP = std::next(itP);
    }

    if (itP == rEnd) {
        throw std::runtime_error("All points are coplanar.");
    }

    std::iter_swap(itP, itP3);
}

template<FaceMeshConcept MeshType, Point3Concept PointType>
MeshType makeTetrahedron(
    const PointType& p0,
    const PointType& p1,
    const PointType& p2,
    const PointType& p3)
{
    using FaceType = MeshType::FaceType;

    MeshType result;

    if (vcl::trianglePointVisibility(p0, p1, p2, p3)) {
        result = vcl::createTetrahedron<MeshType>(p0, p2, p1, p3);
    }
    else {
        result = vcl::createTetrahedron<MeshType>(p0, p1, p2, p3);
    }

    if constexpr (vcl::face::HasOptionalAdjacentFaces<FaceType>) {
        result.enablePerFaceAdjacentFaces();
    }

    vcl::updatePerFaceAdjacentFaces(result);

    return result;
}

template<FaceMeshConcept MeshType, Range R>
auto initConflictGraph(const MeshType& mesh, R&& points)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    using PointType  = std::ranges::range_value_t<R>;
    using MPointType = MeshType::VertexType::CoordType;
    using FaceType   = MeshType::FaceType;
    using GraphType  = vcl::BipartiteGraph<PointType, uint>;

    // left nodes are points, right nodes are faces
    // an arc (conflict) is added if a point is visible from a face
    GraphType graph;

    for (const auto& face : mesh.faces()) {
        graph.addRightNode(face.index());
    }

    for (const auto& point : points) {
        graph.addLeftNode(point);
        for (const auto& face : mesh.faces()) {
            if (vcl::facePointVisibility(face, point)) {
                graph.addArc(point, face.index());
            }
        }
    }

    return graph;
}

/**
 * @brief Returns the horizon, that is composed of a sequence of <face-edge>
 * pairs that looks out the boundary of the visible faces.
 * 
 * The faces listed in the horizon are faces that are not visible from the
 * input set of visible faces, but are adjacent to at least one face in the
 * input set.
 * 
 * @tparam FaceType 
 * @param[in] visibleFaces: The set of visible faces.
 * @param[out] horizonVertices: The set of vertices that are on the horizon.
 * @return std::vector<std::pair<uint, uint>> 
 */
template<FaceConcept FaceType, VertexConcept VertexType>
std::vector<std::pair<uint, uint>> horizonFaces(
    const std::set<const FaceType*>& visibleFaces,
    std::set<const VertexType*>&     horizonVertices)
{
    std::vector<std::pair<uint, uint>> horizon;

    const FaceType* firstFace   = nullptr;
    uint            firstEdge   = UINT_NULL;
    const FaceType* currentFace = nullptr;
    uint            currentEdge = UINT_NULL;

    // looking for the first visible face that lies on the border
    bool found = false;
    for (auto it = visibleFaces.begin(); it != visibleFaces.end() && !found; ++it) {
        uint i = 0;
        for (const FaceType* adjFace : (*it)->adjFaces()) {
            // if the adjacent face is not visible, then it is on the border
            if (visibleFaces.find(adjFace) == visibleFaces.end()) {
                firstFace        = *it;
                firstEdge        = i;
                currentFace      = firstFace;
                currentEdge      = firstEdge;
                found            = true;
                break;
            }
            i++;
        }
    }

    do {
        MeshPos<FaceType> pos(currentFace, currentEdge);
        pos.flipVertex();
        horizonVertices.insert(pos.vertex());

        while(visibleFaces.find(pos.face()) != visibleFaces.end()) {
            pos.nextEdgeAdjacentToV();
        }
        auto p = std::make_pair(pos.face()->index(), pos.edge());
        horizon.push_back(p);

        pos.flipFace();
        currentFace = pos.face();
        currentEdge = pos.edge();
    } while (currentFace != firstFace || currentEdge != firstEdge);

    return horizon;
}

} // namespace detail

/**
 * @brief Compute the convex hull of a set of points.
 *
 * @tparam PointType: The type of the points.
 * @param[in] points: The set of points.
 * @param[in] deterministic: If true, the shuffle will be deterministic.
 * @return The convex hull of the points.
 *
 * @ingroup algorithms_mesh
 */
template<FaceMeshConcept MeshType, Range R>
MeshType convexHull(R&& points, bool deterministic = false)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    using VertexType = MeshType::VertexType;
    using FaceType = MeshType::FaceType;

    static_assert(
        vcl::face::HasAdjacentFaces<FaceType>,
        "MeshType must have per-face adjacent faces.");

    detail::shufflePoints(points, deterministic);

    auto result = detail::makeTetrahedron<MeshType>(
        points[0], points[1], points[2], points[3]);

    auto remainingPoints = points | std::views::drop(4);

    auto conflictGraph = detail::initConflictGraph(result, remainingPoints);

    // for each point in the conflict graph (still not in the convex hull)
    for (const auto& point : conflictGraph.leftNodes()) {

        // if the point is visible from a face in the convex hull
        if (conflictGraph.adjacentLeftNodeNumber(point) != 0) {
            // store the faces that are visible from (conflict with) the point
            // these faces will be deleted from the convex hull
            std::set<const FaceType*> visibleFaces;
            for (const auto& faceIndex : conflictGraph.adjacentLeftNodes(point)) {
                visibleFaces.insert(&result.face(faceIndex));
            }

            // compute the horizon of the visible faces
            std::set<const VertexType*> horizonVertices;
            auto horizon = detail::horizonFaces(visibleFaces, horizonVertices);

            // compute the potential conflict points for the new faces
            // todo: implement this
        }
        else {
            conflictGraph.deleteLeftNode(point);
        }
    }

    return result;
}

template<FaceMeshConcept MeshType, Range R>
MeshType convexHull(const R& points, bool deterministic = false)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    using PointType = std::ranges::range_value_t<R>;
    std::vector<PointType> pointsCopy(
        std::ranges::begin(points), std::ranges::end(points));
    return convexHull<MeshType>(pointsCopy, deterministic);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CONVEX_HULL_H
