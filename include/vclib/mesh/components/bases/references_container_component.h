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

#ifndef VCL_MESH_COMPONENTS_BASES_REFERENCES_CONTAINER_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASES_REFERENCES_CONTAINER_COMPONENT_H

#include "indices_container_component.h"
#include "pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The ReferencesContainerComponent is an alias to @ref
 * IndicesContainerComponent or @ref PointersContainerComponent classes,
 * depending on the STORE_INDICES template parameter.
 *
 * @tparam STORE_INDICES: If true, the component will store indices, otherwise
 * pointers to Elem.
 * @tparam DerivedComponent: The type of the Derived Component. It is used to
 * implement the CRTP pattern.
 * @tparam COMP_ID: The id of the component. It is a value of the enum
 * ComponentIDEnum, or an integer value that is not already used by any other
 * component. It is used to identify the component at compile time.
 * @tparam Elem: The type of the Element of which the pointers are stored. E.g.
 * a VertexPointers component would have VertexType as Elem.
 * @tparam N: The size of the container: if >= 0 the size is static, if < 0 the
 * size is dynamic.
 * @tparam ParentElemType: The Parent Element type is used to get access to the
 * Element that has the component (and, in case, to the Mesh that has the
 * Element). If the component doesn't need to access the Element, this type can
 * be void. Note: if the component is vertical (or optional), this type cannot
 * be void.
 * @tparam VERT: Boolean that tells if the component is vertical. If the
 * component is vertical, this parameter must be true. Note: to be vertical,
 * this parameter must be true, and ElementType must be the type of the Element
 * that has the component (the 'parent' Element Type).
 * @tparam OPT: When a component is vertical, it could be optional, that means
 * that could be enabled/disabled at runtime. To make the component optional,
 * this template parameter must be true.
 * @tparam TTVN: "Tied To Vertex Number", this boolean tells whether the
 * component should leave liberty to control the size of the Container to the
 * user, or it should be tied to the vertex number and changed automatically
 * when the number of vertices changes. E.g. a AdjacentFaces component, if part
 * of a Face element, should be tied to the vertex number of the Face, meaning
 * that the number of face pointers should be always be the same of the number
 * of vertices. Setting this value to true will tie the size of this container
 * to the number of vertices.
 */
template<
    bool STORE_INDICES,
    typename DerivedComponent, // CRTP pattern, derived class
    uint COMP_ID,              // component id
    typename Elem,             // element type for which the pointers are stored
    int N,                     // container size
    typename ParentElemType,   // parent element type
    bool VERT,                 // true if component vertical
    bool OPT,                  // true if component vertical and optional
    bool TTVN>                 // true if container size tied to vertex number
using ReferencesContainerComponent = std::conditional_t<
    STORE_INDICES,
    IndicesContainerComponent<
        DerivedComponent,
        COMP_ID,
        Elem,
        N,
        ParentElemType,
        VERT,
        OPT,
        TTVN>,
    PointersContainerComponent<
        DerivedComponent,
        COMP_ID,
        Elem,
        N,
        ParentElemType,
        VERT,
        OPT,
        TTVN>>;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASES_POINTERS_CONTAINER_COMPONENT_H
