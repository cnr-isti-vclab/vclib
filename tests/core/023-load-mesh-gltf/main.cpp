// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/io.h>
#include <vclib/meshes.h>

#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include <filesystem>

using Meshes         = std::tuple<vcl::TriMesh, vcl::PolyMesh>;
using Meshesf        = std::tuple<vcl::TriMeshf, vcl::PolyMeshf>;
using MeshesIndexed  = std::tuple<vcl::TriMeshIndexed, vcl::PolyMeshIndexed>;
using MeshesIndexedf = std::tuple<vcl::TriMeshIndexedf, vcl::PolyMeshIndexedf>;

// Test to load obj from a istringstream
TEMPLATE_TEST_CASE(
    "Load gltf",
    "",
    Meshes,
    Meshesf,
    MeshesIndexed,
    MeshesIndexedf)
{
    using TriMesh  = std::tuple_element_t<0, TestType>;
    using PolyMesh = std::tuple_element_t<1, TestType>;

    vcl::MeshInfo info;

    SECTION("Load Duck.gltf")
    {
        info.clear();

        TriMesh tm;

        vcl::loadGltf(
            tm, VCLIB_EXAMPLE_MESHES_PATH "/gltf/Duck/Duck.gltf", info);
        REQUIRE(tm.vertexCount() == 2399);
        REQUIRE(tm.faceCount() == 4212);

        REQUIRE(info.hasVertices());
        REQUIRE(info.hasFaces());
        REQUIRE(!info.hasEdges());
    }

    SECTION("Load and Save CompareEmissiveStrength.gltf")
    {
        std::vector<TriMesh>       meshes;
        std::vector<vcl::MeshInfo> infos;

        vcl::loadGltf(
            meshes,
            VCLIB_EXAMPLE_MESHES_PATH
            "/gltf/CompareEmissiveStrength/CompareEmissiveStrength.gltf",
            infos);

        REQUIRE(meshes.size() == 2);
        REQUIRE(meshes[0].materials().size() == 1);
        REQUIRE(meshes[0].material(0).emissiveStrength() == 1.0f);
        REQUIRE(meshes[1].materials().size() == 1);
        REQUIRE(meshes[1].material(0).emissiveStrength() == 3.0f);

        // Test saving as .gltf (ASCII JSON)
        {
            std::string       tmpFile = "temp_emissive_strength.gltf";
            vcl::SaveSettings settings;
            settings.embedBuffers = true;
            vcl::saveGltf(meshes, tmpFile, settings);

            std::vector<TriMesh>       reloadedMeshes;
            std::vector<vcl::MeshInfo> reloadedInfos;
            vcl::loadGltf(reloadedMeshes, tmpFile, reloadedInfos);

            std::filesystem::remove(tmpFile);

            REQUIRE(reloadedMeshes.size() == 2);
            REQUIRE(reloadedMeshes[0].materials().size() == 1);
            REQUIRE(reloadedMeshes[0].material(0).emissiveStrength() == 1.0f);
            REQUIRE(reloadedMeshes[1].materials().size() == 1);
            REQUIRE(reloadedMeshes[1].material(0).emissiveStrength() == 3.0f);
        }

        // Test saving as .glb (binary)
        {
            std::string tmpFile = "temp_emissive_strength.glb";
            vcl::saveGltf(meshes, tmpFile);

            std::vector<TriMesh>       reloadedMeshes;
            std::vector<vcl::MeshInfo> reloadedInfos;
            vcl::loadGltf(reloadedMeshes, tmpFile, reloadedInfos);

            std::filesystem::remove(tmpFile);

            REQUIRE(reloadedMeshes.size() == 2);
            REQUIRE(reloadedMeshes[0].materials().size() == 1);
            REQUIRE(reloadedMeshes[0].material(0).emissiveStrength() == 1.0f);
            REQUIRE(reloadedMeshes[1].materials().size() == 1);
            REQUIRE(reloadedMeshes[1].material(0).emissiveStrength() == 3.0f);
        }
    }
}
