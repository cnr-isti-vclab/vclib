/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_container.h"

namespace mgp {
namespace mesh {

template<class T>
Container<T, mgp::ifIsBaseOf<FaceTriggerer, T>>::Container()
{
	facesEnabled = true;
}

template<class T>
const typename Container<T, mgp::ifIsBaseOf<FaceTriggerer, T> >::FaceType*
Container<T, mgp::ifIsBaseOf<FaceTriggerer, T> >::face(unsigned int i) const
{
	return &faces[i];
}

template<class T>
typename Container<T, mgp::ifIsBaseOf<FaceTriggerer, T> >::FaceType*
mgp::mesh::Container<T, mgp::ifIsBaseOf<FaceTriggerer, T> >::face(unsigned int i)
{
	return &faces[i];
}

template<class T>
unsigned int Container<T, mgp::ifIsBaseOf<FaceTriggerer, T> >::faceNumber() const
{
	return faces.size();
}

template<class T>
unsigned int Container<T, mgp::ifIsBaseOf<FaceTriggerer, T> >::addFace()
{
	faces.push_back(FaceType());
	faces[faces.size() - 1]._id = faces.size() - 1;
	if constexpr(common::hasOptionalInfo<FaceType>::value) {
		faces[faces.size() - 1].setContainerPointer(&optionalComponentsVector);
	}
	return faces[faces.size() - 1]._id;
}

template<class T>
template<class Vertex>
void Container<T, mgp::ifIsBaseOf<FaceTriggerer, T>>::updateVertexReferences(
	const Vertex* oldBase,
	const Vertex* newBase)
{
	if constexpr (face::hasVertexRefsArray<FaceType>::value) {
		for (unsigned int i = 0; i < faces.size(); ++i) {
			faces[i].updateVertexReferences(oldBase, newBase);
		}
	}
}
}
}

