/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_optional_container.h"

namespace mgp::mesh {

/**
 * @brief FaceOptionalContainer::isPerFaceColorEnabled checks if the face Optional Color is
 * enabled. This function is available **only if the Face Element has the OptionalColor
 * Component**.
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalColor<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceColorEnabled() const
{
	return optionalComponentsVector.isColorEnabled();
}

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
 * @brief Container::disableFaceColor disables the Optional Color of the face.
 * This function is available **only if the Face Element has the OptionalColor Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalColor<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceColor()
{
	optionalComponentsVector.disableColor();
}

/**
 * @brief FaceOptionalContainer::isPerFaceMutableBitFlagsEnabled checks if the face Optional
 * Mutable Bit Flags is enabled. This function is available **only if the Face Element has the
 * OptionalMutableBitFlags Component**.
 * @return true if the Optional Mutable Bit Flags component is enabled, false otherwise.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalMutableBitFlags<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceMutableBitFlagsEnabled() const
{
	optionalComponentsVector.isMutableBitFlagsEnabled();
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
 * @brief Container::disableFaceMutableFlags disables the Optional Mutable Flags of the face.
 * This function is available **only if the Face Element has the OptionalMutableBitFlags
 * Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalMutableBitFlags<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceMutableBitFlags()
{
	optionalComponentsVector.disableMutableBitFlags();
}

/**
 * @brief FaceOptionalContainer::isPerFaceNormalEnabled checks if the face Optional Normal is
 * enabled. This function is available **only if the Face Element has the OptionalNormal
 * Component**.
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalNormal<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceNormalEnabled() const
{
	return optionalComponentsVector.isNormalEnabled();
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
 * @brief Container::disableFaceNormal disables the Optional Normal of the face.
 * This function is available **only if the Face Element has the OptionalNormal Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalNormal<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceNormal()
{
	optionalComponentsVector.disableNormal();
}

/**
 * @brief FaceOptionalContainer::isPerFaceScalarEnabled checks if the face Optional Scalar is
 * enabled. This function is available **only if the Face Element has the OptionalScalar
 * Component**.
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalScalar<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceScalarEnabled() const
{
	return optionalComponentsVector.isScalarEnabled();
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

/**
 * @brief Container::disableFaceScalar disables the Optional Scalar of the face.
 * This function is available **only if the Face Element has the OptionalScalar Component**.
 */
template<class T>
template<class U>
face::ReturnIfHasOptionalScalar<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceScalar()
{
	optionalComponentsVector.disableScalar();
}

template<class T>
template<typename K, typename U>
face::ReturnIfHasCustomComponents<U, void> FaceOptionalContainer<T, FaceHasOptional<T>>::addPerFaceCustomComponent(const std::string& name)
{
	optionalComponentsVector.template addNewComponent<K>(name, faceContainerSize());
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T> >::clear()
{
	optionalComponentsVector.clear();
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

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T> >::compact(const std::vector<int>& newIndices)
{
	optionalComponentsVector.compact(newIndices);
}

}

