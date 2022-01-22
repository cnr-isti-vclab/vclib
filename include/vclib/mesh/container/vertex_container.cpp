/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#include "vertex_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Vertices.
 */
template<typename T>
VertexContainer<T>::VertexContainer()
{
}

/**
 * @brief Returns a const reference of the vertex at the i-th position in the Vertex Container of
 * the Mesh, which will be the vertex having index = i.
 *
 * This function does not perform any sanity check: if i is less than vertexContainerSize(), this
 * function will return a valid Vertex reference (note that the Vertex may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the vertex that will be returned.
 */
template<typename T>
const typename VertexContainer<T>::VertexType& VertexContainer<T>::vertex(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the vertex at the i-th position in the Vertex Container of the
 * Mesh, which will be the vertex having index = i.
 *
 * This function does not perform any sanity check: if i is less than vertexContainerSize(), this
 * function will return a valid Vertex reference (note that the Vertex may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the vertex that will be returned.
 */
template<typename T>
typename VertexContainer<T>::VertexType& VertexContainer<T>::vertex(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** vertices contained in the Vertex container of the
 * Mesh.
 *
 * If vertexNumber() != vertexContainerSize(), it means that there are some vertices that are
 * flagged as deleted.
 *
 * @return The number of non-deleted vertices of the Mesh.
 */
template<typename T>
uint VertexContainer<T>::vertexNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of vertices (also deleted) contained in the Vertex container of the
 * Mesh.
 *
 * If vertexNumber() != vertexContainerSize(), it means that there are some vertices that are
 * flagged as deleted.
 *
 * @return The number of all the vertices contained in the Mesh.
 */
template<typename T>
uint VertexContainer<T>::vertexContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted vertices in the Vertex container, that is
 * vertexContainerSize() - vertexNumber().
 *
 * @return The number of deleted vertices in the container.
 */
template<typename T>
uint vcl::mesh::VertexContainer<T>::deletedVertexNumber() const
{
	return Base::deletedElementNumber();
}

/**
 * @brief Marks as deleted the vertex with the given id.
 *
 * This member function does not perform any reallocation of the vertices: the deleted vertices
 * will stay in the Vertex Container, but will be marked as deleted.
 *
 * Deleted vertices are automatically jumped by the iterators provided by the Vertex Container.
 *
 * @param[in] i: the id of the vertex that will be marked as deleted.
 */
template<typename T>
void VertexContainer<T>::deleteVertex(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given vertex, before asserting that the vertex belongs to this
 * container.
 *
 * This member function does not perform any reallocation of the vertices: the deleted vertices
 * will stay in the Vertex Container, but will be marked as deleted.
 *
 * Deleted vertices are automatically jumped by the iterators provided by the Vertex Container.
 *
 * @param[in] v: the pointer of the vertex that will be marked as deleted.
 */
template<typename T>
void VertexContainer<T>::deleteVertex(const VertexType* v)
{
	Base::deleteElement(v);
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the vertex with the given
 * index.
 *
 * Complexity: O(n), with n the number of vertices in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a vertex of the container.
 * @return The index that the vertex with index i would have if this container would be compact.
 */
template<typename T>
uint vcl::mesh::VertexContainer<T>::vertexIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual vertex index, the new index that the vertex
 * would have in a compacted container. For each deleted vertex index, the value of the vector will
 * be -1.
 *
 * This is useful if you need to know the indices of the vertices that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each vertex index, its index if the container would be compact.
 */
template<typename T>
std::vector<int> VertexContainer<T>::vertexCompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted vertices of the container. You can
 * change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return An iterator the the first vertex of the container.
 */
template<typename T>
typename VertexContainer<T>::VertexIterator VertexContainer<T>::vertexBegin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<typename T>
typename VertexContainer<T>::VertexIterator VertexContainer<T>::vertexEnd()
{
	return Base::elementEnd();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted vertices of the container. You can
 * change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return A const iterator the the first vertex of the container.
 */
template<typename T>
typename VertexContainer<T>::ConstVertexIterator
VertexContainer<T>::vertexBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<typename T>
typename VertexContainer<T>::ConstVertexIterator VertexContainer<T>::vertexEnd() const
{
	return Base::elementEnd();
}

/**
 * @brief Returns a small utility object that allows to iterate over the vertices of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the vertices using the C++
 * foreach syntax:
 *
 * @code{.cpp}
 * for (Vertex& v : m.vertices()){
 *     // do something with this vertex
 * }
 * @endcode
 *
 * The iterator used to iterate over vertices is automatically initialized to jump deleted vertices
 * of the container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<typename T>
typename VertexContainer<T>::VertexRangeIterator VertexContainer<T>::vertices(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small utility object that allows to iterate over the vertices of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the vertices using the C++
 * foreach syntax:
 *
 * @code{.cpp}
 * for (const Vertex& v : m.vertices()){
 *     // do something with this vertex
 * }
 * @endcode
 *
 * The iterator used to iterate over vertices is automatically initialized to jump deleted vertices
 * of the container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * vertices.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<typename T>
typename VertexContainer<T>::ConstVertexRangeIterator
VertexContainer<T>::vertices(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the Vertex type contained in the
 * VertexContainer.
 */
template<typename T>
void VertexContainer<T>::enableAllPerVertexOptionalComponents()
{
	if constexpr (vert::hasOptionalAdjacentEdges<T>())
		enablePerVertexAdjacentEdges();
	if constexpr (vert::hasOptionalAdjacentFaces<T>())
		enablePerVertexAdjacentFaces();
	if constexpr (vert::hasOptionalAdjacentVertices<T>())
		enablePerVertexAdjacentVertices();
	if constexpr (vert::hasOptionalColor<T>())
		enablePerVertexColor();
	if constexpr (vert::hasOptionalMark<T>())
		enablePerVertexMark();
	if constexpr (vert::hasOptionalNormal<T>())
		enablePerVertexNormal();
	if constexpr (vert::hasOptionalPrincipalCurvature<T>())
		enablePerVertexPrincipalCurvature();
	if constexpr (vert::hasOptionalScalar<T>())
		enablePerVertexScalar();
	if constexpr (vert::hasOptionalTexCoord<T>())
		enablePerVertexTexCoord();
}

/**
 * @brief Disables all the optional components associated to the Vertex type contained in the
 * VertexContainer.
 */
template<typename T>
void VertexContainer<T>::disableAllPerVertexOptionalComponents()
{
	if constexpr (vert::hasOptionalAdjacentEdges<T>())
		disablePerVertexAdjacentEdges();
	if constexpr (vert::hasOptionalAdjacentFaces<T>())
		disablePerVertexAdjacentFaces();
	if constexpr (vert::hasOptionalAdjacentVertices<T>())
		disablePerVertexAdjacentVertices();
	if constexpr (vert::hasOptionalColor<T>())
		disablePerVertexColor();
	if constexpr (vert::hasOptionalMark<T>())
		disablePerVertexMark();
	if constexpr (vert::hasOptionalNormal<T>())
		disablePerVertexNormal();
	if constexpr (vert::hasOptionalPrincipalCurvature<T>())
		disablePerVertexPrincipalCurvature();
	if constexpr (vert::hasOptionalScalar<T>())
		disablePerVertexScalar();
	if constexpr (vert::hasOptionalTexCoord<T>())
		disablePerVertexTexCoord();
}

/**
 * @brief Checks if the vertex Optional Adjacent Edges component is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentEdges
 * Component.
 *
 * @return true if the Optional Adjacent Edges is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentEdges<U>(), bool)
VertexContainer<T>::isPerVertexAdjacentEdgesEnabled() const
{
	return Base::optionalVec.isAdjacentEdgesEnabled();
}

/**
 * @brief Enables the Optional Adjacent Edges of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentEdges
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentEdges<U>(), void)
VertexContainer<T>::enablePerVertexAdjacentEdges()
{
	Base::optionalVec.enableAdjacentEdges(vertexContainerSize());
}

/**
 * @brief Disables the Optional Adjacent Edges of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentEdges
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentEdges<U>(), void)
VertexContainer<T>::disablePerVertexAdjacentEdges()
{
	Base::optionalVec.disableAdjacentEdges();
}

/**
 * @brief Checks if the vertex Optional Adjacent Faces component is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentFaces
 * Component.
 *
 * @return true if the Optional Adjacent Faces is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentFaces<U>(), bool)
VertexContainer<T>::isPerVertexAdjacentFacesEnabled() const
{
	return Base::optionalVec.isAdjacentFacesEnabled();
}

/**
 * @brief Enables the Optional Adjacent Faces of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentFaces
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentFaces<U>(), void)
VertexContainer<T>::enablePerVertexAdjacentFaces()
{
	Base::optionalVec.enableAdjacentFaces(vertexContainerSize());
}

/**
 * @brief Disables the Optional Adjacent Faces of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentFaces
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentFaces<U>(), void)
VertexContainer<T>::disablePerVertexAdjacentFaces()
{
	Base::optionalVec.disableAdjacentFaces();
}

/**
 * @brief Checks if the vertex Optional Adjacent Vertices component is enabled.
 *
 * @note This function is available **only if the Vertex Element has the OptionalAdjacentVertices
 * Component.
 *
 * @return true if the Optional Adjacent Vertices is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentVertices<U>(), bool)
VertexContainer<T>::isPerVertexAdjacentVerticesEnabled() const
{
	return Base::optionalVec.isAdjacentVerticesEnabled();
}

/**
 * @brief Enables the Optional Adjacent Vertices of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentVertices
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentVertices<U>(), void)
VertexContainer<T>::enablePerVertexAdjacentVertices()
{
	Base::optionalVec.enableAdjacentVertices(vertexContainerSize());
}

/**
 * @brief Disables the Optional Adjacent Vertices of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalAdjacentVertices
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalAdjacentVertices<U>(), void)
VertexContainer<T>::disablePerVertexAdjacentVertices()
{
	Base::optionalVec.disableAdjacentVertices();
}

/**
 * @brief Checks if the vertex Optional Color is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalColor Component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalColor<U>(), bool)
VertexContainer<T>::isPerVertexColorEnabled() const
{
	return Base::optionalVec.isColorEnabled();
}

/**
 * @brief Enables the Optional Color of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalColor Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalColor<U>(), void)
VertexContainer<T>::enablePerVertexColor()
{
	Base::optionalVec.enableColor(vertexContainerSize());
}

/**
 * @brief Disables the Optional Color of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalColor Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalColor<U>(), void)
VertexContainer<T>::disablePerVertexColor()
{
	Base::optionalVec.disableColor();
}

/**
 * @brief Checks if the vertex Optional Mark is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalMark Component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalMark<U>(), bool)
VertexContainer<T>::isPerVertexMarkEnabled() const
{
	return Base::optionalVec.isMarkEnabled();
}

/**
 * @brief Enables the Optional Mark of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalMark Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalMark<U>(), void)
VertexContainer<T>::enablePerVertexMark()
{
	Base::optionalVec.enableMark(vertexContainerSize());
}

/**
 * @brief Container::disableVertexMark disables the Optional Mark of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalMark Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalMark<U>(), void)
VertexContainer<T>::disablePerVertexMark()
{
	Base::optionalVec.disableMark();
}

/**
 * @brief Checks if the vertex Optional Normal is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalNormal
 * Component.
 *
 * @return true if the Optional Normal is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalNormal<U>(), bool)
VertexContainer<T>::isPerVertexNormalEnabled() const
{
	return Base::optionalVec.isNormalEnabled();
}

/**
 * @brief Enables the Optional Normal of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalNormal Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalNormal<U>(), void)
VertexContainer<T>::enablePerVertexNormal()
{
	Base::optionalVec.enableNormal(vertexContainerSize());
}

/**
 * @brief Disables the Optional Normal of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalNormal Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalNormal<U>(), void)
VertexContainer<T>::disablePerVertexNormal()
{
	Base::optionalVec.disableNormal();
}

/**
 * @brief Checks if the vertex Optional PrincipalCurvature is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalPrincipalCurvature
 * Component.
 *
 * @return true if the Optional PrincipalCurvature is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalPrincipalCurvature<U>(), bool)
VertexContainer<T>::isPerVertexPrincipalCurvatureEnabled() const
{
	return Base::optionalVec.isPrincipalCurvatureEnabled();
}

/**
 * @brief Enables the Optional PrincipalCurvature of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalPrincipalCurvature
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalPrincipalCurvature<U>(), void)
VertexContainer<T>::enablePerVertexPrincipalCurvature()
{
	Base::optionalVec.enablePrincipalCurvature(vertexContainerSize());
}

/**
 * @brief Disables the Optional PrincipalCurvature of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalPrincipalCurvature
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalPrincipalCurvature<U>(), void)
VertexContainer<T>::disablePerVertexPrincipalCurvature()
{
	Base::optionalVec.disablePrincipalCurvature();
}

/**
 * @brief Checks if the vertex Optional Scalar is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalScalar Component.
 *
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalScalar<U>(), bool)
VertexContainer<T>::isPerVertexScalarEnabled() const
{
	return Base::optionalVec.isScalarEnabled();
}

/**
 * @brief Enables the Optional Scalar of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalScalar Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalScalar<U>(), void)
VertexContainer<T>::enablePerVertexScalar()
{
	Base::optionalVec.enableScalar(vertexContainerSize());
}

/**
 * @brief Disables the Optional Scalar of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalScalar Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalScalar<U>(), void)
VertexContainer<T>::disablePerVertexScalar()
{
	Base::optionalVec.disableScalar();
}

/**
 * @brief Checks if the vertex Optional TexCoord is enabled.
 *
 * @note This function is available only if the Vertex Element has the OptionalTexCoord Component.
 *
 * @return true if the Optional TexCoord is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalTexCoord<U>(), bool)
VertexContainer<T>::isPerVertexTexCoordEnabled() const
{
	return Base::optionalVec.isTexCoordEnabled();
}

/**
 * @brief Enables the Optional TexCoord of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalTexCoord Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalTexCoord<U>(), void)
VertexContainer<T>::enablePerVertexTexCoord()
{
	Base::optionalVec.enableTexCoord(vertexContainerSize());
}

/**
 * @brief Disables the Optional TexCoord of the vertex.
 *
 * @note This function is available only if the Vertex Element has the OptionalTexCoord Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasOptionalTexCoord<U>(), void)
VertexContainer<T>::disablePerVertexTexCoord()
{
	Base::optionalVec.disableTexCoord();
}

/**
 * @brief Checks if vertices have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @return `true` if the Vertex Element has a custom component with the given name.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasCustomComponents<U>(), bool)
VertexContainer<T>::hasPerVertexCustomComponent(const std::string& name) const
{
	return Base::optionalVec.componentExists(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of any type associated
 * to the Vertex Element.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom components.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasCustomComponents<U>(), std::vector<std::string>)
VertexContainer<T>::getAllPerVertexCustomComponentNames() const
{
	return Base::optionalVec.allComponentNames();
}

/**
 * @brief Checks if the custom component of the Vertex Element having the given name has the same type
 * of the given template argument type of this function.
 *
 * For example, the following code checks if the component called `cc` is of type `double`:
 * @code{.cpp}
 * if (m.isPerVertexCustomComponentOfType<double>("cc")) {
 *   ...
 * }
 * @endcode
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component to check.
 * @param[in] name: the name of the custom component to check.
 * @return `true` if the custom component is of the same type of the template argument.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(vert::hasCustomComponents<U>(), bool)
VertexContainer<T>::isPerVertexCustomComponentOfType(const std::string& name) const
{
	return Base::optionalVec.template isComponentOfType<K>(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the
 * Vertex Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.getPerVertexCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(vert::hasCustomComponents<U>(), std::vector<std::string>)
VertexContainer<T>::getPerVertexCustomComponentNamesOfType() const
{
	return Base::optionalVec.template allComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the Vertex, having the given name.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents
 * Component.
 *
 * @tparam K: the type of the custom component added to the Vertex.
 * @param[in] name: the name of the custom component added to the Vertex.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(vert::hasCustomComponents<U>(), void)
VertexContainer<T>::addPerVertexCustomComponent(const std::string& name)
{
	Base::optionalVec.template addNewComponent<K>(name, vertexContainerSize());
}

/**
 * @brief Deletes the custom component of the given name from the Vertex Element.
 *
 * The function does nothing if the custom component does not exists.
 *
 * @note This function is available only if the Vertex Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component that will be removed from the Vertex.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(vert::hasCustomComponents<U>(), void)
VertexContainer<T>::deletePerVertexCustomComponent(const std::string& name)
{
	Base::optionalVec.deleteComponent(name);
}

/**
 * @brief Returns a vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **references** to the custom component, therefore you can
 * modify the custom component by modifying the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a vertex custom component named "cc" of type int:
 *
 * @code{.cpp}
 * auto handle = m.getPerVertexCustomComponentVectorHandle<int>("cc");
 * for (Vertex& v : m.vertices() {
 *    handle[m.index(v)] = 5; // v.customComponent<int>("cc") == 5
 *    assert(v.customComponent<int>("cc") == 5);
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 * @param name: name of the custom component on which return the handle.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(
	vert::hasCustomComponents<U>(),
	CustomComponentVectorHandle<K>)
VertexContainer<T>::getPerVertexCustomComponentVectorHandle(const std::string& name)
{
	std::vector<std::any>& cc = Base::optionalVec.template componentVector<K>(name);
	CustomComponentVectorHandle<K> v(cc);
	return v;
}

/**
 * @brief Returns a const vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **const references** to the custom component, therefore you
 * can access to the custom component by accessing the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a vertex custom component named "cc" of type int:
 *
 * @code{.cpp}
 * // access to the const handle by making const the template parameter:
 * auto handle = m.getPerVertexCustomComponentVectorHandle<const int>("cc");
 * int sum = 0;
 * for (const Vertex& v : m.vertices() {
 *    sum += handle[m.index(v)];
 *    // handle[m.index(v)] = 5; // not allowed, because the handle is const
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather accessing from an
 * element object. However, note that references are binded to the container of the mesh.
 *
 * @note Since the handle contains references, any operation that changes the size of the container
 * could be destructive and invalidate the references contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 * @param name: name of the custom component on which return the handle.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(
	vert::hasCustomComponents<U>(),
	ConstCustomComponentVectorHandle<K>)
	VertexContainer<T>::getPerVertexCustomComponentVectorHandle(const std::string& name) const
{
	const std::vector<std::any>& cc = Base::optionalVec.template componentVector<K>(name);
	ConstCustomComponentVectorHandle<K> v(cc);
	return cc;
}

template<typename T>
void VertexContainer<T>::clearVertices()
{
	Base::clearElements();
}

template<typename T>
uint VertexContainer<T>::index(const VertexType* v) const
{
	return Base::index(v);
}

template<typename T>
uint VertexContainer<T>::addVertex()
{
	T* oldB = Base::vec.data();
	Base::vec.push_back(VertexType());
	T* newB = Base::vec.data();
	Base::en++;
	if constexpr (vert::hasVerticalInfo<VertexType>()) {
		Base::setContainerPointer(Base::vec[Base::vec.size() - 1]);
		Base::optionalVec.resize(Base::vec.size());
	}
	Base::updateContainerPointers(oldB, newB);
	return Base::vec.size() - 1;
}

/**
 * @brief Container::addVertices adds nVertices to the Vertex Container of the mesh.
 *
 * Returns the id of the first added vertex.
 *
 * @param nVertices
 * @return the id of the first added vertex.
 */
template<typename T>
uint VertexContainer<T>::addVertices(uint nVertices)
{
	uint baseId = Base::vec.size();
	T*   oldB   = Base::vec.data();
	Base::vec.resize(Base::vec.size() + nVertices);
	T* newB = Base::vec.data();
	Base::en += nVertices;
	if constexpr (vert::hasVerticalInfo<VertexType>()) {
		Base::optionalVec.resize(Base::vec.size());
		for (uint i = baseId; i < Base::vec.size(); ++i) {
			Base::setContainerPointer(Base::vec[i]);
		}
	}
	Base::updateContainerPointers(oldB, newB);
	return baseId;
}

template<typename T>
void VertexContainer<T>::reserveVertices(uint size)
{
	T* oldB = Base::vec.data();
	Base::vec.reserve(size);
	T* newB = Base::vec.data();
	if constexpr (vert::hasVerticalInfo<VertexType>()) {
		Base::optionalVec.reserve(size);
	}
	Base::updateContainerPointers(oldB, newB);
}

/**
 * @brief Compacts the vertex container, keeping only the non-deleted
 * vertices.
 *
 * @return a vector that tells, for each old vertex index, the new index of the vertex. Will contain
 * -1 if the vertex has been deleted.
 */
template<typename T>
std::vector<int> vcl::mesh::VertexContainer<T>::compactVertices()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	uint             k          = 0;
	std::vector<int> newIndices = vertexCompactIndices();
	for (uint i = 0; i < newIndices.size(); ++i) {
		if (newIndices[i] >= 0) {
			k = newIndices[i];
			if (i != k)
				Base::vec[k] = Base::vec[i];
		}
	}
	k++;
	Base::vec.resize(k);
	if constexpr (vert::hasVerticalInfo<VertexType>()) {
		Base::optionalVec.compact(newIndices);
	}
	return newIndices;
}

template<typename T>
void VertexContainer<T>::updateVertexReferences(const T* oldBase, const T* newBase)
{
	if constexpr (vcl::vert::hasOptionalAdjacentVertices<T>()) {
		if (Base::optionalVec.isAdjacentVerticesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateVertexReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (vcl::vert::hasAdjacentVertices<T>()) {
		for (VertexType& v : vertices()) {
			v.updateVertexReferences(oldBase, newBase);
		}
	}
}

template<typename T>
void VertexContainer<T>::updateVertexReferencesAfterCompact(
	const T*                base,
	const std::vector<int>& newIndices)
{
	if constexpr (vcl::vert::hasOptionalAdjacentVertices<T>()) {
		if (Base::optionalVec.isAdjacentVerticesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateVertexReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (vcl::vert::hasAdjacentVertices<T>()) {
		for (VertexType& v : vertices()) {
			v.updateVertexReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Face>
void VertexContainer<T>::updateFaceReferences(const Face* oldBase, const Face* newBase)
{
	if constexpr (vcl::vert::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateFaceReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (vcl::vert::hasAdjacentFaces<T>()) {
		for (VertexType& v : vertices()) {
			v.updateFaceReferences(oldBase, newBase);
		}
	}
}

template<typename T>
template<typename Face>
void VertexContainer<T>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	if constexpr (vcl::vert::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (vcl::vert::hasAdjacentFaces<T>()) {
		for (VertexType& v : vertices()) {
			v.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Edge>
void VertexContainer<T>::updateEdgeReferences(const Edge* oldBase, const Edge* newBase)
{
	if constexpr (vcl::vert::hasOptionalAdjacentEdges<T>()) {
		if (Base::optionalVec.isAdjacentEdgesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateEdgeReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (vcl::vert::hasAdjacentEdges<T>()) {
		for (VertexType& v : vertices()) {
			v.updateEdgeReferences(oldBase, newBase);
		}
	}
}

template<typename T>
template<typename Edge>
void VertexContainer<T>::updateEdgeReferencesAfterCompact(
	const Edge*             base,
	const std::vector<int>& newIndices)
{
	if constexpr (vcl::vert::hasOptionalAdjacentEdges<T>()) {
		if (Base::optionalVec.isAdjacentEdgesEnabled()) {
			for (VertexType& v : vertices()) {
				v.updateEdgeReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (vcl::vert::hasAdjacentEdges<T>()) {
		for (VertexType& v : vertices()) {
			v.updateEdgeReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Mesh>
void VertexContainer<T>::enableOptionalComponentsOf(const Mesh& m)
{
	// unfortunately, this function cannot be shortened in a smart way

	// if edges are enabled in the other Mesh
	if constexpr (hasVertices<Mesh>()) {
		using VertexType  = T;
		using MVertexType = typename Mesh::VertexType;

		// disable all the optional components, we will enable them again depending on Mesh
		disableAllPerVertexOptionalComponents();

		// if this Vertex type has optional adjacent edges
		if constexpr (vert::hasOptionalAdjacentEdges<VertexType>()) {
			// if also the other Mesh Vertex type has optional adjacent edges
			if constexpr (vert::hasOptionalAdjacentEdges<MVertexType>()) {
				// if they are enabled on the other Mesh, enable also here
				if (m.isPerVertexAdjacentEdgesEnabled()) {
					enablePerVertexAdjacentEdges();
				}
			}
			else {
				// if the other Mesh has *non-optional* adjacent edges, I need to enable it on this
				// Vert container
				if constexpr (vert::hasAdjacentEdges<MVertexType>()) {
					enablePerVertexAdjacentEdges();
				}
			}
		}
		if constexpr (vert::hasOptionalAdjacentFaces<VertexType>()) {
			if constexpr (vert::hasOptionalAdjacentFaces<MVertexType>()) {
				if (m.isPerVertexAdjacentFacesEnabled()) {
					enablePerVertexAdjacentFaces();
				}
			}
			else {
				if constexpr (vert::hasAdjacentFaces<MVertexType>()) {
					enablePerVertexAdjacentFaces();
				}
			}
		}
		if constexpr (vert::hasOptionalAdjacentVertices<VertexType>()) {
			if constexpr (vert::hasOptionalAdjacentVertices<MVertexType>()) {
				if (m.isPerVertexAdjacentVerticesEnabled()) {
					enablePerVertexAdjacentVertices();
				}
			}
			else {
				if constexpr (vert::hasAdjacentVertices<MVertexType>()) {
					enablePerVertexAdjacentVertices();
				}
			}
		}
		if constexpr (vert::hasOptionalColor<VertexType>()) {
			if constexpr (vert::hasOptionalColor<MVertexType>()) {
				if (m.isPerVertexColorEnabled()) {
					enablePerVertexColor();
				}
			}
			else {
				if constexpr (vert::hasColor<MVertexType>()) {
					enablePerVertexColor();
				}
			}
		}
		if constexpr (vert::hasOptionalMark<VertexType>()) {
			if constexpr (vert::hasOptionalMark<MVertexType>()) {
				if (m.isPerVertexMarkEnabled()) {
					enablePerVertexMark();
				}
			}
			else {
				if constexpr (vert::hasMark<MVertexType>()) {
					enablePerVertexMark();
				}
			}
		}
		if constexpr (vert::hasOptionalNormal<VertexType>()) {
			if constexpr (vert::hasOptionalNormal<MVertexType>()) {
				if (m.isPerVertexNormalEnabled()) {
					enablePerVertexNormal();
				}
			}
			else {
				if constexpr (vert::hasNormal<MVertexType>()) {
					enablePerVertexNormal();
				}
			}
		}
		if constexpr (vert::hasOptionalPrincipalCurvature<VertexType>()) {
			if constexpr (vert::hasOptionalPrincipalCurvature<MVertexType>()) {
				if (m.isPerVertexPrincipalCurvatureEnabled()) {
					enablePerVertexPrincipalCurvature();
				}
			}
			else {
				if constexpr (vert::hasPrincipalCurvature<MVertexType>()) {
					enablePerVertexPrincipalCurvature();
				}
			}
		}
		if constexpr (vert::hasOptionalScalar<VertexType>()) {
			if constexpr (vert::hasOptionalScalar<MVertexType>()) {
				if (m.isPerVertexScalarEnabled()) {
					enablePerVertexScalar();
				}
			}
			else {
				if constexpr (vert::hasScalar<MVertexType>()) {
					enablePerVertexScalar();
				}
			}
		}
		if constexpr (vert::hasOptionalTexCoord<VertexType>()) {
			if constexpr (vert::hasOptionalTexCoord<MVertexType>()) {
				if (m.isPerVertexTexCoordEnabled()) {
					enablePerVertexTexCoord();
				}
			}
			else {
				if constexpr (vert::hasTexCoord<MVertexType>()) {
					enablePerVertexTexCoord();
				}
			}
		}
	}
}

template<typename T>
template<typename Mesh>
void VertexContainer<T>::importFrom(const Mesh& m)
{
	clearVertices();
	if constexpr (hasVertices<Mesh>()) {
		addVertices(m.vertexContainerSize());
		unsigned int vid = 0;
		for (const typename Mesh::VertexType& v : m.vertices(false)) {
			vertex(vid).importFrom(v);
			if constexpr (vert::hasAdjacentVertices<T>()) {
				vertex(vid).importVertexReferencesFrom(v, Base::vec.data(), &m.vertex(0));
			}
			++vid;
		}
	}
}

template<typename T>
template<typename Mesh, typename Face>
void VertexContainer<T>::importFaceReferencesFrom(const Mesh& m, Face* base)
{
	if constexpr (hasFaces<Mesh>() && hasVertices<Mesh>()) {
		for (uint i = 0; i < vertexContainerSize(); ++i) {
			vertex(i).importFaceReferencesFrom(m.vertex(i), base, &m.face(0));
		}
	}
}

template<typename T>
template<typename Mesh, typename Edge>
void VertexContainer<T>::importEdgeReferencesFrom(const Mesh& m, Edge* base)
{
	if constexpr (hasEdges<Mesh>() && hasVertices<Mesh>()) {
		for (uint i = 0; i < vertexContainerSize(); ++i) {
			vertex(i).importEdgeReferencesFrom(m.vertex(i), base, &m.edge(0));
		}
	}
}

} // namespace vcl::mesh
