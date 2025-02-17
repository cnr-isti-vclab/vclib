/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#include "k_nearest.h"
#include "nearest.h"

using Meshes         = std::tuple<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf        = std::tuple<vcl::TriMeshf, vcl::PolyMeshf>;
using MeshesIndexed  = std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;
using MeshesIndexedf = std::tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf>;

static const vcl::uint N_POINTS_TEST = 50;

TEMPLATE_TEST_CASE("Closest faces to points...", "", Meshes)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    using namespace vcl;

    const std::string MESH_PATH = VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj";

    SECTION("TriMesh")
    {
        using PointType = TriMesh::VertexType::CoordType;

        std::size_t seed = std::random_device()();

        TriMesh tm = vcl::load<TriMesh>(MESH_PATH);
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
        using PointType = PolyMesh::VertexType::CoordType;

        std::size_t seed = std::random_device()();

        PolyMesh pm = vcl::load<PolyMesh>(MESH_PATH);
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

TEMPLATE_TEST_CASE("K nearest faces to points...", "", Meshes)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    using namespace vcl;

    const uint K_NEAREST = 5;

    const std::string MESH_PATH = VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj";

    SECTION("TriMesh")
    {
        using PointType = TriMesh::VertexType::CoordType;

        std::size_t        seed = std::random_device()();

        TriMesh tm = vcl::load<TriMesh>(MESH_PATH);
        vcl::updateBoundingBox(tm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, tm.boundingBox(), seed);

        SECTION("HashTableGrid")
        {
            kNearestFacesTest<HSGrid3>(tm, points, K_NEAREST,
            "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            kNearestFacesTest<vcl::StaticGrid3>(tm, points, K_NEAREST,
            "StaticGrid");
        }
    }

    SECTION("PolyMesh")
    {
        using PointType = PolyMesh::VertexType::CoordType;

        std::size_t        seed = std::random_device()();

        PolyMesh pm = vcl::load<PolyMesh>(MESH_PATH);
        vcl::updateBoundingBox(pm);

        std::vector<PointType> points =
            randomPoints(N_POINTS_TEST, pm.boundingBox(), seed);

        SECTION("HashTableGrid")
        {
            kNearestFacesTest<HSGrid3>(pm, points, K_NEAREST,
            "HashTableGrid");
        }

        SECTION("StaticGrid")
        {
            kNearestFacesTest<vcl::StaticGrid3>(pm, points, K_NEAREST,
            "StaticGrid");
        }
    }
}
