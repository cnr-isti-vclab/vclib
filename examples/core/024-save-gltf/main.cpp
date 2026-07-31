// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#include <vclib/io.h>
#include <vclib/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space.h>

#include <filesystem>
#include <iostream>

int main()
{
    // Test saving a PolyMesh

    vcl::LoadSettings loadSettings;
    loadSettings.loadTextureImages = true;

    auto helmet = vcl::loadMesh<vcl::PolyMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/greek_helmet.obj", loadSettings);
    vcl::updateBoundingBox(helmet);
    vcl::updatePerVertexAndFaceNormals(helmet);

    // test normals normalization
    // for (auto& norm : mesh.vertices() | vcl::views::normals)
    //    norm *= 2.0;

    vcl::SaveSettings saveSettings;
    saveSettings.binary = false;
    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_greek_helmet_export_gltf");
    vcl::saveMesh(
        helmet,
        VCLIB_CORE_RESULTS_PATH
        "/024_greek_helmet_export_gltf/024_greek_helmet_export_gltf.gltf",
        saveSettings);

    std::cout << "Saved Greek helmet in gltf format (ASCII)" << std::endl;

    saveSettings.binary = true;
    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_greek_helmet_export_bin");
    vcl::saveMesh(
        helmet,
        VCLIB_CORE_RESULTS_PATH
        "/024_greek_helmet_export_bin/024_greek_helmet_export_bin.glb",
        saveSettings);

    std::cout << "Saved Greek helmet in gltf format (binary)" << std::endl;

    // Test multiple meshes on same file

    auto bunny = vcl::loadMesh<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj", loadSettings);
    auto bimba = vcl::loadMesh<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj", loadSettings);
    vcl::updatePerVertexAndFaceNormals(bimba);

    std::vector<vcl::TriMesh> meshes {std::move(bunny), std::move(bimba)};

    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_bunny_bimba");
    vcl::saveMeshes(
        meshes,
        VCLIB_CORE_RESULTS_PATH "/024_bunny_bimba/024_bunny_bimba.glb",
        saveSettings);

    std::cout << "Saved bunny and bimba in gltf format (binary)" << std::endl;

    // Test saving a PointCloud

    vcl::LoadSettings lS;
    lS.loadTextureImages = true;

    auto pointCloud = vcl::loadMesh<vcl::PointCloud>(
        VCLIB_EXAMPLE_MESHES_PATH "/gltf/FlowerPointCloud/scene.gltf", lS);

    vcl::SaveSettings sS;
    sS.binary = false;
    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_flower_point_cloud_export_gltf");
    vcl::saveMesh(
        pointCloud,
        VCLIB_CORE_RESULTS_PATH
        "/024_flower_point_cloud_export_gltf/"
        "024_flower_point_cloud_export_gltf.gltf",
        sS);

    std::cout << "Saved Flower Point Cloud in gltf format (ASCII)" << std::endl;

    // Test a TriEdgeMesh with edges and faces

    auto bte =
        vcl::loadMesh<vcl::TriEdgeMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    bte.enablePerVertexColor();
    for (auto& c : bte.vertices() | vcl::views::colors)
        c = vcl::Color(vcl::Color::Random());

    auto tmp = vcl::loadMesh<vcl::TriEdgeMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                               "/bunny_edge_sections.obj");
    tmp.enablePerVertexColor();
    for (auto& c : tmp.vertices() | vcl::views::colors)
        c = vcl::Color::Black;

    bte.append(tmp);

    vcl::updatePerVertexAndFaceNormals(bte);

    std::cout << "Number of vertices: " << bte.vertexCount() << std::endl;
    std::cout << "Number of faces: " << bte.faceCount() << std::endl;
    std::cout << "Number of edges: " << bte.edgeCount() << std::endl;

    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_bunny_edge_sections_export_gltf");
    vcl::saveMesh(
        bte,
        VCLIB_CORE_RESULTS_PATH
        "/024_bunny_edge_sections_export_gltf/"
        "024_bunny_edge_sections_export_gltf.gltf",
        sS);

    std::cout << "Saved Bunny Edge Sections in gltf format (ASCII)"
              << std::endl;

    // Test multiple textures

    auto damagedHelmet = vcl::loadMesh<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/gltf/DamagedHelmet/DamagedHelmet.gltf",
        loadSettings);
    vcl::updatePerVertexAndFaceNormals(damagedHelmet);

    saveSettings.binary            = false;
    saveSettings.saveTextureImages = true;
    saveSettings.embedBuffers      = true;
    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_damaged_helmet");
    vcl::saveMesh(
        damagedHelmet,
        VCLIB_CORE_RESULTS_PATH "/024_damaged_helmet/024_damaged_helmet.gltf",
        saveSettings);

    std::cout << "Saved Damaged Helmet in gltf format (ASCII)" << std::endl;

    // Test multiple materials

    auto cesiumMilkTruck = vcl::loadMesh<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/gltf/CesiumMilkTruck/CesiumMilkTruck.gltf",
        loadSettings);
    vcl::updatePerVertexAndFaceNormals(cesiumMilkTruck);

    saveSettings.binary            = false;
    saveSettings.saveTextureImages = true;
    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_cesium_milk_truck");
    vcl::saveMesh(
        cesiumMilkTruck,
        VCLIB_CORE_RESULTS_PATH
        "/024_cesium_milk_truck/024_cesium_milk_truck.gltf",
        saveSettings);

    std::cout << "Saved Cesium Milk Truck in gltf format (ASCII)" << std::endl;

    // Test wedge texcoords (with quads!)

    auto spot = vcl::loadMesh<vcl::PolyMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/spot/spot_quadrangulated.obj",
        loadSettings);
    vcl::updatePerVertexAndFaceNormals(spot);

    saveSettings.binary            = false;
    saveSettings.saveTextureImages = true;
    saveSettings.embedBuffers      = false;
    std::filesystem::create_directories(
        VCLIB_CORE_RESULTS_PATH "/024_spot_quadrangulated");
    vcl::saveMesh(
        spot,
        VCLIB_CORE_RESULTS_PATH
        "/024_spot_quadrangulated/024_spot_quadrangulated.gltf",
        saveSettings);

    std::cout << "Saved Spot Quadrangulated in gltf format (ASCII)"
              << std::endl;

    return 0;
}
