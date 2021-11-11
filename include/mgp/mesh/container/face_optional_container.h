/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_FACE_OPTIONAL_CONTAINER_H
#define MGP_MESH_CONTAINER_FACE_OPTIONAL_CONTAINER_H

#include <mgp/mesh/elements/face.h>

#include "../components_optional/optional_info.h"
#include "../components_vector/components_vector.h"

namespace mgp::mesh {

class FaceOptionalContainerTriggerer {};

// to shorten triggerer for Vertex class
template<class T>
using FaceHasOptional = std::enable_if_t<comp::hasOptionalInfo<T>()>;

template<typename, typename = void>
class FaceOptionalContainer
{
};

template<typename T>
class FaceOptionalContainer<T, FaceHasOptional<T>> : public FaceOptionalContainerTriggerer
{
public:
	virtual unsigned int faceContainerSize() const = 0;

	// Color
	template<typename U = T>
	face::ReturnIfHasOptionalColor<U, bool> isPerFaceColorEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalColor<U, void> enablePerFaceColor();

	template<typename U = T>
	face::ReturnIfHasOptionalColor<U, void> disablePerFaceColor();

	// Mutable Bit Flags
	template<typename U = T>
	face::ReturnIfHasOptionalMutableBitFlags<U, bool> isPerFaceMutableBitFlagsEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalMutableBitFlags<U, void> enablePerFaceMutableBitFlags();

	template<typename U = T>
	face::ReturnIfHasOptionalMutableBitFlags<U, void> disablePerFaceMutableBitFlags();

	// Normal
	template<typename U = T>
	face::ReturnIfHasOptionalNormal<U, bool> isPerFaceNormalEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalNormal<U, void> enablePerFaceNormal();

	template<typename U = T>
	face::ReturnIfHasOptionalNormal<U, void> disablePerFaceNormal();

	// Scalar
	template<typename U = T>
	face::ReturnIfHasOptionalScalar<U, bool> isPerFaceScalarEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalScalar<U, void> enablePerFaceScalar();

	template<typename U = T>
	face::ReturnIfHasOptionalScalar<U, void> disablePerFaceScalar();

	// AdjacentFaces
	template<typename U = T>
	face::ReturnIfHasOptionalAdjacentFaces<U, bool> isPerFaceAdjacentFacesEnabled() const;

	template<typename U = T>
	face::ReturnIfHasOptionalAdjacentFaces<U, void> enablePerFaceAdjacentFaces();

	template<typename U = T>
	face::ReturnIfHasOptionalAdjacentFaces<U, void> disablePerFaceAdjacentFaces();

	// Custom Components
	template<typename K, typename U = T>
	face::ReturnIfHasCustomComponents<U, void> addPerFaceCustomComponent(const std::string& name);

protected:
	void setContainerPointer(T& face);
	void clear();
	void resize(unsigned int size);
	void reserve(unsigned int size);
	void compact(const std::vector<int>& newIndices);

private:
	/**
	 * @brief optionalPropVector contains all the optional component data of the face, that
	 * will be enabled - disabled at runtime.
	 * Each face that has at least one optional component, will store a pointer to this vector.
	 */
	internal::ComponentsVector<T> optionalPropVector;
};

/**
 * Detector to check if a class has (inherits) a VertexOptionalContainer
 */

template <typename  T>
using hasFaceOptionalContainerT = std::is_base_of<FaceOptionalContainerTriggerer, T>;

template<typename U, typename T>
using ReturnIfHasFaceOptionalContainer = typename std::enable_if<hasFaceOptionalContainerT<U>::value, T>::type;

template <typename T>
constexpr bool hasFaceOptionalContainer() { return hasFaceOptionalContainerT<T>::value;}

}

#include "face_optional_container.cpp"

#endif // MGP_MESH_CONTAINER_FACE_OPTIONAL_CONTAINER_H
