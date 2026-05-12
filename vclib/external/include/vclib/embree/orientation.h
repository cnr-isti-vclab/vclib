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

struct VolumeResultMeshes
{
    bool       computeMeshes = true;
    EdgeMesh   rayhitMesh;
    TriMesh    exteriorVolumeMesh;
    PointCloud projectedPointsMesh;
    EdgeMesh   bbox2dMesh;
    EdgeMesh   grid2dMesh;
};

namespace detail {

inline static VolumeResultMeshes NO_VOLUME_MESHES = {false};

struct GridChoice
{
    uint   rows  = 1;
    uint   cols  = 1;
    double sideU = 0.0;
    double sideV = 0.0;
};

GridChoice chooseGrid(const Box2d& bbPlane, const Point2d& gridCellSideLengths)
{
    const double sideU = gridCellSideLengths.x();
    const double sideV = gridCellSideLengths.y();

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

template<FaceMeshConcept MeshType>
double processCell(
    uint                i,
    uint                j,
    const GridChoice&   grid,
    const Box2d&        bbPlane,
    const Point3d&      u,
    const Point3d&      v,
    const Point3d&      planePoint,
    const Scene&        scene,
    const Point3d&      n,
    const MeshType&     m,
    double              epsilon,
    VolumeResultMeshes& outMeshes = detail::NO_VOLUME_MESHES)
{
    Point2d cellUV(grid.sideU, grid.sideV);
    auto [cellCenter, cellCorners] =
        computeCellGeometry(i, j, cellUV, bbPlane, u, v, planePoint);

    double volumeAcc = 0.0;

    std::vector<Scene::HitResult> hits =
        scene.facesIntersectedByRay(cellCenter, n, static_cast<float>(epsilon));

    const double cellArea = cellUV.x() * cellUV.y();

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
                    addSegment(outMeshes.rayhitMesh, prevPoint, endPoint);
                    addQuadPrism(
                        outMeshes.exteriorVolumeMesh, cellCorners, prevT, tHit, n);
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
    if (outMeshes.computeMeshes) {
        projectedPoints.reserve(
            std::distance(m.vertices().begin(), m.vertices().end()));
    }

    for (const auto& vert : m.vertices()) {
        const Point3d projected = plane.projectPoint(vert.position());
        if (outMeshes.computeMeshes) {
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

    const double gridMaxU = minU + grid.sideU * grid.cols;
    const double gridMaxV = minV + grid.sideV * grid.rows;

    if (outMeshes.computeMeshes) {
        // projected points
        outMeshes.projectedPointsMesh.addVertices(projectedPoints);

        // out bbox
        const std::array<Point3d, 4> bboxCorners = {
            planePoint + u * minU + v * minV,
            planePoint + u * gridMaxU + v * minV,
            planePoint + u * gridMaxU + v * gridMaxV,
            planePoint + u * minU + v * gridMaxV};

        uint startCornerId = outMeshes.bbox2dMesh.vertexCount();
        for (uint k = 0; k < bboxCorners.size(); ++k) {
            outMeshes.bbox2dMesh.addVertex(bboxCorners[k]);
        }
        outMeshes.bbox2dMesh.addEdge(startCornerId + 0, startCornerId + 1);
        outMeshes.bbox2dMesh.addEdge(startCornerId + 1, startCornerId + 2);
        outMeshes.bbox2dMesh.addEdge(startCornerId + 2, startCornerId + 3);
        outMeshes.bbox2dMesh.addEdge(startCornerId + 3, startCornerId + 0);

        // out grid
        for (uint ii = 0; ii <= grid.cols; ++ii) {
            const double cu = minU + grid.sideU * ii;
            const uint   a =
                outMeshes.grid2dMesh.addVertex(planePoint + u * cu + v * minV);
            const uint b = outMeshes.grid2dMesh.addVertex(
                planePoint + u * cu + v * gridMaxV);
            outMeshes.grid2dMesh.addEdge(a, b);
        }
        for (uint jj = 0; jj <= grid.rows; ++jj) {
            const double cv = minV + grid.sideV * jj;
            const uint   a =
                outMeshes.grid2dMesh.addVertex(planePoint + u * minU + v * cv);
            const uint b = outMeshes.grid2dMesh.addVertex(
                planePoint + u * gridMaxU + v * cv);
            outMeshes.grid2dMesh.addEdge(a, b);
        }
    }

    double totalVolume = 0.0;

    auto processCellClosure = [&](uint idx) {
        uint j = idx / grid.cols;
        uint i = idx % grid.cols;
        return processCell(
            i,
            j,
            grid,
            bbPlane,
            u,
            v,
            planePoint,
            scene,
            n,
            m,
            epsilon,
            outMeshes);
    };

    std::vector<uint> allCells(grid.rows * grid.cols);
    std::iota(allCells.begin(), allCells.end(), 0);

    std::vector<double> cellVolumes(grid.rows * grid.cols, 0.0);

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

#endif // VCL_EMBREE_ORIENTATION_H
