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
// 1. Add custom components to mesh elements (vertices, faces)
// 2. Add custom components to the mesh itself
// 3. Use custom components for algorithm development and data storage

#include <iostream>
#include <string>

#include <vclib/algorithms.h>
#include <vclib/io.h>
#include <vclib/meshes.h>

int main()
{
    std::cout << "=== VCLib Example 009: Mesh Custom Components ===\n\n";

    // ========================================
    // PART 1: LOADING MESH AND BASIC SETUP
    // ========================================

    std::cout << "PART 1: Loading Mesh and Basic Setup\n";
    std::cout << "-------------------------------------\n";

    // Load a mesh for custom component demonstration
    vcl::TriMesh mesh;
    vcl::load(mesh, VCLIB_EXAMPLE_MESHES_PATH "/bimba_simplified.obj");

    std::cout << "Loaded mesh: " << mesh.vertexNumber() << " vertices, "
              << mesh.faceNumber() << " faces\n\n";

    // ========================================
    // PART 2: VERTEX CUSTOM COMPONENTS
    // ========================================

    std::cout << "PART 2: Vertex Custom Components\n";
    std::cout << "---------------------------------\n";

    // Add custom integer component for vertex processing flags
    mesh.addPerVertexCustomComponent<int>("processingFlag");
    std::cout << "Added per-vertex custom component 'processingFlag'\n";

    // Check if component exists
    std::cout << "Has processingFlag component: " 
              << mesh.hasPerVertexCustomComponent("processingFlag") << "\n";

    // Set values for processing flags
    for (auto& vertex : mesh.vertices()) {
        // Mark boundary vertices differently (simplified check)
        bool isBoundary = (vertex.index() % 10 == 0); // Simplified boundary detection
        vertex.customComponent<int>("processingFlag") = isBoundary ? 1 : 0;
    }

    // Count flagged vertices

    // another way to access custom components, is to use the handle
    auto vertexFlagHandle =
        mesh.perVertexCustomComponentVectorHandle<int>("processingFlag");

    int boundaryCount = 0;
    for (const auto& f : vertexFlagHandle) {
        if (f == 1) {
            boundaryCount++;
        }
    }
    std::cout << "Marked " << boundaryCount << " vertices as boundary candidates\n";

    // Add custom Point3f component for storing original positions
    mesh.addPerVertexCustomComponent<vcl::Point3f>("originalPosition");
    std::cout << "Added per-vertex custom component 'originalPosition'\n";

    // Store original positions before any transformation
    for (auto& vertex : mesh.vertices()) {
        vertex.customComponent<vcl::Point3f>("originalPosition") = 
            vertex.position().cast<float>();
    }

    // Add custom double component for vertex quality/error metrics
    mesh.addPerVertexCustomComponent<double>("quality");
    std::cout << "Added per-vertex custom component 'quality'\n";

    // Calculate a simple quality metric (distance from centroid)
    auto centroid = vcl::barycenter(mesh);
    for (auto& vertex : mesh.vertices()) {
        double distFromCenter = (vertex.position() - centroid).norm();
        vertex.customComponent<double>("quality") = distFromCenter;
    }

    // Find min/max quality values
    auto qualityHandle =
        mesh.perVertexCustomComponentVectorHandle<double>("quality");
    auto minMaxQuality =
        std::minmax_element(qualityHandle.begin(), qualityHandle.end());
    std::cout << "Quality range: [" << *minMaxQuality.first 
              << ", " << *minMaxQuality.second << "]\n\n";

    // ========================================
    // PART 3: FACE CUSTOM COMPONENTS
    // ========================================

    std::cout << "PART 3: Face Custom Components\n";
    std::cout << "------------------------------\n";

    // Add custom component for face areas
    mesh.addPerFaceCustomComponent<double>("area");
    std::cout << "Added per-face custom component 'area'\n";

    // Calculate and store face areas
    for (auto& face : mesh.faces()) {
        face.customComponent<double>("area") = vcl::faceArea(face);
    }

    // Add custom component for face material IDs
    mesh.addPerFaceCustomComponent<int>("materialID");
    std::cout << "Added per-face custom component 'materialID'\n";

    // Assign material IDs based on face area
    auto faceAreaHandle =
        mesh.perFaceCustomComponentVectorHandle<double>("area");
    double totalArea =
        std::accumulate(faceAreaHandle.begin(), faceAreaHandle.end(), 0.0);
    double avgArea = totalArea / mesh.faceNumber();

    for (auto& face : mesh.faces()) {
        double area = face.customComponent<double>("area");
        int    materialID = (area > avgArea) ?
                                1 :
                                0; // Large faces = material 1, small = material 0
        face.customComponent<int>("materialID") = materialID;
    }

    // Count faces per material
    int material0Count = 0, material1Count = 0;
    for (const auto& face : mesh.faces()) {
        if (face.customComponent<int>("materialID") == 0) {
            material0Count++;
        } else {
            material1Count++;
        }
    }
    std::cout << "Material assignment: " << material0Count << " small faces, " 
              << material1Count << " large faces\n";

    // Add custom string component for face labels
    mesh.addPerFaceCustomComponent<std::string>("label");
    std::cout << "Added per-face custom component 'label'\n";

    // Assign descriptive labels
    for (auto& face : mesh.faces()) {
        int materialID = face.customComponent<int>("materialID");
        
        std::string label = (materialID == 1) ? "large_face" : "small_face";
        label += "_" + std::to_string(face.index());
        
        face.customComponent<std::string>("label") = label;
    }

    std::cout << "First 3 face labels: ";
    for (int i = 0; i < 3 && i < mesh.faceNumber(); ++i) {
        std::cout << "\"" << mesh.face(i).customComponent<std::string>("label") << "\" ";
    }
    std::cout << "\n\n";

    // ========================================
    // PART 4: MESH-LEVEL CUSTOM COMPONENTS
    // ========================================

    std::cout << "PART 4: Mesh-Level Custom Components\n";
    std::cout << "------------------------------------\n";

    // Add mesh-level metadata
    mesh.addCustomComponent<std::string>("modelName", "Bimba Model");
    mesh.addCustomComponent<int>("processingVersion", 1);
    mesh.addCustomComponent<double>("meshQuality", 0.85);
    
    std::cout << "Added mesh-level custom components:\n";
    std::cout << "  Model name: " << mesh.customComponent<std::string>("modelName") << "\n";
    std::cout << "  Processing version: " << mesh.customComponent<int>("processingVersion") << "\n";
    std::cout << "  Mesh quality: " << mesh.customComponent<double>("meshQuality") << "\n";

    // Add mesh statistics as custom components
    double totalSurfaceArea = std::accumulate(faceAreaHandle.begin(), faceAreaHandle.end(), 0.0);
    mesh.addCustomComponent<double>("totalSurfaceArea", totalSurfaceArea);
    
    vcl::updateBoundingBox(mesh);
    auto bbox = mesh.boundingBox();
    mesh.addCustomComponent<vcl::Point3d>("boundingBoxSize", bbox.max() - bbox.min());
    mesh.addCustomComponent<vcl::Point3d>("meshCentroid", centroid);

    std::cout << "\nMesh statistics stored as custom components:\n";
    std::cout << "  Total surface area: " << mesh.customComponent<double>("totalSurfaceArea") << "\n";
    std::cout << "  Bounding box size: " << mesh.customComponent<vcl::Point3d>("boundingBoxSize") << "\n";
    std::cout << "  Mesh centroid: " << mesh.customComponent<vcl::Point3d>("meshCentroid") << "\n\n";

    // ========================================
    // PART 5: USING CUSTOM COMPONENTS IN ALGORITHMS
    // ========================================

    std::cout << "PART 5: Using Custom Components in Algorithms\n";
    std::cout << "---------------------------------------------\n";

    // Example algorithm: smooth only vertices marked for processing
    std::cout << "Applying selective smoothing based on processing flags...\n";
    
    // Store positions before smoothing
    for (auto& vertex : mesh.vertices()) {
        vertex.customComponent<vcl::Point3f>("originalPosition") = 
            vertex.position().cast<float>();
    }

    // Simple Laplacian smoothing for flagged vertices only
    std::vector<vcl::Point3d> newPositions(mesh.vertexNumber());
    
    // Copy current positions
    for (const auto& vertex : mesh.vertices()) {
        newPositions[vertex.index()] = vertex.position();
    }

    // Apply smoothing only to flagged vertices
    mesh.enablePerVertexAdjacentVertices();
    vcl::updatePerVertexAdjacentVertices(mesh);
    
    for (const auto& vertex : mesh.vertices()) {
        if (vertex.customComponent<int>("processingFlag") == 1) {
            // Simple averaging with adjacent vertices
            vcl::Point3d avgPos(0, 0, 0);
            int adjCount = 0;
            
            for (const auto* adjVertex : vertex.adjVertices()) {
                avgPos += adjVertex->position();
                adjCount++;
            }
            
            if (adjCount > 0) {
                avgPos /= adjCount;
                // Blend with original position
                newPositions[vertex.index()] = 0.7 * vertex.position() + 0.3 * avgPos;
            }
        }
    }

    // Apply new positions
    for (auto& vertex : mesh.vertices()) {
        vertex.position() = newPositions[vertex.index()];
    }

    // Calculate displacement for quality assessment
    double totalDisplacement = 0.0;
    for (const auto& vertex : mesh.vertices()) {
        auto originalPos = vertex.customComponent<vcl::Point3f>("originalPosition").cast<double>();
        double displacement = (vertex.position() - originalPos).norm();
        totalDisplacement += displacement;
    }
    double avgDisplacement = totalDisplacement / mesh.vertexNumber();
    
    std::cout << "Average vertex displacement: " << avgDisplacement << "\n";
    mesh.addCustomComponent<double>("averageDisplacement", avgDisplacement);

    // save the mesh after processing
    // note: ply format is able to save custom components having primitive types
    vcl::save(mesh, VCLIB_RESULTS_PATH "/009_processed_bimba.ply");

    // ========================================
    // PART 6: COMPONENT MANAGEMENT
    // ========================================

    std::cout << "\n\nPART 6: Component Management\n";
    std::cout << "----------------------------\n";

    // List all custom components
    std::cout << "Current custom components:\n";
    std::cout << "Vertex components:\n";
    // Note: VCLib doesn't provide direct enumeration of component names
    // In practice, you would keep track of component names in your application
    
    std::cout << "  - processingFlag (int): processing status flags\n";
    std::cout << "  - originalPosition (Point3f): original vertex positions\n";
    std::cout << "  - quality (double): vertex quality metrics\n";
    
    std::cout << "Face components:\n";
    std::cout << "  - area (double): face areas\n";
    std::cout << "  - materialID (int): material identifiers\n";
    std::cout << "  - label (string): descriptive labels\n";
    
    std::cout << "Mesh components:\n";
    std::cout << "  - modelName (string): model identifier\n";
    std::cout << "  - processingVersion (int): processing version\n";
    std::cout << "  - meshQuality (double): overall quality metric\n";
    std::cout << "  - totalSurfaceArea (double): total surface area\n";
    std::cout << "  - boundingBoxSize (Point3d): bounding box dimensions\n";
    std::cout << "  - meshCentroid (Point3d): geometric center\n";
    std::cout << "  - averageDisplacement (double): smoothing displacement\n";

    // Type checking
    std::cout << "\nType checking examples:\n";
    std::cout << "  processingFlag is int: " 
              << mesh.isPerVertexCustomComponentOfType<int>("processingFlag") << "\n";
    std::cout << "  processingFlag is double: " 
              << mesh.isPerVertexCustomComponentOfType<double>("processingFlag") << "\n";
    std::cout << "  area is double: " 
              << mesh.isPerFaceCustomComponentOfType<double>("area") << "\n";

    // Remove a component (cleanup example)
    mesh.deletePerVertexCustomComponent("processingFlag");
    std::cout << "\nRemoved processingFlag component\n";
    std::cout << "Has processingFlag component: " 
              << mesh.hasPerVertexCustomComponent("processingFlag") << "\n";

    // ========================================
    // SUMMARY
    // ========================================

    std::cout << "\n\n=== EXAMPLE SUMMARY ===\n";
    std::cout << "\nIn this example we learned:\n\n";

    std::cout << "1. VERTEX CUSTOM COMPONENTS:\n";
    std::cout << "   - addPerVertexCustomComponent<T>(name): add typed component\n";
    std::cout << "   - vertex.customComponent<T>(name): access component data\n";
    std::cout << "   - perVertexCustomComponentVectorHandle<T>(): bulk access\n\n";

    std::cout << "2. FACE CUSTOM COMPONENTS:\n";
    std::cout << "   - addPerFaceCustomComponent<T>(name): add typed component\n";
    std::cout << "   - face.customComponent<T>(name): access component data\n";
    std::cout << "   - Support for various types: int, double, string, Point3d\n\n";

    std::cout << "3. MESH-LEVEL COMPONENTS:\n";
    std::cout << "   - addCustomComponent<T>(name, value): add mesh metadata\n";
    std::cout << "   - mesh.customComponent<T>(name): access mesh data\n";
    std::cout << "   - Store global properties and statistics\n\n";

    std::cout << "4. COMPONENT MANAGEMENT:\n";
    std::cout << "   - hasPerVertexCustomComponent(name): check existence\n";
    std::cout << "   - isPerVertexCustomComponentOfType<T>(name): type checking\n";
    std::cout << "   - deletePerVertexCustomComponent(name): remove component\n\n";

    std::cout << "Custom components are essential for:\n";
    std::cout << "- Algorithm development and intermediate data storage\n";
    std::cout << "- Mesh annotation and metadata management\n";
    std::cout << "- Quality metrics and analysis results\n";
    std::cout << "- Application-specific data attachment\n";
    std::cout << "- Multi-stage processing pipelines\n\n";

    std::cout << "Example completed successfully!\n";

    return 0;
}
