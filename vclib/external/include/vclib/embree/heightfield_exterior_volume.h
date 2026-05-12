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

namespace vcl::embree {

struct VolumeResultMeshes
{
    bool     computeMeshes = true;
    PolyMesh exteriorVolumeMesh;
    EdgeMesh grid2dMesh;
};

namespace detail {

inline static VolumeResultMeshes NO_VOLUME_MESHES = {false};

RegularGrid2<double> makeGrid(
    const Box2d&   bbPlane,
    const Point2d& gridCellSideLengths)
{
    if (gridCellSideLengths.x() <= 0.0 || gridCellSideLengths.y() <= 0.0) {
        Point<uint, 2> oneCell(1u, 1u);
        return RegularGrid2<double>(bbPlane, oneCell);
    }
    return RegularGrid2<double>(bbPlane, gridCellSideLengths);
}

/**
 * @brief A local orthonormal frame anchored to the support plane.
 *
 * Stores the origin of the UV coordinate system (the point on the tangent
 * support plane closest to the world origin along the print direction) and the
 * two in-plane basis vectors.  `toWorld` converts a 2-D grid coordinate back
 * to 3-D world space.
 */
struct PlaneFrame {
    Point3d origin; ///< point on the support plane (origin of the UV frame)
    Point3d u;      ///< first in-plane basis vector
    Point3d v;      ///< second in-plane basis vector

    /// Maps a 2-D grid coordinate to 3-D world space.
    Point3d toWorld(const Point2d& p) const
    {
        return origin + u * p(0) + v * p(1);
    }
};

double accumulateSegment(
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

template<FaceMeshConcept MeshType>
double processCell(
    uint                        i,
    uint                        j,
    const RegularGrid2<double>& grid,
    const PlaneFrame&           frame,
    const Scene&                scene,
    const Point3d&              n,
    const MeshType&             m,
    double                      epsilon,
    VolumeResultMeshes&         outMeshes = detail::NO_VOLUME_MESHES)
{
    RegularGrid2<double>::CellPos cellPos(i, j);

    const auto    c2d        = grid.cellCenter(cellPos);
    const Point3d cellCenter = frame.toWorld(c2d);

    double volumeAcc = 0.0;

    std::vector<Scene::HitResult> hits =
        scene.facesIntersectedByRay(cellCenter, n, static_cast<float>(epsilon));

    const double cellArea = grid.cellVolume();

    Point3d prevPoint  = cellCenter + n * -epsilon;
    double  prevT      = -epsilon;
    Point3d prevNormal = n;
    bool    first      = true;

    int hitsMesh = 0;

    for (const auto& hit : hits) {
        auto [hitFaceId, barCoords, hitTriId, tHit] = hit;

        if (hitFaceId == UINT_NULL) {
            continue;
        }

        const auto& face = m.face(hitFaceId);

        const Point3d endPoint  = cellCenter + n * tHit;
        Point3d       endNormal = face.normal();
        if (endNormal.norm() >= epsilon) {
            endNormal.normalize();
        }

        if (!first && tHit <= prevT) {
            prevPoint  = endPoint;
            prevT      = tHit;
            prevNormal = endNormal;
            continue;
        }

        const double startDot = prevNormal.dot(n);
        const double endDot   = endNormal.dot(n);

        if (endDot < 0.0) {
            if (startDot > 0.0 && hitsMesh == 0) {
                double volSeg =
                    accumulateSegment(cellArea, prevT, tHit, epsilon, first);

                if (outMeshes.computeMeshes && volSeg > 0) {
                    using VMesh = decltype(outMeshes.exteriorVolumeMesh);

                    const double u0  = grid.min()(0) + i * grid.cellLength(0);
                    const double u1  = u0 + grid.cellLength(0);
                    const double v0  = grid.min()(1) + j * grid.cellLength(1);
                    const double v1  = v0 + grid.cellLength(1);
                    VMesh        hex = createHexahedron<VMesh>(
                        Point3d(u0, v0, prevT), Point3d(u1, v1, tHit));
                    for (auto& vert : hex.vertices()) {
                        const Point3d p = vert.position();
                        vert.position() =
                            frame.origin + frame.u * p.x() + frame.v * p.y() +
                            n * p.z();
                    }
                    outMeshes.exteriorVolumeMesh.append(hex);
                }

                volumeAcc += volSeg;
            }
            hitsMesh += 1;
        }
        else {
            hitsMesh -= 1;
        }

        prevPoint  = endPoint;
        prevT      = tHit;
        prevNormal = endNormal;
        first      = false;
    }

    return volumeAcc;
}

} // namespace detail

template<FaceMeshConcept MeshType>
double heightfieldExteriorVolume(
    const MeshType&     m,
    const Scene&        scene,
    const Point2d&      gridCellSideLengths,
    const Point3d&      direction,
    double              epsilon,
    VolumeResultMeshes& outMeshes = detail::NO_VOLUME_MESHES)
{
    using namespace vcl;

    Point3d n = direction;
    if (n.norm() <= epsilon) {
        return std::numeric_limits<double>::infinity();
    }
    n.normalize();

    double minProj = std::numeric_limits<double>::infinity();
    for (const auto& vv : m.vertices()) {
        minProj = std::min(minProj, vv.position().dot(n));
    }
    const Point3d planeOrigin = n * minProj;
    const Planed  plane(planeOrigin, n);

    Point3d u, v;
    n.orthoBase(u, v);
    if (u.norm() <= epsilon || v.norm() <= epsilon) {
        return std::numeric_limits<double>::infinity();
    }
    u.normalize();
    v.normalize();

    const detail::PlaneFrame frame{planeOrigin, u, v};

    Box2d bbPlane;

    for (const auto& vert : m.vertices()) {
        const Point3d projected = plane.projectPoint(vert.position());
        const Point3d rel       = projected - frame.origin;

        const double  pu = rel.dot(frame.u);
        const double  pv = rel.dot(frame.v);
        const Point2d projUV(pu, pv);

        bbPlane.add(projUV);
    }

    if (bbPlane.dim(0) <= epsilon || bbPlane.dim(1) <= epsilon) {
        return std::numeric_limits<double>::infinity();
    }

    const RegularGrid2<double> grid =
        detail::makeGrid(bbPlane, gridCellSideLengths);

    if (outMeshes.computeMeshes) {
        // out grid
        for (uint ii = 0; ii <= grid.cellCount(0); ++ii) {
            const double cu = grid.min()(0) + grid.cellLength(0) * ii;
            const uint   a  = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(Point2d(cu, grid.min()(1))));
            const uint b = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(Point2d(cu, grid.max()(1))));
            outMeshes.grid2dMesh.addEdge(a, b);
            outMeshes.grid2dMesh.vertex(a).normal() = direction;
            outMeshes.grid2dMesh.vertex(b).normal() = direction;
        }
        for (uint jj = 0; jj <= grid.cellCount(1); ++jj) {
            const double cv = grid.min()(1) + grid.cellLength(1) * jj;
            const uint   a  = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(Point2d(grid.min()(0), cv)));
            const uint b = outMeshes.grid2dMesh.addVertex(
                frame.toWorld(Point2d(grid.max()(0), cv)));
            outMeshes.grid2dMesh.addEdge(a, b);
            outMeshes.grid2dMesh.vertex(a).normal() = direction;
            outMeshes.grid2dMesh.vertex(b).normal() = direction;
        }
    }

    double totalVolume = 0.0;

    auto processCellClosure = [&](uint idx) {
        uint i = idx % grid.cellCount(0);
        uint j = idx / grid.cellCount(0);
        return detail::processCell(
            i, j, grid, frame, scene, n, m, epsilon, outMeshes);
    };

    std::vector<uint> allCells(grid.totalCellCount());
    std::iota(allCells.begin(), allCells.end(), 0);

    std::vector<double> cellVolumes(grid.totalCellCount(), 0.0);

    if (outMeshes.computeMeshes) {
        std::for_each(allCells.begin(), allCells.end(), [&](uint idx) {
            cellVolumes[idx] = processCellClosure(idx);
        });
    }
    else {
        vcl::parallelFor(allCells, [&](uint idx) {
            cellVolumes[idx] = processCellClosure(idx);
        });
    }

    totalVolume += std::accumulate(cellVolumes.begin(), cellVolumes.end(), 0.0);

    return totalVolume;
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
 *
 * @return The optimal orientation (e.g., direction vector) that minimizes the
 * required support volume.
 */
template<FaceMeshConcept MeshType, LoggerConcept LogType = NullLogger>
vcl::Point3d findBestOrientationByHeightfieldExteriorVolume(
    const MeshType& m,
    const Point2d&  gridCellSideLengths,
    vcl::uint       nDirections,
    double          epsilon = 1e-6,
    LogType&        log     = nullLogger)
{
    using namespace vcl;

    log.startNewTask(
        0, 100, "Finding best orientation by heightfield exterior volume...");

    requirePerFaceNormal(m);

    Box3d bb = boundingBox(m);

    epsilon *= bb.diagonal();

    // Ray tracing: shoot rays from grid cell centers through the mesh.
    embree::Scene scene(m);

    std::vector<Point3d> fibNormals =
        sphericalFibonacciPointSet<Point3d>(nDirections);

    if (fibNormals.empty()) {
        throw std::runtime_error(
            "Failed to generate Fibonacci normals for orientation search.");
    }

    uint    bestPlaneId = 0;
    double  bestVolume  = std::numeric_limits<double>::infinity();
    Point3d bestNormal  = fibNormals.front();

    for (uint i = 0; i < fibNormals.size(); ++i) {
        double vol = heightfieldExteriorVolume(
            m,
            scene,
            gridCellSideLengths,
            fibNormals[i],
            epsilon,
            detail::NO_VOLUME_MESHES);

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
