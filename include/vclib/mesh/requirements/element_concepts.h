/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_ELEMENT_CONCEPTS_H
#define VCL_MESH_ELEMENT_CONCEPTS_H

#include "../containers/containers_concepts.h"

namespace vcl {

/**
 * @brief HasVertices concepts is satisfied when at least one of its types is (or inherits from)
 * a VertexContainer. It can be used both to check if a Mesh has vertices, or if in a list of types
 * there is a VertexContainer.
 *
 * In the following example, a MyMesh type can be instatiated only if one of its template Args is a
 * VertexContainer:
 * @code{.cpp}
 * template <typename... Args> requires HasVertices<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // ok
 * MyMesh<vcl::FaceContainer<MyFace>> m2; // not ok
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::FaceContainer<MyFace>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) VertexContainer:
 * @code{.cpp}
 * if constexpr (vcl::HasVertices<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 */
template<typename... Args>
concept HasVertices = (vcl::mesh::HasVertexContainer<Args> || ...);

/**
 * @brief HasFaces concepts is satisfied when at least one of its types is (or inherits from)
 * a FaceContainer. It can be used both to check if a Mesh has faces, or if in a list of types
 * there is a FaceContainer.
 *
 * In the following example, a MyMesh type can be instatiated only if one of its template Args is a
 * FaceContainer:
 * @code{.cpp}
 * template <typename... Args> requires HasFaces<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // not ok
 * MyMesh<vcl::FaceContainer<MyFace>> m2; // ok
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::FaceContainer<MyFace>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) FaceContainer:
 * @code{.cpp}
 * if constexpr (vcl::HasFaces<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 */
template<typename... Args>
concept HasFaces = (vcl::mesh::HasFaceContainer<Args> || ...);

/**
 * @brief HasEdges concepts is satisfied when at least one of its types is (or inherits from)
 * a EdgeContainer. It can be used both to check if a Mesh has edges, or if in a list of types
 * there is a EdgeContainer.
 *
 * In the following example, a MyMesh type can be instatiated only if one of its template Args is a
 * EdgeContainer:
 * @code{.cpp}
 * template <typename... Args> requires HasEdges<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // not ok
 * MyMesh<vcl::EdgeContainer<MyEdge>> m2; // ok
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::EdgeContainer<MyEdge>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) EdgeContainer:
 * @code{.cpp}
 * if constexpr (vcl::HasEdges<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 */
template<typename... Args>
concept HasEdges = (vcl::mesh::HasEdgeContainer<Args> || ...);

/**
 * @brief HasHalfEdges concepts is satisfied when at least one of its types is (or inherits from)
 * a HalfEdgeContainer. It can be used both to check if a Mesh has half edges, or if in a list of
 * types there is a HalfEdgeContainer.
 *
 * In the following example, a MyMesh type can be instatiated only if one of its template Args is a
 * HalfEdgeContainer:
 * @code{.cpp}
 * template <typename... Args> requires HasHalfEdges<Args...>
 * class MyMesh {
 *     // ...
 * };
 *
 * // ...
 *
 * MyMesh<vcl::VertexContainer<MyVertex>> m1; // not ok
 * MyMesh<vcl::HalfEdgeContainer<MyHalfEdge>> m2; // ok
 * MyMesh<vcl::VertexContainer<MyVertex>, vcl::HalfEdgeContainer<MyHalfEdge>> m3; // ok
 * @endcode
 *
 * To check if a type has (inherits from) HalfEdgeContainer:
 * @code{.cpp}
 * if constexpr (vcl::HasHalfEdges<MyMeshType>) {
 *     // ...
 * }
 * @endcode
 */
template<typename... Args>
concept HasHalfEdges = (vcl::mesh::HasHalfEdgeContainer<Args> | ...);

} // namespace vcl

#endif // VCL_MESH_ELEMENT_CONCEPTS_H
