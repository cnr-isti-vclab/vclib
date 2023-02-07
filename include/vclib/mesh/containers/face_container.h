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

#ifndef VCL_MESH_CONTAINER_FACE_CONTAINER_H
#define VCL_MESH_CONTAINER_FACE_CONTAINER_H

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/face.h>

#include "element_container.h"

namespace vcl::mesh {

/**
 * @brief The Face Container class, will be used when the template argument given to the Mesh is a
 * Face.
 *
 * This class adds a container (vector) of faces to the Mesh, making available the accessors members
 * to the faces, the face number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the face.
 */
template<FaceConcept T>
class FaceContainer : protected ElementContainer<T>
{
	template <FaceConcept U>
	friend class FaceContainer;

	using FaceContainerType = FaceContainer<T>;
	using Base = ElementContainer<T>;

public:
	using Face              = T;
	using FaceType          = T;
	using FaceIterator      = typename Base::ElementIterator;
	using ConstFaceIterator = typename Base::ConstElementIterator;
	using FaceRangeIterator = typename Base::ElementRangeIterator;
	using ConstFaceRangeIterator = typename Base::ConstElementRangeIterator;

	FaceContainer();

	const FaceType& face(uint i) const;
	FaceType&       face(uint i);

	uint faceNumber() const;
	uint faceContainerSize() const;
	uint deletedFaceNumber() const;

	void deleteFace(uint i);
	void deleteFace(const FaceType* f);

	uint             faceIndexIfCompact(uint id) const;
	std::vector<int> faceCompactIndices() const;

	FaceIterator           faceBegin(bool jumpDeleted = true);
	FaceIterator           faceEnd();
	ConstFaceIterator      faceBegin(bool jumpDeleted = true) const;
	ConstFaceIterator      faceEnd() const;
	FaceRangeIterator      faces(bool jumpDeleted = true);
	ConstFaceRangeIterator faces(bool jumpDeleted = true) const;

	void enableAllPerFaceOptionalComponents();
	void disableAllPerFaceOptionalComponents();

	// AdjacentEdges
	bool isPerFaceAdjacentEdgesEnabled() const requires face::HasOptionalAdjacentEdges<T>;
	void enablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>;
	void disablePerFaceAdjacentEdges() requires face::HasOptionalAdjacentEdges<T>;

	// AdjacentFaces
	bool isPerFaceAdjacentFacesEnabled() const requires face::HasOptionalAdjacentFaces<T>;
	void enablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>;
	void disablePerFaceAdjacentFaces() requires face::HasOptionalAdjacentFaces<T>;

	// Color
	bool isPerFaceColorEnabled() const requires face::HasOptionalColor<T>;
	void enablePerFaceColor() requires face::HasOptionalColor<T>;
	void disablePerFaceColor() requires face::HasOptionalColor<T>;

	// Mark
	bool isPerFaceMarkEnabled() const requires face::HasOptionalMark<T>;
	void enablePerFaceMark() requires face::HasOptionalMark<T>;
	void disablePerFaceMark() requires face::HasOptionalMark<T>;

	// Normal
	bool isPerFaceNormalEnabled() const requires face::HasOptionalNormal<T>;
	void enablePerFaceNormal() requires face::HasOptionalNormal<T>;
	void disablePerFaceNormal() requires face::HasOptionalNormal<T>;

	// PrincipalCurvature
	bool isPerFacePrincipalCurvatureEnabled() const requires face::HasOptionalPrincipalCurvature<T>;
	void enablePerFacePrincipalCurvature() requires face::HasOptionalPrincipalCurvature<T>;
	void disablePerFacePrincipalCurvature() requires face::HasOptionalPrincipalCurvature<T>;

	// Scalar
	bool isPerFaceScalarEnabled() const requires face::HasOptionalScalar<T>;
	void enablePerFaceScalar() requires face::HasOptionalScalar<T>;
	void disablePerFaceScalar() requires face::HasOptionalScalar<T>;

	// Custom Components
	bool hasPerFaceCustomComponent(
		const std::string& name) const requires face::HasCustomComponents<T>;

	std::vector<std::string> getAllPerFaceCustomComponentNames()
		const requires face::HasCustomComponents<T>;

	template<typename K>
	bool isPerFaceCustomComponentOfType(
		const std::string& name) const requires face::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> getPerFaceCustomComponentNamesOfType()
		const requires face::HasCustomComponents<T>;

	template<typename K>
	void addPerFaceCustomComponent(const std::string& name) requires face::HasCustomComponents<T>;

	void deletePerFaceCustomComponent(
		const std::string& name) requires face::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> getPerFaceCustomComponentVectorHandle(
		const std::string& name) requires face::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K> getPerFaceCustomComponentVectorHandle(
		const std::string& name) const requires face::HasCustomComponents<T>;

protected:
	/**
	 * @brief fn: the number of faces in the container. Could be different from the container size
	 * due to faces marked as deleted into the container.
	 */
	//uint fn = 0;

	uint index(const FaceType* f) const;

	void clearFaces();

	template<typename MeshType>
	uint addFace(MeshType* parentMesh);

	template<typename MeshType>
	uint addFaces(uint nFaces, MeshType* parentMesh);

	template<typename MeshType>
	void reserveFaces(uint size, MeshType* parentMesh);

	std::vector<int> compactFaces();

	template<typename Mesh>
	void enableOptionalComponentsOf(const Mesh& m);

	template<typename OtherMesh>
	void importFrom(const OtherMesh& m);

	template<typename Mesh, typename Vertex>
	void importVertexReferencesFrom(const Mesh& m, Vertex* base);

	template<typename Mesh>
	void importFaceReferencesFrom(const Mesh& m, T* base);

	template<typename Mesh, typename Edge>
	void importEdgeReferencesFrom(const Mesh& m, Edge* base);

	template<typename Mesh, typename HalfEdge>
	void importHalfEdgeReferencesFrom(const Mesh& m, HalfEdge* base);

	// WedgeColors
	bool isPerFaceWedgeColorsEnabled() const requires face::HasOptionalWedgeColors<T>;
	void enablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>;
	void disablePerFaceWedgeColors() requires face::HasOptionalWedgeColors<T>;

	// WedgeTexCoords
	bool isPerFaceWedgeTexCoordsEnabled() const requires face::HasOptionalWedgeTexCoords<T>;
	void enablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>;
	void disablePerFaceWedgeTexCoords() requires face::HasOptionalWedgeTexCoords<T>;

};

} // namespace vcl::mesh

#include "face_container.cpp"

#endif // VCL_MESH_CONTAINER_FACE_CONTAINER_H
