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

#include <vclib/mesh/elements/face.h>

#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"
#include "element_container.h"

namespace vcl::mesh {

class FaceContainerTriggerer
{
};

/**
 * @brief The Face Container class, will be used when the template argument given to the Mesh is a
 * Face.
 *
 * This class adds a container (vector) of faces to the Mesh, making available the accessors members
 * to the faces, the face number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the face.
 */
template<typename T>
class FaceContainer : protected ElementContainer<T>, public FaceContainerTriggerer
{
	// Sanity checks for the Face -- all components must be consistent each other
	static_assert(
		vcl::face::hasBitFlags<T>(),
		"You should include BitFlags (or a derived) as Face component in your Mesh definition.");
	static_assert(
		vcl::face::hasVertexReferences<T>(),
		"You should include a VertexReferences as Face component in your Mesh definition.");
	static_assert(
		!vcl::face::hasTriangleBitFlags<T>() || T::VERTEX_NUMBER == 3,
		"You can use TriangleBitFlags only on static sized VertexReferences components, N == 3.");
	static_assert(
		face::sanityCheckAdjacentEdges<T>() && face::sanityCheckOptionalAdjacentEdges<T>(),
		"Size of per Face AdjacentEdges component must be the same of the VertexReferences.");
	static_assert(
		face::sanityCheckAdjacentFaces<T>() && face::sanityCheckOptionalAdjacentFaces<T>(),
		"Size of per Face AdjacentFaces component must be the same of the VertexReferences.");
	static_assert(
		face::sanityCheckWedgeColors<T>() && face::sanityCheckOptionalWedgeColors<T>(),
		"Size of per Face WedgeColors component must be the same of the VertexReferences.");
	static_assert(
		face::sanityCheckWedgeTexCoords<T>() && face::sanityCheckOptionalWedgeTexCoords<T>(),
		"Size of per Face WedgeTexCoords component must be the same of the VertexReferences.");

	using Base              = ElementContainer<T>;
	using FaceContainerType = FaceContainer<T>;

public:
	using FaceType          = T;
	using FaceIterator      = ContainerIterator<std::vector, T>;
	using ConstFaceIterator = ConstContainerIterator<std::vector, T>;
	using FaceRangeIterator = ContainerRangeIterator<FaceContainerType, FaceIterator>;
	using ConstFaceRangeIterator =
		ConstContainerRangeIterator<FaceContainerType, ConstFaceIterator>;

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

	// AdjacentEdges
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalAdjacentEdges<U>(), bool)
	isPerFaceAdjacentEdgesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalAdjacentEdges<U>(), void)
	enablePerFaceAdjacentEdges();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalAdjacentEdges<U>(), void)
	disablePerFaceAdjacentEdges();

	// AdjacentFaces
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalAdjacentFaces<U>(), bool)
	isPerFaceAdjacentFacesEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalAdjacentFaces<U>(), void)
	enablePerFaceAdjacentFaces();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalAdjacentFaces<U>(), void)
	disablePerFaceAdjacentFaces();

	// Color
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalColor<U>(), bool)
	isPerFaceColorEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalColor<U>(), void)
	enablePerFaceColor();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalColor<U>(), void)
	disablePerFaceColor();

	// Mark
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalMark<U>(), bool)
	isPerFaceMarkEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalMark<U>(), void)
	enablePerFaceMark();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalMark<U>(), void)
	disablePerFaceMark();

	// Mutable Bit Flags
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), bool)
	isPerFaceMutableBitFlagsEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), void)
	enablePerFaceMutableBitFlags();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalMutableBitFlags<U>(), void)
	disablePerFaceMutableBitFlags();

	// Normal
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalNormal<U>(), bool)
	isPerFaceNormalEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalNormal<U>(), void)
	enablePerFaceNormal();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalNormal<U>(), void)
	disablePerFaceNormal();

	// PrincipalCurvature
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalPrincipalCurvature<U>(), bool)
	isPerFacePrincipalCurvatureEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalPrincipalCurvature<U>(), void)
	enablePerFacePrincipalCurvature();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalPrincipalCurvature<U>(), void)
	disablePerFacePrincipalCurvature();

	// Scalar
	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalScalar<U>(), bool)
	isPerFaceScalarEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalScalar<U>(), void)
	enablePerFaceScalar();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasOptionalScalar<U>(), void)
	disablePerFaceScalar();

	// WedgeColors
	template<typename U = T>
	VCL_ENABLE_IF(face::hasWedgeColors<U>(), bool)
	isPerFaceWedgeColorsEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasWedgeColors<U>(), void)
	enablePerFaceWedgeColors();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasWedgeColors<U>(), void)
	disablePerFaceWedgeColors();

	// WedgeTexCoords
	template<typename U = T>
	VCL_ENABLE_IF(face::hasWedgeTexCoords<U>(), bool)
	isPerFaceWedgeTexCoordsEnabled() const;

	template<typename U = T>
	VCL_ENABLE_IF(face::hasWedgeTexCoords<U>(), void)
	enablePerFaceWedgeTexCoords();

	template<typename U = T>
	VCL_ENABLE_IF(face::hasWedgeTexCoords<U>(), void)
	disablePerFaceWedgeTexCoords();

	// Custom Components
	template<typename K, typename U = T>
	VCL_ENABLE_IF(face::hasCustomComponents<U>(), void)
	addPerFaceCustomComponent(const std::string& name);

protected:
	/**
	 * @brief fn: the number of faces in the container. Could be different from the container size
	 * due to faces marked as deleted into the container.
	 */
	uint fn = 0;

	uint index(const FaceType* f) const;

	void clearFaces();

	uint addFace();
	uint addFaces(uint nFaces);
	void reserveFaces(uint size);

	void setContainerPointer(FaceType& f);

	std::vector<int> compactFaces();

	void updateAfterAllocation(const T* oldBase, const T* newBase);

	void updateContainerPointers();

	void updateFaceReferences(const T* oldBase, const T* newBase);

	void updateFaceReferencesAfterCompact(const T* base, const std::vector<int>& newIndices);

	template<typename Vertex>
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);

	template<typename Vertex>
	void updateVertexReferencesAfterCompact(const Vertex* base, const std::vector<int>& newIndices);
};

/**
 * Detector to check if a class has (inherits) a FaceContainer
 */

template<typename T>
using hasFaceContainer = std::is_base_of<FaceContainerTriggerer, T>;

template<typename T>
constexpr bool hasFaces()
{
	return hasFaceContainer<T>::value;
}

template<typename T>
constexpr bool hasFaceOptionalContainer()
{
	return comp::hasOptionalInfo<typename T::FaceType>();
}

} // namespace vcl::mesh

#include "face_container.cpp"

#endif // VCL_MESH_CONTAINER_FACE_CONTAINER_H
