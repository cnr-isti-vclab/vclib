/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_optional_container.h"

namespace mgp::mesh {

/**
 * @brief VertexOptionalContainer::isPerVertexColorEnabled checks if the vertex Optional Color is
 * enabled. This function is available **only if the Vertex Element has the OptionalColor
 * Component**.
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalColor<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexColorEnabled() const
{
	return optionalComponentsVector.isColorEnabled();
}

/**
 * @brief Container::enableVertexColor enables the Optional Color of the vertex.
 * This function is available **only if the Vertex Element has the OptionalColor Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalColor<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexColor()
{
	optionalComponentsVector.enableColor(vertexContainerSize());
}

/**
 * @brief Container::disableVertexColor disables the Optional Color of the vertex.
 * This function is available **only if the Vertex Element has the OptionalColor Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalColor<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexColor()
{
	optionalComponentsVector.disableColor();
}

/**
 * @brief VertexOptionalContainer::isPerVertexMutableBitFlagsEnabled checks if the vertex Optional
 * Mutable Bit Flags is enabled. This function is available **only if the Vertex Element has the
 * OptionalMutableBitFlags Component**.
 * @return true if the Optional Mutable Bit Flags component is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalMutableBitFlags<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexMutableBitFlagsEnabled() const
{
	optionalComponentsVector.isMutableBitFlagsEnabled();
}

/**
 * @brief Container::enableVertexMutableFlags enables the Optional Mutable Flags of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMutableBitFlags
 * Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalMutableBitFlags<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexMutableBitFlags()
{
	optionalComponentsVector.enableMutableBitFlags(vertexContainerSize());
}

/**
 * @brief Container::disableVertexMutableFlags disables the Optional Mutable Flags of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMutableBitFlags
 * Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalMutableBitFlags<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexMutableBitFlags()
{
	optionalComponentsVector.disableMutableBitFlags();
}

/**
 * @brief VertexOptionalContainer::isPerVertexNormalEnabled checks if the vertex Optional Normal is
 * enabled. This function is available **only if the Vertex Element has the OptionalNormal
 * Component**.
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexNormalEnabled() const
{
	return optionalComponentsVector.isNormalEnabled();
}

/**
 * @brief Container::enableVertexNormal enables the Optional Normal of the vertex.
 * This function is available **only if the Vertex Element has the OptionalNormal Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexNormal()
{
	optionalComponentsVector.enableNormal(vertexContainerSize());
}

/**
 * @brief Container::disableVertexNormal disables the Optional Normal of the vertex.
 * This function is available **only if the Vertex Element has the OptionalNormal Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexNormal()
{
	optionalComponentsVector.disableNormal();
}

/**
 * @brief VertexOptionalContainer::isPerVertexScalarEnabled checks if the vertex Optional Scalar is
 * enabled. This function is available **only if the Vertex Element has the OptionalScalar
 * Component**.
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalScalar<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexScalarEnabled() const
{
	return optionalComponentsVector.isScalarEnabled();
}

/**
 * @brief Container::enableVertexScalar enables the Optional Scalar of the vertex.
 * This function is available **only if the Vertex Element has the OptionalScalar Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalScalar<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexScalar()
{
	optionalComponentsVector.enableScalar(vertexContainerSize());
}

/**
 * @brief Container::disableVertexScalar disables the Optional Scalar of the vertex.
 * This function is available **only if the Vertex Element has the OptionalScalar Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalScalar<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexScalar()
{
	optionalComponentsVector.disableScalar();
}

/**
 * @brief VertexOptionalContainer::isPerVertexAdjacentFacesEnabled checks if the vertex Optional
 * Adjacent Faces component is enabled. This function is available **only if the Vertex Element has
 * the OptionalAdjacentFaces Component**.
 * @return true if the Optional Adjacent Faces is enabled, false otherwise.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexAdjacentFacesEnabled() const
{
	return optionalComponentsVector.isFaceReferencesEnabled();
}

/**
 * @brief VertexOptionalContainer::enablePerVertexAdjacentFaces enables the Optional Adjacent Faces
 * of the vertex.
 * This function is available **only if the Vertex Element has the OptionalAdjacentFaces
 * Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexAdjacentFaces()
{
	optionalComponentsVector.enableFaceReferences(vertexContainerSize());
}

/**
 * @brief VertexOptionalContainer::disablePerVertexAdjacentFaces disables the Optional Adjacent
 * Faces of the vertex. This function is available **only if the Vertex Element has the
 * OptionalAdjacentFaces Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::disablePerVertexAdjacentFaces()
{
	optionalComponentsVector.disableFaceReferences();
}

/**
 * @brief VertexOptionalContainer::addPerVertexCustomComponent
 */
template<class T>
template<typename K, typename U>
vert::ReturnIfHasCustomComponents<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::addPerVertexCustomComponent(
	const std::string& name)
{
	optionalComponentsVector.template addNewComponent<K>(name, vertexContainerSize());
}

template<typename T>
void mgp::mesh::VertexOptionalContainer<T, VertexHasOptional<T> >::clear()
{
	optionalComponentsVector.clear();
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::setContainerPointer(T& vertex)
{
	vertex.setContainerPointer(&optionalComponentsVector);
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::resize(unsigned int size)
{
	optionalComponentsVector.resize(size);
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::reserve(unsigned int size)
{
	optionalComponentsVector.reserve(size);
}

template<typename T>
void VertexOptionalContainer<T, VertexHasOptional<T> >::compact(const std::vector<int>& newIndices)
{
	optionalComponentsVector.compact(newIndices);
}

} // namespace mgp::mesh
