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

#ifndef VCL_CONCEPTS_MESH_CONTAINERS_FACE_CONTAINER_H
#define VCL_CONCEPTS_MESH_CONTAINERS_FACE_CONTAINER_H

#include <ranges>
#include <vector>

#include "element_container.h"

namespace vcl {
namespace mesh {

/**
 * @brief The HasFaceContainer concept is satisfied only if a container class
 * provides the types and member functions specified in this concept. These
 * types and member functions allow to access to a @ref vcl::mesh::FaceContainer
 * container of a given mesh.
 *
 * @ingroup containers_concepts
 */
template<typename T>
concept HasFaceContainer =
    requires (T o, const T& co, typename T::FaceType* f) {
        // clang-format off
        typename T::FaceType;
        typename T::FaceIterator;
        typename T::ConstFaceIterator;

        { o.face(uint()) } -> std::same_as<typename T::FaceType&>;
        { co.face(uint()) } -> std::same_as<const typename T::FaceType&>;

        { co.faceNumber() } -> std::same_as<uint>;
        { co.faceContainerSize() } -> std::same_as<uint>;
        { co.deletedFaceNumber() } -> std::same_as<uint>;
        o.deleteFace(uint());
        o.deleteFace(f);
        { o.faceIndexIfCompact(uint()) } -> std::same_as<uint>;
        { o.faceCompactIndices() } -> std::same_as<std::vector<uint>>;

        { o.addFace() } -> std::same_as<uint>;
        { o.addFaces(uint()) } -> std::same_as<uint>;
        { o.reserveFaces(uint()) } -> std::same_as<void>;
        { o.compactFaces() } -> std::same_as<void>;

        { o.faceBegin() } -> std::same_as<typename T::FaceIterator>;
        { co.faceBegin() } -> std::same_as<typename T::ConstFaceIterator>;
        { o.faceEnd() } -> std::same_as<typename T::FaceIterator>;
        { co.faceEnd() } -> std::same_as<typename T::ConstFaceIterator>;

        requires std::ranges::range<decltype(o.faces())>;
        requires std::ranges::range<decltype(co.faces())>;
        // clang-format on
    };

} // namespace mesh

/**
 * @brief HasFaces concepts is satisfied when at least one of its
 * template types is (or inherits from) a @ref vcl::mesh::FaceContainer. It can
 * be used both to check if a Mesh has faces, or if in a list of types there is
 * a FaceContainer.
 *
 * In the following example, a MyMesh type can be instantiated only if one of
 * its template Args is a FaceContainer:
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
 *
 * @note This concept does not check if a Mesh is a valid FaceMesh.
 * To do that, use the @ref vcl::FaceMeshConcept.
 *
 * @ingroup containers_concepts
 */
template<typename... Args>
concept HasFaces = (vcl::mesh::HasFaceContainer<Args> || ...);

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_CONTAINERS_FACE_CONTAINER_H
