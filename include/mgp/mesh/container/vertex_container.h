/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
#define MGP_MESH_CONTAINER_VERTEX_CONTAINER_H

#include "../vertex.h"
#include "container_t.h"
#include "optional/optional_components_vector.h"

namespace mgp {
namespace mesh {

template<class T>
/**
 * @brief The Vertex Container class, will be used when the template argument given to the Mesh is a
 * mgp::Vertex.
 *
 * This class adds a container (vector) of vertices to the Mesh, making available the accessors
 * members to the vertices, the vertex number, iterators... This class will also take care to add
 * enablers/disablers of the eventual optional components of the vertex.
 */
class Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>>
{
protected:
	// types:
	typedef T                                                 VertexType;
	typedef Container<T, mgp::ifIsBaseOf<VertexTriggerer, T>> VertexContainer;

public:
	Container();

	const VertexType& vertex(unsigned int i) const;
	VertexType&       vertex(unsigned int i);

	unsigned int vertexNumber() const;

	template<typename U = T>
	typename std::enable_if<common::hasOptionalColor<U>::value, void>::type enableVertexColor();
	
	template<typename U = T>
	typename std::enable_if<common::hasOptionalNormal<U>::value, void>::type enableVertexNormal();
	
	template<typename U = T>
	typename std::enable_if<common::hasOptionalScalar<U>::value, void>::type enableVertexScalar();

protected:
	/**
	 * @brief vertices: the vector of vertices, where each vertex contains only its static
	 * components. Optional components will be contained in the optionalComponentsVector.
	 */
	std::vector<T> vertices;
	/**
	 * @brief optionalComponentsVector contains all the optional components data of the vertex, that
	 * will be enabled - disabled at runtime.
	 * Each vertex that has at least one optional component, will store a pointer to this vector.
	 */
	OptionalComponentsVector<T> optionalComponentsVector;

	unsigned int addVertex();
};

/**
 * Detector to check if a class has (inherits) a VertexContainer
 */

template<typename T>
using hasVertexContainer_t = decltype(std::declval<T&>().vertexNumber());

template <typename T>
using hasVertexContainer = typename detector<hasVertexContainer_t, void, T>::type;

} // namespace mesh
} // namespace mgp

#include "vertex_container.cpp"

#endif // MGP_MESH_CONTAINER_VERTEX_CONTAINER_H
