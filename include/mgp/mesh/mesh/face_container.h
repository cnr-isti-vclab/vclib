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
public:
	typedef T FaceType;
	Container() {facesEnabled = true;}
protected:
	typedef Container<T, std::enable_if_t<std::is_base_of_v<FaceID, T>>> FaceContainer;

	std::vector<T> faces;
};

}

}

#endif // MGP_MESH_MESH_FACE_CONTAINER_H
