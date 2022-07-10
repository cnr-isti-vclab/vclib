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

#include "half_edge_container.h"

namespace vcl::mesh {

/**
 * @brief Empty constructor that creates an empty container of HalfEdges.
 */
template<HalfEdgeConcept T>
HalfEdgeContainer<T>::HalfEdgeContainer()
{
}

/**
 * @brief Returns a const reference of the halfedge at the i-th position in the halfEdge Container
 * of the Mesh, which will be the halfedge having index = i.
 *
 * This function does not perform any sanity check: if i is less than halfedgeContainerSize(), this
 * function will return a valid halfEdge reference (note that the halfEdge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the halfedge that will be returned.
 */
template<HalfEdgeConcept T>
const typename HalfEdgeContainer<T>::HalfEdgeType& HalfEdgeContainer<T>::halfEdge(uint i) const
{
	return Base::element(i);
}

/**
 * @brief Returns a reference of the halfedge at the i-th position in the halfEdge Container of the
 * Mesh, which will be the halfedge having index = i.
 *
 * This function does not perform any sanity check: if i is less than halfedgeContainerSize(), this
 * function will return a valid halfEdge reference (note that the halfEdge may have been flagged as
 * deleted).
 *
 * @param[in] i: the index of the halfedge that will be returned.
 */
template<HalfEdgeConcept T>
typename HalfEdgeContainer<T>::HalfEdgeType& HalfEdgeContainer<T>::halfEdge(uint i)
{
	return Base::element(i);
}

/**
 * @brief Returns the number of **non-deleted** halfedges contained in the halfEdge container of the
 * Mesh.
 *
 * If halfEdgeNumber() != halfEdgeContainerSize(), it means that there are some halfedges that are
 * flagged as deleted.
 *
 * @return the number of non-deleted halfedges of the Mesh.
 */
template<HalfEdgeConcept T>
uint HalfEdgeContainer<T>::halfEdgeNumber() const
{
	return Base::elementNumber();
}

/**
 * @brief Returns the number of halfedges (also deleted) contained in the halfEdge container of the
 * Mesh.
 *
 * If halfEdgeNumber() != halfEdgeContainerSize(), it means that there are some halfedges that are
 * flagged as deleted.
 *
 * @return the number of all the halfedges contained in the Mesh.
 */
template<HalfEdgeConcept T>
uint HalfEdgeContainer<T>::halfEdgeContainerSize() const
{
	return Base::elementContainerSize();
}

/**
 * @brief Returns the number of deleted halfedges in the halfEdge container, that is
 * halfEdgeContainerSize() - halfEdgeNumber().
 *
 * @return The number of deleted halfedges in the container.
 */
template<HalfEdgeConcept T>
uint HalfEdgeContainer<T>::deletedHalfEdgeNumber() const
{
	return Base::deletedElementNumber();
}

/**
 * @brief Marks as deleted the halfedge with the given id.
 *
 * This member function does not perform any reallocation of the halfedges: the deleted halfedges
 * will stay in the halfEdge Container, but will be marked as deleted.
 *
 * Deleted halfedges are automatically jumped by the iterators provided by the halfEdge Container.
 *
 * @param[in] i: the id of the halfedge that will be marked as deleted.
 */
template<HalfEdgeConcept T>
void HalfEdgeContainer<T>::deleteHalfEdge(uint i)
{
	Base::deleteElement(i);
}

/**
 * @brief Marks as deleted the given halfedge, before asserting that the halfedge belongs to this
 * container.
 *
 * This member function does not perform any reallocation of the halfedges: the deleted halfedges
 * will stay in the halfEdge Container, but will be marked as deleted.
 *
 * Deleted halfedges are automatically jumped by the iterators provided by the halfEdge Container.
 *
 * @param[in] e: the pointer of the halfedge that will be marked as deleted.
 */
template<HalfEdgeConcept T>
void HalfEdgeContainer<T>::deleteHalfEdge(const HalfEdgeType* e)
{
	Base::deleteElement(e);
}

/**
 * @brief This is an utility member function that returns the index of an element if the container
 * would be compact, that is the number of non-deleted elements before the halfedge with the given
 * index.
 *
 * Complexity: O(n), with n the number of halfedges in the container.
 *
 * This function does not perform any sanity check on the given index.
 *
 * @param[in] i: the index of a halfedge of the container.
 * @return The index that the halfedge with index i would have if this container would be compact.
 */
template<HalfEdgeConcept T>
uint HalfEdgeContainer<T>::halfEdgeIndexIfCompact(uint i) const
{
	return Base::elementIndexIfCompact(i);
}

/**
 * @brief Returns a vector that tells, for each actual halfedge index, the new index that the
 * halfedge would have in a compacted container. For each deleted halfedge index, the value of the
 * vector will be -1.
 *
 * This is useful if you need to know the indices of the halfedges that they would have in a
 * compact container, without considering the deleted ones.
 *
 * @return A vector containing, for each halfedge index, its index if the container would be
 * compact.
 */
template<HalfEdgeConcept T>
std::vector<int> HalfEdgeContainer<T>::halfEdgeCompactIndices() const
{
	return Base::elementCompactIndices();
}

/**
 * @brief Returns an iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted halfedges of the container. You can
 * change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * halfedges.
 * @return An iterator the the first halfedge of the container.
 */
template<HalfEdgeConcept T>
typename HalfEdgeContainer<T>::HalfEdgeIterator
HalfEdgeContainer<T>::halfEdgeBegin(bool jumpDeleted)
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns an iterator to the end of the container.
 * @return An iterator to the end of the container.
 */
template<HalfEdgeConcept T>
typename HalfEdgeContainer<T>::HalfEdgeIterator HalfEdgeContainer<T>::halfEdgeEnd()
{
	return Base::elementEnd();
}

/**
 * @brief Returns a const iterator to the beginning of the container.
 *
 * The iterator is automatically initialized to jump deleted halfedges of the container. You can
 * change this option by calling this function with jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * halfedges.
 * @return A const iterator the the first halfedge of the container.
 */
template<HalfEdgeConcept T>
typename HalfEdgeContainer<T>::ConstHalfEdgeIterator
HalfEdgeContainer<T>::halfEdgeBegin(bool jumpDeleted) const
{
	return Base::elementBegin(jumpDeleted);
}

/**
 * @brief Returns a const iterator to the end of the container.
 * @return A const iterator to the end of the container.
 */
template<HalfEdgeConcept T>
typename HalfEdgeContainer<T>::ConstHalfEdgeIterator HalfEdgeContainer<T>::halfEdgeEnd() const
{
	return Base::elementEnd();
}

/**
 * @brief Returns a small utility object that allows to iterate over the halfedges of the
 * containers, providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the halfedges using the C++
 * foreach syntax:
 *
 * @code{.cpp}
 * for (HalfEdge& e : m.halfEdges()){
 *     // do something with this halfedge
 * }
 * @endcode
 *
 * The iterator used to iterate over halfedges is automatically initialized to jump deleted
 * halfedges of the container. You can change this option by calling this function with
 * jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * halfedges.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<HalfEdgeConcept T>
typename HalfEdgeContainer<T>::HalfEdgeRangeIterator
HalfEdgeContainer<T>::halfEdges(bool jumpDeleted)
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Returns a small utility object that allows to iterate over the halfedges of the
 * containers, providing two member functions begin() and end().
 *
 * This member function is very useful when you want to iterate over the halfedges using the C++
 * foreach syntax:
 *
 * @code{.cpp}
 * for (const HalfEdge& e : m.halfEdges()){
 *     // do something with this halfedge
 * }
 * @endcode
 *
 * The iterator used to iterate over halfedges is automatically initialized to jump deleted
 * halfedges of the container. You can change this option by calling this function with
 * jumpDeleted=false.
 *
 * @param[in] jumpDeleted (def: true): boolean that tells if the iterator should jump deleted
 * halfedges.
 * @return An object having begin() and end() function, allowing to iterate over the container.
 */
template<HalfEdgeConcept T>
typename HalfEdgeContainer<T>::ConstHalfEdgeRangeIterator
HalfEdgeContainer<T>::halfEdges(bool jumpDeleted) const
{
	return Base::elements(jumpDeleted);
}

/**
 * @brief Checks if the halfedge Optional Color is enabled.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalColor component.
 *
 * @return true if the Optional Color is enabled, false otherwise.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::HasOptionalColor<U>, bool)
HalfEdgeContainer<T>::isPerHalfEdgeColorEnabled() const
{
	return Base::optionalVec.isColorEnabled();
}

/**
 * @brief Enables the Optional Color of the halfedge.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalColor Component.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::HasOptionalColor<U>, void)
HalfEdgeContainer<T>::enablePerHalfEdgeColor()
{
	Base::optionalVec.enableColor(halfEdgeContainerSize());
}

/**
 * @brief Disables the Optional Color of the halfedge.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalColor Component.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::HasOptionalColor<U>, void)
HalfEdgeContainer<T>::disablePerHalfEdgeColor()
{
	Base::optionalVec.disableColor();
}

/**
 * @brief Checks if the halfedge Optional Mark is enabled.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalMark component.
 *
 * @return true if the Optional Mark is enabled, false otherwise.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasOptionalMark<U>(), bool)
HalfEdgeContainer<T>::isPerHalfEdgeMarkEnabled() const
{
	return Base::optionalVec.isMarkEnabled();
}

/**
 * @brief Enables the Optional Mark of the halfedge.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalMark Component.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasOptionalMark<U>(), void)
HalfEdgeContainer<T>::enablePerHalfEdgeMark()
{
	Base::optionalVec.enableMark(halfEdgeContainerSize());
}

/**
 * @brief Disables the Optional Mark of the halfedge.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalMark Component.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasOptionalMark<U>(), void)
HalfEdgeContainer<T>::disablePerHalfEdgeMark()
{
	Base::optionalVec.disableMark();
}

/**
 * @brief Checks if the halfedge Optional Scalar is enabled.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalScalar Component.
 *
 * @return true if the Optional Scalar is enabled, false otherwise.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasOptionalScalar<U>(), bool)
HalfEdgeContainer<T>::isPerHalfEdgeScalarEnabled() const
{
	return Base::optionalVec.isScalarEnabled();
}
/**
 * @brief Enables the Optional Scalar of the halfedge.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalScalar Component.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasOptionalScalar<U>(), void)
HalfEdgeContainer<T>::enablePerHalfEdgeScalar()
{
	Base::optionalVec.enableScalar(halfEdgeContainerSize());
}

/**
 * @brief Disables the Optional Scalar of the halfedge.
 *
 * @note This function is available only if the HalfEdge Element has the OptionalScalar Component.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasOptionalScalar<U>(), void)
HalfEdgeContainer<T>::disablePerHalfEdgeScalar()
{
	Base::optionalVec.disableScalar();
}

/**
 * @brief Checks if halfedges have a custom component with the given name.
 *
 * This function does not take into account the type of the custom component.
 *
 * @note This function is available only if the HalfEdge Element has the CustomComponents Component.
 *
 * @return `true` if the HalfEdge Element has a custom component with the given name.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), bool)
HalfEdgeContainer<T>::hasPerHalfEdgeCustomComponent(const std::string& name) const
{
	return Base::optionalVec.componentExists(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components of any type associated
 * to the HalfEdge Element.
 *
 * @note This function is available only if the HalfEdge Element has the CustomComponents Component.
 *
 * @return A vector of strings representing all the names of the custom components.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), std::vector<std::string>)
HalfEdgeContainer<T>::getAllPerHalfEdgeCustomComponentNames() const
{
	return Base::optionalVec.allComponentNames();
}

/**
 * @brief Checks if the custom component of the HalfEdge Element having the given name has the same
 * type of the given template argument type of this function.
 *
 * For example, the following code checks if the component called `cc` is of type `double`:
 * @code{.cpp}
 * if (m.isPerHalfEdgeCustomComponentOfType<double>("cc")) {
 *   ...
 * }
 * @endcode
 *
 * @note This function is available only if the HalfEdge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component to check.
 * @param[in] name: the name of the custom component to check.
 * @return `true` if the custom component is of the same type of the template argument.
 */
template<HalfEdgeConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), bool)
HalfEdgeContainer<T>::isPerHalfEdgeCustomComponentOfType(const std::string& name) const
{
	return Base::optionalVec.template isComponentOfType<K>(name);
}

/**
 * @brief Returns a vector containing all the names of the custom components associated to the
 * HalfEdge Element having the same type of the given template argument type of this function.
 *
 * For example, the following code gets a vector containing all the custom components of type
 * `double`:
 * @code{.cpp}
 * std::vector<std::string> cdouble = m.getPerHalfEdgeCustomComponentNamesOfType<double>();
 * @endcode
 *
 * @note This function is available only if the HalfEdge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component names.
 * @return A vector of strings representing the names of the custom components of a given type.
 */
template<HalfEdgeConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), std::vector<std::string>)
HalfEdgeContainer<T>::getPerHalfEdgeCustomComponentNamesOfType() const
{
	return Base::optionalVec.template allComponentNamesOfType<K>();
}

/**
 * @brief Adds a custom component of type K to the HalfEdge, having the given name.
 *
 * @note This function is available only if the HalfEdge Element has the CustomComponents Component.
 *
 * @tparam K: the type of the custom component added to the HalfEdge.
 * @param[in] name: the name of the custom component added to the HalfEdge.
 */
template<HalfEdgeConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), void)
HalfEdgeContainer<T>::addPerHalfEdgeCustomComponent(const std::string& name)
{
	Base::optionalVec.template addNewComponent<K>(name, halfEdgeContainerSize());
}

/**
 * @brief Deletes the custom component of the given name from the HalfEdge Element.
 *
 * The function does nothing if the custom component does not exists.
 *
 * @note This function is available only if the HalfEdge Element has the CustomComponents Component.
 *
 * @param[in] name: the name of the custom component that will be removed from the HalfEdge.
 */
template<HalfEdgeConcept T>
template<typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), void)
HalfEdgeContainer<T>::deletePerHalfEdgeCustomComponent(const std::string& name)
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
 * auto handle = m.getPerHalfEdgeCustomComponentVectorHandle<int>("cc");
 * for (HalfEdge& e : m.halfEdges() {
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
template<HalfEdgeConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), CustomComponentVectorHandle<K>)
HalfEdgeContainer<T>::getPerHalfEdgeCustomComponentVectorHandle(const std::string& name)
{
	std::vector<std::any>&         cc = Base::optionalVec.template componentVector<K>(name);
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
 * auto handle = m.getPerHalfEdgeCustomComponentVectorHandle<const int>("cc");
 * int sum = 0;
 * for (const HalfEdge& e : m.halfEdges() {
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
template<HalfEdgeConcept T>
template<typename K, typename U>
VCL_ENABLE_IF(hedge::hasCustomComponents<U>(), ConstCustomComponentVectorHandle<K>)
HalfEdgeContainer<T>::getPerHalfEdgeCustomComponentVectorHandle(const std::string& name) const
{
	const std::vector<std::any>&        cc = Base::optionalVec.template componentVector<K>(name);
	ConstCustomComponentVectorHandle<K> v(cc);
	return cc;
}

template<HalfEdgeConcept T>
uint HalfEdgeContainer<T>::index(const HalfEdgeType *e) const
{
	return Base::index(e);
}

template<HalfEdgeConcept T>
void HalfEdgeContainer<T>::clearHalfEdges()
{
	Base::clearElements();
}

template<HalfEdgeConcept T>
uint HalfEdgeContainer<T>::addHalfEdge()
{
	return Base::addElement();
}

template<HalfEdgeConcept T>
uint HalfEdgeContainer<T>::addHalfEdges(uint nEdges)
{
	return Base::addElements(nEdges);
}

template<HalfEdgeConcept T>
void HalfEdgeContainer<T>::reserveHalfEdges(uint size)
{
	Base::reserveElements(size);
}

template<HalfEdgeConcept T>
std::vector<int> HalfEdgeContainer<T>::compactHalfEdges()
{
	return Base::compactElements();
}

template<HalfEdgeConcept T>
template<typename Mesh>
void HalfEdgeContainer<T>::enableOptionalComponentsOf(const Mesh &m)
{
	// if half edges are enabled in the other Mesh
	if constexpr (hasHalfEdges<Mesh>()) {
		using MHalfEdgeContainer = typename Mesh::HalfEdgeContainer::Base;

		Base::enableOptionalComponentsOf((const MHalfEdgeContainer&)m);
	}
}

template<HalfEdgeConcept T>
template<typename Mesh>
void HalfEdgeContainer<T>::importFrom(const Mesh &m)
{
	// if half edges are enabled in the other Mesh
	if constexpr (hasHalfEdges<Mesh>()) {
		using MHalfEdgeContainer = typename Mesh::HalfEdgeContainer::Base;

		Base::importFrom((const MHalfEdgeContainer&)m);
	}
}

} // namespace vcl::mesh
