/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "face_optional_container.h"

namespace mgp::mesh {

/**
 * @brief FaceOptionalContainer::isPerFaceColorEnabled checks if the face Optional Color is
 * enabled. This function is available **only if the Face Element has the OptionalColor
 * component**.
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalColor<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceColorEnabled() const
{
	return optionalPropVector.isColorEnabled();
}

/**
 * @brief Container::enableFaceColor enable the Optional Color of the face.
 * This function is available **only if the Face Element has the OptionalColor Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalColor<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceColor()
{
	optionalPropVector.enableColor(faceContainerSize());
}

/**
 * @brief Container::disableFaceColor disables the Optional Color of the face.
 * This function is available **only if the Face Element has the OptionalColor Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalColor<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceColor()
{
	optionalPropVector.disableColor();
}

/**
 * @brief FaceOptionalContainer::isPerFaceMutableBitFlagsEnabled checks if the face Optional
 * Mutable Bit Flags is enabled. This function is available **only if the Face Element has the
 * OptionalMutableBitFlags Component**.
 * @return true if the Optional Mutable Bit Flags component is enabled, false otherwise.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalMutableBitFlags<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceMutableBitFlagsEnabled() const
{
	optionalPropVector.isMutableBitFlagsEnabled();
}

/**
 * @brief Container::enableFaceMutableFlags enable the Optional Mutable Flags of the face.
 * This function is available **only if the Face Element has the OptionalMutableFlags Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalMutableBitFlags<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceMutableBitFlags()
{
	optionalPropVector.enableMutableBitFlags(faceContainerSize());
}

/**
 * @brief Container::disableFaceMutableFlags disables the Optional Mutable Flags of the face.
 * This function is available **only if the Face Element has the OptionalMutableBitFlags
 * Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalMutableBitFlags<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceMutableBitFlags()
{
	optionalPropVector.disableMutableBitFlags();
}

/**
 * @brief FaceOptionalContainer::isPerFaceNormalEnabled checks if the face Optional Normal is
 * enabled. This function is available **only if the Face Element has the OptionalNormal
 * Component**.
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalNormal<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceNormalEnabled() const
{
	return optionalPropVector.isNormalEnabled();
}

/**
 * @brief Container::enableFaceNormal enable the Optional Normal of the face.
 * This function is available **only if the Face Element has the OptionalNormal Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalNormal<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceNormal()
{
	optionalPropVector.enableNormal(faceContainerSize());
}

/**
 * @brief Container::disableFaceNormal disables the Optional Normal of the face.
 * This function is available **only if the Face Element has the OptionalNormal Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalNormal<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceNormal()
{
	optionalPropVector.disableNormal();
}

/**
 * @brief FaceOptionalContainer::isPerFaceScalarEnabled checks if the face Optional Scalar is
 * enabled. This function is available **only if the Face Element has the OptionalScalar
 * Component**.
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalScalar<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceScalarEnabled() const
{
	return optionalPropVector.isScalarEnabled();
}
/**
 * @brief Container::enableFaceScalar enable the Optional Scalar of the face.
 * This function is available **only if the Face Element has the OptionalScalar Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalScalar<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceScalar()
{
	optionalPropVector.enableScalar(faceContainerSize());
}

/**
 * @brief Container::disableFaceScalar disables the Optional Scalar of the face.
 * This function is available **only if the Face Element has the OptionalScalar Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalScalar<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceScalar()
{
	optionalPropVector.disableScalar();
}

/**
 * @brief FaceOptionalContainer::isPerFaceAdjacentFacesEnabled checks if the face Optional Adjacent
 * Faces is enabled. This function is available **only if the Face Element has the
 * OptionalAdjacentFaces Component**.
 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalAdjacentFaces<U, bool>
FaceOptionalContainer<T, FaceHasOptional<T>>::isPerFaceAdjacentFacesEnabled() const
{
	return optionalPropVector.isAdjacentFacesEnabled();
}

/**
 * @brief Container::enableFaceAdjacentFaces enable the Optional Adjacent Faces of the face.
 * This function is available **only if the Face Element has the OptionalAdjacentFaces Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalAdjacentFaces<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::enablePerFaceAdjacentFaces()
{
	optionalPropVector.enableAdjacentFaces(faceContainerSize());
}

/**
 * @brief Container::disableFaceAdjacentFaces disables the Optional Adjacent Faces of the face.
 * This function is available **only if the Face Element has the OptionalAdjacentFaces Component**.
 */
template<typename T>
template<typename U>
face::ReturnIfHasOptionalAdjacentFaces<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::disablePerFaceAdjacentFaces()
{
	optionalPropVector.disableAdjacentFaces();
}

template<typename T>
template<typename K, typename U>
face::ReturnIfHasCustomComponents<U, void>
FaceOptionalContainer<T, FaceHasOptional<T>>::addPerFaceCustomComponent(const std::string& name)
{
	optionalPropVector.template addNewComponent<K>(name, faceContainerSize());
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::clear()
{
	optionalPropVector.clear();
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::setContainerPointer(T& face)
{
	face.setContainerPointer(&optionalPropVector);
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::resize(unsigned int size)
{
	optionalPropVector.resize(size);
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::reserve(unsigned int size)
{
	optionalPropVector.reserve(size);
}

template<typename T>
void FaceOptionalContainer<T, FaceHasOptional<T>>::compact(const std::vector<int>& newIndices)
{
	optionalPropVector.compact(newIndices);
}

} // namespace mgp::mesh
