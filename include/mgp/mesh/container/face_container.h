/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_FACE_CONTAINER_H
#define MGP_MESH_CONTAINER_FACE_CONTAINER_H

#include "container_t.h"

#include "face_optional_container.h"
#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"

namespace mgp::mesh {

class FaceContainerTriggerer {};

// to shorten triggerer for Vertex class
template<typename T>
using IfIsFace = std::enable_if_t<std::is_base_of<FaceTriggerer, T>::value>;

/**
 * @brief The Face Container class, will be used when the template argument given to the Mesh is a
 * mgp::Face.
 *
 * This class adds a container (vector) of faces to the Mesh, making available the accessors members
 * to the faces, the face number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the face.
 */
template<typename T>
class Container<T, IfIsFace<T>> :
		public FaceOptionalContainer<T>, public FaceContainerTriggerer
{
	static_assert(
		mgp::face::hasBitFlags<T>(),
		"You should include BitFlags (or a derived) as Face component in your Mesh definition.");
	static_assert(
		mgp::face::hasVertexReferences<T>(),
		"You should include VertexReferences (Array or List) component in your Mesh definition.");
	static_assert (
		!mgp::face::hasTriangleBitFlags<T>() || T::VERTEX_NUMBER == 3,
		"You can use TriangleBitFlags only on static sized VertexReferences components, N == 3.");

protected:
	// types:
	using FaceContainer = Container<T, IfIsFace<T>>;
	using OptionalFaceContainer = FaceOptionalContainer<T>;

public:
	using FaceType               = T;
	using FaceIterator           = ContainerIterator<T>;
	using ConstFaceIterator      = ConstContainerIterator<T>;
	using FaceRangeIterator      = ContainerRangeIterator<FaceContainer, FaceIterator>;
	using ConstFaceRangeIterator = ConstContainerRangeIterator<FaceContainer, ConstFaceIterator>;

	Container();

	const FaceType& face(unsigned int i) const;
	FaceType&       face(unsigned int i);

	unsigned int faceNumber() const;
	unsigned int faceContainerSize() const;
	unsigned int deletedFaceNumber() const;

	void deleteFace(unsigned int i);

	unsigned int faceIdIfCompact(unsigned int id) const;

	FaceIterator           faceBegin(bool jumpDeleted = true);
	FaceIterator           faceEnd();
	ConstFaceIterator      faceBegin(bool jumpDeleted = true) const;
	ConstFaceIterator      faceEnd() const;
	FaceRangeIterator      faces(bool jumpDeleted = true);
	ConstFaceRangeIterator faces(bool jumpDeleted = true) const;

	// hide functions of optional container components
	// needed to update faces sizes
	template<typename U = T>
	face::ReturnIfHasOptionalAdjacentFaces<U, void> enablePerFaceAdjacentFaces();

protected:
	/**
	 * @brief faces: the vector of faces, where each face contains only its static components.
	 * Optional components will be contained in the optionalComponentsVector.
	 */
	std::vector<T> facesVec;

	/**
	 * @brief fn: the number of faces in the container. Could be different from faces.size()
	 * due to faces marked as deleted into the container.
	 */
	unsigned int fn = 0;

	void clearFaces();

	unsigned int addFace();
	unsigned int addFaces(unsigned int nFaces);
	void reserveFaces(unsigned int size);

	std::vector<int> compactFaces();

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

} // namespace mgp::mesh

#include "face_container.cpp"

#endif // MGP_MESH_CONTAINER_FACE_CONTAINER_H
