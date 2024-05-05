/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_ADJACENT_EDGES_H

#include <vclib/concepts/mesh/components/adjacent_edges.h>
#include <vclib/iterators/mesh/components/index_from_pointer_iterator.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The AdjacentEdges class is a container of Edge pointers. It could be
 * used by any Element to save adjacencies information (also the Edge element
 * itself).
 *
 * It is a random access container having static or dynamic size, depending on
 * the value of the template argument N (a negative number means dynamic).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentEdges
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * v.adjEdgesNumber();
 * @endcode
 *
 * @note This component could be *Tied To Vertex Number*: it means that the size
 * of the container, if dynamic, will change automatically along the Vertex
 * Number of the Component. Check the `TTVN` template value on the
 * specialization of your component to check if it is tied to the Vertex Number.
 * For further details check the documentation of the @ref ContainerComponent
 * class.
 *
 * @tparam Edge: The type of the adjacent Edge element.
 * @tparam N: The size of the container, that will represent the number of
 * storable adjacent edges. If negative, the container is dynamic.
 * @tparam TTVN: If true, the size of the container will be tied to the Vertex
 * Number of the component (this is used mostly on Face elements).
 * @tparam ParentElemType: This template argument must be `void` if the
 * component needs to be stored horizontally, or the type of the parent element
 * that will contain this component if the component needs to be stored
 * vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<
    typename Edge,
    int  N,
    bool TTVN,
    typename ParentElemType = void,
    bool OPT                = false>
class AdjacentEdges :
        public PointersContainerComponent<
            AdjacentEdges<Edge, N, TTVN, ParentElemType, OPT>,
            CompId::ADJACENT_EDGES,
            Edge,
            N,
            ParentElemType,
            OPT,
            TTVN>
{
    using Base = PointersContainerComponent<
        AdjacentEdges<Edge, N, TTVN, ParentElemType, OPT>,
        CompId::ADJACENT_EDGES,
        Edge,
        N,
        ParentElemType,
        OPT,
        TTVN>;

public:
    /**
     * @brief Expose the type of the Adjacent Edge.
     */
    using AdjacentEdgeType = Edge;

    /* Iterator Types declaration */

    using AdjacentEdgeIterator      = Base::Iterator;
    using ConstAdjacentEdgeIterator = Base::ConstIterator;
    using ConstAdjacentEdgeIndexIterator =
        IndexFromPointerIterator<ConstAdjacentEdgeIterator>;

    /**
     * @brief Static size of the container. If the container is dynamic, this
     * value will be negative and you should use the adjEdgesNumber() member
     * function.
     */
    static const int ADJ_EDGE_NUMBER = Base::SIZE;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the Adjacent Edges container size is static, initializes all the
     * Adjacent Edges to `nullptr`, otherwise the container will be empty.
     */
    AdjacentEdges() = default;

    /* Member functions */

    /**
     * @brief Returns the number of adjacent edges of this element.
     * @return The number of adjacent edges of this element.
     */
    uint adjEdgesNumber() const { return Base::container().size(); }

    /**
     * @brief Returns the pointer to the i-th adjacent edge of the element.
     *
     * @param[in] i: the position of the required adjacent edge in this
     * container; the value must be between 0 and the number of adj edges.
     * @return The pointer to the i-th adjacent edge of the element.
     */
    Edge* adjEdge(uint i) { return Base::container().at(i); }

    /**
     * @brief Returns a const pointer to the i-th adjacent edge of the element.
     * @param[in] i: the position of the required adjacent edge in this
     * container; the value must be between 0 and the number of adj edges.
     * @return The pointer to the i-th adjacent edge of the element.
     */
    const Edge* adjEdge(uint i) const { return Base::container().at(i); }

    /**
     * @brief Returns the index in the edge container of the i-th adjacent edge
     * of the element.
     * @param[in] i: the position of the required edge in this container.
     * @return The index of the i-th adjacent edge of the element.
     */
    uint adjEdgeIndex(uint i) const
    {
        auto* e = adjEdge(i);
        if (e) [[likely]]
            return e->index();
        else
            return UINT_NULL;
    }

    /**
     * @brief Returns the pointer to the i-th adjacent edge of the element but
     * using as index the module between i and the number of adjacent edges. You
     * can use this function if you need to get the "next adjacent edge after
     * position k", without check if it is less than the number of adj edges.
     * Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adjacent edge
     * auto* next = e.adjEdgeMod(k+1); // the adj edge next to k, that may also
     *                                 // be at pos 0
     * auto* last = e.adjEdgeMod(-1); // the adj edge in position
     *                                // adjEdgeNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required adjacent edge in this
     * container, w.r.t. the position 0; value is modularized on
     * adjEdgeNumber().
     * @return The pointer to the required adjacent edge of the element.
     */
    Edge* adjEdgeMod(int i) { return Base::container().atMod(i); }

    /**
     * @brief Same of adjEdgeMod, but returns a const Pointer to the adjacent
     * edge.
     * @param[in] i: the position of the required adjacent edge in this
     * container, w.r.t. the position 0; value is modularized on
     * adjEdgeNumber().
     * @return The pointer to the required adjacent edge of this element.
     */
    const Edge* adjEdgeMod(int i) const { return Base::container().atMod(i); }

    /**
     * @brief Returns the index in the edge container of the i-th adjacent edge
     * of the element, but using as index the module between i and the number of
     * adjacent edges. You can use this function if you need to get the "index
     * of the adjacent edge next to position k", without check if it is less
     * than the number of adjacent edges. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adjacent edge
     * auto idx = e.adjEdgeIndexMod(k+1); // the index of the adjacent edge next
     *                                    // to k, that may also be at pos 0
     * auto lastIdx = e.adjEdgeIndexMod(-1); // the index of the adjacent edge
     *                                       // in position adjEdgesNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required adjacent edge in this
     * container, w.r.t. the position 0; value is modularized on
     * adjEdgesNumber().
     * @return The index of the required adjacent edge of the element.
     */
    uint adjEdgeIndexMod(int i) const
    {
        auto* e = adjEdgeMod(i);
        if (e) [[likely]]
            return e->index();
        else
            return UINT_NULL;
    }

    /**
     * @brief Sets the i-th adjacent edge of the element.
     * @param[in] i: the position in this container on which set the adj edge;
     * the value must be between 0 and the number of adj edges.
     * @param[in] e: The pointer to the adjacent edge to set to the element.
     */
    void setAdjEdge(uint i, Edge* e) { Base::container().set(i, e); }

    /**
     * @brief Sets the adjacent edge pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * edge; the value must be between begin() and end().
     * @param[in] e: The pointer to the adjacent edge to set to the element.
     */
    void setAdjEdge(AdjacentEdgeIterator it, Edge* e)
    {
        Base::container().set(it, e);
    }

    /**
     * @brief Sets the adjacent edge pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * edge; the value must be between begin() and end().
     * @param[in] e: The pointer to the adjacent edge to set to the element.
     */
    void setAdjEdge(ConstAdjacentEdgeIterator it, Edge* e)
    {
        Base::container().set(it - adjEdgeBegin(), e);
    }

    /**
     * @brief Sets the adjacent edge pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * edge; the value must be between begin() and end().
     * @param[in] e: The pointer to the adjacent edge to set to the element.
     */
    void setAdjEdge(ConstAdjacentEdgeIndexIterator it, Edge* e)
    {
        Base::container().set(it - adjEdgeIndexBegin(), e);
    }

    /**
     * @brief Sets the i-th adjacent edge of the element, but using as index the
     * module between i and the number of adjacent edges. You can use this
     * function if you need to set the "next adjacent edge after position k",
     * without check if it is less than the number of adjacent edges. Works also
     * for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adj edge
     * e.setAdjEdgeMod(k+1, aEdge); // set the adj edge next to k, that may also
     *                               // be at pos 0
     * e.setAdjEdgeMod(-1, aEdge); // set the adj edge in position
     *                              // adjEdgesNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the adj edge; value is modularized on adjEdgesNumber().
     * @param[in] e: The pointer to the adj edge to set to the element.
     */
    void setAdjEdgeMod(int i, Edge* e) { Base::container().atMod(i) = e; }

    /**
     * @brief Sets all the adjacent edges of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of adjacent edges to set. The value
     * type of the range must be convertible to a pointer to an AdjacentEdge.
     *
     * @param[in] r: range of adjacent edges to set.
     */
    template<Range Rng>
    void setAdjEdges(Rng&& r) requires RangeOfConvertibleTo<Rng, Edge*>
    {
        Base::container().set(r);
    }

    /**
     * @brief Returns `true` if the container of adjacent edges contains the
     * given edge, `false` otherwise.
     *
     * @param[in] e: the pointer to the edge to search.
     * @return `true` if the container of adjacent edges contains the given
     * edge, `false` otherwise.
     */
    bool containsAdjEdge(const Edge* e) const
    {
        return Base::container().contains(e);
    }

    /**
     * @brief Returns the index of the given adjacent edge in the container of
     * the element. If the given adjacent edge is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] e: the pointer to the adjacent edge to search.
     * @return the index of the given adjacent edge, or UINT_NULL if it is not
     * found.
     */
    uint indexOfAdjEdge(const Edge* e) const
    {
        return Base::container().indexOf(e);
    }

    /* Member functions specific for vector of adjacent edges */

    /**
     * @brief Resize the container of the adjacent edges to the given size.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] n: The new size of the adjacent edges container.
     */
    void resizeAdjEdges(uint n) requires (N < 0 && !TTVN)
    {
        Base::container().resize(n);
    }

    /**
     * @brief Pushes in the back of the container the given adjacent edge.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] e: The pointer to the adjacent edge to push in the back of the
     * container.
     */
    void pushAdjEdge(Edge* e) requires (N < 0 && !TTVN)
    {
        Base::container().pushBack(e);
    }

    /**
     * @brief Inserts the given adjacent edge in the container at the given
     * position.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] i: The position in this container where to insert the adjacent
     * edge.
     * @param[in] e: The pointer to the adjacent edge to insert in the
     * container.
     */
    void insertAdjEdge(uint i, Edge* e) requires (N < 0 && !TTVN)
    {
        Base::container().insert(i, e);
    }

    /**
     * @brief Removes the adjacent edge at the given position from the
     * container.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] i: The position of the adjacent edge to remove from this
     * container.
     */
    void eraseAdjEdge(uint i) requires (N < 0 && !TTVN)
    {
        Base::container().erase(i);
    }

    /**
     * @brief Clears the container of adjacent edges, making it empty.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     */
    void clearAdjEdges() requires (N < 0 && !TTVN)
    {
        Base::container().clear();
    }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first adjacent edge in the container of
     * this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    AdjacentEdgeIterator adjEdgeBegin() { return Base::container().begin(); }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of this container.
     */
    AdjacentEdgeIterator adjEdgeEnd() { return Base::container().end(); }

    /**
     * @brief Returns a const iterator to the first adjacent edge in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstAdjacentEdgeIterator adjEdgeBegin() const
    {
        return Base::container().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstAdjacentEdgeIterator adjEdgeEnd() const
    {
        return Base::container().end();
    }

    /**
     * @brief Returns an iterator to the first adjacent edge index in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of the adjacent edge indices.
     */
    ConstAdjacentEdgeIndexIterator adjEdgeIndexBegin() const
    {
        return ConstAdjacentEdgeIndexIterator(adjEdgeBegin());
    }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of the adjacent edge indices.
     */
    ConstAdjacentEdgeIndexIterator adjEdgeIndexEnd() const
    {
        return ConstAdjacentEdgeIndexIterator(adjEdgeEnd());
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of adjacent edges of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto* adjEdge : el.adjEdges()) {
     *     // Do something with adjEdge
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent edges.
     */
    View<AdjacentEdgeIterator> adjEdges()
    {
        return View(adjEdgeBegin(), adjEdgeEnd());
    }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the container of adjacent edges of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (const auto* adjEdge : el.adjEdges()) {
     *     // Do something read-only with adjEdge
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent edges.
     */
    View<ConstAdjacentEdgeIterator> adjEdges() const
    {
        return View(adjEdgeBegin(), adjEdgeEnd());
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of adjacent edge indices of the element. The
     * view object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (uint eid : el.adjEdgeIndices()) {
     *     // Do something with adj edge index...
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent edge indices.
     */
    View<ConstAdjacentEdgeIndexIterator> adjEdgeIndices() const
    {
        return View(adjEdgeIndexBegin(), adjEdgeIndexEnd());
    }

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element&)
    {
    }

    // PointersComponent interface functions
    template<typename Element, typename ElEType>
    void importPointersFrom(const Element& e, Edge* base, const ElEType* ebase)
    {
        if constexpr (HasAdjacentEdges<Element>) {
            if (isAdjacentEdgesAvailableOn(e)) {
                if constexpr (N > 0) {
                    // same static size
                    if constexpr (N == Element::ADJ_EDGE_NUMBER) {
                        importPtrsFrom(e, base, ebase);
                    }
                    // from dynamic to static, but dynamic size == static size
                    else if constexpr (Element::ADJ_EDGE_NUMBER < 0) {
                        if (e.adjEdgesNumber() == N) {
                            importPtrsFrom(e, base, ebase);
                        }
                    }
                    else {
                        // do not import in this case: cannot import from
                        // dynamic size != static size
                    }
                }
                else {
                    // from static/dynamic to dynamic size: need to resize
                    // first, then import
                    resize(e.adjEdgesNumber());
                    importPtrsFrom(e, base, ebase);
                }
            }
        }
    }

    // ContainerComponent interface functions
    void resize(uint n) requires (N < 0) { Base::container().resize(n); }

    void pushBack(Edge* e = nullptr) requires (N < 0)
    {
        Base::container().pushBack(e);
    }

    void insert(uint i, Edge* e = nullptr) requires (N < 0)
    {
        Base::container().insert(i, e);
    }

    void erase(uint i) requires (N < 0) { Base::container().erase(i); }

    void clear() requires (N < 0) { Base::container().clear(); }

private:
    template<typename Element, typename ElEType>
    void importPtrsFrom(const Element& e, Edge* base, const ElEType* ebase)
    {
        if (ebase != nullptr && base != nullptr) {
            for (uint i = 0; i < e.adjEdgesNumber(); ++i) {
                if (e.adjEdge(i) != nullptr) {
                    setAdjEdge(i, base + (e.adjEdge(i) - ebase));
                }
            }
        }
    }
};

/* Detector function to check if a class has AdjacentEdges available */

/**
 * @brief Checks if the given Element has AdjacentEdges available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check. Must be of a type that satisfies
 * the ElementConcept.
 * @return `true` if the element has AdjacentEdges available, `false` otherwise.
 */
bool isAdjacentEdgesAvailableOn(const ElementConcept auto& element)
{
    return isComponentAvailableOn<CompId::ADJACENT_EDGES>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
