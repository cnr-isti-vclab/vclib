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

#include <vclib/io.h>
#include <vclib/mesh.h>
#include <vclib/meshes.h>
#include <vclib/space.h>

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
    vcl::saveMesh(
        helmet,
        VCLIB_RESULTS_PATH "/024_greek_helmet_export_gltf.gltf",
        saveSettings);

    std::cout << "Saved Greek helmet in gltf format (ASCII)" << std::endl;

    saveSettings.binary = true;
    vcl::saveMesh(
        helmet,
        VCLIB_RESULTS_PATH "/024_greek_helmet_export_bin.glb",
        saveSettings);

    std::cout << "Saved Greek helmet in gltf format (binary)" << std::endl;

    // Test multiple meshes on same file

    auto bunny = vcl::loadMesh<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj", loadSettings);
    auto bimba = vcl::loadMesh<vcl::TriMesh>(
        VCLIB_EXAMPLE_MESHES_PATH "/bimba.obj", loadSettings);
    vcl::updatePerVertexAndFaceNormals(bimba);

    std::vector<vcl::TriMesh> meshes {std::move(bunny), std::move(bimba)};

    vcl::saveMeshes(
        meshes, VCLIB_RESULTS_PATH "/024_bunny_bimba.glb", saveSettings);

    std::cout << "Saved bunny and bimba in gltf format (binary)" << std::endl;

    // Test saving a PointCloud

    vcl::LoadSettings lS;
    lS.loadTextureImages = true;

    auto pointCloud = vcl::loadMesh<vcl::PointCloud>(
        VCLIB_EXAMPLE_MESHES_PATH "/gltf/FlowerPointCloud/scene.gltf", lS);

    vcl::SaveSettings sS;
    sS.binary = false;
    vcl::saveMesh(
        pointCloud,
        VCLIB_RESULTS_PATH "/024_flower_point_cloud_export_gltf.gltf",
        sS);

    std::cout << "Saved Flower Point Cloud in gltf format (ASCII)" << std::endl;

    // Test a TriEdgeMesh with edges and faces

    auto bte =
        vcl::loadMesh<vcl::TriEdgeMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");
    auto tmp = vcl::loadMesh<vcl::TriEdgeMesh>(VCLIB_EXAMPLE_MESHES_PATH
                                               "/bunny_edge_sections.obj");

    bte.append(tmp);

    std::cout << "Number of vertices: " << bte.vertexCount() << std::endl;
    std::cout << "Number of faces: " << bte.faceCount() << std::endl;
    std::cout << "Number of edges: " << bte.edgeCount() << std::endl;

    vcl::saveMesh(
        bte,
        VCLIB_RESULTS_PATH "/024_bunny_edge_sections_export_gltf.gltf",
        sS);

    std::cout << "Saved Bunny Edge Sections in gltf format (ASCII)" << std::endl;

    return 0;
}
