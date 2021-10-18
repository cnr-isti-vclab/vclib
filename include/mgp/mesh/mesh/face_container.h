/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_MESH_FACE_CONTAINER_H
#define MGP_MESH_MESH_FACE_CONTAINER_H

#include "../face.h"
#include "container_t.h"

namespace mgp {

namespace mesh {

template<class T>
class Container<T, std::enable_if_t<std::is_base_of_v<FaceID, T>>> : public virtual EnabledContainers
{
protected:
	// types:
	typedef T FaceType;
	typedef Container<T, std::enable_if_t<std::is_base_of_v<FaceID, T>>> FaceContainer;

public:
	Container() {facesEnabled = true;}

	const FaceType* face(unsigned int i) const {return &faces[i];}
	FaceType* face(unsigned int i) {return &faces[i];}

	unsigned int faceNumber() const {return faces.size();}
protected:
	std::vector<T> faces;

	FaceType* addFace()
	{
		faces.push_back(FaceType());
		faces[faces.size()-1].id = faces.size()-1;
		return &faces[faces.size()-1];
	}
};

}

}

#endif // MGP_MESH_MESH_FACE_CONTAINER_H
