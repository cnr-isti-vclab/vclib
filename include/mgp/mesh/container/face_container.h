/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_FACE_CONTAINER_H
#define MGP_MESH_CONTAINER_FACE_CONTAINER_H

#include "../face.h"
#include "container_t.h"

namespace mgp {

namespace mesh {

template<class T>
class Container<T, mgp::ifIsBaseOf<FaceID, T>> :
		public virtual EnabledContainers
{
protected:
	// types:
	typedef T                                        FaceType;
	typedef Container<T, mgp::ifIsBaseOf<FaceID, T>> FaceContainer;

public:
	Container() { facesEnabled = true; }

	const FaceType* face(unsigned int i) const { return &faces[i]; }
	FaceType*       face(unsigned int i) { return &faces[i]; }

	unsigned int faceNumber() const { return faces.size(); }

protected:
	std::vector<T> faces;

	unsigned int addFace()
	{
		faces.push_back(FaceType());
		faces[faces.size() - 1].id = faces.size() - 1;
		return faces[faces.size() - 1].id;
	}

	template<typename Vertex>
	void updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
	{
		if constexpr (face::hasVertexRefsArray<FaceType>::value) {
			for (unsigned int i = 0; i < faces.size(); ++i) {
				faces[i].updateVertexReferences(oldBase, newBase);
			}
		}
	}
};

} // namespace mesh

} // namespace mgp

#endif // MGP_MESH_CONTAINER_FACE_CONTAINER_H
