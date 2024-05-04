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

#ifndef VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
#define VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H

#include "vertex_indices.h"
#include "vertex_pointers.h"

namespace vcl::comp {

namespace detail {

template<
    bool INDEXED,
    typename Vertex,
    int N,
    typename ElementType = void,
    bool VERT            = false>
struct VertexReferencesSelector
{
    using type = VertexPointers<Vertex, N, ElementType, VERT>;
};

template<typename Vertex, int N, typename ElementType, bool VERT>
struct VertexReferencesSelector<true, Vertex, N, ElementType, VERT>
{
    using type = VertexIndices<Vertex, N, ElementType, VERT>;
};

} // namespace detail

/**
 * @brief The VertexReferences alias allows to select the correct component
 * between VertexPointers and VertexIndices, depending on the template argument
 * INDEXED.
 *
 * If INDEXED is true, the VertexIndices component is selected, otherwise the
 * VertexPointers component is selected.
 *
 * See the documentation of VertexPointers and VertexIndices for more details.
 *
 * @tparam INDEXED If true, the VertexIndices component is selected, otherwise
 * the VertexPointers component is selected.
 * @tparam Vertex The type of the vertices.
 * @tparam N: The size of the container, that will represent the number of
 * storable vertex pointers. If negative, the container is dynamic.
 * @tparam ElementType: This type is used to get access to the Element that has
 * the component (and, in case, to the Mesh that has the Element). If the
 * component doesn't need to access the Element, this type can be void. Note:
 * if the component is vertical (or optional), this type cannot be void.
 * @tparam VERT: If true, the component will be stored vertically. This argument
 * is considered only if the ElementType is not void.
 *
 * @ingroup components
 */
template<
    bool INDEXED,
    typename Vertex,
    int N,
    typename ElementType = void,
    bool VERT            = false>
using VertexReferences = typename detail::
    VertexReferencesSelector<INDEXED, Vertex, N, ElementType, VERT>::type;

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_VERTEX_REFERENCES_H
