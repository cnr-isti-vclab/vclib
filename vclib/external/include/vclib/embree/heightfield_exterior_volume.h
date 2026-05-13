/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2026                                                    *
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

#ifndef VCL_EMBREE_HEIGHTFIELD_EXTERIOR_VOLUME_H
#define VCL_EMBREE_HEIGHTFIELD_EXTERIOR_VOLUME_H

#include "scene.h"

#include <vclib/algorithms/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space/complex.h>

#include <algorithm>
#include <limits>
#include <mutex>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace vcl::embree {

struct VolumeResultMeshes
{
    bool       computeMeshes = true;
    PolyMesh   exteriorVolumeMesh;
    EdgeMesh   grid2dMesh;
    std::mutex appendMutex;
};

namespace detail {

template<typename ScalarType>
RegularGrid2<ScalarType> makeGrid(
    const Box2<ScalarType>&   bbPlane,
    const Point2<ScalarType>& gridCellSideLengths)
{
    if (gridCellSideLengths.x() <= 0.0 || gridCellSideLengths.y() <= 0.0) {
        Point<uint, 2> oneCell(1u, 1u);
        return RegularGrid2<ScalarType>(bbPlane, oneCell);
    }
    return RegularGrid2<ScalarType>(bbPlane, gridCellSideLengths);
}

inline double segmentVolume(
    double cellArea,
    double startD,
    double endD,
    double epsilon,
    bool   first = false)
{
    const double segLength   = endD - startD;
    const bool   validLength = first || (segLength >= epsilon);

    double segVolume = 0;
    if (validLength) {
        segVolume = cellArea * segLength;
    }

    return segVolume;
}

template<FaceMeshConcept MeshType, typename ScalarType>
ScalarType processCell(
    uint                            i,
    uint                            j,
    const RegularGrid2<ScalarType>& grid,
    const OrthoFrame3<ScalarType>&  frame,
    const Scene&                    scene,
    const MeshType&                 m,
    auto                            epsilon,
    VolumeResultMeshes&             outMeshes)
{
    using PointType = Point3<ScalarType>;

    const PointType& n = frame.axis(2);

    RegularGrid2<double>::CellPos cellPos(i, j);

    const auto      c2d        = grid.cellCenter(cellPos);
    const PointType cellCenter = frame.toWorld(PointType(c2d(0), c2d(1), 0.0));

    ScalarType volumeAcc = 0.0;

    std::vector<Scene::HitResult> hits =
        scene.facesIntersectedByRay(cellCenter, n, static_cast<float>(epsilon));

    const ScalarType cellArea = grid.cellVolume();

    ScalarType prevT      = -epsilon;
    PointType  prevNormal = n;
    bool       first      = true;

    int hitsMesh = 0;

    for (const auto& hit : hits) {
        auto [hitFaceId, _, __, tHit] = hit;

        if (hitFaceId == UINT_NULL) {
            continue;
        }

        const auto& face = m.face(hitFaceId);

        const PointType endPoint  = cellCenter + n * tHit;
        PointType       endNormal = face.normal();
        if (endNormal.norm() >= epsilon) {
            endNormal.normalize();
        }

        if (!first && tHit <= prevT) {
            prevT      = tHit;
            prevNormal = endNormal;
            continue;
        }

        const ScalarType startDot = prevNormal.dot(n);
        const ScalarType endDot   = endNormal.dot(n);

        if (endDot < 0.0) {
            if (startDot > 0.0 && hitsMesh == 0) {
                double volSeg =
                    segmentVolume(cellArea, prevT, tHit, epsilon, first);

                if (outMeshes.computeMeshes && volSeg > 0) {
                    using VMesh = decltype(outMeshes.exteriorVolumeMesh);

                    const auto u0  = grid.min()(0) + i * grid.cellLength(0);
                    const auto u1  = u0 + grid.cellLength(0);
                    const auto v0  = grid.min()(1) + j * grid.cellLength(1);
                    const auto v1  = v0 + grid.cellLength(1);
                    VMesh      hex = createHexahedron<VMesh>(
                        Point3d(u0, v0, prevT), Point3d(u1, v1, tHit));
                    for (auto& vert : hex.vertices()) {
                        const PointType p =
                            vert.position().template cast<ScalarType>();
                        vert.position() =
                            frame.toWorld(p).template cast<double>();
                    }
                    {
                        std::lock_guard<std::mutex> lock(outMeshes.appendMutex);
                        outMeshes.exteriorVolumeMesh.append(hex);
                    }
                }

                volumeAcc += volSeg;
            }
            hitsMesh += 1;
        }
        else {
            hitsMesh -= 1;
        }

        prevT      = tHit;
        prevNormal = endNormal;
        first      = false;
    }

    return volumeAcc;
}

} // namespace detail

/**
 * @brief Computes the exterior heightfield volume of a mesh along a given
 * direction.
 *
 * This function estimates the "exterior volume" of a mesh projected along
 * @p direction. The mesh's bounding plane perpendicular to @p direction is
 * subdivided into a 2D regular grid of cells with the specified side lengths.
 * For each cell, a ray is cast along @p direction through the mesh using
 * Embree ray tracing. The exterior volume is accumulated by integrating the
 * portions of each ray segment that lie outside the mesh (i.e., between the
 * print bed plane and the first downward-facing surface encountered).
 *
 * If @p outMeshes.computeMeshes is `true`, the function also populates
 * @p outMeshes with:
 * - `exteriorVolumeMesh`: a `PolyMesh` made of hexahedral cells representing
 *   each exterior volume segment.
 * - `grid2dMesh`: an `EdgeMesh` representing the 2D grid projected onto the
 *   plane perpendicular to @p direction.
 *
 * @note The input mesh is expected to have updated per-face normals.
 *
 * @throws std::invalid_argument if the direction vector has zero length.
 *
 * @tparam MeshType: A type satisfying `FaceMeshConcept`.
 * @tparam ScalarType: The scalar type used for positions and volume
 * accumulation.
 * @tparam Point2Type: A type satisfying `Point2Concept`, used for
 * @p gridCellSideLengths.
 *
 * @param[in] m: The input 3D mesh to evaluate. Must satisfy `FaceMeshConcept`.
 * @param[in] scene: The Embree scene built from @p m, used for ray intersection
 * queries.
 * @param[in] direction: The direction along which rays are cast and volume is
 * measured. It will be normalized internally.
 * @param[in] gridCellSideLengths: The physical width and height of each 2D grid
 * cell in the plane perpendicular to @p direction. If either component is
 * zero or negative, a single cell covering the full bounding box is used.
 * @param[in] epsilon: A small value used for numerical precision: it filters
 * out degenerate ray segments and near-zero normals.
 * @param[in/out] outMeshes: Output structure to receive the hexahedral
 * exterior volume mesh and 2D grid mesh if `outMeshes.computeMeshes` is `true`.
 *
 * @return The total exterior heightfield volume as a `ScalarType` value.
 */
template<
    FaceMeshConcept MeshType,
    typename ScalarType,
    Point2Concept Point2Type>
auto heightfieldExteriorVolume(
    const MeshType&           m,
    const Scene&              scene,
    const Point3<ScalarType>& direction,
    const Point2Type&         gridCellSideLengths,
    auto                      epsilon,
    VolumeResultMeshes&       outMeshes)
{
    using namespace vcl;

    using PointType      = Point3<ScalarType>;
    using OrthoFrameType = OrthoFrame<ScalarType, 3>;

    PointType n = direction;
    if (n.norm() <= epsilon) {
        throw std::invalid_argument(
            "Direction vector must have non-zero length greater than epsilon.");
    }
    n.normalize();

    ScalarType minProj = std::numeric_limits<ScalarType>::infinity();
    for (const auto& vv : m.vertices()) {
        minProj = std::min(minProj, vv.position().dot(n));
    }
    const PointType         planeOrigin = n * minProj;
    const Plane<ScalarType> plane(planeOrigin, n);

    const OrthoFrameType frame = OrthoFrameType::fromNormal(planeOrigin, n);

    Box2<ScalarType> bbPlane;

    for (const auto& vert : m.vertices()) {
        const PointType projected = plane.projectPoint(vert.position());
        const PointType rel       = projected - frame.origin();

        const ScalarType pu = rel.dot(frame.axis(0));
        const ScalarType pv = rel.dot(frame.axis(1));
        const Point2Type projUV(pu, pv);

        bbPlane.add(projUV.template cast<ScalarType>());
    }

    if (bbPlane.dim(0) <= epsilon || bbPlane.dim(1) <= epsilon) {
        return std::numeric_limits<ScalarType>::infinity();
    }

    const RegularGrid2<ScalarType> grid = detail::makeGrid(
        bbPlane, gridCellSideLengths.template cast<ScalarType>());

    if (outMeshes.computeMeshes) {
        // out grid
        for (uint ii = 0; ii <= grid.cellCount(0); ++ii) {
            const ScalarType cu = grid.min()(0) + grid.cellLength(0) * ii;
            const uint       a  = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(PointType(cu, grid.min()(1), 0.0))
                    .template cast<double>());
            const uint b = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(PointType(cu, grid.max()(1), 0.0))
                    .template cast<double>());
            outMeshes.grid2dMesh.addEdge(a, b);
            outMeshes.grid2dMesh.vertex(a).normal() =
                direction.template cast<double>();
            outMeshes.grid2dMesh.vertex(b).normal() =
                direction.template cast<double>();
        }
        for (uint jj = 0; jj <= grid.cellCount(1); ++jj) {
            const ScalarType cv = grid.min()(1) + grid.cellLength(1) * jj;
            const uint       a  = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(PointType(grid.min()(0), cv, 0.0))
                    .template cast<double>());
            const uint b = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(PointType(grid.max()(0), cv, 0.0))
                    .template cast<double>());
            outMeshes.grid2dMesh.addEdge(a, b);
            outMeshes.grid2dMesh.vertex(a).normal() =
                direction.template cast<double>();
            outMeshes.grid2dMesh.vertex(b).normal() =
                direction.template cast<double>();
        }
    }

    ScalarType totalVolume = 0.0;

    std::vector<uint> allCells(grid.totalCellCount());
    std::iota(allCells.begin(), allCells.end(), 0);

    std::vector<ScalarType> cellVolumes(grid.totalCellCount(), 0.0);

    vcl::parallelFor(allCells, [&](uint idx) {
        uint i = idx % grid.cellCount(0);
        uint j = idx / grid.cellCount(0);

        cellVolumes[idx] = detail::processCell(
            i, j, grid, frame, scene, m, epsilon, outMeshes);
    });

    totalVolume += std::accumulate(cellVolumes.begin(), cellVolumes.end(), 0.0);

    return totalVolume;
}

/**
 * @copydoc heightfieldExteriorVolume
 */
template<
    FaceMeshConcept MeshType,
    typename ScalarType,
    Point2Concept Point2Type>
auto heightfieldExteriorVolume(
    const MeshType&           m,
    const Scene&              scene,
    const Point3<ScalarType>& direction,
    const Point2Type&         gridCellSideLengths,
    auto                      epsilon)
{
    VolumeResultMeshes dummyOutMeshes {false};
    return heightfieldExteriorVolume(
        m, scene, direction, gridCellSideLengths, epsilon, dummyOutMeshes);
}

/**
 * @brief Finds the optimal print orientation by minimizing the exterior
 * heightfield volume.
 *
 * This function determines the best spatial orientation for a given 3D mesh to
 * minimize the amount of support material required during 3D printing (e.g.,
 * for MJP printers). It achieves this by evaluating a discrete set of possible
 * print directions. For each direction, the algorithm computes the "exterior
 * volume" (the space between the print bed and the mesh's downward-facing
 * surfaces) using a heightfield-based sampling approach. The orientation that
 * yields the minimum exterior volume is considered the optimal one.
 *
 * @note The input mesh is expected to have updated per-face normals.
 *
 * @note The candidate directions are uniformly sampled over a sphere using a
 * Fibonacci lattice/sphere algorithm to ensure an even and unbiased coverage of
 * the search space.
 *
 * @throws std::invalid_argument if the number of directions is zero.
 *
 * @param[in] m: The input 3D mesh to be evaluated. It is expected that this
 * mesh is already pre-scaled relative to the single cell dimensions. It must
 * satisfy the FaceMeshConcept.
 * @param[in] gridCellSideLengths: The physical dimensions of the single
 * sampling cell (e.g., the X, Y size of the printing "pixel" used to build the
 * heightfield).
 * @param[in] nDirs: The number of candidate print directions to test.
 * A higher number increases accuracy but significantly increases computation
 * time.
 * @param[in] epsilon: A small value used to handle numerical precision issues,
 * multiplied by the diagonal of the mesh's bounding box.
 * @param[in] log: An optional logger object to report progress and results. If
 * not provided, a null logger will be used, and no logging will occur.
 *
 * @return A vcl::Point3<S> representing the optimal orientation (e.g.,
 * direction vector) that minimizes the required support volume. `S` is the
 * scalar type used for the mesh's vertex positions and normals.
 */
template<
    FaceMeshConcept MeshType,
    typename P2Scalar,
    LoggerConcept LogType = NullLogger>
auto findBestOrientationByHeightfieldExteriorVolume(
    const MeshType&         m,
    const Point2<P2Scalar>& gridCellSideLengths,
    vcl::uint               nDirections,
    double                  epsilon = 1e-6,
    LogType&                log     = nullLogger)
{
    using namespace vcl;

    using PointType  = MeshType::VertexType::PositionType;
    using ScalarType = typename PointType::ScalarType;

    if (nDirections == 0) {
        throw std::invalid_argument(
            "Number of directions must be greater than zero.");
    }

    log.startNewTask(
        0, 100, "Finding best orientation by heightfield exterior volume...");

    requirePerFaceNormal(m);

    Box3<ScalarType> bbox = boundingBox(m);

    epsilon *= bbox.diagonal();

    // Ray tracing: shoot rays from grid cell centers through the mesh.
    embree::Scene scene(m);

    std::vector<PointType> fibNormals =
        sphericalFibonacciPointSet<PointType>(nDirections);

    uint       bestPlaneId = 0;
    ScalarType bestVolume  = std::numeric_limits<ScalarType>::infinity();
    PointType  bestNormal  = fibNormals.front();

    for (uint i = 0; i < fibNormals.size(); ++i) {
        ScalarType vol = heightfieldExteriorVolume(
            m, scene, fibNormals[i], gridCellSideLengths, epsilon);

        if (vol < bestVolume) {
            bestVolume  = vol;
            bestPlaneId = i;
            bestNormal  = fibNormals[i];

            log.setPercentage(i / static_cast<double>(fibNormals.size()) * 100);
            log << "New best: Plane id: " << i << "/" << fibNormals.size() - 1
                << ", normal: " << fibNormals[i]
                << ", volume: " << std::to_string(vol) << "\n";
        }
    }

    log.setPercentage(100);
    log << "Fibonacci planes tested: " << fibNormals.size() << "\n"
        << "Best plane id: " << bestPlaneId << "\n"
        << "Best plane normal: " << bestNormal << "\n"
        << "Minimum volume: " << bestVolume << "\n";
    log.endTask("Best orientation found.");

    return bestNormal;
}

} // namespace vcl::embree

#endif // VCL_EMBREE_HEIGHTFIELD_EXTERIOR_VOLUME_H
