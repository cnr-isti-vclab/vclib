/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_FACE_CONTAINER_H
#define MGP_MESH_CONTAINER_FACE_CONTAINER_H

#include "../face.h"
#include "container_t.h"
#include "optional/optional_components_vector.h"

namespace mgp {
namespace mesh {

template<class T>
class Container<T, mgp::ifIsBaseOf<FaceTriggerer, T>> :
		public virtual EnabledContainers
{
protected:
	// types:
	typedef T                                        FaceType;
	typedef Container<T, mgp::ifIsBaseOf<FaceTriggerer, T>> FaceContainer;

public:
	Container();

	const FaceType* face(unsigned int i) const;
	FaceType*       face(unsigned int i);

	unsigned int faceNumber() const;

protected:
	std::vector<T> faces;
	OptionalComponentsVector<T> optionalComponentsVector;

	unsigned int addFace();

	template<typename Vertex>
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase);
};

} // namespace mesh
} // namespace mgp

#include "face_container.cpp"

#endif // MGP_MESH_CONTAINER_FACE_CONTAINER_H
