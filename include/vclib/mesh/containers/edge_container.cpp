/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
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

#include "edge_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Edges.
 */
template<EdgeConcept T>
EdgeContainer<T>::EdgeContainer()
{
}

/**
 * @brief Returns a const reference of the edge at the i-th position in the Edge Container of the
 * Mesh, which will be the edge having index = i.
 *
 * This function does not perform any sanity check: if i is less than edgeContainerSize(), this
 * function will return a valid Edge reference (note that the Edge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the edge that will be returned.
 */
template<EdgeConcept T>
const typename EdgeContainer<T>::EdgeType& EdgeContainer<T>::edge(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the edge at the i-th position in the Edge Container of the Mesh,
 * which will be the edge having index = i.
 *
 * This function does not perform any sanity check: if i is less than edgeContainerSize(), this
 * function will return a valid Edge reference (note that the Edge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the edge that will be returned.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::EdgeType& EdgeContainer<T>::edge(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** edges contained in the Edge container of the Mesh.
 *
 * If edgeNumber() != edgeContainerSize(), it means that there are some edges that are
 * flagged as deleted.
 *
 * @return the number of non-deleted edges of the Mesh.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::edgeNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of edges (also deleted) contained in the Edge container of the Mesh.
 *
 * If edgeNumber() != edgeContainerSize(), it means that there are some edges that are
 * flagged as deleted.
 *
 * @return the number of all the edges contained in the Mesh.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::edgeContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted edges in the Edge container, that is edgeContainerSize() -
 * edgeNumber().
 *
 * @return The number of deleted edges in the container.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::deletedEdgeNumber() const
{
	return Base::deletedElementNumber();
}

/**
 * @brief Marks as deleted the edge with the given id.
 *
 * This member function does not perform any reallocation of the edges: the deleted edges
 * will stay in the Edge Container, but will be marked as deleted.
 *
 * Deleted edges are automatically jumped by the iterators provided by the Edge Container.
 *
 * @param[in] i: the id of the edge that will be marked as deleted.
 */
template<EdgeConcept T>
void EdgeContainer<T>::deleteEdge(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given edge, before asserting that the edge belongs to this container.
 *
 * This member function does not perform any reallocation of the edges: the deleted edges
 * will stay in the Edge Container, but will be marked as deleted.
 *
 * Deleted edges are automatically jumped by the iterators provided by the Edge Container.
 *
 * @param[in] e: the pointer of the edge that will be marked as deleted.
 */
template<EdgeConcept T>
void EdgeContainer<T>::deleteEdge(const EdgeType* e)
{
	Base::deleteElement(e);
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the edge with the given
 * index.
 *
 * Complexity: O(n), with n the number of edges in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a edge of the container.
 * @return The index that the edge with index i would have if this container would be compact.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::edgeIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual edge index, the new index that the edge
 * would have in a compacted container. For each deleted edge index, the value of the vector will
 * be -1.
 *
 * This is useful if you need to know the indices of the edges that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each edge index, its index if the container would be compact.
 */
template<EdgeConcept T>
std::vector<int> EdgeContainer<T>::edgeCompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted edges of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted edges.
 * @return An iterator the the first edge of the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::EdgeIterator EdgeContainer<T>::edgeBegin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::EdgeIterator EdgeContainer<T>::edgeEnd()
{
	return Base::elementEnd();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted edges of the container. You can change
 * this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted edges.
 * @return A const iterator the the first edge of the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::ConstEdgeIterator EdgeContainer<T>::edgeBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::ConstEdgeIterator EdgeContainer<T>::edgeEnd() const
{
	return Base::elementEnd();
}

/**
 * @brief Returns a small utility object that allows to iterate over the edges of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the edges using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (Edge& e : m.edges()){
 *     // do something with this edge
 * }
 * @endcode
 *
 * The iterator used to iterate over edges is automatically initialized to jump deleted edges of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted edges.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::EdgeView EdgeContainer<T>::edges(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small utility object that allows to iterate over the edges of the containers,
 * providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the edges using the C++ foreach
 * syntax:
 *
 * @code{.cpp}
 * for (const Edge& e : m.edges()){
 *     // do something with this edge
 * }
 * @endcode
 *
 * The iterator used to iterate over edges is automatically initialized to jump deleted edges of the
 * container. You can change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted edges.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::ConstEdgeView EdgeContainer<T>::edges(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the Edge type contained in the
 * EdgeContainer.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enableAllPerEdgeOptionalComponents()
{
	if constexpr (edge::HasOptionalAdjacentEdges<T>)
		enablePerEdgeAdjacentEdges();
	if constexpr (edge::HasOptionalAdjacentFaces<T>)
		enablePerEdgeAdjacentFaces();
	if constexpr (edge::HasOptionalColor<T>)
		enablePerEdgeColor();
	if constexpr (edge::HasOptionalMark<T>)
		enablePerEdgeMark();
	if constexpr (edge::HasOptionalScalar<T>)
		enablePerEdgeScalar();
}

/**
 * @brief Disables all the optional components associated to the Edge type contained in the
 * EdgeContainer.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disableAllPerEdgeOptionalComponents()
{
	if constexpr (edge::HasOptionalAdjacentEdges<T>)
		disablePerEdgeAdjacentEdges();
	if constexpr (edge::HasOptionalAdjacentFaces<T>)
		disablePerEdgeAdjacentFaces();
	if constexpr (edge::HasOptionalColor<T>)
		disablePerEdgeColor();
	if constexpr (edge::HasOptionalMark<T>)
		disablePerEdgeMark();
	if constexpr (edge::HasOptionalScalar<T>)
		disablePerEdgeScalar();
}

/**
 * @brief Checks if the edge Optional Adjacent Edges is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentEdges
 * Component.
 *
 * @return true if the Optional AdjacentEdges is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeAdjacentEdgesEnabled()
	const requires edge::HasOptionalAdjacentEdges<T>
{
	return Base::template isOptionalComponentEnabled<typename T::AdjacentEdgesComponent>();
}

/**
 * @brief Enable the Optional Adjacent Edges of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentEdges
 * Component.
 *
 * @note If the Edge is polygonal (dynamic size, N < 0), when enabled, the adjacent edges number
 * will be the same of the vertex number for each edge of the container. This is because, for Edges,
 * Adjacent Edges number is tied to the number of vertices.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeAdjacentEdges() requires edge::HasOptionalAdjacentEdges<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentEdgesComponent>();
	static const int N = T::VERTEX_NUMBER;
	if constexpr (N < 0) {
		for (T& f : edges()) {
			f.resizeAdjEdges(f.vertexNumber());
		}
	}
}

/**
 * @brief Disables the Optional Adjacent Edges of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentEdges
 * Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeAdjacentEdges() requires edge::HasOptionalAdjacentEdges<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentEdgesComponent>();
}

/**
 * @brief Checks if the edge Optional Adjacent Faces is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentFaces
 * Component.
 *
 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeAdjacentFacesEnabled() const requires edge::HasOptionalAdjacentFaces<T>
{
	return Base::template isOptionalComponentEnabled<typename T::AdjacentFacesComponent>();
}

/**
 * @brief Enable the Optional Adjacent Faces of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentFaces
 * Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeAdjacentFaces() requires edge::HasOptionalAdjacentFaces<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentFacesComponent>();
}

/**
 * @brief Disables the Optional Adjacent Faces of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentFaces
 * Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeAdjacentFaces() requires edge::HasOptionalAdjacentFaces<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentFacesComponent>();
}

/**
 * @brief Checks if the edge Optional Color is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalColor component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeColorEnabled() const requires edge::HasOptionalColor<T>
{
	return Base::template isOptionalComponentEnabled<typename T::ColorComponent>();
}

/**
 * @brief Enables the Optional Color of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalColor Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeColor() requires edge::HasOptionalColor<T>
{
	return Base::template enableOptionalComponent<typename T::ColorComponent>();
}

/**
 * @brief Disables the Optional Color of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalColor Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeColor() requires edge::HasOptionalColor<T>
{
	return Base::template disableOptionalComponent<typename T::ColorComponent>();
}

/**
 * @brief Checks if the edge Optional Mark is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalMark component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeMarkEnabled() const requires edge::HasOptionalMark<T>
{
	return Base::template isOptionalComponentEnabled<typename T::MarkComponent>();
}

/**
 * @brief Enables the Optional Mark of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalMark Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeMark() requires edge::HasOptionalMark<T>
{
	return Base::template enableOptionalComponent<typename T::MarkComponent>();
}

/**
 * @brief Disables the Optional Mark of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalMark Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeMark() requires edge::HasOptionalMark<T>
{
	return Base::template disableOptionalComponent<typename T::MarkComponent>();
}

/**
 * @brief Checks if the edge Optional Scalar is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalScalar Component.
 *
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeScalarEnabled() const requires edge::HasOptionalScalar<T>
{
	return Base::template isOptionalComponentEnabled<typename T::ScalarComponent>();
}
/**
 * @brief Enables the Optional Scalar of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalScalar Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeScalar() requires edge::HasOptionalScalar<T>
{
	return Base::template enableOptionalComponent<typename T::ScalarComponent>();
}

/**
 * @brief Disables the Optional Scalar of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalScalar Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeScalar() requires edge::HasOptionalScalar<T>
{
	return Base::template disableOptionalComponent<typename T::ScalarComponent>();
}

/**
 * @brief Checks if edges have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @return `true` if the Edge Element has a custom component with the given name.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::hasPerEdgeCustomComponent(const std::string& name) const
	requires edge::HasCustomComponents<T>
{
	return Base::hasElemCustomComponent(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of any type associated
 * to the Edge Element.
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom components.
 */
template<EdgeConcept T>
std::vector<std::string> EdgeContainer<T>::perEdgeCustomComponentNames() const
	requires edge::HasCustomComponents<T>
{
	return Base::elemCustomComponentNames();
}

/**
 * @brief Checks if the custom component of the Edge Element having the given name has the same type
 * of the given template argument type of this function.
 *
 * For example, the following code checks if the component called `cc` is of type `double`:
 * @code{.cpp}
 * if (m.isPerEdgeCustomComponentOfType<double>("cc")) {
 *   ...
 * }
 * @endcode
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component to check.
 * @param[in] name: the name of the custom component to check.
 * @return `true` if the custom component is of the same type of the template argument.
 */
template<EdgeConcept T>
template<typename K>
bool EdgeContainer<T>::isPerEdgeCustomComponentOfType(const std::string& name) const
	requires edge::HasCustomComponents<T>
{
	return Base::template isElemCustomComponentOfType<K>(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the Edge
 * Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.perEdgeCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<EdgeConcept T>
template<typename K>
std::vector<std::string> EdgeContainer<T>::perEdgeCustomComponentNamesOfType() const
	requires edge::HasCustomComponents<T>
{
	return Base::template elemCustomComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the Edge, having the given name.
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component added to the Edge.
 * @param[in] name: the name of the custom component added to the Edge.
 */
template<EdgeConcept T>
template<typename K>
void EdgeContainer<T>::addPerEdgeCustomComponent(const std::string& name)
	requires edge::HasCustomComponents<T>
{
	Base::template addElemCustomComponent<K>(name);
}

/**
 * @brief Deletes the custom component of the given name from the Edge Element.
 *
 * The function does nothing if the custom component does not exists.
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component that will be removed from the Edge.
 */
template<EdgeConcept T>
void EdgeContainer<T>::deletePerEdgeCustomComponent(const std::string& name)
	requires edge::HasCustomComponents<T>
{
	Base::deleteElemCustomComponent(name);
}

/**
 * @brief Returns a vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **references** to the custom component, therefore you can
 * modify the custom component by modifying the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a edge custom component named "cc" of type int:
 *
 * @code{.cpp}
 * auto handle = m.getPerEdgeCustomComponentVectorHandle<int>("cc");
 * for (Edge& e : m.edges() {
 *    handle[m.index(e)] = 5; // e.customComponent<int>("cc") == 5
 *    assert(e.customComponent<int>("cc") == 5);
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
template<EdgeConcept T>
template<typename K>
CustomComponentVectorHandle<K>
EdgeContainer<T>::perEdgeCustomComponentVectorHandle(const std::string& name)
	requires edge::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

/**
 * @brief Returns a const vector handle to the given custom component. The handle can be used like a
 * normal std::vector, but does not have access to the modifiers member functions (resize,
 * push_back...). The handle contains **const references** to the custom component, therefore you
 * can access to the custom component by accessing the element of the handle vector normally. Since
 * the handle stores references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a edge custom component named "cc" of type int:
 *
 * @code{.cpp}
 * // access to the const handle by making const the template parameter:
 * auto handle = m.perEdgeCustomComponentVectorHandle<const int>("cc");
 * int sum = 0;
 * for (const Edge& e : m.edges() {
 *    sum += handle[m.index(e)];
 *    // handle[m.index(e)] = 5; // not allowed, because the handle is const
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
template<EdgeConcept T>
template<typename K>
ConstCustomComponentVectorHandle<K>
EdgeContainer<T>::perEdgeCustomComponentVectorHandle(const std::string& name) const
	requires edge::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

} // namespace vcl::mesh
