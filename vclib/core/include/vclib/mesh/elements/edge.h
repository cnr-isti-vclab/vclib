// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_EDGE_H
#define VCL_MESH_ELEMENTS_EDGE_H

#include "base/element.h"
#include "edge_components.h"

namespace vcl {

/**
 * @brief The Edge class represents an Edge element of the
 * vcl::Mesh class.
 *
 * Using the EdgeContainer class, it is possible to add a vector of
 * Edge elements to a mesh, and manage them with the member functions
 * exposed by the EdgeContainer. Each Edge element exposes
 * all the member functions of its Component types.
 *
 * @tparam MeshType The type of the parent mesh.
 * @tparam Comps The types of the components of the element.
 *
 * @ingroup elements
 */
template<typename MeshType, comp::ComponentConcept... Comps>
class Edge : public Element<ElemId::EDGE, MeshType, Comps...>
{
    // VertexPointers or VertexIndices component of the Edge
    using VRefs = typename Edge::VertexReferences;

public:
    using VertexType = VRefs::VertexType;
    using VRefs::setVertices;

    /**
     * @brief Empty constructor.
     *
     * Calls automatically all the empty constructors of all the components
     * available in the Edge (for all the components non-available, their empty
     * constructor is called only when they become available).
     */
    Edge() = default;

    /**
     * @brief Swap function that delegates to the base Element swap.
     *
     * This friend function enables ADL (Argument Dependent Lookup) to find
     * the correct swap function when swapping Edge objects.
     */
    friend void swap(Edge& a, Edge& b) { a.swap(b); }

    /**
     * @brief Sets the vertices of the edge.
     *
     * @param [in] v0: the first vertex of the edge.
     * @param [in] v1: the second vertex of the edge.
     */
    void setVertices(VertexType* v0, VertexType* v1)
    {
        VRefs::setVertex(0u, v0);
        VRefs::setVertex(1u, v1);
    }

    /**
     * @brief Sets the vertices of the edge.
     *
     * @param [in] vi0: the index first vertex of the edge.
     * @param [in] vi1: the index second vertex of the edge.
     */
    void setVertices(uint vi0, uint vi1)
    {
        VRefs::setVertex(0u, vi0);
        VRefs::setVertex(1u, vi1);
    }
};

template<typename MeshType, comp::ComponentConcept... Comps>
class Edge<MeshType, TypeWrapper<Comps...>> : public Edge<MeshType, Comps...>
{
};

/* Concepts */

/**
 * @brief A concept that checks whether a class has (inherits from) an
 * Edge class.
 *
 * The concept is satisfied when `T` is a class that instantiates or derives
 * from a Edge class having any ParentMesh type and any Component types.
 * The concept checks also that the Edge has a BitFlags component,
 * VertexPointers or VertexIndices components, and that the number of vertices
 * is 2.
 *
 * @tparam T: The type to be tested for conformity to the EdgeConcept.
 *
 * @ingroup edge_concepts
 */
template<typename T>
concept EdgeConcept =
    IsDerivedFromSpecializationOfV<T, Edge> &&
    RemoveRef<T>::ELEMENT_ID == ElemId::EDGE && edge::HasBitFlags<T> &&
    edge::HasVertexReferences<T> && RemoveRef<T>::VERTEX_COUNT == 2;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_H
