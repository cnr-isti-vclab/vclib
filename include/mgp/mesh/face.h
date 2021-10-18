/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_FACE_H
#define MGP_MESH_FACE_H

#include "face/vertex_ref_array.h"

namespace mgp {

namespace mesh {
template <typename, typename>
class Container;
}

namespace face {

template<class T>
class Component : public T
{
};

}

class FaceID {
public:
	FaceID() : id(0) {}
protected:
	unsigned int id;
};

template<class... Args>
class Face : public FaceID, public face::Component<Args>...
{
	template<typename, typename> friend class mesh::Container;
};

}

#endif // MGP_MESH_FACE_H
