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

#ifndef VCL_MESH_CONTAINER_EDGE_CONTAINER_H
#define VCL_MESH_CONTAINER_EDGE_CONTAINER_H

#include <vclib/mesh/containers/custom_component_vector_handle.h>
#include <vclib/mesh/elements/edge.h>
#include <vclib/mesh/elements/edge_components.h>

#include "element_container.h"

namespace vcl::mesh {

/******************************************************************************
 *                                Declarations                                *
 ******************************************************************************/

/**
 * @brief The EdgeContainer class represents a container of Edge
 * elements that can be used in a Mesh class.
 * 
 * This class adds a container (vector) of Edges to the Mesh, making
 * available the accessors members to them and their components.
 * 
 * It provides all the add, delete, reserve, compact and other functions to
 * manage the Edges, plus the iterators and views to iterate over them.
 * It also provides the functions to enable and disable the optional components
 * of the Edges.
 *
 * @tparam T: The type of the Edge elements. It must satisfy the
 * EdgeConcept.
 *
 * @ingroup containers
 */
template<EdgeConcept T>
class EdgeContainer : public ElementContainer<T>
{
	template <EdgeConcept U>
	friend class EdgeContainer;

	using EdgeContainerType = EdgeContainer<T>;
	using Base = ElementContainer<T>;

public:
	using Edge              = T;
	using EdgeType          = T;
	using EdgeIterator      = Base::ElementIterator;
	using ConstEdgeIterator = Base::ConstElementIterator;

	EdgeContainer();

	const EdgeType& edge(uint i) const;
	EdgeType&       edge(uint i);

	uint edgeNumber() const;
	uint edgeContainerSize() const;
	uint deletedEdgeNumber() const;

	uint addEdge();
	uint addEdge(uint v0, uint v1);
	uint addEdge(typename T::VertexType* v0, typename T::VertexType* v1);
	uint addEdges(uint n);

	void clearEdges();
	void resizeEdges(uint n);
	void reserveEdges(uint n);

	void compactEdges();

	void deleteEdge(uint i);
	void deleteEdge(const EdgeType* e);

	uint              edgeIndexIfCompact(uint id) const;
	std::vector<uint> edgeCompactIndices() const;

	void updateEdgeIndices(const std::vector<uint>& newIndices);

	EdgeIterator      edgeBegin(bool jumpDeleted = true);
	EdgeIterator      edgeEnd();
	ConstEdgeIterator edgeBegin(bool jumpDeleted = true) const;
	ConstEdgeIterator edgeEnd() const;
	auto              edges(bool jumpDeleted = true);
	auto              edges(bool jumpDeleted = true) const;

	void enableAllPerEdgeOptionalComponents();
	void disableAllPerEdgeOptionalComponents();

	// AdjacentEdges
	bool isPerEdgeAdjacentEdgesEnabled() const
		requires edge::HasOptionalAdjacentEdges<T>;
	void enablePerEdgeAdjacentEdges()
		requires edge::HasOptionalAdjacentEdges<T>;
	void disablePerEdgeAdjacentEdges()
		requires edge::HasOptionalAdjacentEdges<T>;

	// AdjacentFaces
	bool isPerEdgeAdjacentFacesEnabled() const
		requires edge::HasOptionalAdjacentFaces<T>;
	void enablePerEdgeAdjacentFaces()
		requires edge::HasOptionalAdjacentFaces<T>;
	void disablePerEdgeAdjacentFaces()
		requires edge::HasOptionalAdjacentFaces<T>;

	// Color
	bool isPerEdgeColorEnabled() const requires edge::HasOptionalColor<T>;
	void enablePerEdgeColor() requires edge::HasOptionalColor<T>;
	void disablePerEdgeColor() requires edge::HasOptionalColor<T>;

	// Mark
	bool isPerEdgeMarkEnabled() const requires edge::HasOptionalMark<T>;
	void enablePerEdgeMark() requires edge::HasOptionalMark<T>;
	void disablePerEdgeMark() requires edge::HasOptionalMark<T>;

	// Quality
	bool isPerEdgeQualityEnabled() const requires edge::HasOptionalQuality<T>;
	void enablePerEdgeQuality() requires edge::HasOptionalQuality<T>;
	void disablePerEdgeQuality() requires edge::HasOptionalQuality<T>;

	// Custom Components
	bool hasPerEdgeCustomComponent(const std::string& name) const
		requires edge::HasCustomComponents<T>;

	std::vector<std::string> perEdgeCustomComponentNames() const
		requires edge::HasCustomComponents<T>;

	template<typename K>
	bool isPerEdgeCustomComponentOfType(const std::string& name) const
		requires edge::HasCustomComponents<T>;

	std::type_index perEdgeCustomComponentType(const std::string& name) const
		requires edge::HasCustomComponents<T>;

	template<typename K>
	std::vector<std::string> perEdgeCustomComponentNamesOfType() const
		requires edge::HasCustomComponents<T>;

	template<typename K>
	void addPerEdgeCustomComponent(const std::string& name)
		requires edge::HasCustomComponents<T>;

	void deletePerEdgeCustomComponent(const std::string& name)
		requires edge::HasCustomComponents<T>;

	template<typename K>
	CustomComponentVectorHandle<K> perEdgeCustomComponentVectorHandle(
		const std::string& name) requires edge::HasCustomComponents<T>;

	template<typename K>
	ConstCustomComponentVectorHandle<K>
	perEdgeCustomComponentVectorHandle(const std::string& name) const
		requires edge::HasCustomComponents<T>;
};

/******************************************************************************
 *                                Definitions                                 *
 ******************************************************************************/

/**
 * @brief Empty constructor that creates an empty container of Edges.
 */
template<EdgeConcept T>
EdgeContainer<T>::EdgeContainer()
{
}

/**
 * @brief Returns a const reference of the Edge at the i-th position
 * in the Edge Container of the Mesh, which will be the Edge having
 * index = i.
 *
 * This function does not perform any sanity check: if i is less than
 * edgeContainerSize(), this function will return a valid Edge
 * reference (note that the Edge may have been flagged as deleted).
 *
 * @param[in] i: the index of the Edge that will be returned.
 */
template<EdgeConcept T>
const typename EdgeContainer<T>::EdgeType& EdgeContainer<T>::edge(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the Edge at the i-th position in the
 * Edge Container of the Mesh, which will be the Edge having
 * index = i.
 *
 * This function does not perform any sanity check: if i is less than
 * edgeContainerSize(), this function will return a valid Edge
 * reference (note that the Edge may have been flagged as deleted).
 *
 * @param[in] i: the index of the Edge that will be returned.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::EdgeType& EdgeContainer<T>::edge(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** Edges contained in the
 * Edge container of the Mesh.
 *
 * If edgeNumber() != edgeContainerSize(), it means that there are some
 * Edges that are flagged as deleted.
 *
 * @return the number of non-deleted Edges of the Mesh.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::edgeNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of Edges (also deleted) contained in the
 * Edge container of the Mesh.
 *
 * If edgeNumber() != edgeContainerSize(), it means that there are some
 * Edges that are flagged as deleted.
 *
 * @return the number of all the Edges contained in the Mesh.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::edgeContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted Edges in the Edge container,
 * that is edgeContainerSize() - edgeNumber().
 *
 * @return The number of deleted Edges in the container.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::deletedEdgeNumber() const
{
	return Base::deletedElementNumber();
}

/**
 * @brief Add an Edge to the container, returning its index.
 *
 * If the call of this function will cause a reallocation of the
 * EdgeContainer, the function will automatically take care of updating
 * all the Edge pointers contained in the Mesh.
 *
 * @return the id of the newly added Edge.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::addEdge()
{
	return Base::addElement();
}

/**
 * @brief Add an Edge having the two given vertices to the container, returning
 * its index.
 *
 * If the call of this function will cause a reallocation of the
 * EdgeContainer, the function will automatically take care of updating
 * all the Edge pointers contained in the Mesh.
 *
 * @param[in] v0: the index of the first vertex of the edge.
 * @param[in] v1: the index of the second vertex of the edge.
 * @return the id of the newly added Edge.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::addEdge(uint v0, uint v1)
{
	uint eid = addEdge();
	edge(eid).vertex(0) = &Base::parentMesh->vertex(v0);
	edge(eid).vertex(1) = &Base::parentMesh->vertex(v1);
	return eid;
}

/**
 * @brief Add an Edge having the two given vertices to the container, returning
 * its index.
 *
 * If the call of this function will cause a reallocation of the
 * EdgeContainer, the function will automatically take care of updating
 * all the Edge pointers contained in the Mesh.
 *
 * @param[in] v0: the pointer to the first vertex of the edge.
 * @param[in] v1: the pointer to the second vertex of the edge.
 * @return the id of the newly added Edge.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::addEdge(
	typename T::VertexType* v0,
	typename T::VertexType* v1)
{
	uint eid = addEdge();
	edge(eid).vertex(0) = v0;
	edge(eid).vertex(1) = v1;
	return eid;
}

/**
 * @brief Add an arbitrary number of n Edges, returning the id of the
 * first added Edge.
 *
 * This means that, if you want to add 5 Edges and this member function
 * returns 4, the added Edges will have id from 4 to id 8 included.
 *
 * If the call of this function will cause a reallocation of the
 * EdgeContainer, the function will automatically take care of updating
 * all the Edge pointers contained in the Mesh.
 *
 * @param n: the number of Edges to add to the mesh.
 * @return the id of the first added Edge.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::addEdges(uint n)
{
	return Base::addElements(n);
}

/**
 * @brief Clears the Edge container of the Mesh, deleting all the
 * Edges.
 *
 * The contained edges are actually removed from the container, not only
 * marked as deleted. Therefore, the container will have size 0
 * (`edgeContainerSize() == 0`) after the call of this function.
 *
 * @note This function does not cause a reallocation of the Edge
 * container.
 *
 * @warning Any pointer to Edges in the Mesh will be left unchanged, and
 * therefore will point to invalid Edges. This means that, if you have a
 * pointer to a Edge and you call this function, you will have a dangling
 * pointer.
 */
template<EdgeConcept T>
void EdgeContainer<T>::clearEdges()
{
	Base::clearElements();
}

/**
 * @brief Resizes the Edge container to contain `n` Edges.
 *
 * If the new size is greater than the old one, new Edges are added to the
 * container, and a reallocation may happen. If the new size is smaller than the
 * old one, the container will keep its first non-deleted `n` Edges, and
 * the remaining Edges are marked as deleted.
 *
 * If the call of this function will cause a reallocation of the Edge
 * container, the function will automatically take care of updating all the
 * Edge pointers contained in the Mesh.
 *
 * @warning The given size `n` is relative to the number of non-deleted
 * Edges, not to the size of the Edge container. For example, if you
 * have a mesh with 10 Edges and edgeContainerSize() == 20, calling
 * resizeEdges(5) will not cause a reallocation of the container, but will
 * mark as deleted the least 5 non-deleted Edges of the container. In the
 * same scenario, calling resizeEdges(15) will result in a Edge
 * container having 15 new Edges and edgeContainerSize() == 25.
 * The latest 5 Edges will be the newly added.
 *
 * @warning Any pointer to deleted Edges in the Mesh will be left
 * unchanged, and therefore will point to invalid Edges. This means that
 * if you call this member function with a lower number of Edges, you'll
 * need to manually manage the pointers to the deleted Edges.
 *
 * @param[in] n: the new size of the Edge container.
 */
template<EdgeConcept T>
void EdgeContainer<T>::resizeEdges(uint n)
{
	Base::resizeElements(n);
}

/**
 * @brief Reserve a number of Edges in the container of Edges. This
 * is useful when you know (or you have an idea) of how much Edges are
 * going to add into a newly of existing mesh. Calling this function before any
 * addEdge() call will avoid unuseful reallocations of the container,
 * saving execution time.
 *
 * The filosofy of this function is similar to the one of the
 * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
 * function of the
 * [std::vector class](https://en.cppreference.com/w/cpp/container/vector).
 *
 * If the call of this function will cause a reallocation of the Edge
 * container, the function will automatically take care of updating all the
 * Edge pointers contained in the Mesh.
 *
 * @param n: the new capacity of the Edge container.
 */
template<EdgeConcept T>
void EdgeContainer<T>::reserveEdges(uint n)
{
	Base::reserveElements(n);
}

/**
 * @brief Compacts the EdgeContainer, removing all the Edges marked
 * as deleted. Edges indices will change accordingly. The function will
 * automatically take care of updating all the Edge pointers contained
 * in the Mesh.
 */
template<EdgeConcept T>
void EdgeContainer<T>::compactEdges()
{
	Base::compactElements();
}

/**
 * @brief Marks as deleted the Edge with the given id.
 *
 * This member function does not perform any reallocation of the Edges:
 * the deleted Edges will stay in the Edge Container, but will be
 * marked as deleted.
 *
 * Deleted Edges are automatically jumped by the iterators provided by
 * the Edge Container.
 *
 * @param[in] i: the id of the Edge that will be marked as deleted.
 */
template<EdgeConcept T>
void EdgeContainer<T>::deleteEdge(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given Edge, before asserting that the
 * Edge belongs to this container.
 *
 * This member function does not perform any reallocation of the Edges:
 * the deleted Edges will stay in the Edge Container, but will be
 * marked as deleted.
 *
 * Deleted Edges are automatically jumped by the iterators provided by
 * the Edge Container.
 *
 * @param[in] e: the pointer of the Edge that will be marked as deleted.
 */
template<EdgeConcept T>
void EdgeContainer<T>::deleteEdge(const EdgeType* e)
{
	Base::deleteElement(e);
}

/**
 * @brief This is an utility member function that returns the index of an
 * element if the container would be compact, that is the number of non-deleted
 * elements before the Edge with the given index.
 *
 * Complexity: O(n), with n the number of Edges in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a Edge of the container.
 * @return The index that the Edge with index i would have if this
 * container would be compact.
 */
template<EdgeConcept T>
uint EdgeContainer<T>::edgeIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual Edge index, the
 * new index that the Edge would have in a compacted container. For each
 * deleted Edge index, the value of the vector will be UINT_NULL.
 *
 * This is useful if you need to know the indices of the Edges that they
 * would have in a compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each Edge index, its index if the
 * container would be compact.
 */
template<EdgeConcept T>
std::vector<uint> EdgeContainer<T>::edgeCompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Updates all the indices and pointers of the Edges of this
 * container that are stored in any container of the mesh, according to
 * the mapping stored in the newIndices vector, that tells for each old
 * Edge index, the new Edge index.
 *
 * This function is useful when delete some Edges, and you want to update
 * the indices/pointers stored in all the containers of the mesh accordingly.
 *
 * E.g. Supposing you deleted a set of Edges, you can give to this
 * function the vector telling, for each one of the old Edge indices,
 * the new Edge index (or UINT_NULL if you want to leave it unreferenced).
 * This function will update all the pointers stored in the mesh containers
 * accordingly (if they store adjacencies to the Edges).
 *
 * @note This function *does not change the position of the Edges in
 * this container*. It just updates the indices/pointers of the Edges
 * stored in this or other containers.
 *
 * @param[in] newIndices: a vector that tells, for each old Edge index,
 * the new Edge index. If the old Edge must be left as unreferenced
 * (setting `nullptr` to the pointers), the value of the vector must be
 * UINT_NULL.
 */
template<EdgeConcept T>
void EdgeContainer<T>::updateEdgeIndices(
	const std::vector<uint>& newIndices)
{
	Base::updateElementIndices(newIndices);
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted Edges of
 * the container. You can change this option by calling this function with
 * `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator
 * should jump deleted Edges.
 * @return An iterator the the first Edge of the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::EdgeIterator
EdgeContainer<T>::edgeBegin(bool jumpDeleted)
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
 * The iterator is automatically initialized to jump deleted Edges of
 * the container. You can change this option by calling this function with
 * `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
 * deleted Edges.
 * @return A const iterator the the first Edge of the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::ConstEdgeIterator
EdgeContainer<T>::edgeBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<EdgeConcept T>
typename EdgeContainer<T>::ConstEdgeIterator
EdgeContainer<T>::edgeEnd() const
{
	return Base::elementEnd();
}

/**
 * @brief Returns a small view object that allows to iterate over the
 * Edges of the containers, providing two member functions begin()
 * and end().
 *
 * This member function is very useful when you want to iterate over the
 * Edges using the C++ foreach syntax:
 *
 * @code{.cpp}
 * for (Edge& e : m.edges()){
 *     // do something with this Edge
 * }
 * @endcode
 *
 * The iterator used to iterate over Edges is automatically initialized
 * to jump deleted Edges of the container. You can change this option by
 * calling this function with `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
 * deleted Edges.
 * @return An object having begin() and end() function, allowing to iterate
 * over the container.
 */
template<EdgeConcept T>
auto EdgeContainer<T>::edges(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small view object that allows to iterate over the
 * Edges of the containers, providing two member functions begin()
 * and end().
 *
 * This member function is very useful when you want to iterate over the
 * Edges using the C++ foreach syntax:
 *
 * @code{.cpp}
 * for (const Edge& e : m.edges()){
 *     // do something with this Edge
 * }
 * @endcode
 *
 * The iterator used to iterate over Edges is automatically initialized
 * to jump deleted Edges of the container. You can change this option by
 * calling this function with `jumpDeleted=false`.
 *
 * @param[in] jumpDeleted: boolean that tells if the iterator should jump
 * deleted Edges.
 * @return An object having begin() and end() function, allowing to iterate over
 * the container.
 */
template<EdgeConcept T>
auto EdgeContainer<T>::edges(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Enables all the optional components associated to the Edge
 * type contained in the EdgeContainer.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enableAllPerEdgeOptionalComponents()
{
	Base::enableAllOptionalComponents();
}

/**
 * @brief Disables all the optional components associated to the Edge
 * type contained in the EdgeContainer.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disableAllPerEdgeOptionalComponents()
{
	Base::disableAllOptionalComponents();
}

/**
 * @brief Checks if the Edge Optional AdjacentEdges is enabled.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalAdjacentEdges Component.
 *
 * @return true if the Optional AdjacentEdges is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeAdjacentEdgesEnabled() const
	requires edge::HasOptionalAdjacentEdges<T>
{
	return Base::template isOptionalComponentEnabled<
		typename T::AdjacentEdges>();
}

/**
 * @brief Enable the Optional AdjacentEdges of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalAdjacentEdges Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeAdjacentEdges()
	requires edge::HasOptionalAdjacentEdges<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentEdges>();
}

/**
 * @brief Disables the Optional AdjacentEdges of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalAdjacentEdges Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeAdjacentEdges()
	requires edge::HasOptionalAdjacentEdges<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentEdges>();
}

/**
 * @brief Checks if the Edge Optional AdjacentFaces is enabled.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalAdjacentFaces Component.
 *
 * @return true if the Optional AdjacentFaces is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeAdjacentFacesEnabled() const
	requires edge::HasOptionalAdjacentFaces<T>
{
	return Base::template isOptionalComponentEnabled<
		typename T::AdjacentFaces>();
}

/**
 * @brief Enable the Optional AdjacentFaces of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalAdjacentFaces Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeAdjacentFaces()
	requires edge::HasOptionalAdjacentFaces<T>
{
	Base::template enableOptionalComponent<typename T::AdjacentFaces>();
}

/**
 * @brief Disables the Optional AdjacentFaces of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalAdjacentFaces Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeAdjacentFaces()
	requires edge::HasOptionalAdjacentFaces<T>
{
	Base::template disableOptionalComponent<typename T::AdjacentFaces>();
}

/**
 * @brief Checks if the Edge Optional Color is enabled.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalColor Component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeColorEnabled() const
	requires edge::HasOptionalColor<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Color>();
}

/**
 * @brief Enable the Optional Color of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalColor Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeColor()
	requires edge::HasOptionalColor<T>
{
	Base::template enableOptionalComponent<typename T::Color>();
}

/**
 * @brief Disables the Optional Color of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalColor Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeColor()
	requires edge::HasOptionalColor<T>
{
	Base::template disableOptionalComponent<typename T::Color>();
}

/**
 * @brief Checks if the Edge Optional Mark is enabled.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalMark Component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeMarkEnabled() const
	requires edge::HasOptionalMark<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Mark>();
}

/**
 * @brief Enable the Optional Mark of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalMark Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeMark()
	requires edge::HasOptionalMark<T>
{
	Base::template enableOptionalComponent<typename T::Mark>();
}

/**
 * @brief Disables the Optional Mark of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalMark Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeMark()
	requires edge::HasOptionalMark<T>
{
	Base::template disableOptionalComponent<typename T::Mark>();
}

/**
 * @brief Checks if the Edge Optional Quality is enabled.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalQuality Component.
 *
 * @return true if the Optional Quality is enabled, false otherwise.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::isPerEdgeQualityEnabled() const
	requires edge::HasOptionalQuality<T>
{
	return Base::template isOptionalComponentEnabled<typename T::Quality>();
}

/**
 * @brief Enable the Optional Quality of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalQuality Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::enablePerEdgeQuality()
	requires edge::HasOptionalQuality<T>
{
	Base::template enableOptionalComponent<typename T::Quality>();
}

/**
 * @brief Disables the Optional Quality of the Edge.
 *
 * @note This function is available only if the Edge Element has
 * the OptionalQuality Component.
 */
template<EdgeConcept T>
void EdgeContainer<T>::disablePerEdgeQuality()
	requires edge::HasOptionalQuality<T>
{
	Base::template disableOptionalComponent<typename T::Quality>();
}

/**
 * @brief Checks if Edges have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @return `true` if the Edge Element has a custom component with the
 * given name.
 */
template<EdgeConcept T>
bool EdgeContainer<T>::hasPerEdgeCustomComponent(
	const std::string& name) const requires edge::HasCustomComponents<T>
{
	return Base::hasElemCustomComponent(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of
 * any type associated to the Edge Element.
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom
 * components.
 */
template<EdgeConcept T>
std::vector<std::string>
EdgeContainer<T>::perEdgeCustomComponentNames() const
	requires edge::HasCustomComponents<T>
{
	return Base::elemCustomComponentNames();
}

/**
 * @brief Checks if the custom component of the Edge Element having the
 * given name has the same type of the given template argument type of this
 * function.
 *
 * For example, the following code checks if the component called `cc` is of
 * type `double`:
 * @code{.cpp}
 * if (m.isPerEdgeCustomComponentOfType<double>("cc")) {
 *   ...
 * }
 * @endcode
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @tparam K: the type of the custom component to check.
 *
 * @param[in] name: the name of the custom component to check.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return `true` if the custom component is of the same type of the template
 * argument.
 */
template<EdgeConcept T>
template<typename K>
bool EdgeContainer<T>::isPerEdgeCustomComponentOfType(
	const std::string& name) const requires edge::HasCustomComponents<T>
{
	return Base::template isElemCustomComponentOfType<K>(name);
}

/**
 * @brief Returns the std::type_index of the custom component of the
 * Edge Element having the given input name.
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @param[in] name: the name of the custom component to get the std::type_index
 * from.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return The std::type_index of the custom component having the given input
 * name.
 */
template<vcl::EdgeConcept T>
std::type_index
EdgeContainer<T>::perEdgeCustomComponentType(
	const std::string& name) const requires edge::HasCustomComponents<T>
{
	return Base::elemComponentType(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components
 * associated to the Edge Element having the same type of the given
 * template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom
 * components of type `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble =
 *     m.perEdgeCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components
 * of a given type.
 */
template<EdgeConcept T>
template<typename K>
std::vector<std::string>
EdgeContainer<T>::perEdgeCustomComponentNamesOfType() const
	requires edge::HasCustomComponents<T>
{
	return Base::template elemCustomComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the Edge, having the
 * given name.
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
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
 * @brief Deletes the custom component of the given name from the
 * Edge Element.
 *
 * The function does nothing if the custom component does not exists.
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @param[in] name: the name of the custom component that will be removed from
 * the Edge.
 */
template<EdgeConcept T>
void EdgeContainer<T>::deletePerEdgeCustomComponent(
	const std::string& name) requires edge::HasCustomComponents<T>
{
	Base::deleteElemCustomComponent(name);
}

/**
 * @brief Returns a vector handle to the given custom component. The handle can
 * be used like a normal std::vector, but does not have access to the modifiers
 * member functions (resize, push_back...). The handle contains **references**
 * to the custom component, therefore you can modify the custom component by
 * modifying the element of the handle vector normally. Since the handle stores
 * references, there are no copies performed when calling this function.
 *
 * For example, assuming that the mesh has a Edge custom component named
 * `cc` of type int:
 *
 * @code{.cpp}
 * auto handle = m.getPerEdgeCustomComponentVectorHandle<int>("cc");
 * for (Edge& e : m.edges() {
 *    handle[m.index(e)] = 5; // e.customComponent<int>("cc") == 5
 *    assert(e.customComponent<int>("cc") == 5);
 * }
 * @endcode
 *
 * Using handles allows to access more efficiently to custom components rather
 * accessing from an element object. However, note that references are binded to
 * the container of the mesh.
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @note Since the handle contains references, any operation that changes the
 * size of the container could be destructive and invalidate the references
 * contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 *
 * @param[in] name: name of the custom component on which return the handle.
 * @throws std::out_of_range if no custom component of the given name was found.
 *
 * @return a vector handle that allows to access to the custom component.
 */
template<EdgeConcept T>
template<typename K>
CustomComponentVectorHandle<K>
EdgeContainer<T>::perEdgeCustomComponentVectorHandle(
	const std::string& name) requires edge::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

/**
 * @brief Returns a const vector handle to the given custom component. The
 * handle can be used like a normal std::vector, but does not have access to the
 * modifiers member functions (resize, push_back...). The handle contains
 * **const references** to the custom component, therefore you can access to the
 * custom component by accessing the element of the handle vector normally.
 * Since the handle stores references, there are no copies performed when
 * calling this function.
 *
 * For example, assuming that the mesh has a Edge custom component named
 * `cc` of type int:
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
 * Using handles allows to access more efficiently to custom components rather
 * accessing from an element object. However, note that references are binded to
 * the container of the mesh.
 *
 * @note This function is available only if the Edge Element has the
 * CustomComponents Component.
 *
 * @note Since the handle contains references, any operation that changes the
 * size of the container could be destructive and invalidate the references
 * contained in the handle.
 *
 * @tparam K: the type of the custom component on which return the handle.
 * @param[in] name: name of the custom component on which return the handle.
 *
 * @return a const vector handle that allows to access to the custom component.
 */
template<EdgeConcept T>
template<typename K>
ConstCustomComponentVectorHandle<K>
EdgeContainer<T>::perEdgeCustomComponentVectorHandle(
	const std::string& name) const requires edge::HasCustomComponents<T>
{
	return Base::template customComponentVectorHandle<K>(name);
}

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINER_EDGE_CONTAINER_H
