/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#include "element_container.h"
#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"

namespace vcl::mesh {

class FaceContainerTriggerer {};

/**
 * @brief The Face Container class, will be used when the template argument given to the Mesh is a
 * Face.
 *
 * This class adds a container (vector) of faces to the Mesh, making available the accessors members
 * to the faces, the face number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the face.
 */
template<typename T>
class FaceContainer :
		public ElementContainer<T>, public FaceContainerTriggerer
{
	static_assert(
		vcl::face::hasBitFlags<T>(),
		"You should include BitFlags (or a derived) as Face component in your Mesh definition.");
	static_assert(
		vcl::face::hasVertexReferences<T>(),
		"You should include a VertexReferences as Face component in your Mesh definition.");
	static_assert (
		!vcl::face::hasTriangleBitFlags<T>() || T::VERTEX_NUMBER == 3,
		"You can use TriangleBitFlags only on static sized VertexReferences components, N == 3.");
	static_assert (
		face::sanityCheckAdjacentFaces<T>() && face::sanityCheckOptionalAdjacentFaces<T>(),
		"Size of per Face AdjacentFaces component must be the same of the VertexReferences.");
	static_assert (
		face::sanityCheckWedgeTexCoords<T>() && face::sanityCheckOptionalWedgeTexCoords<T>(),
		"Size of per Face WedgeTexCoords component must be the same of the VertexReferences.");

	using Base = ElementContainer<T>;
	using FaceContainerType = FaceContainer<T>;

public:
	using FaceType               = T;
	using FaceIterator           = ContainerIterator<std::vector, T>;
	using ConstFaceIterator      = ConstContainerIterator<std::vector, T>;
	using FaceRangeIterator      = ContainerRangeIterator<FaceContainerType, FaceIterator>;
	using ConstFaceRangeIterator = ConstContainerRangeIterator<FaceContainerType, ConstFaceIterator>;

	FaceContainer();

	const FaceType& face(unsigned int i) const;
	FaceType&       face(unsigned int i);

	unsigned int faceNumber() const;
	unsigned int faceContainerSize() const;
	unsigned int deletedFaceNumber() const;

	void deleteFace(unsigned int i);
	void deleteFace(const FaceType* f);

	unsigned int faceIndexIfCompact(unsigned int id) const;

	FaceIterator           faceBegin(bool jumpDeleted = true);
	FaceIterator           faceEnd();
	ConstFaceIterator      faceBegin(bool jumpDeleted = true) const;
	ConstFaceIterator      faceEnd() const;
	FaceRangeIterator      faces(bool jumpDeleted = true);
	ConstFaceRangeIterator faces(bool jumpDeleted = true) const;

	// AdjacentFaces
	template<typename U = T>
	face::ReturnIfHasOptionalAdjacentFaces<U, bool> isPerFaceAdjacentFacesEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalAdjacentFaces<U, void> enablePerFaceAdjacentFaces();

	template<typename U = T>
	face::ReturnIfHasOptionalAdjacentFaces<U, void> disablePerFaceAdjacentFaces();

	// Color
	template<typename U = T>
	face::ReturnIfHasOptionalColor<U, bool> isPerFaceColorEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalColor<U, void> enablePerFaceColor();

	template<typename U = T>
	face::ReturnIfHasOptionalColor<U, void> disablePerFaceColor();

	// Mark
	template<typename U = T>
	face::ReturnIfHasOptionalMark<U, bool> isPerFaceMarkEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalMark<U, void> enablePerFaceMark();

	template<typename U = T>
	face::ReturnIfHasOptionalMark<U, void> disablePerFaceMark();

	// Mutable Bit Flags
	template<typename U = T>
	face::ReturnIfHasOptionalMutableBitFlags<U, bool> isPerFaceMutableBitFlagsEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalMutableBitFlags<U, void> enablePerFaceMutableBitFlags();

	template<typename U = T>
	face::ReturnIfHasOptionalMutableBitFlags<U, void> disablePerFaceMutableBitFlags();

	// Normal
	template<typename U = T>
	face::ReturnIfHasOptionalNormal<U, bool> isPerFaceNormalEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalNormal<U, void> enablePerFaceNormal();

	template<typename U = T>
	face::ReturnIfHasOptionalNormal<U, void> disablePerFaceNormal();

	// Scalar
	template<typename U = T>
	face::ReturnIfHasOptionalScalar<U, bool> isPerFaceScalarEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalScalar<U, void> enablePerFaceScalar();

	template<typename U = T>
	face::ReturnIfHasOptionalScalar<U, void> disablePerFaceScalar();

	// WedgeTexCoords
	template<typename U = T>
	face::ReturnIfHasOptionalWedgeTexCoords<U, bool> isPerFaceWedgeTexCoordsEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalWedgeTexCoords<U, void> enablePerFaceWedgeTexCoords();

	template<typename U = T>
	face::ReturnIfHasOptionalWedgeTexCoords<U, void> disablePerFaceWedgeTexCoords();

	// Custom Components
	template<typename K, typename U = T>
	face::ReturnIfHasCustomComponents<U, void> addPerFaceCustomComponent(const std::string& name);

protected:
	/**
	 * @brief fn: the number of faces in the container. Could be different from faces.size()
	 * due to faces marked as deleted into the container.
	 */
	unsigned int fn = 0;

	unsigned int index(const FaceType* f) const;

	void clearFaces();

	unsigned int addFace();
	unsigned int addFaces(unsigned int nFaces);
	void reserveFaces(unsigned int size);

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

template<typename U, typename T>
using ReturnIfHasFaceContainer = typename std::enable_if<hasFaceContainer<U>::value, T>::type;

template <typename T>
constexpr bool hasFaces() { return hasFaceContainer<T>::value;}

template <typename T>
constexpr bool hasFaceOptionalContainer() { return comp::hasOptionalInfo<typename T::FaceType>();}

} // namespace vcl::mesh

#include "face_container.cpp"

#endif // VCL_MESH_CONTAINER_FACE_CONTAINER_H
