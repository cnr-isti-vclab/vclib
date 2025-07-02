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

// This example demonstrates how to:
// 1. Copy and clone meshes
// 2. Apply geometric transformations (rotation, translation, scaling)
// 3. Use transformation matrices
// 4. Combine multiple meshes through append

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

// Mathematical constants
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int main()
{
    std::cout << "=== VCLib Example 004: Mesh Copy and Transform ===\n\n";

    // ========================================
    // PART 1: MESH COPYING AND CLONING
    // ========================================

    std::cout << "PART 1: Mesh Copying and Cloning\n";
    std::cout << "---------------------------------\n";

    // Create a starting mesh: an icosahedron
    vcl::TriMesh originalMesh = vcl::createIcosahedron<vcl::TriMesh>();

    std::cout << "Original mesh created: icosahedron with "
              << originalMesh.vertexNumber() << " vertices and "
              << originalMesh.faceNumber() << " faces\n";

    // METHOD 1: Copy via copy constructor
    std::cout << "\n1. Copy via copy constructor:\n";
    vcl::TriMesh copiedMesh1(originalMesh);
    std::cout << "   Mesh copied with " << copiedMesh1.vertexNumber()
              << " vertices\n";

    // METHOD 2: Copy via assignment operator
    std::cout << "\n2. Copy via assignment operator:\n";
    vcl::TriMesh copiedMesh2;
    copiedMesh2 = originalMesh;
    std::cout << "   Mesh copied with " << copiedMesh2.vertexNumber()
              << " vertices\n";

    // Verify that copies are independent
    std::cout << "\nVerifying independence of copies:\n";
    originalMesh.clear();
    std::cout << "   Original mesh cleared: " << originalMesh.vertexNumber()
              << " vertices\n";
    std::cout << "   Copy 1 maintains: " << copiedMesh1.vertexNumber()
              << " vertices\n";
    std::cout << "   Copy 2 maintains: " << copiedMesh2.vertexNumber()
              << " vertices\n";

    // ========================================
    // PART 2: GEOMETRIC TRANSFORMATIONS
    // ========================================

    std::cout << "\n\nPART 2: Geometric Transformations\n";
    std::cout << "----------------------------------\n";

    // Recreate the original mesh for transformations
    vcl::TriMesh transformMesh = vcl::createIcosahedron<vcl::TriMesh>();
    vcl::updateBoundingBox(transformMesh);

    // 2.1 TRANSLATION
    std::cout << "\n2.1 Translation:\n";
    vcl::Point3d translation(2.0, 1.0, 0.5);

    // Calculate bounding box before translation
    auto bbBefore = transformMesh.boundingBox();
    std::cout << "   Bounding box before: min(" << bbBefore.min().x() << ", "
              << bbBefore.min().y() << ", " << bbBefore.min().z() << ") max("
              << bbBefore.max().x() << ", " << bbBefore.max().y() << ", "
              << bbBefore.max().z() << ")\n";

    // Apply translation
    vcl::translate(transformMesh, translation);
    vcl::updateBoundingBox(transformMesh);

    auto bbAfter = transformMesh.boundingBox();
    std::cout << "   Bounding box after translation by (" << translation.x()
              << ", " << translation.y() << ", " << translation.z() << "):\n";
    std::cout << "   min(" << bbAfter.min().x() << ", " << bbAfter.min().y()
              << ", " << bbAfter.min().z() << ") max(" << bbAfter.max().x()
              << ", " << bbAfter.max().y() << ", " << bbAfter.max().z()
              << ")\n";

    // 2.2 SCALING
    std::cout << "\n2.2 Scaling:\n";
    vcl::TriMesh scaleMesh = vcl::createCube<vcl::TriMesh>();
    vcl::updateBoundingBox(scaleMesh);

    auto   bbScaleBefore  = scaleMesh.boundingBox();
    double diagonalBefore = (bbScaleBefore.max() - bbScaleBefore.min()).norm();
    std::cout << "   Bounding box diagonal before scaling: " << diagonalBefore
              << "\n";

    // Apply uniform scaling
    double scaleFactor = 2.5;
    vcl::scale(scaleMesh, scaleFactor);
    vcl::updateBoundingBox(scaleMesh);

    auto   bbScaleAfter  = scaleMesh.boundingBox();
    double diagonalAfter = (bbScaleAfter.max() - bbScaleAfter.min()).norm();
    std::cout << "   Diagonal after scaling " << scaleFactor
              << "x: " << diagonalAfter << "\n";
    std::cout << "   Diagonal ratio: " << diagonalAfter / diagonalBefore
              << " (should be ~" << scaleFactor << ")\n";

    // Non-uniform scaling
    std::cout << "\n   Non-uniform scaling:\n";
    vcl::TriMesh nonUniformMesh = vcl::createCube<vcl::TriMesh>();
    vcl::Point3d scaleFactors(
        1.0, 2.0, 0.5); // x unchanged, y doubled, z halved
    vcl::scale(nonUniformMesh, scaleFactors);
    vcl::updateBoundingBox(nonUniformMesh);

    auto         bbNonUniform = nonUniformMesh.boundingBox();
    vcl::Point3d size         = bbNonUniform.max() - bbNonUniform.min();
    std::cout << "   Dimensions after non-uniform scaling (" << scaleFactors.x()
              << ", " << scaleFactors.y() << ", " << scaleFactors.z() << "): ("
              << size.x() << ", " << size.y() << ", " << size.z() << ")\n";

    // 2.3 ROTATION
    std::cout << "\n2.3 Rotation:\n";
    vcl::TriMesh rotateMesh = vcl::createCube<vcl::TriMesh>();

    // Rotation around Z-axis by 45 degrees
    double       angleRadians = M_PI / 4.0; // 45 degrees
    vcl::Point3d axis(0, 0, 1);             // Z-axis

    std::cout << "   Rotation by " << (angleRadians * 180.0 / M_PI)
              << " degrees around axis (" << axis.x() << ", " << axis.y()
              << ", " << axis.z() << ")\n";

    vcl::rotate(rotateMesh, axis, angleRadians);

    auto bbRotated = rotateMesh.boundingBox();
    std::cout << "   Bounding box after rotation: dimensions ("
              << (bbRotated.max().x() - bbRotated.min().x()) << ", "
              << (bbRotated.max().y() - bbRotated.min().y()) << ", "
              << (bbRotated.max().z() - bbRotated.min().z()) << ")\n";

    // ========================================
    // PART 3: TRANSFORMATION MATRICES
    // ========================================

    std::cout << "\n\nPART 3: Transformation Matrices\n";
    std::cout << "--------------------------------\n";

    // Create a composite transformation matrix
    std::cout << "Creating a composite transformation matrix:\n";

    vcl::TriMesh matrixMesh = vcl::createIcosahedron<vcl::TriMesh>();

    // 4x4 transformation matrix (translation + rotation + scale)
    vcl::Matrix44d transformMatrix = vcl::Matrix44d::Identity();

    // Rotation component (45° around Y)
    double rotAngle = M_PI / 4.0;
    double cosA     = std::cos(rotAngle);
    double sinA     = std::sin(rotAngle);

    // Rotation matrix around Y
    vcl::Matrix33d rotMatrix;
    rotMatrix << cosA, 0, sinA, 0, 1, 0, -sinA, 0, cosA;

    // Insert rotation into 4x4 matrix
    transformMatrix.block<3, 3>(0, 0) = rotMatrix;

    // Add translation
    transformMatrix(0, 3) = 1.5; // translation X
    transformMatrix(1, 3) = 1.0; // translation Y
    transformMatrix(2, 3) = 0.5; // translation Z

    // Add scale factor (multiplying the rotational part)
    scaleFactor = 1.5;
    transformMatrix.block<3, 3>(0, 0) *= scaleFactor;

    std::cout << "Transformation matrix:\n" << transformMatrix << "\n\n";

    // Apply the transformation matrix
    auto bbMatrixBefore = matrixMesh.boundingBox();
    std::cout << "Before transformation: center = ("
              << bbMatrixBefore.center().x() << ", "
              << bbMatrixBefore.center().y() << ", "
              << bbMatrixBefore.center().z() << ")\n";

    vcl::applyTransformMatrix(matrixMesh, transformMatrix);
    vcl::updateBoundingBox(matrixMesh);

    auto bbMatrixAfter = matrixMesh.boundingBox();
    std::cout << "After transformation: center = ("
              << bbMatrixAfter.center().x() << ", "
              << bbMatrixAfter.center().y() << ", "
              << bbMatrixAfter.center().z() << ")\n";

    // ========================================
    // PART 4: MESH COMBINATION (APPEND)
    // ========================================

    std::cout << "\n\nPART 4: Mesh Combination (Append)\n";
    std::cout << "----------------------------------\n";

    // Create a main mesh
    vcl::TriMesh combinedMesh = vcl::createCube<vcl::TriMesh>();
    std::cout << "Main mesh (cube): " << combinedMesh.vertexNumber()
              << " vertices, " << combinedMesh.faceNumber() << " faces\n";

    // Create different primitives to add
    vcl::TriMesh sphere1 = vcl::createSphereIcosahedron<vcl::TriMesh>(
        vcl::Sphere<vcl::TriMesh::ScalarType>(
            vcl::Point3<vcl::TriMesh::ScalarType>(0, 0, 0), 1.0),
        3);
    vcl::translate(sphere1, vcl::Point3d(3.0, 0.0, 0.0)); // Move the sphere

    vcl::TriMesh sphere2 = vcl::createSphereUV<vcl::TriMesh>(
        vcl::Sphere<vcl::TriMesh::ScalarType>(
            vcl::Point3<vcl::TriMesh::ScalarType>(0, 0, 0), 1.0),
        16,
        16);
    vcl::translate(
        sphere2, vcl::Point3d(-3.0, 0.0, 0.0)); // Move the other sphere
    vcl::scale(sphere2, 0.8);                   // Reduce the second sphere

    vcl::TriMesh tetrahedron = vcl::createTetrahedron<vcl::TriMesh>();
    vcl::translate(
        tetrahedron, vcl::Point3d(0.0, 3.0, 0.0)); // Move the tetrahedron
    vcl::scale(tetrahedron, 1.5);                  // Enlarge the tetrahedron

    std::cout << "\nPrimitives to add:\n";
    std::cout << "- Icosphere (3 subdivisions): " << sphere1.vertexNumber()
              << " vertices, " << sphere1.faceNumber() << " faces\n";
    std::cout << "- UV Sphere (16x16): " << sphere2.vertexNumber()
              << " vertices, " << sphere2.faceNumber() << " faces\n";
    std::cout << "- Tetrahedron: " << tetrahedron.vertexNumber()
              << " vertices, " << tetrahedron.faceNumber() << " faces\n";

    // Combine meshes using append
    std::cout << "\nCombination through append:\n";

    auto originalCount = combinedMesh.vertexNumber();
    combinedMesh.append(sphere1);
    std::cout << "After appending sphere 1: " << combinedMesh.vertexNumber()
              << " vertices (+" << (combinedMesh.vertexNumber() - originalCount)
              << " from original)\n";

    auto afterSphere1 = combinedMesh.vertexNumber();
    combinedMesh.append(sphere2);
    std::cout << "After appending sphere 2: " << combinedMesh.vertexNumber()
              << " vertices (+" << (combinedMesh.vertexNumber() - afterSphere1)
              << " from sphere 2)\n";

    auto afterSphere2 = combinedMesh.vertexNumber();
    combinedMesh.append(tetrahedron);
    std::cout << "After appending tetrahedron: " << combinedMesh.vertexNumber()
              << " vertices (+" << (combinedMesh.vertexNumber() - afterSphere2)
              << " from tetrahedron)\n";

    std::cout << "\nFinal combined mesh: " << combinedMesh.vertexNumber()
              << " vertices, " << combinedMesh.faceNumber() << " faces\n";

    vcl::updateBoundingBox(combinedMesh);
    auto bbCombined = combinedMesh.boundingBox();
    std::cout << "Bounding box of combined mesh:\n";
    std::cout << "Min: (" << bbCombined.min().x() << ", "
              << bbCombined.min().y() << ", " << bbCombined.min().z() << ")\n";
    std::cout << "Max: (" << bbCombined.max().x() << ", "
              << bbCombined.max().y() << ", " << bbCombined.max().z() << ")\n";

    // ========================================
    // PART 5: SAVING RESULTS
    // ========================================

    std::cout << "\n\nPART 5: Saving Results\n";
    std::cout << "----------------------\n";

    try {
        // Save some of the created meshes
        std::string resultsPath = VCLIB_RESULTS_PATH;

        vcl::save(
            transformMesh, resultsPath + "/004_transformed_icosahedron.ply");
        std::cout << "Saved: 004_transformed_icosahedron.ply (translated "
                     "icosahedron)\n";

        vcl::save(scaleMesh, resultsPath + "/004_scaled_cube.ply");
        std::cout << "Saved: 004_scaled_cube.ply (scaled cube)\n";

        vcl::save(rotateMesh, resultsPath + "/004_rotated_cube.ply");
        std::cout << "Saved: 004_rotated_cube.ply (rotated cube)\n";

        vcl::save(matrixMesh, resultsPath + "/004_matrix_transformed.ply");
        std::cout
            << "Saved: 004_matrix_transformed.ply (matrix transformation)\n";

        vcl::save(combinedMesh, resultsPath + "/004_combined_scene.ply");
        std::cout << "Saved: 004_combined_scene.ply (combined scene)\n";

        std::cout << "\nAll files have been saved to: " << resultsPath << "\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error in saving: " << e.what() << "\n";
    }

    std::cout << "Example completed successfully!\n";

    return 0;
}
