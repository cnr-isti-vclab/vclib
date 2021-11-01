/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef GP_MESH_CONTAINER_VERTEX_OPTIONAL_CONTAINER_H
#define GP_MESH_CONTAINER_VERTEX_OPTIONAL_CONTAINER_H

#include "../vertex.h"
#include "../components_optional/optional_info.h"
#include "../components_vector/components_vector.h"

namespace mgp::mesh {

// to shorten triggerer for Vertex class
template<class T>
using VertexHasOptional = std::enable_if_t<components::hasOptionalInfo<T>()>;

template<typename, typename = void>
class VertexOptionalContainer
{
};

template<typename T>
class VertexOptionalContainer<T, VertexHasOptional<T>>
{
public:
	virtual unsigned int vertexContainerSize() const = 0;

	template<typename U = T>
	vert::ReturnIfHasOptionalColor<U, void> enablePerVertexColor();

	template<typename U = T>
	vert::ReturnIfHasOptionalMutableBitFlags<U, void> enablePerVertexMutableFlags();

	template<typename U = T>
	vert::ReturnIfHasOptionalNormal<U, void> enablePerVertexNormal();
	template<typename U = T>
	vert::ReturnIfHasOptionalNormal<U, bool> isPerVertexNormalEnabled() const;

	template<typename U = T>
	vert::ReturnIfHasOptionalScalar<U, void> enablePerVertexScalar();

	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentFaces<U, void> enablePerVertexAdjacentFaces();
	template<typename U = T>
	vert::ReturnIfHasOptionalAdjacentFaces<U, bool> isPerVertexAdjacentFacesEnabled() const;

	template<typename K, typename U = T>
	vert::ReturnIfHasCustomComponents<U, void> addPerVertexCustomComponent(const std::string& name);

protected:
	void setContainerPointer(T& vertex);
	void resize(unsigned int size);
	void reserve(unsigned int size);

private:
	/**
	 * @brief optionalComponentsVector contains all the optional components data of the vertex, that
	 * will be enabled - disabled at runtime.
	 * Each vertex that has at least one optional component, will store a pointer to this vector.
	 */
	internal::ComponentsVector<T> optionalComponentsVector;
};

}

#include "vertex_optional_container.cpp"

#endif // GP_MESH_CONTAINER_VERTEX_OPTIONAL_CONTAINER_H
