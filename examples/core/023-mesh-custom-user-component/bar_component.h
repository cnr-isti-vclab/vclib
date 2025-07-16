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

#ifndef BAR_COMPONENT_H
#define BAR_COMPONENT_H

#include <vclib/concepts/const_correctness.h>
#include <vclib/concepts/mesh/components/component.h>
#include <vclib/mesh/components/bases/component.h>
#include <vclib/mesh/mesh.h>
#include <vclib/types.h>

#include <iostream>
#include <vector>

// =============================================================================
// BAR COMPONENT - Advanced Optional Component Implementation
// =============================================================================

// Concept to check if an element/mesh has the Bar component
template<typename T>
concept HasBarComponent = requires (T&& obj) {
    // Check for both data accessors in the Bar component
    { obj.bar() } -> std::convertible_to<double>;
    { obj.barVector() } -> std::convertible_to<std::vector<vcl::uint>>;

    // For non-const objects, ensure accessors return modifiable references
    requires vcl::IsConst<T> || requires {
        { obj.bar() } -> std::same_as<double&>;
        { obj.barVector() } -> std::same_as<std::vector<vcl::uint>&>;
    };
};

// Component ID for Bar component (different from Foo component)
inline const vcl::uint BAR_COMPONENT = vcl::CompId::COMPONENTS_NUMBER + 1;

namespace detail {
// Complex components often use a data structure to organize multiple fields
struct BarData
{
    double                 bar       = 0.0; // Floating-point value
    std::vector<vcl::uint> barVector = {};  // Vector of unsigned integers
};
} // namespace detail

// Forward declaration for runtime availability checking
bool isBarComponentAvailableOn(const vcl::ElementOrMeshConcept auto& element);

// Advanced component using VCLib's Component base class
// This approach enables optional (runtime-controlled) components
template<
    typename ElementType = void,  // Element type (void for horizontal only)
    bool VERTICAL        = false, // True for vertical components
    bool OPTIONAL        = false>        // True for optional components
class BarComponentT :
        public vcl::comp::Component<
            BarComponentT<ElementType, VERTICAL, OPTIONAL>, // CRTP pattern
            BAR_COMPONENT,                                  // Component ID
            detail::BarData,                                // Data structure
            ElementType,                                    // Element type
            VERTICAL,                                       // Vertical flag
            OPTIONAL>                                       // Optional flag
{
    using Base = vcl::comp::Component<
        BarComponentT<ElementType, VERTICAL, OPTIONAL>,
        BAR_COMPONENT,
        detail::BarData,
        ElementType,
        VERTICAL,
        OPTIONAL>;

public:
    // Component interface: access data fields through Base::data()
    double& bar() { return Base::data().bar; }

    double bar() const { return Base::data().bar; }

    std::vector<vcl::uint>& barVector() { return Base::data().barVector; }

    const std::vector<vcl::uint>& barVector() const
    {
        return Base::data().barVector;
    }

protected:
    // Import function with both compile-time and runtime checks
    template<typename Element>
    void importFrom(const Element& e, bool = true)
    {
        // Compile-time check: does source element have Bar component?
        if constexpr (HasBarComponent<Element>) {
            // Runtime check: is the component actually enabled?
            // (Important for optional components)
            if (isBarComponentAvailableOn(e)) {
                bar()       = e.bar();
                barVector() = e.barVector();
            }
        }
    }
};

// Runtime availability checker for optional components
bool isBarComponentAvailableOn(const vcl::ElementOrMeshConcept auto& element)
{
    return vcl::comp::isComponentAvailableOn<BAR_COMPONENT>(element);
}

// Convenient type aliases for different usage patterns
using BarComponent = BarComponentT<>; // Simple horizontal component

template<typename ElementType>
using VerticalBarComponent =
    BarComponentT<ElementType, true>; // Vertical component

template<typename ElementType>
using OptionalBarComponent =
    BarComponentT<ElementType, true, true>; // Optional component

// Compile-time validation
static_assert(vcl::comp::ComponentConcept<BarComponent>);
static_assert(HasBarComponent<BarComponent>);

// =============================================================================
// MESH DEFINITION WITH BAR COMPONENT
// =============================================================================

// Forward declaration
class BarMesh;

namespace barmesh {

// Vertex with optional Bar component
class Vertex :
        public vcl::Vertex<
            BarMesh,                      // Parent mesh type
            vcl::vert::BitFlags,          // Standard vertex flags
            vcl::vert::Position3d,        // 3D position
            vcl::vert::Normal3d,          // 3D normal vector
            vcl::vert::Color,             // Color information
            OptionalBarComponent<Vertex>> // Optional Bar component
{
};

// Standard triangle face
class Face :
        public vcl::Face<
            BarMesh,
            vcl::face::TriangleBitFlags,
            vcl::face::TriangleVertexPtrs<Vertex, Face>,
            vcl::face::Normal3d>
{
};

} // namespace barmesh

// Complete mesh type with optional Bar component in vertices
class BarMesh :
        public vcl::Mesh<
            vcl::mesh::VertexContainer<barmesh::Vertex>,
            vcl::mesh::FaceContainer<barmesh::Face>,
            vcl::mesh::BoundingBox3d>
{
public:
    using ScalarType = double;
};

// =============================================================================
// DEMONSTRATION FUNCTION
// =============================================================================

inline void demonstrateBarComponent()
{
    std::cout << "\n--- Bar Component (Optional) ---" << std::endl;

    // Create a mesh with optional Bar component
    BarMesh mesh;
    mesh.addVertices(3);

    // IMPORTANT: Enable the Bar component before using it
    // Optional components must be explicitly enabled at runtime
    std::cout << "Enabling Bar component..." << std::endl;
    mesh.enablePerElementComponent<vcl::ElemId::VERTEX, BAR_COMPONENT>();

    // Set values in the Bar component
    std::cout << "Setting Bar component values..." << std::endl;
    for (auto& vertex : mesh.vertices()) {
        vertex.bar()       = vertex.index() * 3.14 + 1.0;
        vertex.barVector() = {
            vertex.index(), vertex.index() + 1, vertex.index() + 2};
    }

    // Display the values stored in both Bar component fields
    std::cout << "Bar component values:" << std::endl;
    for (const auto& vertex : mesh.vertices()) {
        std::cout << "  Vertex " << vertex.index() << ":" << std::endl;
        std::cout << "    bar = " << vertex.bar() << std::endl;
        std::cout << "    barVector = [";
        for (size_t i = 0; i < vertex.barVector().size(); ++i) {
            std::cout << vertex.barVector()[i];
            if (i < vertex.barVector().size() - 1)
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;
    }

    // Demonstrate optional component runtime checking
    std::cout << "\nTesting optional component availability..." << std::endl;
    for (const auto& vertex : mesh.vertices()) {
        bool available = isBarComponentAvailableOn(vertex);
        std::cout << "  Vertex " << vertex.index()
                  << " has Bar component: " << (available ? "Yes" : "No")
                  << std::endl;
    }

    // Test component transfer between meshes
    BarMesh mesh2;
    mesh2.addVertices(2);
    // Enable Bar component on destination mesh too
    mesh2.enablePerElementComponent<vcl::ElemId::VERTEX, BAR_COMPONENT>();

    std::cout << "\nImporting Bar component data..." << std::endl;
    mesh2.importFrom(mesh);

    std::cout << "Imported mesh Bar component values:" << std::endl;
    for (const auto& vertex : mesh2.vertices()) {
        if (isBarComponentAvailableOn(vertex)) {
            std::cout << "  Vertex " << vertex.index()
                      << ": bar = " << vertex.bar() << std::endl;
        }
    }
}

#endif // BAR_COMPONENT_H
