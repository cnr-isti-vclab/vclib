/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include "bases/reference_container_component.h"

#include <vclib/concepts.h>
#include <vclib/types.h>

namespace vcl::comp {

/**
 * @brief The AdjacentVertices class is a container of Vertex indices or
 * pointers. It is a component that makes sense to use mostly on Vertex
 * Elements. For Faces and Edges, see the VertexPointers component (which is
 * similar, but has different member function names).
 *
 * It is a random access container having dynamic size.
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Vertex Element `v` that has the AdjacentVertices
 * component, you'll be able to access to this component member functions from
 * `v`:
 *
 * @code{.cpp}
 * v.adjVerticesNumber();
 * auto* v = v.adjVertex(0);
 * uint vi = v.adjVertexIndex(0);
 * @endcode
 *
 * @note With respect to the other components that store adjacencies, this
 * component does not have two template arguments:
 * - `N`, because the container of adjacent vertices is always dynamic (there
 *   are no contexts in which it would make sense to have a fixed size container
 *   of adjacent vertices);
 * - `TTVN`, because this argument is used on components that could be part of
 *   face elements, and this component does not apply to faces.
 *
 * @tparam STORE_INDICES: If true, the component will store indices, otherwise
 * pointers to Face.
 * @tparam Vertex: The type of the adjacent Vertex element.
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
    typename Vertex,
    typename ParentElemType = void,
    bool VERT               = false,
    bool OPT                = false>
class AdjacentVertices :
        public ReferenceContainerComponent<
            STORE_INDICES,
            AdjacentVertices<STORE_INDICES, Vertex, ParentElemType, VERT, OPT>,
            CompId::ADJACENT_VERTICES,
            Vertex,
            -1,
            ParentElemType,
            VERT,
            OPT,
            false>
{
    using Base = ReferenceContainerComponent<
        STORE_INDICES,
        AdjacentVertices<STORE_INDICES, Vertex, ParentElemType, VERT, OPT>,
        CompId::ADJACENT_VERTICES,
        Vertex,
        -1,
        ParentElemType,
        VERT,
        OPT,
        false>;

public:
    /**
     * @brief Expose the type of the Adjacent Vertex.
     */
    using AdjacentVertexType = Vertex;

    /* Iterator Types declaration */

    using AdjacentVertexIterator           = Base::Iterator;
    using ConstAdjacentVertexIterator      = Base::ConstIterator;
    using ConstAdjacentVertexIndexIterator = Base::ConstIndexIterator;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * Initializes an empty container of Adjacent Vertices.
     */
    AdjacentVertices() = default;

    /* Member functions */

    /**
     * @brief Returns the number of adjacent vertices of the element.
     * @return The number of adjacent vertices of the element.
     */
    uint adjVerticesNumber() const { return Base::size(); }

    /**
     * @brief Returns the pointer to the i-th adjacent vertex of an element.
     *
     * @param[in] i: the position of the required adjacent vertex in this
     * container.
     * @return The pointer i-th adjacent vertex of the element.
     */
    Vertex* adjVertex(uint i) { return Base::element(i); }

    /**
     * @brief Returns a const pointer to the i-th adjacent vertex of the
     * element.
     * @param[in] i: the position of the required adjacent vertex in this
     * container; the value must be between 0 and the number of adj vertices.
     * @return The pointer to the i-th adjacent vertex of the element.
     */
    const Vertex* adjVertex(uint i) const { return Base::element(i); }

    /**
     * @brief Returns the index in the vertex container of the i-th adjacent
     * vertex of the element.
     * @param[in] i: the position of the required vertex in this container.
     * @return The index of the i-th adjacent vertex of the element.
     */
    uint adjVertexIndex(uint i) const { return Base::elementIndex(i); }

    /**
     * @brief Returns the pointer to the i-th adjacent vertex of the element,
     * but using as index the module between i and the number of adjacent
     * vertices. You can use this function if you need to get the "next adjacent
     * vertex after position k", without check if it is less than the number of
     * adj vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adjacent vertex
     * auto* next = e.adjVertexMod(k+1); // the adj vertex next to k, that may
     *                                   // also be at pos 0
     * auto* last = e.adjVertexMod(-1); // the adj vertex in position
     *                                  // adjVertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required adjacent vertex in this
     * container, w.r.t. the position 0; value is modularized on
     * adjVertexNumber().
     * @return The pointer to the required adjacent vertex of the element.
     */
    Vertex* adjVertexMod(int i) { return Base::elementMod(i); }

    /**
     * @brief Same of adjVertexMod, but returns a const Pointer to the adjacent
     * vertex.
     * @param[in] i: the position of the required adjacent vertex in this
     * container, w.r.t. the position 0; value is modularized on
     * adjVertexNumber().
     * @return The pointer to the required adjacent vertex of the element.
     */
    const Vertex* adjVertexMod(int i) const { return Base::elementMod(i); }

    /**
     * @brief Returns the index in the vertex container of the i-th adjacent
     * vertex of the element, but using as index the module between i and the
     * number of adjacent vertices. You can use this function if you need to get
     * the "index of the adjacent vertex next to position k", without check if
     * it is less than the number of adjacent vertices. Works also for negative
     * numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adjacent edge
     * auto idx = e.adjVertexIndexMod(k+1); // the index of the adjacent vertex
     *                                      // next to k, that may also be at
     *                                      // pos 0
     * auto lastIdx = e.adjVertexIndexMod(-1); // the index of the adjacent
     *                                         // vertex in position
     *                                         // adjVerticesNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required adjacent vertex in this
     * container, w.r.t. the position 0; value is modularized on
     * adjVerticesNumber().
     * @return The index of the required adjacent vertex of the element.
     */
    uint adjVertexIndexMod(int i) const { return Base::elementIndexMod(i); }

    /**
     * @brief Sets the i-th adjacent vertex of the element.
     * @param[in] i: the position in this container on which set the adj vertex;
     * the value must be between 0 and the number of adj vertices.
     * @param[in] v: The pointer to the adjacent vertex to set to this element.
     */
    void setAdjVertex(uint i, Vertex* v) { Base::setElement(i, v); }

    /**
     * @brief Sets the i-th adjacent vertex of the element.
     * @param[in] i: the position in this container on which set the adj vertex;
     * the value must be between 0 and the number of adj vertices.
     * @param[in] vi: The index in the vertex container of the adjacent vertex
     * to set.
     */
    void setAdjVertex(uint i, uint vi) { Base::setElement(i, vi); }

    /**
     * @brief Sets the adjacent vertex pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * vertex; the value must be between begin() and end().
     * @param[in] v: The pointer to the adjacent vertex to set to the element.
     */
    void setAdjVertex(ConstAdjacentVertexIterator it, Vertex* v)
    {
        Base::setElement(it, v);
    }

    /**
     * @brief Sets the adjacent vertex pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * vertex; the value must be between begin() and end().
     * @param[in] vi: The index in the vertex container of the adjacent vertex
     * to set.
     */
    void setAdjVertex(ConstAdjacentVertexIterator it, uint vi)
    {
        Base::setElement(it, vi);
    }

    /**
     * @brief Sets the adjacent vertex pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * vertex; the value must be between begin() and end().
     * @param[in] v: The pointer to the adjacent vertex to set to the element.
     */
    void setAdjVertex(ConstAdjacentVertexIndexIterator it, Vertex* v)
    {
        Base::setElement(it, v);
    }

    /**
     * @brief Sets the adjacent vertex pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the adjacent
     * vertex; the value must be between begin() and end().
     * @param[in] vi: The index in the vertex container of the adjacent vertex
     * to set.
     */
    void setAdjVertex(ConstAdjacentVertexIndexIterator it, uint vi)
    {
        Base::setElement(it, vi);
    }

    /**
     * @brief Sets the i-th adjacent vertex of the element, but using as index
     * the module between i and the number of adjacent vertices. You can use
     * this function if you need to set the "next adjacent vertex after position
     * k", without check if it is less than the number of adjacent vertices.
     * Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adj vertex
     * e.setAdjVertexMod(k+1, aVertex); // set the adj vertex next to k, that
     *                                  // may also be at pos 0
     * e.setAdjVertexMod(-1, aVertex); // set the adj vertex in position
     *                                 // adjVerticesNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the adj vertex; value is modularized on adjVerticesNumber().
     * @param[in] v: The pointer to the adj vertex to set to the element.
     */
    void setAdjVertexMod(int i, Vertex* v) { Base::setElementMod(i, v); }

    /**
     * @brief Sets the i-th adjacent vertex of the element, but using as index
     * the module between i and the number of adjacent vertices. You can use
     * this function if you need to set the "next adjacent vertex after position
     * k", without check if it is less than the number of adjacent vertices.
     * Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adj vertex
     * e.setAdjVertexMod(k+1, aVertInd); // set the adj vertex next to k, that
     *                                   // may also be at pos 0
     * e.setAdjVertexMod(-1, aVertInd); // set the adj vertex in position
     *                                  // adjVerticesNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the adj vertex; value is modularized on adjVerticesNumber().
     * @param[in] vi: The index in the vertex container of the adj vertex to
     * set.
     */
    void setAdjVertexMod(int i, uint vi) { Base::setElementMod(i, vi); }

    /**
     * @brief Sets all the adjacent vertices of this element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of adjacent vertices to set. The value
     * type of the range must be convertible to a pointer to an AdjacentVertex.
     *
     * @param[in] r: range of adjacent vertices to set.
     */
    template<Range Rng>
    void setAdjVertices(Rng&& r) requires InputRange<Rng, Vertex*>
    {
        Base::setElements(r);
    }

    /**
     * @brief Sets all the adjacent vertices of this element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of adjacent vertex indices to set. The
     * value type of the range must be convertible to unsigned integer.
     *
     * @param[in] r: range of indices of adjacent vertices to set.
     */
    template<Range Rng>
    void setAdjVertices(Rng&& r) requires InputRange<Rng, uint>
    {
        Base::setElements(r);
    }

    /**
     * @brief Returns `true` if the container of adjacent vertices contains the
     * given vertex, `false` otherwise.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return `true` if the container of adjacent vertices contains the given
     * vertex, `false` otherwise.
     */
    bool containsAdjVertex(const Vertex* v) const
    {
        return Base::containsElement(v);
    }

    /**
     * @brief Returns `true` if the container of adjacent vertices contains the
     * given vertex, `false` otherwise.
     *
     * @param[in] vi: the index of the vertex to search.
     * @return `true` if the container of adjacent vertices contains the given
     * vertex, `false` otherwise.
     */
    bool containsAdjVertex(uint vi) const { return Base::containsElement(vi); }

    /**
     * @brief Returns the index of the given adjacent vertex in the container of
     * the element. If the given adjacent vertex is not in the container,
     * returns UINT_NULL.
     *
     * @param[in] v: the pointer to the adjacent vertex to search.
     * @return the index of the given adjacent vertex, or UINT_NULL if it is not
     * found.
     */
    uint indexOfAdjVertex(const Vertex* v) const
    {
        return Base::indexOfElement(v);
    }

    /**
     * @brief Returns the index of the adjacent vertex with the given index in
     * the container of the element. If the given adjacent vertex is not in the
     * container, returns UINT_NULL.
     *
     * @param[in] vi: the index of the adjacent vertex to search.
     * @return the index of the given adjacent vertex, or UINT_NULL if it is not
     * found.
     */
    uint indexOfAdjVertex(uint vi) const { return Base::indexOfElement(vi); }

    /**
     * @brief Resize the container of the adjacent vertices to the given size.
     * @note This function is available only if the container of the Adjacent
     * Vertices has dynamic size.
     * @param[in] n: The new size of the adjacent vertices container.
     */
    void resizeAdjVertices(uint n) { Base::resize(n); }

    /**
     * @brief Pushes in the back of the container the given adjacent vertex.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] v: The pointer to the adjacent vertex to push in the back of
     * the container.
     */
    void pushAdjVertex(Vertex* v) { Base::pushBack(v); }

    /**
     * @brief Pushes in the back of the container the given adjacent vertex..
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] vi: The index to the vertex to push in the back of the
     * container.
     */
    void pushAdjVertex(uint vi) { Base::pushBack(vi); }

    /**
     * @brief Inserts the given adjacent vertex in the container at the given
     * position.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] i: The position in this container where to insert the adjacent
     * vertex.
     * @param[in] v: The pointer to the adjacent vertex to insert in the
     * container.
     */
    void insertAdjVertex(uint i, Vertex* v) { Base::insert(i, v); }

    /**
     * @brief Inserts the given adjacent vertex in the container at the given
     * position.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] i: The position in this container where to insert the adjacent
     * vertex.
     * @param[in] vi: The index of the vertex to insert in the container.
     */
    void insertAdjVertex(uint i, uint vi) { Base::insert(i, vi); }

    /**
     * @brief Removes the adjacent vertex at the given position from the
     * container.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] i: The position of the adjacent vertex to remove from this
     * container.
     */
    void eraseAdjVertex(uint i) { Base::erase(i); }

    /**
     * @brief Clears the container of adjacent vertices, making it empty.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     */
    void clearAdjVertices() { Base::clear(); }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first adjacent vertex in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    AdjacentVertexIterator adjVertexBegin() { return Base::elementBegin(); }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of this container.
     */
    AdjacentVertexIterator adjVertexEnd() { return Base::elementEnd(); }

    /**
     * @brief Returns a const iterator to the first adjacent vertex in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstAdjacentVertexIterator adjVertexBegin() const
    {
        return Base::elementBegin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstAdjacentVertexIterator adjVertexEnd() const
    {
        return Base::elementEnd();
    }

    /**
     * @brief Returns an iterator to the first adjacent vertex index in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of the adjacent vertex indices.
     */
    ConstAdjacentVertexIndexIterator adjVertexIndexBegin() const
    {
        return Base::elementIndexBegin();
    }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of the adjacent vertex indices.
     */
    ConstAdjacentVertexIndexIterator adjVertexIndexEnd() const
    {
        return Base::elementIndexEnd();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and
     * end iterators of the container of adjacent vertices of the element. The
     * view object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto* adjEdge : el.adjVertices()) {
     *     // Do something with adjVertex
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent edges.
     */
    View<AdjacentVertexIterator> adjVertices() { return Base::elements(); }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the container of adjacent vertices of the element. The
     * view object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (const auto* adjEdge : el.adjVertices()) {
     *     // Do something read-only with adjVertex
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent edges.
     */
    View<ConstAdjacentVertexIterator> adjVertices() const
    {
        return Base::elements();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of adjacent vertex indices of the element. The
     * view object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (uint eid : el.adjVertexIndices()) {
     *     // Do something with adj vertex index...
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent vertex indices.
     */
    View<ConstAdjacentVertexIndexIterator> adjVertexIndices() const
    {
        return Base::elementIndices();
    }

    // dummy member to discriminate between AdjacentVertices and
    // VertexHalfEdgePointers
    void __adjacentVertices() const {}

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool importRefs = true)
    {
        if (importRefs) {
            if constexpr (HasAdjacentVertices<Element>) {
                if (isAdjacentVerticesAvailableOn(e)) {
                    // from static/dynamic to dynamic size: need to resize
                    // first, then import
                    resizeAdjVertices(e.adjVerticesNumber());
                    importIndicesFrom(e);
                }
            }
        }
    }

    void serialize(std::ostream& os) const
    {
        // regardless of the type of the container (indices or pointers), the
        // serialization is always done using the indices

        vcl::serialize(os, adjVerticesNumber());
        for (uint i = 0; i < adjVerticesNumber(); ++i) {
            vcl::serialize(os, adjVertexIndex(i));
        }
    }

    void deserialize(std::istream& is)
    {
        uint n;
        vcl::deserialize(is, n);
        resizeAdjVertices(n);

        for (uint i = 0; i < adjVerticesNumber(); ++i) {
            uint aei;
            vcl::deserialize(is, aei);
            setAdjVertex(i, aei);
        }
    }

private:
    template<typename Element>
    void importIndicesFrom(const Element& e)
    {
        for (uint i = 0; i < e.adjVerticesNumber(); ++i) {
            setAdjVertex(i, e.adjVertexIndex(i));
        }
    }
};

/* Detector function to check if a class has AdjacentVertices available */

/**
 * @brief Checks if the given Element has AdjacentVertices component
 * available.
 *
 * This function returns `true` also if the component is horizontal and always
 * available in the element. The runtime check is performed only when the
 * component is optional.
 *
 * @param[in] element: The element to check.
 * @return `true` if the element has AdjacentVertices component available,
 * `false` otherwise.
 */
bool isAdjacentVerticesAvailableOn(const auto& element)
{
    return isComponentAvailableOn<CompId::ADJACENT_VERTICES>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
