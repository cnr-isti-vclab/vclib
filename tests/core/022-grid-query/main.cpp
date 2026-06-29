// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include "k_nearest.h"
#include "nearest.h"
#include "sphere.h"

using Meshes         = std::tuple<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf        = std::tuple<vcl::TriMeshf, vcl::PolyMeshf>;
using MeshesIndexed  = std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;
using MeshesIndexedf = std::tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf>;

static const vcl::uint N_POINTS_TEST = 50;

TEMPLATE_TEST_CASE(
    "Closest faces to points...",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    using namespace vcl;

    const std::string MESH_PATH = VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj";

    SECTION("TriMesh")
    {
        using PointType = TriMesh::VertexType::PositionType;

        std::size_t seed = std::random_device()();

        TriMesh tm = vcl::loadMesh<TriMesh>(MESH_PATH);
        vcl::updateBoundingBox(tm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, tm.boundingBox(), seed);

        SECTION("HashTableGrid")
        {
            closestFacesTest<HSGrid3>(tm, points, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            closestFacesTest<vcl::StaticGrid3>(tm, points, "StaticGrid");
        }
    }

    SECTION("PolyMesh")
    {
        using PointType = PolyMesh::VertexType::PositionType;

        std::size_t seed = std::random_device()();

        PolyMesh pm = vcl::loadMesh<PolyMesh>(MESH_PATH);
        vcl::updateBoundingBox(pm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, pm.boundingBox(), seed);

        SECTION("HashTableGrid")
        {
            closestFacesTest<HSGrid3>(pm, points, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            closestFacesTest<vcl::StaticGrid3>(pm, points, "StaticGrid");
        }
    }
}

TEMPLATE_TEST_CASE(
    "K nearest faces to points...",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    using namespace vcl;

    const uint K_NEAREST = 5;

    const std::string MESH_PATH = VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj";

    SECTION("TriMesh")
    {
        using PointType = TriMesh::VertexType::PositionType;

        std::size_t seed = std::random_device()();

        TriMesh tm = vcl::loadMesh<TriMesh>(MESH_PATH);
        vcl::updateBoundingBox(tm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, tm.boundingBox(), seed);

        SECTION("HashTableGrid")
        {
            kNearestFacesTest<HSGrid3>(tm, points, K_NEAREST, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            kNearestFacesTest<vcl::StaticGrid3>(
                tm, points, K_NEAREST, "StaticGrid");
        }
    }

    SECTION("PolyMesh")
    {
        using PointType = PolyMesh::VertexType::PositionType;

        std::size_t seed = std::random_device()();

        PolyMesh pm = vcl::loadMesh<PolyMesh>(MESH_PATH);
        vcl::updateBoundingBox(pm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, pm.boundingBox(), seed);

        SECTION("HashTableGrid")
        {
            kNearestFacesTest<HSGrid3>(pm, points, K_NEAREST, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            kNearestFacesTest<vcl::StaticGrid3>(
                pm, points, K_NEAREST, "StaticGrid");
        }
    }
}

TEMPLATE_TEST_CASE(
    "Faces in spheres...",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    using namespace vcl;

    const std::string MESH_PATH = VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj";

    SECTION("TriMesh")
    {
        using PointType  = TriMesh::VertexType::PositionType;
        using ScalarType = PointType::ScalarType;
        using SphereType = Sphere<ScalarType>;

        std::size_t seed = std::random_device()();

        TriMesh tm = vcl::loadMesh<TriMesh>(MESH_PATH);
        vcl::updateBoundingBox(tm);

        std::vector<SphereType> spheres =
            randomSpheres(N_POINTS_TEST, tm, seed);

        SECTION("HashTableGrid")
        {
            facesInSpheresTest<HSGrid3>(tm, spheres, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            facesInSpheresTest<vcl::StaticGrid3>(tm, spheres, "StaticGrid");
        }
    }

    SECTION("PolyMesh")
    {
        using PointType  = PolyMesh::VertexType::PositionType;
        using ScalarType = PointType::ScalarType;
        using SphereType = Sphere<ScalarType>;

        std::size_t seed = std::random_device()();

        PolyMesh pm = vcl::loadMesh<PolyMesh>(MESH_PATH);
        vcl::updateBoundingBox(pm);

        std::vector<SphereType> spheres =
            randomSpheres(N_POINTS_TEST, pm, seed);

        SECTION("HashTableGrid")
        {
            facesInSpheresTest<HSGrid3>(pm, spheres, "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            facesInSpheresTest<vcl::StaticGrid3>(pm, spheres, "StaticGrid");
        }
    }
}
