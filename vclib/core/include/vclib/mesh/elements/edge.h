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

#ifndef VCL_MESH_ELEMENTS_EDGE_H
#define VCL_MESH_ELEMENTS_EDGE_H

#include "edge_components.h"
#include "element.h"

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

// template<typename T>
// concept EdgeConcept = std::derived_from< // same type or derived type
//     std::remove_cvref_t<T>,
//     Edge<typename RemoveRef<T>::ParentMeshType,
//          typename RemoveRef<T>::Components>>;

/**
 * @brief The EdgeConcept describes how a Edge element that can be
 * used for a EdgeContainer should be organized.
 *
 * The Edge concept is satisfied for a class E if ALL the following
 * sentences are true:
 * - The class E has the BitFlags component (or an equivalent one);
 * - The class E has the either VertexPointers or VertexIndices components;
 * - The number of vertices of the VertexPointers/VertexIndices is 2.
 *
 * @ingroup edge_concepts
 */
template<typename T>
concept EdgeConcept =
    ElementConcept<T> && RemoveRef<T>::ELEMENT_ID == ElemId::EDGE &&
    edge::HasBitFlags<T> && edge::HasVertexReferences<T> &&
    RemoveRef<T>::VERTEX_NUMBER == 2;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_H
