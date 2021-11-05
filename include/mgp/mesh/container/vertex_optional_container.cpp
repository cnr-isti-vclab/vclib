/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_optional_container.h"

namespace mgp::mesh {

/**
 * @brief VertexOptionalContainer::isPerVertexColorEnabled checks if the vertex Optional Color is
 * enabled. This function is available **only if the Vertex Element has the OptionalColor
 * Property**.
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalColor<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexColorEnabled() const
{
	return optionalPropVector.isColorEnabled();
}

/**
 * @brief Container::enableVertexColor enables the Optional Color of the vertex.
 * This function is available **only if the Vertex Element has the OptionalColor Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalColor<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexColor()
{
	optionalPropVector.enableColor(vertexContainerSize());
}

/**
 * @brief Container::disableVertexColor disables the Optional Color of the vertex.
 * This function is available **only if the Vertex Element has the OptionalColor Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalColor<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexColor()
{
	optionalPropVector.disableColor();
}

/**
 * @brief VertexOptionalContainer::isPerVertexMutableBitFlagsEnabled checks if the vertex Optional
 * Mutable Bit Flags is enabled. This function is available **only if the Vertex Element has the
 * OptionalMutableBitFlags Property**.
 * @return true if the Optional Mutable Bit Flags property is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalMutableBitFlags<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexMutableBitFlagsEnabled() const
{
	optionalPropVector.isMutableBitFlagsEnabled();
}

/**
 * @brief Container::enableVertexMutableFlags enables the Optional Mutable Flags of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMutableBitFlags
 * Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalMutableBitFlags<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexMutableBitFlags()
{
	optionalPropVector.enableMutableBitFlags(vertexContainerSize());
}

/**
 * @brief Container::disableVertexMutableFlags disables the Optional Mutable Flags of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMutableBitFlags
 * Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalMutableBitFlags<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexMutableBitFlags()
{
	optionalPropVector.disableMutableBitFlags();
}

/**
 * @brief VertexOptionalContainer::isPerVertexNormalEnabled checks if the vertex Optional Normal is
 * enabled. This function is available **only if the Vertex Element has the OptionalNormal
 * Property**.
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexNormalEnabled() const
{
	return optionalPropVector.isNormalEnabled();
}

/**
 * @brief Container::enableVertexNormal enables the Optional Normal of the vertex.
 * This function is available **only if the Vertex Element has the OptionalNormal Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexNormal()
{
	optionalPropVector.enableNormal(vertexContainerSize());
}

/**
 * @brief Container::disableVertexNormal disables the Optional Normal of the vertex.
 * This function is available **only if the Vertex Element has the OptionalNormal Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexNormal()
{
	optionalPropVector.disableNormal();
}

/**
 * @brief VertexOptionalContainer::isPerVertexScalarEnabled checks if the vertex Optional Scalar is
 * enabled. This function is available **only if the Vertex Element has the OptionalScalar
 * Property**.
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalScalar<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexScalarEnabled() const
{
	return optionalPropVector.isScalarEnabled();
}

/**
 * @brief Container::enableVertexScalar enables the Optional Scalar of the vertex.
 * This function is available **only if the Vertex Element has the OptionalScalar Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalScalar<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexScalar()
{
	optionalPropVector.enableScalar(vertexContainerSize());
}

/**
 * @brief Container::disableVertexScalar disables the Optional Scalar of the vertex.
 * This function is available **only if the Vertex Element has the OptionalScalar Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalScalar<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexScalar()
{
	optionalPropVector.disableScalar();
}

/**
 * @brief VertexOptionalContainer::isPerVertexAdjacentFacesEnabled checks if the vertex Optional
 * Adjacent Faces property is enabled. This function is available **only if the Vertex Element has
 * the OptionalAdjacentFaces Property**.
 * @return true if the Optional Adjacent Faces is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexAdjacentFacesEnabled() const
{
	return optionalPropVector.isFaceReferencesEnabled();
}

/**
 * @brief VertexOptionalContainer::enablePerVertexAdjacentFaces enables the Optional Adjacent Faces
 * of the vertex.
 * This function is available **only if the Vertex Element has the OptionalAdjacentFaces
 * Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexAdjacentFaces()
{
	optionalPropVector.enableFaceReferences(vertexContainerSize());
}

/**
 * @brief VertexOptionalContainer::disablePerVertexAdjacentFaces disables the Optional Adjacent
 * Faces of the vertex. This function is available **only if the Vertex Element has the
 * OptionalAdjacentFaces Property**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexAdjacentFaces()
{
	optionalPropVector.disableFaceReferences();
}

/**
 * @brief VertexOptionalContainer::addPerVertexCustomProperty
 */
template<class T>
template<typename K, typename U>
vert::ReturnIfHasCustomProperties<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::addPerVertexCustomProperty(
	const std::string& name)
{
	optionalPropVector.template addNewProperty<K>(name, vertexContainerSize());
}

template<typename T>
void mgp::mesh::VertexOptionalContainer<T, VertexHasOptional<T> >::clear()
{
	optionalPropVector.clear();
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::setContainerPointer(T& vertex)
{
	vertex.setContainerPointer(&optionalPropVector);
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::resize(unsigned int size)
{
	optionalPropVector.resize(size);
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::reserve(unsigned int size)
{
	optionalPropVector.reserve(size);
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::compact(const std::vector<int>& newIndices)
{
	optionalPropVector.compact(newIndices);
}

} // namespace mgp::mesh
