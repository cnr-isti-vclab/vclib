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

#ifndef VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include <vclib/concepts/mesh/components/adjacent_vertices.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The AdjacentVertices class is a container of Vertex pointers. It is a
 * component that makes sense to use mostly on Vertex Elements. For Faces and
 * Edges, see the VertexPointers component (which is similar, but has different
 * member function names).
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
 * @tparam Vertex: The type of the adjacent Vertex element.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 * @tparam OPT: If true, the component will be optional. This argument is
 * considered only if the component is stored vertically.
 *
 * @ingroup components
 */
template<typename Vertex, typename ElementType = void, bool OPT = false>
class AdjacentVertices :
        public PointersContainerComponent<
            AdjacentVertices<Vertex, ElementType, OPT>,
            ADJACENT_VERTICES,
            Vertex,
            -1,
            ElementType,
            OPT,
            false>
{
    using Base = PointersContainerComponent<
        AdjacentVertices<Vertex, ElementType, OPT>,
        ADJACENT_VERTICES,
        Vertex,
        -1,
        ElementType,
        OPT,
        false>;

public:
    /**
     * @brief Expose the type of the Adjacent Vertex.
     */
    using AdjacentVertexType = Vertex;

    /* Iterator Types declaration */

    using AdjacentVertexIterator      = Base::Iterator;
    using ConstAdjacentVertexIterator = Base::ConstIterator;

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
    uint adjVerticesNumber() const { return Base::container().size(); }

    /**
     * @brief Returns a reference of the pointer to the i-th adjacent vertex of
     * an element.
     *
     * You can use this function to set the i-th adjacent vertex:
     *
     * @code{.cpp}
     * e.adjVertex(2) = &m.vertex(k); // the second adj vertex of e will point
     *                                // to the k-th vertex of the mesh.
     * @endcode
     *
     * @param[in] i: the position of the required adjacent vertex in the
     * container.
     * @return The pointer i-th adjacent vertex of the element.
     */
    Vertex*& adjVertex(uint i) { return Base::container().at(i); }

    /**
     * @brief Returns a const pointer to the i-th adjacent vertex of the
     * element.
     * @param[in] i: the position of the required adjacent vertex in the
     * container; the value must be between 0 and the number of adj vertices.
     * @return The pointer to the i-th adjacent vertex of the element.
     */
    const Vertex* adjVertex(uint i) const { return Base::container().at(i); }

    /**
     * @brief Returns a reference of the pointer to the i-th adjacent vertex of
     * the element, but using as index the module between i and the number of
     * adjacent vertices. You can use this function if you need to get the "next
     * adjacent vertex after position k", without check if it is less than the
     * number of adj vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of an adjacent vertex
     * auto* next = e.adjVertexMod(k+1); // the adj vertex next to k, that may
     *                                   // also be at pos 0
     * auto* last = e.adjVertexMod(-1); // the adj vertex in position
     *                                  // adjVertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required adjacent vertex in the
     * container, w.r.t. the position 0; value is modularized on
     * adjVertexNumber().
     * @return The pointer to the required adjacent vertex of the element.
     */
    Vertex*& adjVertexMod(int i) { return Base::container().atMod(i); }

    /**
     * @brief Same of adjVertexMod, but returns a const Pointer to the adjacent
     * vertex.
     * @param[in] i: the position of the required adjacent vertex in the
     * container, w.r.t. the position 0; value is modularized on
     * adjVertexNumber().
     * @return The pointer to the required adjacent vertex of the element.
     */
    const Vertex* adjVertexMod(int i) const
    {
        return Base::container().atMod(i);
    }

    /**
     * @brief Sets the i-th adjacent vertex of the element.
     * @param[in] v: The pointer to the adjacent vertex to set to this element.
     * @param[in] i: the position in the container on which set the adj vertex;
     * the value must be between 0 and the number of adj vertices.
     */
    void setAdjVertex(Vertex* v, uint i) { Base::container().set(v, i); }

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
    void setAdjVertices(Rng&& r) requires RangeOfConvertibleTo<Rng, Vertex*>
    {
        Base::container().set(r);
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
        return Base::container().contains(v);
    }

    /**
     * @brief Returns an iterator to the first adjacent vertex in the container
     * of this component that is equal to the given vertex. If no such adjacent
     * vertex is found, past-the-end iterator is returned.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return an iterator pointing to the first adjacent vertex equal to the
     * given vertex, or end if no such adjacent vertex is found.
     */
    AdjacentVertexIterator findAdjVertex(const Vertex* v)
    {
        return Base::container().find(v);
    }

    /**
     * @brief Returns a const iterator to the first adjacent vertex in the
     * container of this component that is equal to the given vertex. If no such
     * adjacent vertex is found, past-the-end iterator is returned.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return a const iterator pointing to the first adjacent vertex equal to
     * the given vertex, or end if no such adjacent vertex is found.
     */
    ConstAdjacentVertexIterator findAdjVertex(const Vertex* v) const
    {
        return Base::container().find(v);
    }

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
        return Base::container().indexOf(v);
    }

    /**
     * @brief Resize the container of the adjacent vertices to the given size.
     * @note This function is available only if the container of the Adjacent
     * Vertices has dynamic size.
     * @param[in] n: The new size of the adjacent vertices container.
     */
    void resizeAdjVertices(uint n) { Base::container().resize(n); }

    /**
     * @brief Pushes in the back of the container the given adjacent vertex.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] v: The pointer to the adjacent vertex to push in the back of
     * the container.
     */
    void pushAdjVertex(Vertex* v) { Base::container().pushBack(v); }

    /**
     * @brief Inserts the given adjacent vertex in the container at the given
     * position.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] i: The position in the container where to insert the adjacent
     * vertex.
     * @param[in] v: The pointer to the adjacent vertex to insert in the
     * container.
     */
    void insertAdjVertex(uint i, Vertex* v) { Base::container().insert(i, v); }

    /**
     * @brief Removes the adjacent vertex at the given position from the
     * container.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     * @param[in] i: The position of the adjacent vertex to remove from the
     * container.
     */
    void eraseAdjVertex(uint i) { Base::container().erase(i); }

    /**
     * @brief Clears the container of adjacent vertices, making it empty.
     * @note This function is available only if the container of the Adjacent
     * Vertices component has dynamic size.
     */
    void clearAdjVertices() { Base::container().clear(); }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first adjacent vertex in the container
     * of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    AdjacentVertexIterator adjVertexBegin()
    {
        return Base::container().begin();
    }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of this container.
     */
    AdjacentVertexIterator adjVertexEnd() { return Base::container().end(); }

    /**
     * @brief Returns a const iterator to the first adjacent vertex in the
     * container of this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstAdjacentVertexIterator adjVertexBegin() const
    {
        return Base::container().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstAdjacentVertexIterator adjVertexEnd() const
    {
        return Base::container().end();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of adjacent vertices of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto* adjVertex : el.adjVertices()) {
     *     // Do something with adjVertex
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over adjacent vertices.
     */
    View<AdjacentVertexIterator> adjVertices()
    {
        return View(adjVertexBegin(), adjVertexEnd());
    }

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
        return View(adjVertexBegin(), adjVertexEnd());
    }

    // dummy member to discriminate between AdjacentVertices and
    // VertexHalfEdgePointers
    void __adjacentVertices() const {}

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e)
    {
    }

    // PointersContainerComponent interface functions
    template<typename Element, typename ElVType>
    void importPointersFrom(
        const Element& e,
        Vertex*        base,
        const ElVType* ebase)
    {
        if constexpr (HasAdjacentVertices<Element>) {
            if (isAdjacentVerticesAvailableOn(e)) {
                // from static/dynamic to dynamic size: need to resize first,
                // then import
                resizeAdjVertices(e.adjVerticesNumber());
                importPtrsFrom(e, base, ebase);
            }
        }
    }

private:
    template<typename Element, typename ElVType>
    void importPtrsFrom(const Element& e, Vertex* base, const ElVType* ebase)
    {
        if (ebase != nullptr && base != nullptr) {
            for (uint i = 0; i < e.adjVerticesNumber(); ++i) {
                if (e.adjVertex(i) != nullptr) {
                    adjVertex(i) = base + (e.adjVertex(i) - ebase);
                }
            }
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
 * @param[in] element: The element to check. Must be of a type that
 * satisfies the ElementConcept.
 * @return `true` if the element has AdjacentVertices component available,
 * `false` otherwise.
 */
bool isAdjacentVerticesAvailableOn(const ElementConcept auto& element)
{
    return isComponentAvailableOn<ADJACENT_VERTICES>(element);
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
