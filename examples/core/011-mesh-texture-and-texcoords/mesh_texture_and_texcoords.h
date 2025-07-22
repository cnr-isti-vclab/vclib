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

#ifndef MESH_TEXTURE_AND_TEXCOORDS_H
#define MESH_TEXTURE_AND_TEXCOORDS_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>
#include <vclib/views.h>

// This example demonstrates how to work with textures and texture coordinates:
// 1. Loading meshes with vertex texture coordinates
// 2. Loading meshes with wedge texture coordinates
// 3. Accessing and manipulating texture coordinates
// 4. Working with texture images and paths
// 5. Enabling texture coordinate components dynamically

auto meshTextureAndTexCoords()
{
    std::cout << "=== VCLib - Texture and TexCoords Example ===" << std::endl;

    // Example 1: Load mesh with vertex texture coordinates
    std::cout << "\n1. Loading mesh with vertex texture coordinates..."
              << std::endl;
    vcl::TriMesh meshVertexTexCoords;

    // note: by default, the texture images are not loaded when loading a mesh
    // only the texture paths will be available
    // To load the texture images, you can set the 'loadTextureImages' option of
    // the LoadSettings object to true, that can be passed to the load function.
    // example of loading also the textures shown below (wedges)
    vcl::loadMesh(
        meshVertexTexCoords,
        VCLIB_EXAMPLE_MESHES_PATH "/bunny_verttexcoords.ply");

    std::cout << "   Loaded " << meshVertexTexCoords.vertexNumber()
              << " vertices, " << meshVertexTexCoords.faceNumber() << " faces"
              << std::endl;

    // Check if mesh has vertex texture coordinates
    if (meshVertexTexCoords.isPerVertexTexCoordEnabled()) {
        std::cout << "   ✓ Mesh has vertex texture coordinates" << std::endl;

        // Access first few vertex texture coordinates
        std::cout << "   First 3 vertex texture coordinates:" << std::endl;
        for (vcl::uint i = 0;
             i < std::min(3u, meshVertexTexCoords.vertexNumber());
             ++i) {
            auto& texCoord = meshVertexTexCoords.vertex(i).texCoord();
            std::cout << "     Vertex " << i << ": (" << texCoord.u() << ", "
                      << texCoord.v() << ") tex_id " << texCoord.index()
                      << std::endl;
        }
    }
    else {
        std::cout << "   ✗ Mesh does not have vertex texture coordinates"
                  << std::endl;
    }

    // Example 2: Load mesh with wedge texture coordinates
    std::cout << "\n2. Loading mesh with wedge texture coordinates..."
              << std::endl;
    vcl::TriMesh meshWedgeTexCoords;

    vcl::LoadSettings settings;
    settings.loadTextureImages = true; // load texture images
    vcl::loadMesh(
        meshWedgeTexCoords,
        VCLIB_EXAMPLE_MESHES_PATH "/bunny_textured.ply",
        settings);
    std::cout << "   Loaded " << meshWedgeTexCoords.vertexNumber()
              << " vertices, " << meshWedgeTexCoords.faceNumber() << " faces"
              << std::endl;

    // Check if mesh has wedge texture coordinates
    if (meshWedgeTexCoords.isPerFaceWedgeTexCoordsEnabled()) {
        std::cout << "   ✓ Mesh has wedge texture coordinates" << std::endl;

        // Access first few face wedge texture coordinates
        std::cout << "   First face wedge texture coordinates:" << std::endl;
        if (meshWedgeTexCoords.faceNumber() > 0) {
            auto& face = meshWedgeTexCoords.face(0);
            for (vcl::uint i = 0; i < face.vertexNumber(); ++i) {
                auto& texCoord = face.wedgeTexCoord(i);
                std::cout << "     Wedge " << i << ": (" << texCoord.u() << ", "
                          << texCoord.v() << ") tex_id " << face.textureIndex()
                          << std::endl;
            }
        }
    }
    else {
        std::cout << "   ✗ Mesh does not have wedge texture coordinates"
                  << std::endl;
    }

    // Example 3: Working with texture images
    std::cout << "\n3. Working with texture images..." << std::endl;
    std::cout << "   ✓ Mesh has " << meshWedgeTexCoords.textureNumber()
              << " textures" << std::endl;

    for (vcl::uint i = 0; i < meshWedgeTexCoords.textureNumber(); ++i) {
        // if the images are not loaded, the texture paths will be available
        // and the image will be empty (width and height will be 0)
        const auto& texture = meshWedgeTexCoords.texture(i);
        std::cout << "     Texture " << i << ": " << texture.path()
                  << std::endl;
        std::cout << "       Size: " << texture.image().width() << "x"
                  << texture.image().height() << std::endl;
    }

    // Example 4: Creating mesh with texture coordinates
    std::cout << "\n4. Creating mesh with texture coordinates..." << std::endl;
    vcl::TriMesh customMesh;
    customMesh.name() = "Custom Textured Mesh";

    // Enable texture coordinate components
    customMesh.enablePerVertexTexCoord();
    customMesh.enablePerFaceWedgeTexCoords();

    // Add vertices with texture coordinates
    customMesh.addVertices(4);
    customMesh.vertex(0).position() = vcl::Point3d(0, 0, 0);
    customMesh.vertex(0).texCoord() = vcl::Point2d(0, 0);
    customMesh.vertex(1).position() = vcl::Point3d(1, 0, 0);
    customMesh.vertex(1).texCoord() = vcl::Point2d(1, 0);
    customMesh.vertex(2).position() = vcl::Point3d(1, 1, 0);
    customMesh.vertex(2).texCoord() = vcl::Point2d(1, 1);
    customMesh.vertex(3).position() = vcl::Point3d(0, 1, 0);
    customMesh.vertex(3).texCoord() = vcl::Point2d(0, 1);

    // Add faces with wedge texture coordinates
    customMesh.addFaces(2);
    customMesh.face(0).setVertices(0, 1, 2);
    customMesh.face(0).wedgeTexCoord(0) = vcl::Point2d(0, 0);
    customMesh.face(0).wedgeTexCoord(1) = vcl::Point2d(1, 0);
    customMesh.face(0).wedgeTexCoord(2) = vcl::Point2d(1, 1);

    customMesh.face(1).setVertices(0, 2, 3);
    customMesh.face(1).wedgeTexCoord(0) = vcl::Point2d(0, 0);
    customMesh.face(1).wedgeTexCoord(1) = vcl::Point2d(1, 1);
    customMesh.face(1).wedgeTexCoord(2) = vcl::Point2d(0, 1);

    customMesh.pushTexture(meshWedgeTexCoords.texture(0));

    // change the path of the texture to a custom one (will be relative to the
    // mesh file path when saving)
    customMesh.texture(0).path() =
        "custom_texture_path.png"; // just an example path

    std::cout << "   Created custom mesh with " << customMesh.vertexNumber()
              << " vertices and " << customMesh.faceNumber() << " faces"
              << std::endl;
    std::cout << "   ✓ Has vertex texture coordinates: "
              << customMesh.isPerVertexTexCoordEnabled() << std::endl;
    std::cout << "   ✓ Has wedge texture coordinates: "
              << customMesh.isPerFaceWedgeTexCoordsEnabled() << std::endl;

    // Example 5: Iterating through texture coordinates using views
    std::cout << "\n5. Iterating through texture coordinates..." << std::endl;
    if (meshVertexTexCoords.isPerVertexTexCoordEnabled()) {
        std::cout << "   Vertex texture coordinates range:" << std::endl;
        vcl::Point2d minTexCoord(
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max());
        vcl::Point2d maxTexCoord(
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest());

        for (const auto& tc :
             meshVertexTexCoords.vertices() | vcl::views::texCoords) {
            minTexCoord = vcl::Point2d(
                std::min(minTexCoord.x(), tc.u()),
                std::min(minTexCoord.y(), tc.v()));
            maxTexCoord = vcl::Point2d(
                std::max(maxTexCoord.x(), tc.u()),
                std::max(maxTexCoord.y(), tc.v()));
        }

        std::cout << "     Min: (" << minTexCoord.x() << ", " << minTexCoord.y()
                  << ")" << std::endl;
        std::cout << "     Max: (" << maxTexCoord.x() << ", " << maxTexCoord.y()
                  << ")" << std::endl;
    }

    std::cout << "\n=== Example completed ===" << std::endl;
    return std::make_tuple(customMesh);
}

#endif // MESH_TEXTURE_AND_TEXCOORDS_H
