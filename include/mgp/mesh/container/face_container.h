/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_FACE_CONTAINER_H
#define MGP_MESH_CONTAINER_FACE_CONTAINER_H

#include "../face.h"
#include "container_t.h"
#include "../component_vector/components_vector.h"

namespace mgp::mesh {

/**
 * @brief The Face Container class, will be used when the template argument given to the Mesh is a
 * mgp::Face.
 *
 * This class adds a container (vector) of faces to the Mesh, making available the accessors members
 * to the faces, the face number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the face.
 */
template<class T>
class Container<T, mgp::ifIsBaseOf<FaceTriggerer, T>>
{
	static_assert(
		mgp::common::hasBitFlags<T>::value,
		"You should include BitFlags as Face component in your Mesh definition.");
protected:
	// types:
	typedef T                                               FaceType;
	typedef Container<T, mgp::ifIsBaseOf<FaceTriggerer, T>> FaceContainer;

public:
	Container();

	const FaceType& face(unsigned int i) const;
	FaceType&       face(unsigned int i);

	unsigned int faceNumber() const;

	void reserveFaces(unsigned int size);

	template<typename U = T>
	typename std::enable_if<common::hasOptionalColor<U>::value, void>::type enableFaceColor();
	template<typename U = T>
	typename std::enable_if<common::hasOptionalNormal<U>::value, void>::type enableFaceNormal();
	template<typename U = T>
	typename std::enable_if<common::hasOptionalScalar<U>::value, void>::type enableFaceScalar();

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
using hasFaceContainer_t = decltype(std::declval<T&>().faceNumber());

template <typename T>
using hasFaceContainer = typename detector<hasFaceContainer_t, void, T>::type;


} // namespace mgp::mesh

#include "face_container.cpp"

#endif // MGP_MESH_CONTAINER_FACE_CONTAINER_H
