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

#ifndef VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
#define VCL_MESH_CONTAINER_VERTEX_CONTAINER_H

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/vertex.h>

#include "element_container.h"

namespace vcl::mesh {

/**
 * @brief The Vertex Container class, will be used when the template argument given to the Mesh is a
 * Vertex.
 *
 * This class adds a container (vector) of vertices to the Mesh, making available the accessors
 * members to the vertices, the vertex number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the vertex.
 *
 * This container can be templated on a type that satisfies the VertexConcept concept.
 */
template<vcl::VertexConcept T>
class VertexContainer : public ElementContainer<T>
{
	template <VertexConcept U>
	friend class VertexContainer;

	using VertexContainerType = VertexContainer<T>;
	using Base                = ElementContainer<T>;

public:
	using Vertex                   = T;
	using VertexType               = T;
	using VertexIterator           = typename Base::ElementIterator;
	using ConstVertexIterator      = typename Base::ConstElementIterator;
	using VertexRangeIterator      = typename Base::ElementRangeIterator;
	using ConstVertexRangeIterator = typename Base::ConstElementRangeIterator;

	VertexContainer();

	const VertexType& vertex(uint i) const;
	VertexType&       vertex(uint i);

	uint vertexNumber() const;
	uint vertexContainerSize() const;
	uint deletedVertexNumber() const;

	void deleteVertex(uint i);
	void deleteVertex(const VertexType* v);

	uint             vertexIndexIfCompact(uint id) const;
	std::vector<int> vertexCompactIndices() const;

	VertexIterator           vertexBegin(bool jumpDeleted = true);
	VertexIterator           vertexEnd();
	ConstVertexIterator      vertexBegin(bool jumpDeleted = true) const;
	ConstVertexIterator      vertexEnd() const;
	VertexRangeIterator      vertices(bool jumpDeleted = true);
	ConstVertexRangeIterator vertices(bool jumpDeleted = true) const;

	void enableAllPerVertexOptionalComponents();
	void disableAllPerVertexOptionalComponents();

	// Adjacent Edges
	bool isPerVertexAdjacentEdgesEnabled() const requires vert::HasOptionalAdjacentEdges<T>;
	void enablePerVertexAdjacentEdges() requires vert::HasOptionalAdjacentEdges<T>;
	void disablePerVertexAdjacentEdges() requires vert::HasOptionalAdjacentEdges<T>;

	// Adjacent Faces
	bool isPerVertexAdjacentFacesEnabled() const requires vert::HasOptionalAdjacentFaces<T>;
	void enablePerVertexAdjacentFaces() requires vert::HasOptionalAdjacentFaces<T>;
	void disablePerVertexAdjacentFaces() requires vert::HasOptionalAdjacentFaces<T>;

	// Adjacent Vertices
	bool isPerVertexAdjacentVerticesEnabled() const requires vert::HasOptionalAdjacentVertices<T>;
	void enablePerVertexAdjacentVertices() requires vert::HasOptionalAdjacentVertices<T>;
	void disablePerVertexAdjacentVertices() requires vert::HasOptionalAdjacentVertices<T>;

	// Color
	bool isPerVertexColorEnabled() const requires vert::HasOptionalColor<T>;
	void enablePerVertexColor() requires vert::HasOptionalColor<T>;
	void disablePerVertexColor() requires vert::HasOptionalColor<T>;

	// Mark
	bool isPerVertexMarkEnabled() const requires vert::HasOptionalMark<T>;
	void enablePerVertexMark() requires vert::HasOptionalMark<T>;
	void disablePerVertexMark() requires vert::HasOptionalMark<T>;

	// Normal
	bool isPerVertexNormalEnabled() const requires vert::HasOptionalNormal<T>;
	void enablePerVertexNormal() requires vert::HasOptionalNormal<T>;
	void disablePerVertexNormal() requires vert::HasOptionalNormal<T>;

	// PrincipalCurvature
	bool isPerVertexPrincipalCurvatureEnabled()
		const requires vert::HasOptionalPrincipalCurvature<T>;
	void enablePerVertexPrincipalCurvature() requires vert::HasOptionalPrincipalCurvature<T>;
	void disablePerVertexPrincipalCurvature() requires vert::HasOptionalPrincipalCurvature<T>;

	// Scalar
	bool isPerVertexScalarEnabled() const requires vert::HasOptionalScalar<T>;
	void enablePerVertexScalar() requires vert::HasOptionalScalar<T>;
	void disablePerVertexScalar() requires vert::HasOptionalScalar<T>;

	// TexCoord
	bool isPerVertexTexCoordEnabled() const requires vert::HasOptionalTexCoord<T>;
	void enablePerVertexTexCoord() requires vert::HasOptionalTexCoord<T>;
	void disablePerVertexTexCoord() requires vert::HasOptionalTexCoord<T>;

	// Custom Components
	bool hasPerVertexCustomComponent(const std::string& name)
		const requires vert::HasCustomComponents<T>;

	std::vector<std::string> getAllPerVertexCustomComponentNames()
		const requires vert::HasCustomComponents<T>;

	template<typename K>
	bool isPerVertexCustomComponentOfType(
		const std::string& name) const requires vert::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> getPerVertexCustomComponentNamesOfType()
		const requires vert::HasCustomComponents<T>;

	template<typename K>
	void addPerVertexCustomComponent(const std::string& name) requires vert::HasCustomComponents<T>;

	void deletePerVertexCustomComponent(const std::string& name)
		requires vert::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> getPerVertexCustomComponentVectorHandle(
		const std::string& name) requires vert::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K> getPerVertexCustomComponentVectorHandle(
		const std::string& name) const requires vert::HasCustomComponents<T>;

protected:
	template<typename Mesh>
	void enableOptionalComponentsOf(const Mesh& m);

	template<typename OtherMesh, typename ParentMeshType>
	void importFrom(const OtherMesh& m, ParentMeshType* parent);

	template<typename Mesh>
	void importVertexReferencesFrom(const Mesh& m, T* base);

	template<typename Mesh, typename Face>
	void importFaceReferencesFrom(const Mesh& m, Face* base);

	template<typename Mesh, typename Edge>
	void importEdgeReferencesFrom(const Mesh& m, Edge* base);

	template<typename Mesh, typename HalfEdge>
	void importHalfEdgeReferencesFrom(const Mesh& m, HalfEdge* base);
};

} // namespace vcl::mesh

#include "vertex_container.cpp"

#endif // VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
