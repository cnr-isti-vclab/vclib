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

#ifndef VCL_MESH_ELEMENTS_VERTEX_H
#define VCL_MESH_ELEMENTS_VERTEX_H

#include "element.h"

#include <vclib/concepts/mesh/elements/vertex.h>

namespace vcl {

/**
 * @brief The Vertex class represents an Vertex element of the vcl::Mesh class.
 *
 * Using the VertexContainer class, it is possible to add a vector of Vertex
 * elements to a mesh, and manage them with the member functions exposed by the
 * VertexContainer. Each Vertex element exposes all the member functions of its
 * Component types.
 *
 * @tparam MeshType The type of the parent mesh.
 * @tparam Comps The types of the components of the element.
 *
 * @ingroup elements
 */
template<typename MeshType, typename... Comps>
class Vertex : public Element<ElemId::VERTEX, MeshType, Comps...>
{
};

template<typename MeshType, typename... Comps>
class Vertex<MeshType, TypeWrapper<Comps...>> :
        public Vertex<MeshType, Comps...>
{
public:
    /**
     * @brief Empty constructor.
     *
     * Calls automatically all the empty constructors of all the components
     * available in the Vertex (for all the components non-available, their
     * empty constructor is called only when they become available).
     */
    Vertex() = default;
};

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_VERTEX_H
