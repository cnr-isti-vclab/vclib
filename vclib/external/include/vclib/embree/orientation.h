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

#ifndef VCL_EMBREE_ORIENTATION_H
#define VCL_EMBREE_ORIENTATION_H

#include "scene.h"

#include <vclib/algorithms/core.h>
#include <vclib/meshes.h>

namespace vcl::embree {

namespace detail {

struct GridChoice
{
    uint   rows  = 1;
    uint   cols  = 1;
    double sideU = 0.0;
    double sideV = 0.0;
};

struct HitEvent
{
    Point3d point;
    double  t      = 0.0;
    Point3d normal = Point3d(0, 0, 0);
};

struct PlaneEvalStats
{
    double minU  = 0.0;
    double minV  = 0.0;
    double maxU  = 0.0;
    double maxV  = 0.0;
    uint   rows  = 0;
    uint   cols  = 0;
    uint   cells = 0;
};

GridChoice chooseGrid(
    const Box2d&               bbPlane,
    const std::vector<double>& gridCellSideLengths)
{
    if (bbPlane.dim(0) <= 0.0 || bbPlane.dim(1) <= 0.0) {
        return {1, 1, bbPlane.dim(0), bbPlane.dim(1)};
    }

    const double sideU = (gridCellSideLengths.size() >= 1) ?
                             gridCellSideLengths[0] :
                             bbPlane.dim(0);
    const double sideV =
        (gridCellSideLengths.size() >= 2) ? gridCellSideLengths[1] : sideU;

    if (sideU <= 0.0 || sideV <= 0.0) {
        return {1, 1, bbPlane.dim(0), bbPlane.dim(1)};
    }

    const uint cols =
        static_cast<uint>(std::max(1.0, std::ceil(bbPlane.dim(0) / sideU)));
    const uint rows =
        static_cast<uint>(std::max(1.0, std::ceil(bbPlane.dim(1) / sideV)));

    return {rows, cols, sideU, sideV};
}

void addSegment(EdgeMesh& em, const Point3d& a, const Point3d& b)
{
    const uint va = em.addVertex(a);
    const uint vb = em.addVertex(b);
    em.addEdge(va, vb);
}

void addQuadPrism(
    TriMesh&                      tm,
    const std::array<Point3d, 4>& baseCorners,
    double                        startOffset,
    double                        endOffset,
    const Point3d&                dir)
{
    std::array<Point3d, 4> b;
    std::array<Point3d, 4> t;
    for (uint k = 0; k < 4; ++k) {
        b[k] = baseCorners[k] + dir * startOffset;
        t[k] = baseCorners[k] + dir * endOffset;
    }

    std::array<uint, 8> ids;
    for (uint k = 0; k < 4; ++k) {
        ids[k + 0] = tm.addVertex(b[k]);
        ids[k + 4] = tm.addVertex(t[k]);
    }

    // Bottom
    tm.addFace(ids[0], ids[2], ids[1]);
    tm.addFace(ids[0], ids[3], ids[2]);
    tm.addFace(ids[4], ids[5], ids[6]);
    tm.addFace(ids[4], ids[6], ids[7]);

    // Sides
    tm.addFace(ids[0], ids[1], ids[5]);
    tm.addFace(ids[0], ids[5], ids[4]);
    tm.addFace(ids[1], ids[2], ids[6]);
    tm.addFace(ids[1], ids[6], ids[5]);
    tm.addFace(ids[2], ids[3], ids[7]);
    tm.addFace(ids[2], ids[7], ids[6]);
    tm.addFace(ids[3], ids[0], ids[4]);
    tm.addFace(ids[3], ids[4], ids[7]);
}

template<FaceMeshConcept MeshType>
std::vector<HitEvent> collectHits(
    const Scene&    scene,
    const Point3d&  rayOrigin,
    const Point3d&  rayDir,
    const MeshType& m,
    double          epsilon)
{
    std::vector<HitEvent>         hitEvents;
    std::vector<Scene::HitResult> hits = scene.facesIntersectedByRay(
        rayOrigin, rayDir, static_cast<float>(epsilon));
    hitEvents.reserve(hits.size());

    for (const auto& h : hits) {
        auto [hitFaceId, barCoords, hitTriId, tHit] = h;

        if (hitFaceId == UINT_NULL) {
            continue;
        }

        const auto& face = m.face(hitFaceId);

        const Point3d hitPoint  = rayOrigin + rayDir * tHit;
        Point3d       triNormal = face.normal();
        if (triNormal.norm() >= epsilon) {
            triNormal.normalize();
        }

        hitEvents.push_back({hitPoint, tHit, triNormal});
    }

    return hitEvents;
}

auto computeCellGeometry(
    uint           i,
    uint           j,
    const Point2d& cellUV,
    const Box2d&   bbPlane,
    const Point3d& u,
    const Point3d& v,
    const Point3d& planePoint)
{
    const double u0 = bbPlane.min().x() + i * cellUV.x();
    const double u1 = u0 + cellUV.x();
    const double v0 = bbPlane.min().y() + j * cellUV.y();
    const double v1 = v0 + cellUV.y();

    std::array<Point3d, 4> cellCorners = {
        planePoint + u * u0 + v * v0,
        planePoint + u * u1 + v * v0,
        planePoint + u * u1 + v * v1,
        planePoint + u * u0 + v * v1};

    const double centerU    = bbPlane.min().x() + (i + 0.5) * cellUV.x();
    const double centerV    = bbPlane.min().y() + (j + 0.5) * cellUV.y();
    Point3d      cellCenter = planePoint + u * centerU + v * centerV;

    return std::make_pair(cellCenter, cellCorners);
}

} // namespace detail

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
template<FaceMeshConcept MeshType>
vcl::Point3d findBestOrientationByHeightfieldExteriorVolume(
    const MeshType&            m,
    const std::vector<double>& gridCellSideLengths,
    vcl::uint                  nDirections,
    double                     epsilon = 1e-6,
    bool                       debug   = false)
{
    using namespace vcl;

    requirePerFaceNormal(m);

    Box3d bb = boundingBox(m);

    epsilon *= bb.diagonal();

    // Ray tracing: shoot rays from grid cell centers through the mesh.
    embree::Scene scene(m);

    auto evaluatePlane = [&](const Point3d&          rawPlaneNormal,
                             bool                    collectDebug,
                             EdgeMesh*               outRayhitMesh,
                             TriMesh*                outPrismsMesh,
                             EdgeMesh*               outProjectedPointsMesh,
                             EdgeMesh*               outBbox2dMesh,
                             EdgeMesh*               outGrid2dMesh,
                             detail::PlaneEvalStats* outStats) {
        const bool collectDebugEnabled = debug && collectDebug;

        Point3d n = rawPlaneNormal;
        if (n.norm() <= epsilon) {
            return std::numeric_limits<double>::infinity();
        }
        n.normalize();

        double minProj = std::numeric_limits<double>::infinity();
        for (const auto& vv : m.vertices()) {
            minProj = std::min(minProj, vv.position().dot(n));
        }
        const Point3d planePoint = n * minProj;
        const Planed  plane(planePoint, n);

        Point3d u, v;
        n.orthoBase(u, v);
        if (u.norm() <= epsilon || v.norm() <= epsilon) {
            return std::numeric_limits<double>::infinity();
        }
        u.normalize();
        v.normalize();

        double minU = std::numeric_limits<double>::infinity();
        double minV = std::numeric_limits<double>::infinity();
        double maxU = -std::numeric_limits<double>::infinity();
        double maxV = -std::numeric_limits<double>::infinity();
        Box2d  bbPlane;

        std::vector<Point3d> projectedPoints;
        if (collectDebugEnabled && outProjectedPointsMesh) {
            projectedPoints.reserve(
                std::distance(m.vertices().begin(), m.vertices().end()));
        }

        for (const auto& vert : m.vertices()) {
            const Point3d projected = plane.projectPoint(vert.position());
            if (collectDebugEnabled && outProjectedPointsMesh) {
                projectedPoints.push_back(projected);
            }
            const Point3d rel = projected - planePoint;

            const double  pu = rel.dot(u);
            const double  pv = rel.dot(v);
            const Point2d projUV(pu, pv);

            minU = std::min(minU, pu);
            minV = std::min(minV, pv);
            maxU = std::max(maxU, pu);
            maxV = std::max(maxV, pv);
            bbPlane.add(projUV);
        }

        if (bbPlane.dim(0) <= epsilon || bbPlane.dim(1) <= epsilon) {
            return std::numeric_limits<double>::infinity();
        }

        const detail::GridChoice grid =
            detail::chooseGrid(bbPlane, gridCellSideLengths);
        if (outStats) {
            outStats->minU  = bbPlane.min().x();
            outStats->minV  = minV;
            outStats->maxU  = maxU;
            outStats->maxV  = maxV;
            outStats->rows  = grid.rows;
            outStats->cols  = grid.cols;
            outStats->cells = grid.rows * grid.cols;
        }

        const double gridMaxU = minU + grid.sideU * grid.cols;
        const double gridMaxV = minV + grid.sideV * grid.rows;

        if (collectDebugEnabled && outProjectedPointsMesh) {
            outProjectedPointsMesh->reserveVertices(projectedPoints.size());
            for (const Point3d& p : projectedPoints) {
                outProjectedPointsMesh->addVertex(p);
            }
        }

        if (collectDebugEnabled && outBbox2dMesh) {
            const std::array<Point3d, 4> bboxCorners = {
                planePoint + u * minU + v * minV,
                planePoint + u * gridMaxU + v * minV,
                planePoint + u * gridMaxU + v * gridMaxV,
                planePoint + u * minU + v * gridMaxV};

            std::array<uint, 4> cornerIds;
            for (uint k = 0; k < bboxCorners.size(); ++k) {
                cornerIds[k] = outBbox2dMesh->addVertex(bboxCorners[k]);
            }
            outBbox2dMesh->addEdge(cornerIds[0], cornerIds[1]);
            outBbox2dMesh->addEdge(cornerIds[1], cornerIds[2]);
            outBbox2dMesh->addEdge(cornerIds[2], cornerIds[3]);
            outBbox2dMesh->addEdge(cornerIds[3], cornerIds[0]);
        }

        if (collectDebugEnabled && outGrid2dMesh) {
            for (uint ii = 0; ii <= grid.cols; ++ii) {
                const double cu = minU + grid.sideU * ii;
                const uint   a =
                    outGrid2dMesh->addVertex(planePoint + u * cu + v * minV);
                const uint b = outGrid2dMesh->addVertex(
                    planePoint + u * cu + v * gridMaxV);
                outGrid2dMesh->addEdge(a, b);
            }
            for (uint jj = 0; jj <= grid.rows; ++jj) {
                const double cv = minV + grid.sideV * jj;
                const uint   a =
                    outGrid2dMesh->addVertex(planePoint + u * minU + v * cv);
                const uint b = outGrid2dMesh->addVertex(
                    planePoint + u * gridMaxU + v * cv);
                outGrid2dMesh->addEdge(a, b);
            }
        }

        double totalVolume = 0.0;

        auto accumulateSegment = [&](double                        localVolume,
                                     const Point3d&                segStart,
                                     const Point3d&                segEnd,
                                     const std::array<Point3d, 4>& cellCorners,
                                     double                        cellArea,
                                     double                        startD,
                                     double                        endD,
                                     bool first = false) {
            const double segLength   = endD - startD;
            const bool   validLength = first || (segLength >= epsilon);
            if (validLength) {
                const double segVolume = cellArea * segLength;
                localVolume += segVolume;
            }

            if (collectDebugEnabled && outRayhitMesh && validLength) {
                detail::addSegment(*outRayhitMesh, segStart, segEnd);
            }
            if (collectDebugEnabled && outPrismsMesh && validLength) {
                detail::addQuadPrism(
                    *outPrismsMesh, cellCorners, startD, endD, n);
            }

            return localVolume;
        };

        auto processCell = [&](uint i, uint j) {
            Point2d cellUV(grid.sideU, grid.sideV);
            auto [cellCenter, cellCorners] = detail::computeCellGeometry(
                i, j, cellUV, bbPlane, u, v, planePoint);

            double volumeAcc = 0.0;

            std::vector<detail::HitEvent> hitEvents =
                detail::collectHits(scene, cellCenter, n, m, epsilon);

            const double cellArea = cellUV.x() * cellUV.y();

            if (!hitEvents.empty()) {
                const Point3d segStart = cellCenter + n * -epsilon;
                const Point3d segEnd   = hitEvents[0].point;
                const double  startD   = -epsilon;
                const double  endD     = hitEvents[0].t;
                volumeAcc              = accumulateSegment(
                    volumeAcc,
                    segStart,
                    segEnd,
                    cellCorners,
                    cellArea,
                    startD,
                    endD,
                    true);
            }

            int hitsMesh = 1;
            for (int h = 0; h + 1 < (int) hitEvents.size(); h += 1) {
                const Point3d segStart = hitEvents[h].point;
                const Point3d segEnd   = hitEvents[h + 1].point;
                const double  startD   = hitEvents[h].t;
                const double  endD     = hitEvents[h + 1].t;
                if (endD <= startD) {
                    continue;
                }

                const Point3d& startNormal = hitEvents[h].normal;
                const Point3d& endNormal   = hitEvents[h + 1].normal;
                const double   startDot    = startNormal.dot(n);
                const double   endDot      = endNormal.dot(n);

                if (endDot < 0.0) {
                    if ((startDot > 0.0) && (hitsMesh == 0)) {
                        volumeAcc = accumulateSegment(
                            volumeAcc,
                            segStart,
                            segEnd,
                            cellCorners,
                            cellArea,
                            startD,
                            endD);
                    }
                    hitsMesh += 1;
                }
                else {
                    hitsMesh -= 1;
                }
            }

            return volumeAcc;
        };

        if (!collectDebugEnabled) {
            std::vector<uint> allCells(grid.rows * grid.cols);
            std::iota(allCells.begin(), allCells.end(), 0);

            std::vector<double> cellVolumes(grid.rows * grid.cols, 0.0);
            vcl::parallelFor(allCells, [&](uint idx) {
                uint j           = idx / grid.cols;
                uint i           = idx % grid.cols;
                cellVolumes[idx] = processCell(i, j);
            });

            totalVolume +=
                std::accumulate(cellVolumes.begin(), cellVolumes.end(), 0.0);
        }
        else {
            std::vector<uint> allCells(grid.rows * grid.cols);
            std::iota(allCells.begin(), allCells.end(), 0);

            totalVolume += std::accumulate(
                allCells.begin(),
                allCells.end(),
                0.0,
                [&](double acc, uint idx) {
                    const uint j = idx / grid.cols;
                    const uint i = idx % grid.cols;
                    return acc + processCell(i, j);
                });
        }

        return totalVolume;
    };

    std::vector<Point3d> fibNormals =
        sphericalFibonacciPointSet<Point3d>(nDirections);
    if (fibNormals.empty()) {
        std::cerr << "No Fibonacci planes generated.\n";
        return Point3d(0, 0, 0);
    }

    uint    bestPlaneId = 0;
    double  bestVolume  = std::numeric_limits<double>::infinity();
    Point3d bestNormal  = fibNormals.front();

    for (uint i = 0; i < fibNormals.size(); ++i) {
        detail::PlaneEvalStats stats;
        double                 vol = evaluatePlane(
            fibNormals[i],
            false,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            nullptr,
            &stats);

        if (debug) {
            std::cout << "Plane id: " << i << "/" << (fibNormals.size() - 1)
                      << ", normal: " << fibNormals[i] << ", volume: " << vol
                      << ", bbox2d(u,v): [(" << stats.minU << ", " << stats.minV
                      << ") -> (" << stats.maxU << ", " << stats.maxV << ")]"
                      << ", cells: " << stats.cells << " (" << stats.cols << "x"
                      << stats.rows << ")";
        }
        if (vol < bestVolume) {
            bestVolume  = vol;
            bestPlaneId = i;
            bestNormal  = fibNormals[i];
            if (debug) {
                std::cout << " [new best]";
            }
        }
        if (debug) {
            std::cout << "\n";
        }
    }

    EdgeMesh bestRayhitMesh;
    TriMesh  bestPrismsMesh;
    EdgeMesh bestProjectedPointsMesh;
    EdgeMesh bestBbox2dMesh;
    EdgeMesh bestGrid2dMesh;

    bestVolume = evaluatePlane(
        bestNormal,
        true,
        &bestRayhitMesh,
        &bestPrismsMesh,
        &bestProjectedPointsMesh,
        &bestBbox2dMesh,
        &bestGrid2dMesh,
        nullptr);

    // if (debug) {
    //     saveMesh(
    //         bestRayhitMesh,
    //         VCLIB_EXTERNAL_RESULTS_PATH "/777_plane_beam_rayhits.ply");
    //     saveMesh(
    //         bestPrismsMesh,
    //         VCLIB_EXTERNAL_RESULTS_PATH "/777_plane_beam_prisms.ply");
    //     saveMesh(
    //         bestProjectedPointsMesh,
    //         VCLIB_EXTERNAL_RESULTS_PATH
    //         "/777_plane_beam_projected_points.ply");
    //     saveMesh(
    //         bestBbox2dMesh,
    //         VCLIB_EXTERNAL_RESULTS_PATH "/777_plane_beam_bbox2d.ply");
    //     saveMesh(
    //         bestGrid2dMesh,
    //         VCLIB_EXTERNAL_RESULTS_PATH "/777_plane_beam_grid2d.ply");
    // }

    auto saveDebugMeshes = [&](const std::string& suffix,
                               const Point3d&     normal) {
        EdgeMesh rayhitMesh;
        TriMesh  prismsMesh;
        EdgeMesh projectedPointsMesh;
        EdgeMesh bbox2dMesh;
        EdgeMesh grid2dMesh;

        evaluatePlane(
            normal,
            true,
            &rayhitMesh,
            &prismsMesh,
            &projectedPointsMesh,
            &bbox2dMesh,
            &grid2dMesh,
            nullptr);

        // const std::string base = std::string(VCLIB_EXTERNAL_RESULTS_PATH) +
        //                          "/777_plane_beam_" + suffix;
        // saveMesh(rayhitMesh, base + "_rayhits.ply");
        // saveMesh(prismsMesh, base + "_prisms.ply");
        // saveMesh(projectedPointsMesh, base + "_projected_points.ply");
        // saveMesh(bbox2dMesh, base + "_bbox2d.ply");
        // saveMesh(grid2dMesh, base + "_grid2d.ply");
    };

    if (debug) {
        std::cout << "Fibonacci planes tested: " << fibNormals.size() << "\n"
                  << "Best plane id: " << bestPlaneId << "\n"
                  << "Best plane normal: " << bestNormal << "\n"
                  << "Grid cell side lengths (u, v): "
                  << ((gridCellSideLengths.size() >= 1) ?
                          gridCellSideLengths[0] :
                          0.0)
                  << ", "
                  << ((gridCellSideLengths.size() >= 2) ?
                          gridCellSideLengths[1] :
                          ((gridCellSideLengths.size() >= 1) ?
                               gridCellSideLengths[0] :
                               0.0))
                  << "\n"
                  << "Minimum volume: " << bestVolume << "\n"
            // << "Saved debug meshes:\n"
            // << " - " << VCLIB_EXTERNAL_RESULTS_PATH
            // << "/777_plane_beam_prisms.ply\n"
            // << " - " << VCLIB_EXTERNAL_RESULTS_PATH
            // << "/777_plane_beam_projected_points.ply\n"
            // << " - " << VCLIB_EXTERNAL_RESULTS_PATH
            // << "/777_plane_beam_bbox2d.ply\n"
            // << " - " << VCLIB_EXTERNAL_RESULTS_PATH
            // << "/777_plane_beam_grid2d.ply\n"
            // << " - " << VCLIB_EXTERNAL_RESULTS_PATH
            // << "/777_plane_beam_rayhits.ply\n"
            ;
    }

    return bestNormal;
}

} // namespace vcl::embree

#endif // VCL_EMBREE_ORIENTATION_H
