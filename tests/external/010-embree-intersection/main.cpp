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

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

#include <vclib/embree/scene.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

template<vcl::Box3Concept BoxType>
auto randomRays(
    vcl::uint      n,
    const BoxType& bbox,
    std::size_t    seed = std::random_device()())
{
    using PointType  = BoxType::PointType;
    using ScalarType = PointType::ScalarType;
    using DistrType  = std::uniform_real_distribution<ScalarType>;

    std::vector<vcl::Ray<PointType>> rays(n);

    ScalarType ext = bbox.diagonal() * 0.1;

    std::cout << "Random seed: " << seed << std::endl;

    std::mt19937 gen(seed);
    DistrType    disX(bbox.min().x() - ext, bbox.max().x() + ext);
    DistrType    disY(bbox.min().y() - ext, bbox.max().y() + ext);
    DistrType    disZ(bbox.min().z() - ext, bbox.max().z() + ext);
    DistrType    disDir(-1, 1);

    for (vcl::uint i = 0; i < n; i++) {
        auto      p = PointType(disX(gen), disY(gen), disZ(gen));
        PointType dir;
        do {
            dir = PointType(disDir(gen), disDir(gen), disDir(gen));
        } while (dir.norm() == ScalarType(0));
        rays[i] = vcl::Ray<PointType>(p, dir.normalized());
    }

    return rays;
}

template<vcl::FaceMeshConcept MeshType, vcl::Ray3Concept RayType>
auto bruteForceRayIntersection(const MeshType& m, const RayType& ray)
{
    using ScalarType = typename RayType::PointType::ScalarType;

    vcl::uint               closestFace = vcl::UINT_NULL;
    ScalarType              minT = std::numeric_limits<ScalarType>::max();
    vcl::Point3<ScalarType> closestPoint;

    for (const auto& f : m.faces()) {
        ScalarType t = -1; // modified when p has value
        auto       p = vcl::intersection(ray, f, t);
        if (p.has_value() && (t < minT)) {
            minT         = t;
            closestFace  = m.index(f);
            closestPoint = p.value();
        }
    }

    return std::make_pair(closestFace, closestPoint);
}

using Meshes         = std::tuple<vcl::TriMesh, vcl::PolyMesh>;
using MeshesIndexed  = std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;

static const vcl::uint N_RAYS_TEST = 10000;

TEMPLATE_TEST_CASE(
    "Embree ray-triangle intersection vs brute force",
    "",
    Meshes,
    MeshesIndexed)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    using namespace vcl;

    SECTION(vcl::meshTypeName<TriMesh>().c_str())
    {
        using PointType  = TriMesh::VertexType::PositionType;
        using ScalarType = PointType::ScalarType;
        using RayType    = Ray<PointType>;

        std::size_t seed = std::random_device()();

        const std::string MESH_PATH = VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj";

        TriMesh tm = vcl::loadMesh<TriMesh>(MESH_PATH);
        vcl::updateBoundingBox(tm);

        std::vector<RayType> rays =
            randomRays(N_RAYS_TEST, tm.boundingBox(), seed);

        // Create Embree scene
        vcl::embree::Scene scene(tm);

        uint matches          = 0;
        uint embreeMisses     = 0;
        uint bruteForceMisses = 0;

        vcl::Timer tEmbree("Embree intersection rays");
        auto embreeResults = scene.firstFaceIntersectedByRays(rays);
        tEmbree.stopAndPrint();

        std::vector<uint> indices(N_RAYS_TEST);
        std::vector<std::pair<uint, PointType>> bruteResults(N_RAYS_TEST);

        std::iota(indices.begin(), indices.end(), 0);

        vcl::Timer tBrute("Brute force intersection rays");
        vcl::parallelFor(indices, [&](uint i){
            bruteResults[i] = bruteForceRayIntersection(tm, rays[i]);
        });
        tBrute.stopAndPrint();

        for (uint i = 0; i < N_RAYS_TEST; i++) {
            // Embree intersection
            uint embreeFaceID = std::get<0>(embreeResults[i]);

            // Brute force intersection
            auto [bruteFaceID, brutePoint] = bruteResults[i];

            if (embreeFaceID == vcl::UINT_NULL &&
                bruteFaceID == vcl::UINT_NULL) {
                matches++;
            }
            else if (
                embreeFaceID != vcl::UINT_NULL &&
                bruteFaceID != vcl::UINT_NULL) {
                // Both found an intersection
                if (embreeFaceID == bruteFaceID) {
                    matches++;
                }
                else {
                    // Even if face IDs differ, check if the hit points are
                    // close (could happen at face boundaries)
                    vcl::Point3f baryCoords = std::get<1>(embreeResults[i]);
                    uint         triID      = std::get<2>(embreeResults[i]);

                    auto&                   face = tm.face(embreeFaceID);
                    vcl::Point3<ScalarType> embreePoint =
                        face.vertex(0)->position() * baryCoords.x() +
                        face.vertex(1)->position() * baryCoords.y() +
                        face.vertex(2)->position() * baryCoords.z();

                    ScalarType dist = (embreePoint - brutePoint).norm();
                    if (dist < ScalarType(1e-5)) {
                        matches++;
                    }
                }
            }
            else {
                if (embreeFaceID == vcl::UINT_NULL) {
                    embreeMisses++;
                }
                if (bruteFaceID == vcl::UINT_NULL) {
                    bruteForceMisses++;
                }
            }
        }

        std::cout << vcl::meshTypeName<TriMesh>() << " - Matches: " << matches
                  << "/" << N_RAYS_TEST << " (Embree misses: " << embreeMisses
                  << ", Brute force misses: " << bruteForceMisses << ")"
                  << std::endl;

        REQUIRE(matches == N_RAYS_TEST);
    }

    SECTION(vcl::meshTypeName<PolyMesh>().c_str())
    {
        using PointType  = PolyMesh::VertexType::PositionType;
        using ScalarType = PointType::ScalarType;
        using RayType    = Ray<PointType>;

        std::size_t seed = std::random_device()();

        const std::string MESH_PATH =
            VCLIB_EXAMPLE_MESHES_PATH "/greek_helmet.obj";

        PolyMesh pm = vcl::loadMesh<PolyMesh>(MESH_PATH);
        vcl::updateBoundingBox(pm);

        std::vector<RayType> rays =
            randomRays(N_RAYS_TEST, pm.boundingBox(), seed);

        // Create Embree scene
        vcl::embree::Scene scene(pm);

        uint matches          = 0;
        uint embreeMisses     = 0;
        uint bruteForceMisses = 0;

        // here use firstFaceIntersectedByRays
        vcl::Timer tEmbree("Embree intersection rays");
        auto embreeResults = scene.firstFaceIntersectedByRays(rays);
        tEmbree.stopAndPrint();

        std::vector<uint> indices(N_RAYS_TEST);
        std::vector<std::pair<uint, PointType>> bruteResults(N_RAYS_TEST);

        std::iota(indices.begin(), indices.end(), 0);

        vcl::Timer tBrute("Brute force intersection rays");
        vcl::parallelFor(indices, [&](uint i){
            bruteResults[i] = bruteForceRayIntersection(pm, rays[i]);
        });
        tBrute.stopAndPrint();

        for (uint i = 0; i < N_RAYS_TEST; i++) {
            // Embree intersection
            uint embreeFaceID = std::get<0>(embreeResults[i]);

            // Brute force intersection
            auto [bruteFaceID, brutePoint] = bruteResults[i];

            if (embreeFaceID == vcl::UINT_NULL &&
                bruteFaceID == vcl::UINT_NULL) {
                matches++;
            }
            else if (
                embreeFaceID != vcl::UINT_NULL &&
                bruteFaceID != vcl::UINT_NULL) {
                // Both found an intersection - for polymesh just check they're
                // the same
                if (embreeFaceID == bruteFaceID) {
                    matches++;
                }
                else {
                    // Could be at boundary, check distance
                    vcl::Point3f baryCoords = std::get<1>(embreeResults[i]);
                    uint         triID      = std::get<2>(embreeResults[i]);

                    auto& face = pm.face(embreeFaceID);
                    auto  tris = vcl::earCut(face);

                    vcl::Point3<ScalarType> embreePoint =
                        pm.vertex(tris[triID * 3]).position() * baryCoords.x() +
                        pm.vertex(tris[triID * 3 + 1]).position() *
                            baryCoords.y() +
                        pm.vertex(tris[triID * 3 + 2]).position() *
                            baryCoords.z();

                    ScalarType dist = (embreePoint - brutePoint).norm();
                    if (dist < ScalarType(1e-5)) {
                        matches++;
                    }
                }
            }
            else {
                if (embreeFaceID == vcl::UINT_NULL) {
                    embreeMisses++;
                }
                if (bruteFaceID == vcl::UINT_NULL) {
                    bruteForceMisses++;
                }
            }
        }

        std::cout << vcl::meshTypeName<PolyMesh>() << " - Matches: " << matches
                  << "/" << N_RAYS_TEST << " (Embree misses: " << embreeMisses
                  << ", Brute force misses: " << bruteForceMisses << ")"
                  << std::endl;

        REQUIRE(matches == N_RAYS_TEST);
    }
}
