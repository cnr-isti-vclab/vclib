/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_container.h"

namespace mgp::mesh {

template<class T>
Container<T, IfIsFace<T>>::Container()
{
}

template<class T>
const typename Container<T, IfIsFace<T>>::FaceType&
Container<T, IfIsFace<T>>::face(unsigned int i) const
{
	return faces[i];
}

template<class T>
typename Container<T, IfIsFace<T>>::FaceType&
mgp::mesh::Container<T, IfIsFace<T>>::face(unsigned int i)
{
	return faces[i];
}

template<class T>
unsigned int Container<T, IfIsFace<T>>::faceNumber() const
{
	return fn;
}

template<class T>
void Container<T, IfIsFace<T>>::reserveFaces(unsigned int size)
{
	faces.reserve(size);
	if constexpr (common::hasOptionalInfo<FaceType>::value) {
		optionalComponentsVector.reserve(size);
	}
}

template<class T>
template<class U>
common::ReturnIfHasOptionalColor<U, void> Container<T, IfIsFace<T>>::enableFaceColor()
{
	optionalComponentsVector.enableColor(faceNumber());
}

template<class T>
template<class U>
common::ReturnIfHasOptionalNormal<U, void> Container<T, IfIsFace<T>>::enableFaceNormal()
{
	optionalComponentsVector.enableNormal(faceNumber());
}

template<class T>
template<class U>
common::ReturnIfHasOptionalScalar<U, void> Container<T, IfIsFace<T>>::enableFaceScalar()
{
	optionalComponentsVector.enableScalar(faceNumber());
}

template<class T>
typename Container<T, IfIsFace<T>>::FaceIterator
Container<T, IfIsFace<T>>::faceBegin(bool jumpDeleted)
{
	if (jumpDeleted) {
		auto it = faces.begin();
		while (it->isDeleted()) {
			++it;
		}
		return FaceIterator(it, faces, jumpDeleted);
	}
	else
		return FaceIterator(faces.begin(), faces, jumpDeleted);
}

template<class T>
typename Container<T, IfIsFace<T>>::FaceIterator Container<T, IfIsFace<T>>::faceEnd()
{
	return FaceIterator(faces.end(), faces);
}

template<class T>
typename Container<T, IfIsFace<T>>::ConstFaceIterator
Container<T, IfIsFace<T>>::faceBegin(bool jumpDeleted) const
{
	if (jumpDeleted) {
		auto it = faces.begin();
		while (it->isDeleted()) {
			++it;
		}
		return ConstFaceIterator(it, faces, jumpDeleted);
	}
	else
		return ConstFaceIterator(faces.begin(), faces, jumpDeleted);
}

template<class T>
typename Container<T, IfIsFace<T>>::ConstFaceIterator Container<T, IfIsFace<T>>::faceEnd() const
{
	return ConstFaceIterator(faces.end(), faces);
}

template<class T>
typename Container<T, IfIsFace<T>>::FaceRangeIterator
Container<T, IfIsFace<T>>::faceIterator(bool jumpDeleted)
{
	return FaceRangeIterator(
		*this, jumpDeleted, &FaceContainer::faceBegin, &FaceContainer::faceEnd);
}

template<class T>
typename Container<T, IfIsFace<T>>::ConstFaceRangeIterator
Container<T, IfIsFace<T>>::faceIterator(bool jumpDeleted) const
{
	return ConstFaceRangeIterator(
		*this, jumpDeleted, &FaceContainer::faceBegin, &FaceContainer::faceEnd);
}

template<class T>
unsigned int Container<T, IfIsFace<T>>::addFace()
{
	faces.push_back(FaceType());
	++fn;
	faces[faces.size() - 1]._id = faces.size() - 1;
	if constexpr (common::hasOptionalInfo<FaceType>::value) {
		faces[faces.size() - 1].setContainerPointer(&optionalComponentsVector);
		optionalComponentsVector.resize(faces.size());
	}
	return faces[faces.size() - 1]._id;
}

template<class T>
template<class Vertex>
void Container<T, IfIsFace<T>>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	if constexpr (face::hasVertexRefsArray<FaceType>::value) {
		for (unsigned int i = 0; i < faces.size(); ++i) {
			faces[i].updateVertexReferences(oldBase, newBase);
		}
	}
}

} // namespace mgp::mesh
