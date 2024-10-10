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
#include <vclib/types/view.h>

#include "bases/reference_container_component.h"

namespace vcl::comp {

/**
 * @brief The AdjacentEdges class is a container of Edge indices or pointers. It
 * could be used by any Element to save adjacencies information (also the Edge
 * element itself).
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
 * auto* e = v.adjEdge(0);
 * uint ei = v.adjEdgeIndex(0);
 * @endcode
 *
 * @note This component could be *Tied To Vertex Number*: it means that the size
 * of the container, if dynamic, will change automatically along the Vertex
 * Number of the Component. Check the `TTVN` template value on the
 * specialization of your component to check if it is tied to the Vertex Number.
 * For further details check the documentation of the @ref ContainerComponent
 * class.
 *
 * @tparam STORE_INDICES: If true, the component will store indices, otherwise
 * pointers to Edge.
 * @tparam Edge: The type of the adjacent Edge element.
 * @tparam N: The size of the container, that will represent the number of
 * storable adjacent edges. If negative, the container is dynamic.
 * @tparam TTVN: If true, the size of the container will be tied to the Vertex
 * Number of the component (this is used mostly on Face elements).
 * @tparam ParentElemType: This type is used to get access to the Element that
 * has the component (and, in case, to the Mesh that has the Element). If the
 * component doesn't need to access the Element, this type can be void. Note:
 * if the component is vertical (or optional), this type cannot be void.
 * @tparam VERT: If true, the component will be stored vertically. This argument
 * is considered only if the ElementType is not void.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<
    bool STORE_INDICES,
    typename Edge,
    int  N,
    bool TTVN,
    typename ParentElemType = void,
    bool VERT               = false,
    bool OPT                = false>
class AdjacentEdges :
        public ReferenceContainerComponent<
            STORE_INDICES,
            AdjacentEdges<
                STORE_INDICES,
                Edge,
                N,
                TTVN,
                ParentElemType,
                VERT,
                OPT>,
            CompId::ADJACENT_EDGES,
            Edge,
            N,
            ParentElemType,
            VERT,
            OPT,
            TTVN>
{
    using Base = ReferenceContainerComponent<
        STORE_INDICES,
        AdjacentEdges<STORE_INDICES, Edge, N, TTVN, ParentElemType, VERT, OPT>,
        CompId::ADJACENT_EDGES,
        Edge,
        N,
        ParentElemType,
        VERT,
        OPT,
        TTVN>;

public:
    /**
     * @brief Expose the type of the Adjacent Edge.
     */
    using AdjacentEdgeType = Edge;

    /* Iterator Types declaration */

    using AdjacentEdgeIterator           = Base::Iterator;
    using ConstAdjacentEdgeIterator      = Base::ConstIterator;
    using ConstAdjacentEdgeIndexIterator = Base::ConstIndexIterator;

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
    uint adjEdgesNumber() const { return Base::size(); }

    /**
     * @brief Returns the pointer to the i-th adjacent edge of the element.
     *
     * @param[in] i: the position of the required adjacent edge in this
     * container; the value must be between 0 and the number of adj edges.
     * @return The pointer to the i-th adjacent edge of the element.
     */
    Edge* adjEdge(uint i) { return Base::element(i); }

    /**
     * @brief Returns a const pointer to the i-th adjacent edge of the element.
     * @param[in] i: the position of the required adjacent edge in this
     * container; the value must be between 0 and the number of adj edges.
     * @return The pointer to the i-th adjacent edge of the element.
     */
    const Edge* adjEdge(uint i) const { return Base::element(i); }

    /**
     * @brief Returns the index in the edge container of the i-th adjacent edge
     * of the element.
     * @param[in] i: the position of the required edge in this container.
     * @return The index of the i-th adjacent edge of the element.
     */
    uint adjEdgeIndex(uint i) const { return Base::elementIndex(i); }

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
    Edge* adjEdgeMod(int i) { return Base::elementMod(i); }

    /**
     * @brief Same of adjEdgeMod, but returns a const Pointer to the adjacent
     * edge.
     * @param[in] i: the position of the required adjacent edge in this
     * container, w.r.t. the position 0; value is modularized on
     * adjEdgeNumber().
     * @return The pointer to the required adjacent edge of this element.
     */
    const Edge* adjEdgeMod(int i) const { return Base::elementMod(i); }

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
    uint adjEdgeIndexMod(int i) const { return Base::elementIndexMod(i); }

    /**
     * @brief Sets the i-th adjacent edge of the element.
     * @param[in] i: the position in this container on which set the adj edge;
     * the value must be between 0 and the number of adj edges.
     * @param[in] e: The pointer to the adjacent edge to set to the element.
     */
    void setAdjEdge(uint i, Edge* e) { Base::setElement(i, e); }

    /**
     * @brief Sets the i-th adjacent edge of the element.
     *
     * @param[in] i: the position in this container on which set the adj edge;
     * the value must be between 0 and the number of adj edges.
     * @param[in] ei: The index in the edge container of the edge to set.
     */
    void setAdjEdge(uint i, uint ei) { Base::setElement(i, ei); }

    /**
     * @brief Sets the adjacent edge pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * edge; the value must be between begin() and end().
     * @param[in] e: The pointer to the adjacent edge to set to the element.
     */
    void setAdjEdge(ConstAdjacentEdgeIterator it, Edge* e)
    {
        Base::setElement(it, e);
    }

    /**
     * @brief Sets the adjacent edge pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * edge; the value must be between begin() and end().
     * @param[in] ei: The index in the edge container of the edge to set.
     */
    void setAdjEdge(ConstAdjacentEdgeIterator it, uint ei)
    {
        Base::setElement(it, ei);
    }

    /**
     * @brief Sets the adjacent edge pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * edge; the value must be between begin() and end().
     * @param[in] e: The pointer to the adjacent edge to set to the element.
     */
    void setAdjEdge(ConstAdjacentEdgeIndexIterator it, Edge* e)
    {
        Base::setElement(it, e);
    }

    /**
     * @brief Sets the adjacent edge pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * edge; the value must be between begin() and end().
     * @param[in] ei: The index in the edge container of the edge to set.
     */
    void setAdjEdge(ConstAdjacentEdgeIndexIterator it, uint ei)
    {
        Base::setElement(it, ei);
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
    void setAdjEdgeMod(int i, Edge* e) { Base::setElementMod(i, e); }

    /**
     * @brief Sets the i-th adjacent edge of the element, but using as index the
     * module between i and the number of adjacent edges. You can use this
     * function if you need to set the "next adjacent edge after position k",
     * without check if it is less than the number of adjacent edges. Works also
     * for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adj edge
     * e.setAdjEdgeMod(k+1, aEdgeInd); // set the adj edge next to k, that may
     *                                 // also be at pos 0
     * e.setAdjEdgeMod(-1, aEdgeInd); // set the adj edge in position
     *                                // adjEdgesNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the adj edge; value is modularized on adjEdgesNumber().
     * @param[in] ei: The index in the edge container of the edge to set.
     */
    void setAdjEdgeMod(int i, uint ei) { Base::setElementMod(i, ei); }

    /**
     * @brief Sets all the adjacent edges of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of adjacent edges to set. The value
     * type of the range must be convertible to a pointer to an AdjacentEdge.
     *
     * @param[in] r: range of edge pointers to set.
     */
    template<Range Rng>
    void setAdjEdges(Rng&& r) requires RangeOfConvertibleTo<Rng, Edge*>
    {
        Base::setElements(r);
    }

    /**
     * @brief Sets all the adjacent edges of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of adjacent edges to set. The value
     * type of the range must be convertible to an unsigned integer.
     *
     * @param[in] r: range of edges indices to set.
     */
    template<Range Rng>
    void setAdjEdges(Rng&& r) requires RangeOfConvertibleTo<Rng, uint>
    {
        Base::setElements(r);
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
        return Base::containsElement(e);
    }

    /**
     * @brief Returns `true` if the container of adjacent edges contains the
     * given index, `false` otherwise.
     *
     * @param[in] ei: the index to the edge to search.
     * @return `true` if the container of adjacent edges contains the given
     * index, `false` otherwise.
     */
    bool containsAdjEdge(uint ei) const { return Base::containsElement(ei); }

    /**
     * @brief Returns the index of the given adjacent edge in the container of
     * the element. If the given adjacent edge is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] e: the pointer to the adjacent edge to search.
     * @return the index of the given adjacent edge, or UINT_NULL if it is not
     * found.
     */
    uint indexOfAdjEdge(const Edge* e) const { return Base::indexOfElement(e); }

    /**
     * @brief Returns the index of the adjacent edge with the given index in the
     * container of the element. If the adjacent edge with the given index is
     * not in the container, returns UINT_NULL.
     *
     * @param[in] ei: the index to the adjacent edge to search.
     * @return the index of the adjacent edge with the given index, or UINT_NULL
     * if it is not found.
     */
    uint indexOfAdjEdge(uint ei) const { return Base::indexOfElement(ei); }

    /* Member functions specific for dynamic vector of adjacent edges */

    /**
     * @brief Resize the container of the adjacent edges to the given size.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] n: The new size of the adjacent edges container.
     */
    void resizeAdjEdges(uint n) requires (N < 0 && !TTVN) { Base::resize(n); }

    /**
     * @brief Pushes in the back of the container the given adjacent edge.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] e: The pointer to the adjacent edge to push in the back of the
     * container.
     */
    void pushAdjEdge(Edge* e) requires (N < 0 && !TTVN) { Base::pushBack(e); }

    /**
     * @brief Pushes in the back of the container the given adjacent edge.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] ei: The index to the adjacent edge to push in the back of the
     * container.
     */
    void pushAdjEdge(uint ei) requires (N < 0 && !TTVN) { Base::pushBack(ei); }

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
        Base::insert(i, e);
    }

    /**
     * @brief Inserts the adjacent edge with the given index in the container at
     * the given position.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] i: The position in this container where to insert the adjacent
     * edge.
     * @param[in] ei: The index to the adjacent edge to insert in the container.
     */
    void insertAdjEdge(uint i, uint ei) requires (N < 0 && !TTVN)
    {
        Base::insert(i, ei);
    }

    /**
     * @brief Removes the adjacent edge at the given position from the
     * container.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     * @param[in] i: The position of the adjacent edge to remove from this
     * container.
     */
    void eraseAdjEdge(uint i) requires (N < 0 && !TTVN) { Base::erase(i); }

    /**
     * @brief Clears the container of adjacent edges, making it empty.
     * @note This function is available only if the container of the Adjacent
     * Edges component has dynamic size.
     */
    void clearAdjEdges() requires (N < 0 && !TTVN) { Base::clear(); }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first adjacent edge in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    AdjacentEdgeIterator adjEdgeBegin() { return Base::elementBegin(); }

    /**
     * @brief Returns an iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    AdjacentEdgeIterator adjEdgeEnd() { return Base::elementEnd(); }

    /**
     * @brief Returns a const iterator to the first adjacent edge in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstAdjacentEdgeIterator adjEdgeBegin() const
    {
        return Base::elementBegin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstAdjacentEdgeIterator adjEdgeEnd() const { return Base::elementEnd(); }

    /**
     * @brief Returns an iterator to the first adjacent edge index in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of the adjacent edge indices.
     */
    ConstAdjacentEdgeIndexIterator adjEdgeIndexBegin() const
    {
        return Base::elementIndexBegin();
    }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of the adjacent edge indices.
     */
    ConstAdjacentEdgeIndexIterator adjEdgeIndexEnd() const
    {
        return Base::elementIndexEnd();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and
     * end iterators of the container of adjacent edges of the element. The view
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
    View<AdjacentEdgeIterator> adjEdges() { return Base::elements(); }

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
        return Base::elements();
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
        return Base::elementIndices();
    }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool importRefs = true)
    {
        if (importRefs) {
            if constexpr (HasAdjacentEdges<Element>) {
                if (isAdjacentEdgesAvailableOn(e)) {
                    if constexpr (N > 0) {
                        // same static size
                        if constexpr (N == Element::ADJ_EDGE_NUMBER) {
                            importIndicesFrom(e);
                        }
                        // from dynamic to static, but dynamic size == static
                        // size
                        else if constexpr (Element::ADJ_EDGE_NUMBER < 0) {
                            if (e.adjEdgesNumber() == N) {
                                importIndicesFrom(e);
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
                        Base::resize(e.adjEdgesNumber());
                        importIndicesFrom(e);
                    }
                }
            }
        }
    }

    void serialize(std::ostream& os) const
    {
        // regardless of the type of the container (indices or pointers), the
        // serialization is always done using the indices
        if constexpr (N < 0) {
            vcl::serialize(os, adjEdgesNumber());
        }
        for (uint i = 0; i < adjEdgesNumber(); ++i) {
            vcl::serialize(os, adjEdgeIndex(i));
        }
    }

    void deserialize(std::istream& is)
    {
        if constexpr (N < 0) {
            uint n;
            vcl::deserialize(is, n);
            Base::resize(n);
        }
        for (uint i = 0; i < adjEdgesNumber(); ++i) {
            uint aei;
            vcl::deserialize(is, aei);
            setAdjEdge(i, aei);
        }
    }

private:
    template<typename Element>
    void importIndicesFrom(const Element& e)
    {
        for (uint i = 0; i < e.adjEdgesNumber(); ++i) {
            setAdjEdge(i, e.adjEdgeIndex(i));
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
