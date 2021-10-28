/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_FACE_CONTAINER_H
#define MGP_MESH_CONTAINER_FACE_CONTAINER_H

#include "container_t.h"

#include "../component_vector/components_vector.h"
#include "../face.h"
#include "../iterators/container_iterator.h"
#include "../iterators/container_range_iterator.h"

namespace mgp::mesh {

class FaceContainerTriggerer {};

// to shorten triggerer for Vertex class
template<class T>
using IfIsFace = std::enable_if_t<std::is_base_of<FaceTriggerer, T>::value>;

/**
 * @brief The Face Container class, will be used when the template argument given to the Mesh is a
 * mgp::Face.
 *
 * This class adds a container (vector) of faces to the Mesh, making available the accessors members
 * to the faces, the face number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the face.
 */
template<class T>
class Container<T, IfIsFace<T>> : public FaceContainerTriggerer
{
	static_assert(
		mgp::face::hasBitFlags<T>(),
		"You should include BitFlags as Face component in your Mesh definition.");

protected:
	// types:
	using FaceContainer = Container<T, IfIsFace<T>>;

public:
	using FaceType               = T;
	using FaceIterator           = ContainerIterator<T>;
	using ConstFaceIterator      = ConstContainerIterator<T>;
	using FaceRangeIterator      = RangeIterator<FaceContainer, FaceIterator>;
	using ConstFaceRangeIterator = ConstRangeIterator<FaceContainer, ConstFaceIterator>;

	Container();

	const FaceType& face(unsigned int i) const;
	FaceType&       face(unsigned int i);

	unsigned int faceNumber() const;

	void reserveFaces(unsigned int size);

	template<typename U = T>
	face::ReturnIfHasOptionalColor<U, void> enableFaceColor();

	template<typename U = T>
	face::ReturnIfHasOptionalMutableBitFlags<U, void> enableFaceMutableFlags();

	template<typename U = T>
	face::ReturnIfHasOptionalNormal<U, void> enableFaceNormal();

	template<typename U = T>
	face::ReturnIfHasOptionalScalar<U, void> enableFaceScalar();

	FaceIterator           faceBegin(bool jumpDeleted = true);
	FaceIterator           faceEnd();
	ConstFaceIterator      faceBegin(bool jumpDeleted = true) const;
	ConstFaceIterator      faceEnd() const;
	FaceRangeIterator      faceIterator(bool jumpDeleted = true);
	ConstFaceRangeIterator faceIterator(bool jumpDeleted = true) const;

protected:
	/**
	 * @brief faces: the vector of faces, where each face contains only its static components.
	 * Optional components will be contained in the optionalComponentsVector.
	 */
	std::vector<T> faces;
	/**
	 * @brief optionalComponentsVector contains all the optional components data of the face, that
	 * will be enabled - disabled at runtime.
	 * Each face that has at least one optional component, will store a pointer to this vector.
	 */
	ComponentsVector<T> optionalComponentsVector;

	/**
	 * @brief fn: the number of faces in the container. Could be different from faces.size()
	 * due to faces marked as deleted into the container.
	 */
	unsigned int fn = 0;

	unsigned int addFace();

	template<typename Vertex>
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
};

/**
 * Detector to check if a class has (inherits) a FaceContainer
 */

template<typename T>
using hasFaceContainer = std::is_base_of<FaceContainerTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasFaceContainer = typename std::enable_if<hasFaceContainer<U>::value, T>::type;

template <typename T>
constexpr bool hasFaces() { return mesh::hasFaceContainer<T>::value;}

} // namespace mgp::mesh

#include "face_container.cpp"

#endif // MGP_MESH_CONTAINER_FACE_CONTAINER_H
