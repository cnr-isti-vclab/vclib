/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#include "vertex_optional_container.h"

namespace mgp::mesh {

/**
 * @brief Container::enableVertexColor enable the Optional Color of the vertex.
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
 * @brief Container::enableVertexMutableFlags enable the Optional Mutable Flags of the vertex.
 * This function is available **only if the Vertex Element has the OptionalMutableBitFlags
 * Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalMutableBitFlags<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexMutableFlags()
{
	optionalComponentsVector.enableMutableBitFlags(vertexContainerSize());
}

/**
 * @brief Container::enableVertexNormal enable the Optional Normal of the vertex.
 * This function is available **only if the Vertex Element has the OptionalNormal Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexNormal()
{
	optionalComponentsVector.enableNormal(vertexContainerSize());
}

template<class T>
template<class U>
vert::ReturnIfHasOptionalNormal<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexNormalEnabled() const
{
	return optionalComponentsVector.isNormalEnabled();
}

/**
 * @brief Container::enableVertexScalar enable the Optional Scalar of the vertex.
 * This function is available **only if the Vertex Element has the OptionalScalar Component**.
 */
template<class T>
template<class U>
vert::ReturnIfHasOptionalScalar<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexScalar()
{
	optionalComponentsVector.enableScalar(vertexContainerSize());
}

template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::enablePerVertexAdjacentFaces()
{
	optionalComponentsVector.enableFaceReferences(vertexContainerSize());
}

template<class T>
template<class U>
vert::ReturnIfHasOptionalAdjacentFaces<U, bool>
VertexOptionalContainer<T, VertexHasOptional<T>>::isPerVertexAdjacentFacesEnabled() const
{
	return optionalComponentsVector.isFaceReferencesEnabled();
}

template<class T>
template<typename K, typename U>
vert::ReturnIfHasCustomComponents<U, void>
VertexOptionalContainer<T, VertexHasOptional<T>>::addPerVertexCustomComponent(
	const std::string& name)
{
	optionalComponentsVector.template addNewComponent<K>(name, vertexContainerSize());
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

} // namespace mgp::mesh
