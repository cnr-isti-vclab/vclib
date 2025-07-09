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

#ifndef FACE_FLOODING_H
#define FACE_FLOODING_H

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// This example demonstrates how to flood a mesh starting from a seed face
// using a custom selection criterion. It uses the face flooding algorithm
// to find all connected faces that satisfy the selection condition, using a
// depth-first search approach.

auto faceFlooding()
{
    std::cout << "=== VCLib Example 016: Face Flooding ===\n\n";

    // Load mesh
    vcl::TriMesh m =
        vcl::load<vcl::TriMesh>(VCLIB_EXAMPLE_MESHES_PATH "/bunny.obj");

    // Enable required components
    m.enablePerFaceAdjacentFaces();
    m.enablePerFaceColor();

    // Update topology and normals
    vcl::updatePerFaceAdjacentFaces(m);
    vcl::updatePerVertexAndFaceNormals(m);
    vcl::updateBoundingBox(m);

    // Initialize face colors
    vcl::setPerFaceColor(m, vcl::Color::LightGray);

    std::cout << "Mesh loaded: " << m.faceNumber() << " faces\n";

    // Custom flooding function: select faces with normal pointing upward
    auto upwardFacingSelector = [](const vcl::TriMesh::Face& f) {
        return f.normal().y() > 0.5; // faces pointing significantly upward
    };

    // Find a seed face that satisfies our condition
    vcl::uint seedFaceId = vcl::UINT_NULL;
    for (const auto& f : m.faces()) {
        if (upwardFacingSelector(f)) {
            seedFaceId = f.index();
            break;
        }
    }

    if (seedFaceId == vcl::UINT_NULL) {
        std::cout << "No suitable seed face found!\n";
        return std::make_tuple(m);
        ;
    }

    // Mark seed face in red
    m.face(seedFaceId).color() = vcl::Color::Red;
    std::cout << "Seed face: " << seedFaceId << "\n";

    // Perform flooding from seed face using the algorithm from face_topology.h
    auto floodedFaces =
        vcl::floodFacePatch(m.face(seedFaceId), upwardFacingSelector);

    // Color flooded faces in green
    for (const auto* fPtr : floodedFaces) {
        if (fPtr->index() != seedFaceId) {
            m.face(fPtr->index()).color() = vcl::Color::Green;
        }
    }

    std::cout << "Flooded " << floodedFaces.size() << " faces\n";

    return std::make_tuple(m);
}

#endif // FACE_FLOODING_H
