/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_container.h"

namespace mgp::mesh {

/**
 * @brief Container::Container Empty constructors that created an empty container of Faces.
 */
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
unsigned int Container<T, IfIsFace<T>>::faceContainerSize() const
{
	return faces.size();
}

/**
 * @brief Container::enableFaceColor enable the Optional Color of the face.
 * This function is available **only if the Face Element has the OptionalColor Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalColor<U, void> Container<T, IfIsFace<T>>::enablePerFaceColor()
{
	optionalComponentsVector.enableColor(faceNumber());
}

/**
 * @brief Container::enableFaceMutableFlags enable the Optional Mutable Flags of the face.
 * This function is available **only if the Face Element has the OptionalMutableFlags Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalMutableBitFlags<U, void> Container<T, IfIsFace<T>>::enablePerFaceMutableFlags()
{
	optionalComponentsVector.enableMutableBitFlags(faceNumber());
}

/**
 * @brief Container::enableFaceNormal enable the Optional Normal of the face.
 * This function is available **only if the Face Element has the OptionalNormal Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalNormal<U, void> Container<T, IfIsFace<T>>::enablePerFaceNormal()
{
	optionalComponentsVector.enableNormal(faceNumber());
}

/**
 * @brief Container::enableFaceScalar enable the Optional Scalar of the face.
 * This function is available **only if the Face Element has the OptionalScalar Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalScalar<U, void> Container<T, IfIsFace<T>>::enablePerFaceScalar()
{
	optionalComponentsVector.enableScalar(faceNumber());
}

template<class T>
template<typename K, typename U>
face::ReturnIfHasCustomComponents<U, void> Container<T, IfIsFace<T> >::addPerFaceCustomComponent(const std::string& name)
{
	optionalComponentsVector.template addNewComponent<K>(name, faces.size());
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
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		faces[faces.size() - 1].setContainerPointer(&optionalComponentsVector);
		optionalComponentsVector.resize(faces.size());
	}
	return faces[faces.size() - 1]._id;
}

template<class T>
void Container<T, IfIsFace<T>>::reserveFaces(unsigned int size)
{
	faces.reserve(size);
	if constexpr (face::hasOptionalInfo<FaceType>()) {
		optionalComponentsVector.reserve(size);
	}
}

template<class T>
template<class Vertex>
void Container<T, IfIsFace<T>>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	for (unsigned int i = 0; i < faces.size(); ++i) {
		faces[i].updateVertexReferences(oldBase, newBase);
	}
}

} // namespace mgp::mesh
