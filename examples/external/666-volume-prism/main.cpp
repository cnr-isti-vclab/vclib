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

#include <vclib/embree/scene.h>
#include <vclib/algorithms/core/fibonacci.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
//#include <vclib/qt/mesh_viewer.h>
//#include <vclib/render/drawable/drawable_mesh.h>

//#include <QApplication>
#include <array>
#include <chrono>
#include <cmath>
#include <limits>
#include <iostream>
#include <tuple>
#include <vector>

int main(int argc, char** argv)
{
    using namespace vcl;

    //QApplication app(argc, argv);

    // Runtime/configuration knobs for search, debug geometry and visualization.
    constexpr bool   VISUAL             = false;
    constexpr uint   NUM_PLANES         = 100;
    constexpr bool   VERBOSE_TRIANGLES  = false;

    PolyMesh m = loadMesh<PolyMesh>(VCLIB_EXAMPLE_MESHES_PATH "/brain_test.ply");
    embree::Scene scene(m);

    updateBoundingBox(m);

    const double EPS = 1e-6 * m.boundingBox().diagonal();

    EdgeMesh bestRaysMesh;
    TriMesh  bestPrismsMesh;
    TriMesh  bestPlaneMesh;

    auto addSegment = [](EdgeMesh& em, const Point3d& a, const Point3d& b) {
        uint va = em.addVertex(a);
        uint vb = em.addVertex(b);
        em.addEdge(va, vb);
    };

    auto addRegularPrism = [&](TriMesh&         tm,
                               const Point3d&   p0,
                               const Point3d&   p1,
                               const Point3d&   p2,
                               const Point3d&   n,
                               double           projectionOffset,
                               double           height) {
        const Planed projPlane(n * projectionOffset, n);
        Point3d      b0 = projPlane.projectPoint(p0);
        Point3d      b1 = projPlane.projectPoint(p1);
        Point3d      b2 = projPlane.projectPoint(p2);

        const Point3d offsetVec = n * height;
        const Point3d t0        = b0 + offsetVec;
        const Point3d t1        = b1 + offsetVec;
        const Point3d t2        = b2 + offsetVec;

        const std::array<Point3d, 6> prismVerts = {b0, b1, b2, t0, t1, t2};
        std::array<uint, 6>          prismIds;
        for (uint i = 0; i < prismVerts.size(); ++i) {
            prismIds[i] = tm.addVertex(prismVerts[i]);
        }

        tm.addFace(prismIds[3], prismIds[5], prismIds[4]); // t0, t2, t1
        tm.addFace(prismIds[0], prismIds[1], prismIds[2]); // b0, b1, b2

        tm.addFace(prismIds[0], prismIds[4], prismIds[1]); // b0, t1, b1
        tm.addFace(prismIds[0], prismIds[3], prismIds[4]); // b0, t0, t1

        tm.addFace(prismIds[1], prismIds[5], prismIds[2]); // b1, t2, b2
        tm.addFace(prismIds[1], prismIds[4], prismIds[5]); // b1, t1, t2

        tm.addFace(prismIds[2], prismIds[3], prismIds[0]); // b2, t0, b0
        tm.addFace(prismIds[2], prismIds[5], prismIds[3]); // b2, t2, t0
    };

    struct SourceTriangle
    {
        uint    sourceFaceId;
        Point3d p0;
        Point3d p1;
        Point3d p2;
        Point3d centroid;
        Point3d normalUnit;
        double  area;
    };

    std::vector<std::vector<uint>> faceTriangulations;

    std::vector<SourceTriangle> sourceTriangles;

    for (const auto& face : m.faces()) {
        const uint faceId = face.index();
        if (faceId >= faceTriangulations.size()) {
            faceTriangulations.resize(faceId + 1);
        }

        auto& triangulation = faceTriangulations[faceId];
        triangulation       = earCut(face);

        if (triangulation.size() < 3) {
            continue;
        }

        for (uint i = 0; i + 2 < triangulation.size(); i += 3) {
            const Point3d& p0 = face.vertex(triangulation[i + 0])->position();
            const Point3d& p1 = face.vertex(triangulation[i + 1])->position();
            const Point3d& p2 = face.vertex(triangulation[i + 2])->position();

            const double area = Triangle<Point3d>::area(p0, p1, p2);
            if (area <= EPS) {
                continue;
            }

            Point3d triNormalUnit = Triangle<Point3d>::normal(p0, p1, p2);
            if (triNormalUnit.norm() <= EPS) {
                continue;
            }
            triNormalUnit.normalize();

            sourceTriangles.push_back(
                {faceId, p0, p1, p2, (p0 + p1 + p2) / 3.0, triNormalUnit, area});
        }
    }

    // Evaluate one candidate plane orientation and return the accumulated volume.
    auto evaluatePlane = [&](const Point3d& rawPlaneNormal,
                             bool           collectDebug,
                             EdgeMesh*      raysMesh,
                             TriMesh*       prismsMesh,
                             uint&          outTriCount) -> double {
        Point3d planeNormal = rawPlaneNormal;
        if (planeNormal.norm() <= EPS) {
            outTriCount = 0;
            return std::numeric_limits<double>::infinity();
        }
        planeNormal.normalize();

        // Build a support plane: n·x = min_v(n·v). This keeps the plane tangent
        // to the mesh instead of slicing through it.
        double minProj = std::numeric_limits<double>::infinity();
        for (const auto& v : m.vertices()) {
            minProj = std::min(minProj, v.position().dot(planeNormal));
        }
        const Point3d planePoint = planeNormal * minProj;
        const Planed  plane(planePoint, planeNormal);

        uint   localTriId  = 0;
        double localVolume = 0.0;

#ifdef VCL_EMBREE_FORCE_CHUNK_16
        struct RayCandidate
        {
            uint    sourceFaceId;
            Point3d p0;
            Point3d p1;
            Point3d p2;
            Point3d triCentroid;
            Point3d rayDirection;
            Point3d rayOrigin;
            double  area;
            double  dot;
        };

        std::vector<RayCandidate> candidates;
        std::vector<Point3d>      rayOrigins;
        std::vector<Point3d>      rayDirections;
        candidates.reserve(sourceTriangles.size());
        rayOrigins.reserve(sourceTriangles.size());
        rayDirections.reserve(sourceTriangles.size());

        for (const auto& tri : sourceTriangles) {
            const double dot = tri.normalUnit.dot(planeNormal);
            // Keep only triangles with opposite orientation w.r.t. plane normal.
            if (dot >= 0.0) {
                continue;
            }

            const Point3d rayDirection = -planeNormal;
            const Point3d rayOrigin    = tri.centroid + rayDirection * EPS;

            candidates.push_back(
                {tri.sourceFaceId,
                 tri.p0,
                 tri.p1,
                 tri.p2,
                 tri.centroid,
                 rayDirection,
                 rayOrigin,
                 tri.area,
                 dot});
            rayOrigins.push_back(rayOrigin);
            rayDirections.push_back(rayDirection);
        }

        std::vector<embree::Scene::HitResult> hits =
            scene.firstFaceIntersectedByRays(rayOrigins, rayDirections, EPS);

        std::vector<std::size_t> retryMap;
        std::vector<Point3d>     retryOrigins;
        std::vector<Point3d>     retryDirections;

        for (std::size_t i = 0; i < candidates.size(); ++i) {
            auto [hitFaceId, barCoords, hitTriId, _] = hits[i];
            if (hitFaceId == candidates[i].sourceFaceId) {
                retryMap.push_back(i);
                retryOrigins.push_back(
                    candidates[i].triCentroid +
                    candidates[i].rayDirection * (100.0 * EPS));
                retryDirections.push_back(candidates[i].rayDirection);
            }
        }

        if (!retryMap.empty()) {
            std::vector<embree::Scene::HitResult> retryHits =
                scene.firstFaceIntersectedByRays(
                    retryOrigins, retryDirections, EPS);

            for (std::size_t i = 0; i < retryMap.size(); ++i) {
                const std::size_t idx = retryMap[i];
                hits[idx]             = retryHits[i];
                candidates[idx].rayOrigin = retryOrigins[i];
            }
        }

        for (std::size_t i = 0; i < candidates.size(); ++i) {
            const auto& c = candidates[i];
            auto [hitFaceId, barCoords, hitTriId, _] = hits[i];

            Point3d targetPoint;
            bool    hitMesh = hitFaceId != UINT_NULL;

            if (hitMesh && hitFaceId < faceTriangulations.size()) {
                const auto& hitFace = m.face(hitFaceId);
                const auto& hitTris = faceTriangulations[hitFaceId];
                const uint  base    = hitTriId * 3;

                if (base + 2 < hitTris.size()) {
                    const Point3d& q0 =
                        hitFace.vertex(hitTris[base + 0])->position();
                    const Point3d& q1 =
                        hitFace.vertex(hitTris[base + 1])->position();
                    const Point3d& q2 =
                        hitFace.vertex(hitTris[base + 2])->position();

                    Point3d hitTriNormal = Triangle<Point3d>::normal(q0, q1, q2);
                    if (hitTriNormal.norm() <= EPS) {
                        continue;
                    }
                    hitTriNormal.normalize();

                    // Discard rays hitting triangles whose normal points
                    // along the ray direction.
                    if (hitTriNormal.dot(c.rayDirection) > 0.0) {
                        continue;
                    }

                    targetPoint =
                        q0 * barCoords.x() + q1 * barCoords.y() + q2 * barCoords.z();
                }
                else {
                    hitMesh = false;
                }
            }
            else {
                hitMesh = false;
            }

            if (!hitMesh) {
                const double denom = planeNormal.dot(c.rayDirection);
                if (std::abs(denom) <= EPS) {
                    continue;
                }

                const double t =
                    (plane.offset() - planeNormal.dot(c.rayOrigin)) / denom;
                if (t < 0.0) {
                    continue;
                }
                targetPoint = c.rayOrigin + c.rayDirection * t;
            }

            if ((targetPoint - c.triCentroid).norm() <= EPS) {
                continue;
            }

            // Prism contribution for this triangle.
            const double height = (targetPoint - c.triCentroid).norm();
            const double volume = c.area * -c.dot * height;
            localVolume += volume;

            if (collectDebug && raysMesh && prismsMesh) {
                addSegment(*raysMesh, c.triCentroid, targetPoint);

                const double projectionOffset =
                    hitMesh ? planeNormal.dot(targetPoint) : plane.offset();
                addRegularPrism(*prismsMesh,
                                c.p0,
                                c.p1,
                                c.p2,
                                planeNormal,
                                projectionOffset,
                                height);

                if (VERBOSE_TRIANGLES) {
                    std::cout << "  Ray result: "
                              << (hitMesh ? "hit mesh" : "hit plane")
                              << ", area = " << c.area << ", dot = " << c.dot
                              << ", height = " << height << ", V = "
                              << volume << "\n";
                }
            }

            ++localTriId;
        }
#else
        for (const auto& tri : sourceTriangles) {
            const double dot = tri.normalUnit.dot(planeNormal);
            // Keep only triangles with opposite orientation w.r.t. plane normal.
            if (dot >= 0.0) {
                continue;
            }

            const Point3d rayDirection = -planeNormal;
            Point3d       rayOrigin    = tri.centroid + rayDirection * EPS;

            auto [hitFaceId, barCoords, hitTriId] =
                scene.firstFaceIntersectedByRay(rayOrigin, rayDirection, EPS);

            if (hitFaceId == tri.sourceFaceId) {
                rayOrigin = tri.centroid + rayDirection * (100.0 * EPS);
                std::tie(hitFaceId, barCoords, hitTriId) =
                    scene.firstFaceIntersectedByRay(rayOrigin, rayDirection, EPS);
            }

            Point3d targetPoint;
            bool    hitMesh = hitFaceId != UINT_NULL;

            if (hitMesh && hitFaceId < faceTriangulations.size()) {
                const auto& hitFace = m.face(hitFaceId);
                const auto& hitTris = faceTriangulations[hitFaceId];
                const uint  base    = hitTriId * 3;

                if (base + 2 < hitTris.size()) {
                    const Point3d& q0 =
                        hitFace.vertex(hitTris[base + 0])->position();
                    const Point3d& q1 =
                        hitFace.vertex(hitTris[base + 1])->position();
                    const Point3d& q2 =
                        hitFace.vertex(hitTris[base + 2])->position();

                    Point3d hitTriNormal = Triangle<Point3d>::normal(q0, q1, q2);
                    if (hitTriNormal.norm() <= EPS) {
                        continue;
                    }
                    hitTriNormal.normalize();

                    // Discard rays hitting triangles whose normal points
                    // along the ray direction.
                    if (hitTriNormal.dot(rayDirection) > EPS) {
                        continue;
                    }

                    targetPoint =
                        q0 * barCoords.x() + q1 * barCoords.y() + q2 * barCoords.z();
                }
                else {
                    hitMesh = false;
                }
            }
            else {
                hitMesh = false;
            }

            if (!hitMesh) {
                const double denom = planeNormal.dot(rayDirection);
                if (std::abs(denom) <= EPS) {
                    continue;
                }

                const double t =
                    (plane.offset() - planeNormal.dot(rayOrigin)) / denom;
                if (t < 0.0) {
                    continue;
                }
                targetPoint = rayOrigin + rayDirection * t;
            }

            if ((targetPoint - tri.centroid).norm() <= EPS) {
                continue;
            }

            // Prism contribution for this triangle.
            const double height = (targetPoint - tri.centroid).norm();
            const double volume = tri.area * -dot * height;
            localVolume += volume;

            if (collectDebug && raysMesh && prismsMesh) {
                addSegment(*raysMesh, tri.centroid, targetPoint);

                const double projectionOffset =
                    hitMesh ? planeNormal.dot(targetPoint) : plane.offset();
                addRegularPrism(
                    *prismsMesh,
                    tri.p0,
                    tri.p1,
                    tri.p2,
                    planeNormal,
                    projectionOffset,
                    height);

                if (VERBOSE_TRIANGLES) {
                    std::cout << "  Ray result: "
                              << (hitMesh ? "hit mesh" : "hit plane")
                              << ", area = " << tri.area << ", dot = " << dot
                              << ", height = " << height << ", V = "
                              << volume << "\n";
                }
            }

            ++localTriId;
        }
#endif

        outTriCount = localTriId;
        std::cout << "Plane normal: " << planeNormal
                  << ", volume: " << localVolume
                  << ", triangles processed: " << localTriId << "\n";
        return localVolume;
    };

    // Generate evenly distributed normals on the sphere (candidate planes).
    std::vector<Point3d> fibNormals = sphericalFibonacciPointSet<Point3d>(NUM_PLANES);
    if (fibNormals.empty()) {
        std::cerr << "No Fibonacci planes generated.\n";
        return 1;
    }

    uint    bestPlaneId    = 0;
    double  bestVolume     = std::numeric_limits<double>::infinity();
    Point3d bestNormal     = fibNormals.front();
    uint    bestTriCount   = 0;

    const auto totalRuntimeStart = std::chrono::steady_clock::now();

    // First pass: evaluate all planes and keep the one with minimum volume.
    for (uint i = 0; i < fibNormals.size(); ++i) {
        uint   tmpTriCount = 0;
        double vol =
            evaluatePlane(fibNormals[i], false, nullptr, nullptr, tmpTriCount);

        if (vol < bestVolume) {
            bestVolume   = vol;
            bestPlaneId  = i;
            bestNormal   = fibNormals[i];
            bestTriCount = tmpTriCount;
        }
    }

    // Second pass on the best plane: collect full debug geometry.
    uint debugTriCount = 0;
    bestVolume = evaluatePlane(
        bestNormal,
        true,
        &bestRaysMesh,
        &bestPrismsMesh,
        debugTriCount);

    const auto totalRuntimeEnd = std::chrono::steady_clock::now();
    const auto totalRuntimeMs  = std::chrono::duration_cast<std::chrono::milliseconds>(
        totalRuntimeEnd - totalRuntimeStart);

    {
        // Build a finite quad for Qt rendering of the best support plane.
        Point3d n = bestNormal;
        n.normalize();

        double minProj = std::numeric_limits<double>::infinity();
        uint   minVid  = UINT_NULL;
        for (const auto& vtx : m.vertices()) {
            const double proj = vtx.position().dot(n);
            if (proj < minProj) {
                minProj = proj;
                minVid  = vtx.index();
            }
        }

        Point3d u, v;
        n.orthoBase(u, v);
        if (u.norm() > EPS) {
            u.normalize();
        }
        if (v.norm() > EPS) {
            v.normalize();
        }

        const double halfSize = std::max(0.05, m.boundingBox().diagonal() * 0.75);
        const Point3d c = (minVid != UINT_NULL) ? m.vertex(minVid).position() :
                             (n * minProj);

        const uint v0 = bestPlaneMesh.addVertex(c - u * halfSize - v * halfSize);
        const uint v1 = bestPlaneMesh.addVertex(c + u * halfSize - v * halfSize);
        const uint v2 = bestPlaneMesh.addVertex(c + u * halfSize + v * halfSize);
        const uint v3 = bestPlaneMesh.addVertex(c - u * halfSize + v * halfSize);

        bestPlaneMesh.addFace(v0, v1, v2);
        bestPlaneMesh.addFace(v0, v2, v3);
    }

    vcl::saveMesh(bestRaysMesh, VCLIB_RESULTS_PATH "/666_volume_prism_rays.ply");
    vcl::saveMesh(bestPrismsMesh, VCLIB_RESULTS_PATH "/666_volume_prism_prisms.ply");

    std::cout << "\nFibonacci planes tested: " << fibNormals.size()
              << "\nBest plane id: " << bestPlaneId
              << "\nBest plane normal: " << bestNormal
              << "\nTriangles processed on best plane: " << bestTriCount
              << "\nTriangles processed while collecting debug: " << debugTriCount
              << "\nMinimum volume: " << bestVolume
#ifdef VCL_EMBREE_FORCE_CHUNK_16
              << "\nBranch: chunk16"
#else
              << "\nBranch: single-ray"
#endif
              << "\nTotal runtime ms: " << totalRuntimeMs.count() << "\n";
    std::cout << "Saved debug meshes:\n"
              << " - " << VCLIB_RESULTS_PATH << "/666_volume_prism_rays.ply\n"
              << " - " << VCLIB_RESULTS_PATH
              << "/666_volume_prism_prisms.ply\n";

/*             
    if (VISUAL) {
        // Show mesh + best plane + debug rays/prisms.
        vcl::qt::MeshViewer mv;

        DrawableMesh<PolyMesh> drawableInput(std::move(m));
        drawableInput.name() = "input_mesh";
        drawableInput.updateBuffers();

        DrawableMesh<TriMesh> drawablePlane(std::move(bestPlaneMesh));
        drawablePlane.name() = "best_plane";
        drawablePlane.updateBuffers();

        using enum vcl::MeshRenderInfo::Buffers;

        DrawableMesh<EdgeMesh> drawableRays(std::move(bestRaysMesh));
        drawableRays.name() = "debug_rays";
        drawableRays.updateBuffers({VERTICES, EDGES});

        DrawableMesh<TriMesh> drawablePrisms(std::move(bestPrismsMesh));
        drawablePrisms.name() = "debug_prisms";
        drawablePrisms.updateBuffers();

        auto vec = std::make_shared<DrawableObjectVector>();
        vec->pushBack(std::move(drawableInput));
        vec->pushBack(std::move(drawablePlane));
        vec->pushBack(std::move(drawableRays));
        vec->pushBack(std::move(drawablePrisms));

        mv.setDrawableObjectVector(vec);

        mv.show();
        mv.showMaximized();

        return app.exec();
    }
*/

    return 0;
}
