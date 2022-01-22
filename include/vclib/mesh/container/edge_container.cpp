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

#include "edge_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of Edges.
 */
template<typename T>
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
template<typename T>
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
template<typename T>
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
template<typename T>
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
template<typename T>
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
template<typename T>
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
template<typename T>
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
 * @param[in] f: the pointer of the edge that will be marked as deleted.
 */
template<typename T>
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
template<typename T>
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
template<typename T>
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
template<typename T>
typename EdgeContainer<T>::EdgeIterator EdgeContainer<T>::edgeBegin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<typename T>
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
template<typename T>
typename EdgeContainer<T>::ConstEdgeIterator EdgeContainer<T>::edgeBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<typename T>
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
 * for (Edge& f : m.edges()){
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
template<typename T>
typename EdgeContainer<T>::EdgeRangeIterator EdgeContainer<T>::edges(bool jumpDeleted)
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
 * for (const Edge& f : m.edges()){
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
template<typename T>
typename EdgeContainer<T>::ConstEdgeRangeIterator EdgeContainer<T>::edges(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the Edge type contained in the
 * EdgeContainer.
 */
template<typename T>
void EdgeContainer<T>::enableAllPerEdgeOptionalComponents()
{
	if constexpr (edge::hasOptionalAdjacentEdges<T>())
		enablePerEdgeAdjacentEdges();
	if constexpr (edge::hasOptionalAdjacentFaces<T>())
		enablePerEdgeAdjacentFaces();
	if constexpr (edge::hasOptionalColor<T>())
		enablePerEdgeColor();
	if constexpr (edge::hasOptionalMark<T>())
		enablePerEdgeMark();
	if constexpr (edge::hasOptionalScalar<T>())
		enablePerEdgeScalar();
}

/**
 * @brief Disables all the optional components associated to the Edge type contained in the
 * EdgeContainer.
 */
template<typename T>
void EdgeContainer<T>::disableAllPerEdgeOptionalComponents()
{
	if constexpr (edge::hasOptionalAdjacentEdges<T>())
		disablePerEdgeAdjacentEdges();
	if constexpr (edge::hasOptionalAdjacentFaces<T>())
		disablePerEdgeAdjacentFaces();
	if constexpr (edge::hasOptionalColor<T>())
		disablePerEdgeColor();
	if constexpr (edge::hasOptionalMark<T>())
		disablePerEdgeMark();
	if constexpr (edge::hasOptionalScalar<T>())
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
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalAdjacentEdges<U>(), bool)
EdgeContainer<T>::isPerEdgeAdjacentEdgesEnabled() const
{
	return Base::optionalVec.isAdjacentEdgesEnabled();
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
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalAdjacentEdges<U>(), void)
EdgeContainer<T>::enablePerEdgeAdjacentEdges()
{
	Base::optionalVec.enableAdjacentEdges(Base::vec.size());
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
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalAdjacentEdges<U>(), void)
EdgeContainer<T>::disablePerEdgeAdjacentEdges()
{
	Base::optionalVec.disableAdjacentEdges();
}

/**
 * @brief Checks if the edge Optional Adjacent Faces is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentFaces
 * Component.
 *
 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalAdjacentFaces<U>(), bool)
EdgeContainer<T>::isPerEdgeAdjacentFacesEnabled() const
{
	return Base::optionalVec.isAdjacentFacesEnabled();
}

/**
 * @brief Enable the Optional Adjacent Faces of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentFaces
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalAdjacentFaces<U>(), void)
EdgeContainer<T>::enablePerEdgeAdjacentFaces()
{
	Base::optionalVec.enableAdjacentFaces(Base::vec.size());
}

/**
 * @brief Disables the Optional Adjacent Faces of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalAdjacentFaces
 * Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalAdjacentFaces<U>(), void)
EdgeContainer<T>::disablePerEdgeAdjacentFaces()
{
	Base::optionalVec.disableAdjacentFaces();
}

/**
 * @brief Checks if the edge Optional Color is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalColor component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalColor<U>(), bool)
EdgeContainer<T>::isPerEdgeColorEnabled() const
{
	return Base::optionalVec.isColorEnabled();
}

/**
 * @brief Enables the Optional Color of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalColor Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalColor<U>(), void)
EdgeContainer<T>::enablePerEdgeColor()
{
	Base::optionalVec.enableColor(edgeContainerSize());
}

/**
 * @brief Disables the Optional Color of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalColor Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalColor<U>(), void)
EdgeContainer<T>::disablePerEdgeColor()
{
	Base::optionalVec.disableColor();
}

/**
 * @brief Checks if the edge Optional Mark is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalMark component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalMark<U>(), bool)
EdgeContainer<T>::isPerEdgeMarkEnabled() const
{
	return Base::optionalVec.isMarkEnabled();
}

/**
 * @brief Enables the Optional Mark of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalMark Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalMark<U>(), void)
EdgeContainer<T>::enablePerEdgeMark()
{
	Base::optionalVec.enableMark(edgeContainerSize());
}

/**
 * @brief Disables the Optional Mark of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalMark Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalMark<U>(), void)
EdgeContainer<T>::disablePerEdgeMark()
{
	Base::optionalVec.disableMark();
}

/**
 * @brief Checks if the edge Optional Scalar is enabled.
 *
 * @note This function is available only if the Edge Element has the OptionalScalar Component.
 *
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalScalar<U>(), bool)
EdgeContainer<T>::isPerEdgeScalarEnabled() const
{
	return Base::optionalVec.isScalarEnabled();
}
/**
 * @brief Enables the Optional Scalar of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalScalar Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalScalar<U>(), void)
EdgeContainer<T>::enablePerEdgeScalar()
{
	Base::optionalVec.enableScalar(edgeContainerSize());
}

/**
 * @brief Disables the Optional Scalar of the edge.
 *
 * @note This function is available only if the Edge Element has the OptionalScalar Component.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasOptionalScalar<U>(), void)
EdgeContainer<T>::disablePerEdgeScalar()
{
	Base::optionalVec.disableScalar();
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
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasCustomComponents<U>(), bool)
EdgeContainer<T>::hasPerEdgeCustomComponent(const std::string& name) const
{
	return Base::optionalVec.componentExists(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of any type associated
 * to the Edge Element.
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom components.
 */
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasCustomComponents<U>(), std::vector<std::string>)
EdgeContainer<T>::getAllPerEdgeCustomComponentNames() const
{
	return Base::optionalVec.allComponentNames();
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
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(edge::hasCustomComponents<U>(), bool)
EdgeContainer<T>::isPerEdgeCustomComponentOfType(const std::string& name) const
{
	return Base::optionalVec.template isComponentOfType<K>(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the Edge
 * Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.getPerEdgeCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(edge::hasCustomComponents<U>(), std::vector<std::string>)
EdgeContainer<T>::getPerEdgeCustomComponentNamesOfType() const
{
	return Base::optionalVec.template allComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the Edge, having the given name.
 *
 * @note This function is available only if the Edge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component added to the Edge.
 * @param[in] name: the name of the custom component added to the Edge.
 */
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(edge::hasCustomComponents<U>(), void)
EdgeContainer<T>::addPerEdgeCustomComponent(const std::string& name)
{
	Base::optionalVec.template addNewComponent<K>(name, edgeContainerSize());
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
template<typename T>
template<typename U>
VCL_ENABLE_IF(edge::hasCustomComponents<U>(), void)
EdgeContainer<T>::deletePerEdgeCustomComponent(const std::string& name)
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
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(
	edge::hasCustomComponents<U>(),
	CustomComponentVectorHandle<K>)
	EdgeContainer<T>::getPerEdgeCustomComponentVectorHandle(const std::string& name)
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
 * For example, assuming that the mesh has a edge custom component named "cc" of type int:
 *
 * @code{.cpp}
 * // access to the const handle by making const the template parameter:
 * auto handle = m.getPerEdgeCustomComponentVectorHandle<const int>("cc");
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
template<typename T>
template<typename K, typename U>
VCL_ENABLE_IF(
	edge::hasCustomComponents<U>(),
	ConstCustomComponentVectorHandle<K>)
	EdgeContainer<T>::getPerEdgeCustomComponentVectorHandle(const std::string& name) const
{
	const std::vector<std::any>& cc = Base::optionalVec.template componentVector<K>(name);
	ConstCustomComponentVectorHandle<K> v(cc);
	return cc;
}

/**
 * @brief Returns the index of the given edge.
 * @param f: edge pointer.
 * @return The index of f.
 */
template<typename T>
uint EdgeContainer<T>::index(const EdgeType* e) const
{
	return Base::index(e);
}

template<typename T>
void vcl::mesh::EdgeContainer<T>::clearEdges()
{
	Base::clearElements();
}

template<typename T>
uint EdgeContainer<T>::addEdge()
{
	return Base::addElement();
}

/**
 * @brief Adds nEdges to the Edge Container of the mesh.
 *
 * Returns the id of the first added edge.
 *
 * @param nEdges
 * @return the id of the first added edge.
 */
template<typename T>
uint vcl::mesh::EdgeContainer<T>::addEdges(uint nEdges)
{
	return Base::addElements(nEdges);
}

template<typename T>
void EdgeContainer<T>::reserveEdges(uint size)
{
	T* oldB = Base::vec.data();
	Base::vec.reserve(size);
	T* newB = Base::vec.data();
	if constexpr (edge::hasVerticalInfo<EdgeType>()) {
		Base::optionalVec.reserve(size);
	}
	Base::updateContainerPointers(oldB, newB);
}

/**
 * @brief Compacts the edge container, keeping only the non-deleted edges.
 *
 * @return a vector that tells, for each old edge index, the new index of the edge. Will contain -1
 * if the edge has been deleted.
 */
template<typename T>
std::vector<int> vcl::mesh::EdgeContainer<T>::compactEdges()
{
	// k will indicate the position of the ith non-deleted vertices after compacting
	uint k = 0;

	std::vector<int> newIndices = edgeCompactIndices();
	for (uint i = 0; i < newIndices.size(); ++i) {
		if (newIndices[i] >= 0) {
			k = newIndices[i];
			if (i != k)
				Base::vec[k] = Base::vec[i];
		}
	}
	k++;
	Base::vec.resize(k);
	if constexpr (edge::hasVerticalInfo<EdgeType>()) {
		Base::optionalVec.compact(newIndices);
	}
	return newIndices;
}

template<typename T>
void EdgeContainer<T>::updateEdgeReferences(const T* oldBase, const T* newBase)
{
	if constexpr (vcl::edge::hasOptionalAdjacentEdges<T>()) {
		if (Base::optionalVec.isAdjacentEdgesEnabled()) {
			for (EdgeType& f : edges()) {
				f.updateEdgeReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (vcl::edge::hasAdjacentEdges<T>()) {
		for (EdgeType& f : edges()) {
			f.updateEdgeReferences(oldBase, newBase);
		}
	}
}

template<typename T>
void EdgeContainer<T>::updateEdgeReferencesAfterCompact(
	const T*                base,
	const std::vector<int>& newIndices)
{
	if constexpr (vcl::edge::hasOptionalAdjacentEdges<T>()) {
		if (Base::optionalVec.isAdjacentEdgesEnabled()) {
			for (EdgeType& f : edges()) {
				f.updateEdgeReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (vcl::edge::hasAdjacentEdges<T>()) {
		for (EdgeType& f : edges()) {
			f.updateEdgeReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Vertex>
void EdgeContainer<T>::updateVertexReferences(const Vertex* oldBase, const Vertex* newBase)
{
	for (EdgeType& e : edges()) {
		e.updateVertexReferences(oldBase, newBase);
	}
}

template<typename T>
template<typename Vertex>
void EdgeContainer<T>::updateVertexReferencesAfterCompact(
	const Vertex*           base,
	const std::vector<int>& newIndices)
{
	for (EdgeType& e : edges()) {
		e.updateVertexReferencesAfterCompact(base, newIndices);
	}
}

template<typename T>
template<typename Face>
void EdgeContainer<T>::updateFaceReferences(const Face* oldBase, const Face* newBase)
{
	if constexpr (vcl::edge::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (EdgeType& e : edges()) {
				e.updateFaceReferences(oldBase, newBase);
			}
		}
	}
	else if constexpr (vcl::edge::hasAdjacentFaces<T>()) {
		for (EdgeType& e : edges()) {
			e.updateFaceReferences(oldBase, newBase);
		}
	}
}

template<typename T>
template<typename Face>
void EdgeContainer<T>::updateFaceReferencesAfterCompact(
	const Face*             base,
	const std::vector<int>& newIndices)
{
	if constexpr (vcl::edge::hasOptionalAdjacentFaces<T>()) {
		if (Base::optionalVec.isAdjacentFacesEnabled()) {
			for (EdgeType& e : edges()) {
				e.updateFaceReferencesAfterCompact(base, newIndices);
			}
		}
	}
	else if constexpr (vcl::edge::hasAdjacentFaces<T>()) {
		for (EdgeType& e : edges()) {
			e.updateFaceReferencesAfterCompact(base, newIndices);
		}
	}
}

template<typename T>
template<typename Mesh>
void EdgeContainer<T>::enableOptionalComponentsOf(const Mesh& m)
{
	// unfortunately, this function cannot be shortened in a smart way

	// if edges are enabled in the other Mesh
	if constexpr (hasEdges<Mesh>()) {
		using EdgeType  = T;
		using MEdgeType = typename Mesh::EdgeType;

		// disable all the optional components, we will enable them again depending on Mesh
		disableAllPerEdgeOptionalComponents();

		// if this Edge type has optional adjacent edges
		if constexpr (edge::hasOptionalAdjacentEdges<EdgeType>()) {
			// if also the other Mesh Edge type has optional adjacent edges
			if constexpr (edge::hasOptionalAdjacentEdges<MEdgeType>()) {
				// if they are enabled on the other Mesh, enable also here
				if (m.isPerEdgeAdjacentEdgesEnabled()) {
					enablePerEdgeAdjacentEdges();
				}
			}
			else {
				// if the other Mesh has *non-optional* adjacent edges, I need to enable it on this
				// Edge container
				if constexpr (edge::hasAdjacentEdges<MEdgeType>()) {
					enablePerEdgeAdjacentEdges();
				}
			}
		}
		if constexpr (edge::hasOptionalAdjacentFaces<EdgeType>()) {
			if constexpr (edge::hasOptionalAdjacentFaces<MEdgeType>()) {
				if (m.isPerEdgeAdjacentFacesEnabled()) {
					enablePerEdgeAdjacentFaces();
				}
			}
			else {
				if constexpr (edge::hasAdjacentFaces<MEdgeType>()) {
					enablePerEdgeAdjacentFaces();
				}
			}
		}
		if constexpr (edge::hasOptionalColor<EdgeType>()) {
			if constexpr (edge::hasOptionalColor<MEdgeType>()) {
				if (m.isPerEdgeColorEnabled()) {
					enablePerEdgeColor();
				}
			}
			else {
				if constexpr (edge::hasColor<MEdgeType>()) {
					enablePerEdgeColor();
				}
			}
		}
		if constexpr (edge::hasOptionalMark<EdgeType>()) {
			if constexpr (edge::hasOptionalMark<MEdgeType>()) {
				if (m.isPerEdgeMarkEnabled()) {
					enablePerEdgeMark();
				}
			}
			else {
				if constexpr (edge::hasMark<MEdgeType>()) {
					enablePerEdgeMark();
				}
			}
		}
		if constexpr (edge::hasOptionalScalar<EdgeType>()) {
			if constexpr (edge::hasOptionalScalar<MEdgeType>()) {
				if (m.isPerEdgeScalarEnabled()) {
					enablePerEdgeScalar();
				}
			}
			else {
				if constexpr (edge::hasScalar<MEdgeType>()) {
					enablePerEdgeScalar();
				}
			}
		}
	}
}

template<typename T>
template<typename Mesh>
void EdgeContainer<T>::importFrom(const Mesh& m)
{
	clearEdges();
	if constexpr (hasEdges<Mesh>()) {
		addEdges(m.edgeContainerSize());
		unsigned int eid = 0;
		for (const typename Mesh::EdgeType& e : m.edges(false)) {
			edge(eid).importFrom(e);
			if constexpr (edge::hasAdjacentEdges<T>()) {
				edge(eid).importEdgeReferencesFrom(e, Base::vec.data(), &m.edge(0));
			}
			++eid;
		}
	}
}

template<typename T>
template<typename Mesh, typename Vertex>
void EdgeContainer<T>::importVertexReferencesFrom(const Mesh& m, Vertex* base)
{
	if constexpr (hasVertices<Mesh>() && hasEdges<Mesh>()) {
		for (uint i = 0; i < edgeContainerSize(); ++i) {
			edge(i).importVertexReferencesFrom(m.edge(i), base, &m.vertex(0));
		}
	}
}

template<typename T>
template<typename Mesh, typename Face>
void EdgeContainer<T>::importFaceReferencesFrom(const Mesh& m, Face* base)
{
	if constexpr (hasFaces<Mesh>() && hasEdges<Mesh>()) {
		for (uint i = 0; i < edgeContainerSize(); ++i) {
			edge(i).importEdgeReferencesFrom(m.edge(i), base, &m.face(0));
		}
	}
}

} // namespace vcl::mesh
