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

#include <vclib/algorithms/core/box/box3.h>
#include <vclib/algorithms/core/visibility.h>
#include <vclib/algorithms/mesh/create/tetrahedron.h>
#include <vclib/algorithms/mesh/update/topology.h>
#include <vclib/concepts/mesh.h>
#include <vclib/mesh/requirements.h>
#include <vclib/misc/logger.h>
#include <vclib/misc/parallel.h>
#include <vclib/misc/shuffle.h>
#include <vclib/space/complex/graph/bipartite_graph.h>
#include <vclib/space/complex/mesh_pos.h>

namespace vcl {

namespace detail {

/**
 * @brief The first points are not actually random, but are the points that are
 * closest to the box sides. This optimization makes the first tetrahedron
 * bigger, and thus the convex hull computation faster at the beginning.
 *
 * @tparam R
 * @param points
 */
template<vcl::Range R>
void firstTetOptimization(R&& points)
{
    using PointType = std::ranges::range_value_t<R>;
    using Iterator  = std::ranges::iterator_t<R>;

    vcl::Box<PointType> box;

    for (const auto& p : points) {
        box.add(p);
    }

    std::array<std::pair<double, Iterator>, 6> distances;

    // save in the array the closest point to each of the 6 sides of the box
    for (uint i = 0; i < 6; ++i) {
        auto        it        = std::ranges::begin(points);
        const auto& boxCorner = i < 3 ? box.min() : box.max();
        distances[i] =
            std::make_pair(std::abs(boxCorner(i % 3) - (*it)(i % 3)), it);
        for (++it; it != std::ranges::end(points); ++it) {
            double d = std::abs(boxCorner(i % 3) - (*it)(i % 3));
            if (d < distances[i].first) {
                distances[i] = std::make_pair(d, it);
            }
        }
    }

    // sort the array by distance
    std::sort(distances.begin(), distances.end(), [](auto& a, auto& b) {
        return a.first < b.first;
    });

    // swap the first points with the closest points to the box corners
    for (uint i = 0; i < 6; ++i) {
        std::iter_swap(std::ranges::begin(points) + i, distances[i].second);
    }
}

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

    firstTetOptimization(points);

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
        bool added = false;
        for (const auto& face : mesh.faces()) {
            if (vcl::facePointVisibility(face, point)) {
                if (!added) {
                    graph.addLeftNode(point);
                    added = true;
                }
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
    for (auto it = visibleFaces.begin(); it != visibleFaces.end() && !found;
         ++it) {
        uint i = 0;
        for (const FaceType* adjFace : (*it)->adjFaces()) {
            // if the adjacent face is not visible, then it is on the border
            if (visibleFaces.find(adjFace) == visibleFaces.end()) {
                firstFace   = *it;
                firstEdge   = i;
                currentFace = firstFace;
                currentEdge = firstEdge;
                found       = true;
                break;
            }
            i++;
        }
    }

    do {
        MeshPos<FaceType> pos(currentFace, currentEdge);
        pos.flipVertex();
        horizonVertices.insert(pos.vertex());

        while (visibleFaces.find(pos.face()) != visibleFaces.end()) {
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

template<Point3Concept PointType, FaceMeshConcept MeshType>
auto potentialConflictPoints(
    const BipartiteGraph<PointType, uint>& conflictGraph,
    const PointType&                       currentPoint,
    const MeshType&                        convexHull,
    const auto&                            horizon)
{
    using FaceType = MeshType::FaceType;

    std::vector<std::set<PointType>> potentialConflictPoints(horizon.size());

    uint i = 0;
    for (const auto& [faceIndex, edgeIndex] : horizon) {
        for (const auto& p : conflictGraph.adjacentRightNodes(faceIndex)) {
            potentialConflictPoints[i].insert(p);
        }

        const FaceType& face = convexHull.face(faceIndex);

        uint adjFaceIndex = face.adjFace(edgeIndex)->index();

        for (const auto& p : conflictGraph.adjacentRightNodes(adjFaceIndex)) {
            potentialConflictPoints[i].insert(p);
        }

        potentialConflictPoints[i].erase(currentPoint);

        i++;
    }

    return potentialConflictPoints;
}

template<
    FaceMeshConcept MeshType,
    FaceConcept     FaceType,
    VertexConcept   VertexType>
void deleteVisibleFaces(
    MeshType&                          convexHull,
    const std::set<const FaceType*>&   visibleFaces,
    const std::set<const VertexType*>& horizonVertices)
{
    std::set<const VertexType*> verticesToDelete;

    for (const FaceType* face : visibleFaces) {
        for (const VertexType* vertex : face->vertices()) {
            if (horizonVertices.find(vertex) == horizonVertices.end()) {
                verticesToDelete.insert(vertex);
            }
        }
        convexHull.deleteFace(face->index());
    }

    for (const VertexType* vertex : verticesToDelete) {
        convexHull.deleteVertex(vertex->index());
    }
}

template<Point3Concept PointType, FaceMeshConcept MeshType>
std::vector<uint> addNewFaces(
    MeshType&                                 convexHull,
    const PointType&                          currentPoint,
    const std::vector<std::pair<uint, uint>>& horizon)
{
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    std::vector<uint> newFaces;

    newFaces.reserve(horizon.size());

    uint v = convexHull.addVertex(currentPoint);

    uint firstFace = UINT_NULL;
    uint lastFace  = UINT_NULL;

    for (const auto& [faceIndex, edgeIndex] : horizon) {
        uint v1 = convexHull.face(faceIndex).vertexIndexMod(edgeIndex + 1);
        uint v2 = convexHull.face(faceIndex).vertexIndex(edgeIndex);

        uint f = convexHull.addFace(v, v1, v2);
        newFaces.push_back(f);

        if (firstFace == UINT_NULL) {
            firstFace = f;
        }

        convexHull.face(f).setAdjFace(0, lastFace);
        if (lastFace != UINT_NULL)
            convexHull.face(lastFace).setAdjFace(2, f);

        convexHull.face(f).setAdjFace(1, faceIndex);
        convexHull.face(faceIndex).setAdjFace(edgeIndex, f);

        lastFace = f;
    }

    convexHull.face(firstFace).setAdjFace(0, lastFace);
    convexHull.face(lastFace).setAdjFace(2, firstFace);

    return newFaces;
}

template<Point3Concept PointType, FaceMeshConcept MeshType>
void updateNewConflicts(
    BipartiteGraph<PointType, uint>&        conflictGraph,
    const MeshType&                         convexHull,
    const std::vector<uint>&                newFaces,
    const std::vector<std::set<PointType>>& potentialConflictPoints)
{
    using FaceType = MeshType::FaceType;

    for (uint i = 0; i < newFaces.size(); ++i) {
        conflictGraph.addRightNode(newFaces[i]);

        const FaceType& face = convexHull.face(newFaces[i]);

        for (const auto& p : potentialConflictPoints[i]) {
            if (vcl::facePointVisibility(face, p)) {
                conflictGraph.addArc(p, newFaces[i]);
            }
        }
    }
}

} // namespace detail

/**
 * @brief Compute the convex hull of a set of points.
 *
 * @tparam PointType: The type of the points.
 * @param[in] points: The set of points.
 * @param[in] deterministic: If true, the shuffle will be deterministic.
 * @param[in] log: The logger.
 * @return The convex hull of the points.
 *
 * @ingroup algorithms_mesh
 */
template<FaceMeshConcept MeshType, Range R, LoggerConcept LogType = NullLogger>
MeshType convexHull(
    const R& points,
    bool     deterministic = false,
    LogType& log           = nullLogger)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    using PointType  = std::ranges::range_value_t<R>;
    using VertexType = MeshType::VertexType;
    using FaceType   = MeshType::FaceType;

    static_assert(
        vcl::face::HasAdjacentFaces<FaceType>,
        "MeshType must have per-face adjacent faces.");

    log.log(0, "Shuffling points...");

    std::vector<PointType> pointsCopy(
        std::ranges::begin(points), std::ranges::end(points));

    detail::shufflePoints(pointsCopy, deterministic);

    log.log(0, "Making first tetrahedron...");

    auto result = detail::makeTetrahedron<MeshType>(
        pointsCopy[0], pointsCopy[1], pointsCopy[2], pointsCopy[3]);

    auto remainingPoints = pointsCopy | std::views::drop(4);

    auto conflictGraph = detail::initConflictGraph(result, remainingPoints);

    log.log(0, "Computing convex hull...");

    log.startProgress("Processing Points...", pointsCopy.size() - 4);
    uint iteration = 0;
    // for each point in the conflict graph (still not in the convex hull)
    for (const auto& point : conflictGraph.leftNodes()) {
        // if the point is visible from a face in the convex hull
        if (conflictGraph.adjacentLeftNodeNumber(point) != 0) {
            // store the faces that are visible from (conflict with) the point
            // these faces will be deleted from the convex hull
            std::set<const FaceType*> visibleFaces;
            for (const auto& faceIndex :
                 conflictGraph.adjacentLeftNodes(point)) {
                visibleFaces.insert(&result.face(faceIndex));
            }

            // compute the horizon of the visible faces
            std::set<const VertexType*> horizonVertices;
            auto horizon = detail::horizonFaces(visibleFaces, horizonVertices);

            // for each edge in the horizon, compute the points that could
            // conflict with the new face that will be added to the convex hull
            // in that edge
            auto potentialConflictPoints = detail::potentialConflictPoints(
                conflictGraph, point, result, horizon);

            // delete the point and faces from the conflict graph - no more
            // conflicts
            conflictGraph.deleteLeftNode(point);
            for (const FaceType* face : visibleFaces) {
                conflictGraph.deleteRightNode(face->index());
            }

            // delete the visible faces from the convex hull, and the vertices
            // that are not on the horizon
            detail::deleteVisibleFaces(result, visibleFaces, horizonVertices);

            // add the new faces to the convex hull
            auto newFaces = detail::addNewFaces(result, point, horizon);

            // update the conflict graph with the new faces
            detail::updateNewConflicts(
                conflictGraph, result, newFaces, potentialConflictPoints);
        }
        else {
            conflictGraph.deleteLeftNode(point);
        }
        iteration++;
        log.progress(iteration);
    }

    log.endProgress();
    log.log(100, "Convex hull computed.");

    return result;
}

/**
 * @brief Compute the convex hull of a set of points.
 *
 * @tparam PointType: The type of the points.
 * @param[in] points: The set of points.
 * @param[in] log: The logger.
 * @return The convex hull of the points.
 *
 * @ingroup algorithms_mesh
 */
template<FaceMeshConcept MeshType, Range R, LoggerConcept LogType = NullLogger>
MeshType convexHull(const R& points, LogType& log)
    requires vcl::Point3Concept<std::ranges::range_value_t<R>>
{
    return convexHull<MeshType>(points, false, log);
}

} // namespace vcl

#endif // VCL_ALGORITHMS_MESH_CONVEX_HULL_H
