/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_optional_container.h"

namespace mgp::mesh {

/**
 * @brief Container::enableFaceColor enable the Optional Color of the face.
 * This function is available **only if the Face Element has the OptionalColor Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalColor<U, void> FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceColor()
{
	optionalComponentsVector.enableColor(faceContainerSize());
}

/**
 * @brief Container::enableFaceMutableFlags enable the Optional Mutable Flags of the face.
 * This function is available **only if the Face Element has the OptionalMutableFlags Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalMutableBitFlags<U, void> FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceMutableBitFlags()
{
	optionalComponentsVector.enableMutableBitFlags(faceContainerSize());
}

/**
 * @brief Container::enableFaceNormal enable the Optional Normal of the face.
 * This function is available **only if the Face Element has the OptionalNormal Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalNormal<U, void> FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceNormal()
{
	optionalComponentsVector.enableNormal(faceContainerSize());
}

/**
 * @brief Container::enableFaceScalar enable the Optional Scalar of the face.
 * This function is available **only if the Face Element has the OptionalScalar Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalScalar<U, void> FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceScalar()
{
	optionalComponentsVector.enableScalar(faceContainerSize());
}

template<class T>
template<typename K, typename U>
face::ReturnIfHasCustomComponents<U, void> FaceOptionalContainer<T, FaceHasOptional<T>>::addPerFaceCustomComponent(const std::string& name)
{
	optionalComponentsVector.template addNewComponent<K>(name, faceContainerSize());
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::setContainerPointer(T& face)
{
	face.setContainerPointer(&optionalComponentsVector);
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::resize(unsigned int size)
{
	optionalComponentsVector.resize(size);
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::reserve(unsigned int size)
{
	optionalComponentsVector.reserve(size);
}

}

