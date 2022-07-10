/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#include <vclib/mesh/components/vertical/vectors/custom_component_vector_handle.h>
#include <vclib/iterators/container_iterator.h>
#include <vclib/iterators/container_range_iterator.h>
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
class FaceContainer : protected ElementContainer<T>, public FaceContainerTriggerer
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
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalAdjacentEdges<U>, bool)
	isPerFaceAdjacentEdgesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalAdjacentEdges<U>, void)
	enablePerFaceAdjacentEdges();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalAdjacentEdges<U>, void)
	disablePerFaceAdjacentEdges();

	// AdjacentFaces
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalAdjacentFaces<U>, bool)
	isPerFaceAdjacentFacesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalAdjacentFaces<U>, void)
	enablePerFaceAdjacentFaces();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalAdjacentFaces<U>, void)
	disablePerFaceAdjacentFaces();

	// Color
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalColor<U>, bool)
	isPerFaceColorEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalColor<U>, void)
	enablePerFaceColor();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalColor<U>, void)
	disablePerFaceColor();

	// Mark
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalMark<U>, bool)
	isPerFaceMarkEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalMark<U>, void)
	enablePerFaceMark();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalMark<U>, void)
	disablePerFaceMark();

	// Normal
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalNormal<U>, bool)
	isPerFaceNormalEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalNormal<U>, void)
	enablePerFaceNormal();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalNormal<U>, void)
	disablePerFaceNormal();

	// PrincipalCurvature
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalPrincipalCurvature<U>, bool)
	isPerFacePrincipalCurvatureEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalPrincipalCurvature<U>, void)
	enablePerFacePrincipalCurvature();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalPrincipalCurvature<U>, void)
	disablePerFacePrincipalCurvature();

	// Scalar
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalScalar<U>, bool)
	isPerFaceScalarEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalScalar<U>, void)
	enablePerFaceScalar();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalScalar<U>, void)
	disablePerFaceScalar();

	// WedgeColors
	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalWedgeColors<U>, bool)
	isPerFaceWedgeColorsEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalWedgeColors<U>, void)
	enablePerFaceWedgeColors();

	template<typename U = T>
	VCL_ENABLE_IF(face::HasOptionalWedgeColors<U>, void)
	disablePerFaceWedgeColors();

	// WedgeTexCoords
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalWedgeTexCoords<U>(), bool)
	isPerFaceWedgeTexCoordsEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalWedgeTexCoords<U>(), void)
	enablePerFaceWedgeTexCoords();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalWedgeTexCoords<U>(), void)
	disablePerFaceWedgeTexCoords();

	// Custom Components
	template<typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, bool)
	hasPerFaceCustomComponent(const std::string& name) const;

	template<typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, std::vector<std::string>)
	getAllPerFaceCustomComponentNames() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, bool)
	isPerFaceCustomComponentOfType(const std::string& name) const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, std::vector<std::string>)
	getPerFaceCustomComponentNamesOfType() const;

	template<typename K, typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, void)
	addPerFaceCustomComponent(const std::string& name);

	template<typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, void)
	deletePerFaceCustomComponent(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, CustomComponentVectorHandle<K>)
	getPerFaceCustomComponentVectorHandle(const std::string& name);

	template<typename K, typename U = T>
	VCL_ENABLE_IF(face::HasCustomComponents<U>, ConstCustomComponentVectorHandle<K>)
	getPerFaceCustomComponentVectorHandle(const std::string& name) const;

protected:
	/**
	 * @brief fn: the number of faces in the container. Could be different from the container size
	 * due to faces marked as deleted into the container.
	 */
	//uint fn = 0;

	uint index(const FaceType* f) const;

	void clearFaces();

	uint addFace();
	uint addFaces(uint nFaces);
	void reserveFaces(uint size);

	std::vector<int> compactFaces();

	template<typename Mesh>
	void enableOptionalComponentsOf(const Mesh& m);

	template<typename Mesh>
	void importFrom(const Mesh& m);

	template<typename Mesh, typename Vertex>
	void importVertexReferencesFrom(const Mesh& m, Vertex* base);

	template<typename Mesh>
	void importFaceReferencesFrom(const Mesh& m, T* base);

	template<typename Mesh, typename Edge>
	void importEdgeReferencesFrom(const Mesh& m, Edge* base);
};

} // namespace vcl::mesh

#include "face_container.cpp"

#endif // VCL_MESH_CONTAINER_FACE_CONTAINER_H
