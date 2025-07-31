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

#ifndef FOO_COMPONENT_H
#define FOO_COMPONENT_H

#include <vclib/mesh.h>
#include <vclib/meshes.h>
#include <vclib/types.h>

#include <iostream>

// =============================================================================
// FOO COMPONENT - Simple Horizontal Component Implementation
// =============================================================================

// Concept to check if an element/mesh has the Foo component
// This enables compile-time type checking and safe component access
template<typename T>
concept HasFooComponent = requires (T&& obj) {
    // Verify that foo() accessor exists and returns an int-compatible type
    { obj.foo() } -> std::convertible_to<int>;

    // For non-const objects, ensure foo() returns a modifiable reference
    requires vcl::IsConst<T> || requires {
        { obj.foo() } -> std::same_as<int&>;
    };
};

// Simple horizontal component that stores an integer value
class FooComponent
{
public:
    // Unique component ID: must not conflict with VCLib's built-in components
    static const unsigned int COMPONENT_ID = vcl::CompId::COMPONENTS_NUMBER + 0;

    // Component data accessors
    int& foo() { return data; }

    int foo() const { return data; }

protected:
    // Required by VCLib: enables mesh copying and conversion operations
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        // Only import if the source element also has a Foo component
        if constexpr (HasFooComponent<Element>) {
            data = e.foo();
        }
        // If source doesn't have Foo component, data keeps its default value
    }

private:
    int data = 0; // Component data with default initialization
};

// Register component with VCLib's string system for debugging/reflection
namespace vcl {
template<>
struct ComponentString<FooComponent::COMPONENT_ID>
{
    const char* str = "FooComponent";
};
} // namespace vcl

// Compile-time validation
static_assert(vcl::comp::ComponentConcept<FooComponent>);
static_assert(HasFooComponent<FooComponent>);

// =============================================================================
// MESH DEFINITION WITH FOO COMPONENT
// =============================================================================

// Forward declaration
class FooMesh;

namespace foomesh {

// Vertex definition: extends VCLib's Vertex with FooComponent
class Vertex :
        public vcl::Vertex<
            FooMesh,               // Parent mesh type
            vcl::vert::BitFlags,   // Standard vertex flags
            vcl::vert::Position3d, // 3D position
            vcl::vert::Normal3d,   // 3D normal vector
            vcl::vert::Color,      // Color information
            FooComponent>          // Our custom Foo component
{
};

// Face definition: standard triangle face
class Face :
        public vcl::Face<
            FooMesh,
            vcl::face::TriangleBitFlags,
            vcl::face::TriangleVertexPtrs<Vertex, Face>,
            vcl::face::Normal3d>
{
};

} // namespace foomesh

// Complete mesh type with FooComponent in vertices
class FooMesh :
        public vcl::Mesh<
            vcl::mesh::VertexContainer<foomesh::Vertex>,
            vcl::mesh::FaceContainer<foomesh::Face>,
            vcl::mesh::BoundingBox3d>
{
public:
    using ScalarType = double;
};

// =============================================================================
// DEMONSTRATION FUNCTION
// =============================================================================

inline void demonstrateFooComponent()
{
    std::cout << "\n--- Foo Component (Simple Horizontal) ---" << std::endl;

    // Create a mesh with Foo component in vertices
    FooMesh mesh;
    mesh.addVertices(5);

    // Set values in the Foo component using the foo() accessor
    std::cout << "Setting Foo component values..." << std::endl;
    for (auto& vertex : mesh.vertices()) {
        vertex.foo() = mesh.index(vertex) * 10 + 42;
    }

    // Display the values stored in the Foo component
    std::cout << "Foo component values:" << std::endl;
    for (const auto& vertex : mesh.vertices()) {
        std::cout << "  Vertex " << vertex.index() << ": foo = " << vertex.foo()
                  << std::endl;
    }

    // Test compatibility with standard VCLib meshes
    vcl::TriMesh standardMesh;
    standardMesh.addVertices(3);

    std::cout << "\nImporting from FooMesh to TriMesh..." << std::endl;
    // The Foo component will NOT be copied because TriMesh doesn't have it
    // Only common components (position, normal, etc.) are transferred
    standardMesh.importFrom(mesh);
    std::cout << "TriMesh now has " << standardMesh.vertexNumber()
              << " vertices" << std::endl;

    std::cout << "\nImporting back from TriMesh to FooMesh..." << std::endl;
    // This works but Foo component values will be initialized to default (0)
    FooMesh mesh2;
    mesh2.importFrom(standardMesh);
    mesh2.addVertices(2); // Add some vertices to see default Foo values

    for (const auto& vertex : mesh2.vertices()) {
        std::cout << "  Vertex " << vertex.index() << ": foo = " << vertex.foo()
                  << " (default value)" << std::endl;
    }
}

#endif // FOO_COMPONENT_H
