/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
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

#ifndef  VCL_CONCEPTS_MESH_CONTAINER_H
#define  VCL_CONCEPTS_MESH_CONTAINER_H

#include <vector>

#include <vclib/types.h>

namespace vcl {
namespace mesh {

class ElementContainerTriggerer
{
};

/**
 * @brief ElementContainerConcept is a concept satisfied when the type T is an Element Container.
 */
template<typename T>
concept ElementContainerConcept =
	std::is_base_of<ElementContainerTriggerer, T>::value;

/**
 * @brief The predicate IsElementContainerPred sets its bool `value` to `true` when the type T
 * satisfies the ElementContainerConcept concept
 */
template<typename T>
struct IsElementContainerPred
{
	static const bool value = ElementContainerConcept<T>;
};

template <typename T>
concept HasEdgeContainer = requires(T o)
{
	typename T::EdgeType;
	o.edge(uint());
};

template <typename T>
concept HasHalfEdgeContainer = requires(T o)
{
	typename T::HalfEdgeType;
	o.halfEdge(uint());
};

template <typename T>
concept HasFaceContainer = requires(T o, const T& co, typename T::FaceType* f)
{
	typename T::FaceType;
	typename T::FaceIterator;
	typename T::ConstFaceIterator;
	typename T::FaceView;
	typename T::ConstFaceView;
	{ o.face(uint()) } -> std::same_as<typename T::FaceType&>;
	{ co.face(uint()) } -> std::same_as<const typename T::FaceType&>;
	{ co.faceNumber() } -> std::same_as<uint>;
	{ co.faceContainerSize() } -> std::same_as<uint>;
	{ co.deletedFaceNumber() } -> std::same_as<uint>;
	o.deleteFace(uint());
	o.deleteFace(f);
	{ o.faceIndexIfCompact(uint()) } -> std::same_as<uint>;
	{ o.faceCompactIndices() } -> std::same_as<std::vector<int>>;

	{ o.faceBegin() } -> std::same_as<typename T::FaceIterator>;
	{ co.faceBegin() } -> std::same_as<typename T::ConstFaceIterator>;
	{ o.faceEnd() } -> std::same_as<typename T::FaceIterator>;
	{ co.faceEnd() } -> std::same_as<typename T::ConstFaceIterator>;
	{ o.faces() } -> std::same_as<typename T::FaceView>;
	{ co.faces() } -> std::same_as<typename T::ConstFaceView>;
};

template <typename T>
concept HasVertexContainer = requires(T o, const T& co, typename T::VertexType* v)
{
	typename T::VertexType;
	typename T::VertexIterator;
	typename T::ConstVertexIterator;
	typename T::VertexView;
	typename T::ConstVertexView;
	{ o.vertex(uint()) } -> std::same_as<typename T::VertexType&>;
	{ co.vertex(uint()) } -> std::same_as<const typename T::VertexType&>;
	{ co.vertexNumber() } -> std::same_as<uint>;
	{ co.vertexContainerSize() } -> std::same_as<uint>;
	{ co.deletedVertexNumber() } -> std::same_as<uint>;
	o.deleteVertex(uint());
	o.deleteVertex(v);
	{ o.vertexIndexIfCompact(uint()) } -> std::same_as<uint>;
	{ o.vertexCompactIndices() } -> std::same_as<std::vector<int>>;

	{ o.vertexBegin() } -> std::same_as<typename T::VertexIterator>;
	{ co.vertexBegin() } -> std::same_as<typename T::ConstVertexIterator>;
	{ o.vertexEnd() } -> std::same_as<typename T::VertexIterator>;
	{ co.vertexEnd() } -> std::same_as<typename T::ConstVertexIterator>;
	{ o.vertices() } -> std::same_as<typename T::VertexView>;
	{ co.vertices() } -> std::same_as<typename T::ConstVertexView>;
};

} // namespace vcl::mesh

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

} // namespace vcl


#endif //  VCL_CONCEPTS_MESH_CONTAINER_H
