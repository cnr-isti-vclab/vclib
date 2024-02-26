/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
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

#include <vclib/concepts/mesh/elements/edge.h>

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
template<typename MeshType, typename... Comps>
class Edge : public Element<ElemId::EDGE, MeshType, Comps...>
{
    // VertexPointers component of the Edge
    using VPtrs = typename Edge::VertexPointers;

public:
    using VertexType = VPtrs::VertexType;
    using VPtrs::setVertices;

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
        VPtrs::vertex(0) = v0;
        VPtrs::vertex(1) = v1;
    }
};

template<typename MeshType, typename... Comps>
class Edge<MeshType, TypeWrapper<Comps...>> : public Edge<MeshType, Comps...>
{
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_H
