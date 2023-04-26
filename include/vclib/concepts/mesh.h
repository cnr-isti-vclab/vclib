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

#ifndef VCL_CONCEPTS_MESH_H
#define VCL_CONCEPTS_MESH_H

#include "mesh/components.h"
#include "mesh/containers.h"
#include "mesh/per_face.h"
#include "mesh/per_half_edge.h"
#include "mesh/per_vertex.h"

/**
 * @defgroup mesh_concepts Mesh Concepts
 * @ingroup lconcepts
 * @ingroup mesh
 *
 * @brief List of concepts for types related to the Mesh data structures of the library. They allow
 * to discriminate between different Mesh types, their elements and the element components.
 */

/**
 * @defgroup element_concepts Element Concepts
 * @ingroup mesh_concepts
 * @ingroup elements
 *
 * @brief List of concepts for types related to the Elements of a Mesh.
 */

namespace vcl {

template<typename... Args> requires HasVertices<Args...>
class Mesh;

namespace mesh {

// checks if a type derives from vcl::Mesh<Args...>
template<typename Derived>
using IsDerivedFromMesh = IsDerivedFromTemplateSpecialization<Derived, Mesh>;

// checks if a type is a vcl::Mesh<Args...>
template<class T>
struct IsAMesh : // Default case, no pattern match
				 std::false_type
{
};

template<class... Args>
struct IsAMesh<Mesh<Args...>> : // For types matching the pattern Mesh<Args...>
								std::true_type
{
};

/* Port concepts into the mesh namespace */
template<typename T>
concept HasBoundingBox = comp::HasBoundingBox<T>;
template<typename T>
concept HasColor = comp::HasColor<T>;
template<typename T>
concept HasMark = comp::HasMark<T>;
template<typename T>
concept HasName = comp::HasName<T>;
template<typename T>
concept HasTexturePaths = comp::HasTexturePaths<T>;
template<typename T>
concept HasTransformMatrix = comp::HasTransformMatrix<T>;

} // namespace mesh

/**
 * @brief The Mesh Concept is evaluated to true when the type is a Mesh.
 *
 * A type T is a Mesh whem it inherits from or it is a template specialization of the vcl::Mesh
 * class, and it contains a VertexContainer, which is the only container that is mandatory in
 * a vcl::Mesh.
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept MeshConcept =
	(mesh::IsDerivedFromMesh<T>::value || mesh::IsAMesh<T>::value) &&
	mesh::HasVertexContainer<T> &&
	requires(
		T o,
		const T& co,
		typename T::VertexType v,
		typename T::VertexType::CoordType c)
{
	typename T::VertexType;
	typename T::VertexContainer;

	{ o.clear() } -> std::same_as<void>;
	{ o.enableSameOptionalComponentsOf(T()) } -> std::same_as<void>;
	{ o.importFrom(T()) } -> std::same_as<void>;
	{ co.index(v) } -> std::same_as<uint>;
	{ co.index(&v) } -> std::same_as<uint>;
	{ o.addVertex() } -> std::same_as<uint>;
	{ o.addVertex(c) } -> std::same_as<uint>;
	{ o.addVertices(uint()) } -> std::same_as<uint>;
	{ o.addVertices(c, c, c, c) } -> std::same_as<uint>;
	{ o.reserveVertices(uint()) } -> std::same_as<void>;
	{ o.compactVertices() } -> std::same_as<void>;
};

template<typename MeshType>
concept HasTriangles =
	vcl::HasFaces<MeshType> && MeshType::FaceType::VERTEX_NUMBER == 3;

template<typename MeshType>
concept HasQuads =
	vcl::HasFaces<MeshType> && MeshType::FaceType::VERTEX_NUMBER == 4;

template<typename MeshType>
concept HasPolygons =
	vcl::HasFaces<MeshType> && MeshType::FaceType::VERTEX_NUMBER == -1;

template<typename T>
concept FaceMeshConcept =
	MeshConcept<T> && mesh::HasFaceContainer<T> &&
	requires(
		T o,
		const T& co,
		typename T::FaceType f)
{
	typename T::FaceType;
	typename T::FaceContainer;

	{ co.index(f) } -> std::same_as<uint>;
	{ co.index(&f) } -> std::same_as<uint>;
	{ o.addFace() } -> std::same_as<uint>;
	{ o.addFace(uint(), uint(), uint()) } -> std::same_as<uint>;
	{ o.addFaces(uint()) } -> std::same_as<uint>;
	{ o.reserveFaces(uint()) } -> std::same_as<void>;
	{ o.compactFaces() } -> std::same_as<void>;
};

template<typename T>
concept TriangleMeshConcept =
	FaceMeshConcept<T> && HasTriangles<T>;

template<typename T>
concept QuadMeshConcept =
	FaceMeshConcept<T> && HasQuads<T>;

template<typename T>
concept PolygonMeshConcept =
	FaceMeshConcept<T> && HasPolygons<T>;

template<typename T>
concept EdgeMeshConcept =
	MeshConcept<T> && mesh::HasEdgeContainer<T> &&
	requires(
		T o,
		const T& co,
		typename T::EdgeType e)
{
	typename T::EdgeType;
	typename T::EdgeContainer;

	{ co.index(e) } -> std::same_as<uint>;
	{ co.index(&e) } -> std::same_as<uint>;
	{ o.addEdge() } -> std::same_as<uint>;
	{ o.addEdges(uint()) } -> std::same_as<uint>;
	{ o.reserveEdges(uint()) } -> std::same_as<void>;
	{ o.compactEdges() } -> std::same_as<void>;
};

/**
 * @brief The DcelMeshConcept is satisfied when:
 * - The FaceMeshConcpt is satisfied
 * - The Mesh has HalfEdge, Face and Vertex containers
 * - The HalfEdge element has HalfEdgeReferences component
 * - The Vertex Element has HalfEdgeReference component
 * - The Face Element has HalfEdgeReference component
 * - The Vertex Element does not have AdjacentVertices component (it is simulated by half edges)
 * - The Face Element does not have AdjacentFaces component (it is simulated by half edges)
 * - The Face Element does not have WedgeColors component (it is simulated by half edges)
 * - The Face Element does not have WedgeTexCoords component (it is simulated by half edges)
 *
 * @ingroup mesh_concepts
 */
template<typename T>
concept DcelMeshConcept =
	FaceMeshConcept<T> &&
	HasHalfEdges<T> &&
	HasPerVertexHalfEdgeReference<T> &&
	HasPerFaceHalfEdgeReference<T> &&
	!comp::HasAdjacentVerticesComponent<typename T::VertexType> &&
	!comp::HasAdjacentFacesComponent<typename T::FaceType> &&
	!comp::HasWedgeColorsComponent<typename T::FaceType> &&
	!comp::HasWedgeTexCoordsComponent<typename T::FaceType> &&
	requires(
		T o,
		const T& co,
		typename T::HalfEdgeType e)
{
	typename T::HalfEdgeType;
	typename T::HalfEdgeContainer;

	{ co.index(e) } -> std::same_as<uint>;
	{ co.index(&e) } -> std::same_as<uint>;
	{ o.addHalfEdge() } -> std::same_as<uint>;
	{ o.addHalfEdges(uint()) } -> std::same_as<uint>;
	{ o.reserveHalfEdges(uint()) } -> std::same_as<void>;
	{ o.compactHalfEdges() } -> std::same_as<void>;
};

/**
 * @brief Concept that is evaluated true if a Mesh has the BoundingBox component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasBoundingBox = MeshConcept<MeshType> && mesh::HasBoundingBox<MeshType>;

/**
 * @brief Concept that is evaluated true if a Mesh has the Color component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasColor = MeshConcept<MeshType> && mesh::HasColor<MeshType>;

/**
 * @brief Concept that is evaluated true if a Mesh has the Mark component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasMark = MeshConcept<MeshType> && mesh::HasMark<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the Name component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasName = MeshConcept<MeshType> && mesh::HasName<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the TexturePaths component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTexturePaths = MeshConcept<MeshType> && mesh::HasTexturePaths<MeshType>;

/**
 * @brief Concept that checks if a Mesh has the TransformMatrix component.
 *
 * @ingroup mesh_concepts
 */
template<typename MeshType>
concept HasTransformMatrix = MeshConcept<MeshType> && mesh::HasTransformMatrix<MeshType>;

namespace internal {

// Concept used to enable PerFaceWedgeColors member functions in Mesh class
// they can be enabled if:
// - they have per face optional WedgeColors, or
// - if the mesh is a Dcel, HalfEdges have optional Color
template <typename M>
concept OptionalWedgeColorsConcept =
	FaceMeshConcept<M> &&
	(face::HasOptionalWedgeColors<typename M::FaceType> ||
	 (HasHalfEdges<M> && hedge::HasOptionalColor<typename M::HalfEdge>));

// Concept used to enable PerFaceWedgeTexCoords member functions in Mesh class
// they can be enabled if:
// - they have per face optional WedgeTexCoords, or
// - if the mesh is a Dcel, HalfEdges have optional TexCoord
template <typename M>
concept OptionalWedgeTexCoordsConcept =
	FaceMeshConcept<M> &&
	(face::HasOptionalWedgeTexCoords<typename M::FaceType> ||
	 (HasHalfEdges<M> && hedge::HasOptionalTexCoord<typename M::HalfEdge>));

} // namespace vcl::internal

} // namespace vcl

#endif // VCL_CONCEPTS_MESH_H
