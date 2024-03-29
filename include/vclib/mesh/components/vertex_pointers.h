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

#ifndef VCL_MESH_COMPONENTS_VERTEX_POINTERS_H
#define VCL_MESH_COMPONENTS_VERTEX_POINTERS_H

#include <vclib/concepts/mesh/components/vertex_pointers.h>
#include <vclib/views/view.h>

#include "bases/pointers_container_component.h"

namespace vcl::comp {

/**
 * @brief The VertexPointers class represents a component that stores a
 * container of pointers to vertices that will be part of an Element (e.g.
 * Face, Edge, Tetrahedron, etc.).
 *
 * The container of Vertex pointers is a random access container that could have
 * static or dynamic size, depending on the value of the template argument N
 * (a negative number means dynamic).
 *
 * For example, if a Face element should represent only triangles, N could be
 * 3. In this case, the container will have static size and it can't be resized
 * in any way. If N is -1, the container will have dynamic size and it will be
 * possible to resize it (e.g. if the Face element should represent a polygon).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the VertexPointers
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * uint n = f.vertexNumber();
 * auto* v = f.vertex(0);
 * @endcode
 *
 * @note This component is usually the main component of an Element, and
 * therefore it cannot be optional.
 *
 * @note Several components are *Tied To Vertex Number*: in other words, they
 * are composed by a container that has the same size of the VertexPointers
 * and, when the VertexPointers container is resized, also the container of
 * these components is resized automatically.
 *
 * @tparam Vertex The type of the vertices.
 * @tparam N: The size of the container, that will represent the number of
 * storable vertex pointers. If negative, the container is dynamic.
 * @tparam ElementType: This template argument must be `void` if the component
 * needs to be stored horizontally, or the type of the element that will contain
 * this component if the component needs to be stored vertically.
 *
 * @ingroup components
 */
template<typename Vertex, int N, typename ElementType = void>
class VertexPointers :
        public PointersContainerComponent<
            VertexPointers<Vertex, N, ElementType>,
            CompId::VERTEX_PTRS,
            Vertex,
            N,
            ElementType,
            false,
            false>
{
    using Base = PointersContainerComponent<
        VertexPointers<Vertex, N, ElementType>,
        CompId::VERTEX_PTRS,
        Vertex,
        N,
        ElementType,
        false,
        false>;

public:
    /**
     * @brief Expose the type of the Vertex.
     */
    using VertexType = Vertex;

    static const int VERTEX_NUMBER = Base::SIZE;

    /* Iterator Types declaration */

    using VertexIterator      = Base::Iterator;
    using ConstVertexIterator = Base::ConstIterator;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the Vertex Pointers container size is static, initializes all the
     * Vertex Pointers to `nullptr`, otherwise the container will be empty.
     */
    VertexPointers() = default;

    /* Member functions */

    /**
     * @brief Returns the number of vertices of the element.
     * @return The number of vertices of the element.
     */
    uint vertexNumber() const { return Base::container().size(); }

    /**
     * @brief Returns a reference of the pointer to the i-th vertex of the
     * element.
     *
     * You can use this function to set the i-th vertex:
     *
     * @code{.cpp}
     * f.vertex(2) = &m.vertex(k); // the second vertex of f will point to
     *                             // the k-th vertex of the mesh m.
     * @endcode
     *
     * @param[in] i: the position of the required vertex in the container.
     * @return The pointer i-th vertex of the element.
     */
    Vertex*& vertex(uint i) { return Base::container().at(i); }

    /**
     * @brief Returns a const pointer to the i-th vertex of the element.
     * @param[in] i: the position of the required vertex in the container;
     * the value must be between 0 and the number of vertices.
     * @return The pointer to the i-th vertex of the element.
     */
    const Vertex* vertex(uint i) const { return Base::container().at(i); }

    /**
     * @brief Returns a reference of the pointer to the i-th vertex of the
     * element, but using as index the module between i and the number of
     * vertices. You can use this function if you need to get the "next vertex
     * after position k", without check if it is less than the number of
     * vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * auto* next = e.vertexMod(k+1); // the vertex next to k, that may also
     *                                // be at pos 0
     * auto* last = e.vertexMod(-1); // the vertex in position vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required vertex in the container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The pointer to the required vertex of the element.
     */
    Vertex*& vertexMod(int i) { return Base::container().atMod(i); }

    /**
     * @brief Same of vertexMod, but returns a const pointer to the vertex.
     * @param[in] i: the position of the required vertex in the container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The pointer to the required vertex of the element.
     */
    const Vertex* vertexMod(int i) const { return Base::container().atMod(i); }

    /**
     * @brief Sets the i-th vertex of the element.
     * @param[in] v: The pointer to the vertex to set to the element.
     * @param[in] i: the position in the container on which set the vertex; the
     * value must be between 0 and the number of vertices.
     */
    void setVertex(Vertex* v, uint i) { Base::container().set(v, i); }

    /**
     * @brief Sets all the vertex pointers of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of vertex pointers to set. The value
     * type of the range must be convertible to a pointer to a Vertex.
     *
     * @param[in] r: range of vertex pointers to set.
     */
    template<Range Rng>
    void setVertices(Rng&& r) requires RangeOfConvertibleTo<Rng, Vertex*>
    {
        Base::container().set(r);
    }

    /**
     * @brief Returns `true` if the container of vertices contains the given
     * vertex, `false` otherwise.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return `true` if the container of vertices contains the given vertex,
     * `false` otherwise.
     */
    bool containsVertex(const Vertex* v) const
    {
        return Base::container().contains(v);
    }

    /**
     * @brief Returns an iterator to the first vertex in the container of this
     * component that is equal to the given vertex. If no such vertex is
     * found, past-the-end iterator is returned.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return an iterator pointing to the first vertex equal to the given
     * vertex, or end if no such vertex is found.
     */
    VertexIterator findVertex(const Vertex* v)
    {
        return Base::container().find(v);
    }

    /**
     * @brief Returns a const iterator to the first vertex in the container of
     * this component that is equal to the given vertex. If no such vertex is
     * found, past-the-end iterator is returned.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return a const iterator pointing to the first vertex equal to the given
     * vertex, or end if no such vertex is found.
     */
    ConstVertexIterator findVertex(const Vertex* v) const
    {
        return Base::container().find(v);
    }

    /**
     * @brief Returns the index of the given vertex in the container of
     * the element. If the given vertex is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] v: the pointer to the vertex to search.
     * @return the index of the given vertex, or UINT_NULL if it is not
     * found.
     */
    uint indexOfVertex(const Vertex* v) const
    {
        return Base::container().indexOf(v);
    }

    /**
     * @brief Returns the index of the given edge composed of the two vertices
     * v1 and v2 in the container of the element.
     *
     * The order of the two vertices is not important. The index of the edge
     * corresponds to the index of the first vertex found in the container. If
     * the edge formed by the two vertices is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] v1: the pointer to the first vertex that compose the edge to
     * search.
     * @param[in] v2: the pointer to the second vertex that compose the edge to
     * search.
     * @return the index of the edge composed of v1 and v2, or UINT_NULL if it
     * is not found.
     */
    uint indexOfEdge(const Vertex* v1, const Vertex* v2) const
    {
        uint vid = indexOfVertex(v1);
        if (vid == UINT_NULL) {
            return UINT_NULL;
        }
        else if (vertexMod(vid + 1) == v2) {
            return vid;
        }
        else if (vertexMod((int) vid - 1) == v2) {
            uint n = vertexNumber();
            return (((int) vid - 1) % n + n) % n;
        }
        else {
            return UINT_NULL;
        }
    }

    /* Member functions specific for vector of pointers */

    /**
     * @brief Resize the container of the vertices to the given size.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] n: The new size of the vertices container.
     */
    void resizeVertices(uint n) requires (N < 0)
    {
        Base::container().resize(n);
    }

    /**
     * @brief Pushes in the back of the container the given vertex.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] v: The pointer to the vertex to push in the back of the
     * container.
     */
    void pushVertex(Vertex* v) requires (N < 0)
    {
        Base::container().pushBack(v);
    }

    /**
     * @brief Inserts the given vertex in the container at the given position.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position in the container where to insert the vertex.
     * @param[in] v: The pointer to the vertex to insert in the container.
     */
    void insertVertex(uint i, Vertex* v) requires (N < 0)
    {
        Base::container().insert(i, v);
    }

    /**
     * @brief Removes the vertex at the given position from the container.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position of the vertex to remove from the container.
     */
    void eraseVertex(uint i) requires (N < 0) { Base::container().erase(i); }

    /**
     * @brief Clears the container of vertices, making it empty.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     */
    void clearVertices() requires (N < 0) { Base::container().clear(); }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first vertex in the container of this
     * component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    VertexIterator vertexBegin() { return Base::container().begin(); }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of this container.
     */
    VertexIterator vertexEnd() { return Base::container().end(); }

    /**
     * @brief Returns a const iterator to the first vertex in the container of
     * this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstVertexIterator vertexBegin() const
    {
        return Base::container().begin();
    }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstVertexIterator vertexEnd() const { return Base::container().end(); }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of vertices of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (auto* vertex : el.vertices()) {
     *     // Do something with vertex...
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over vertices.
     */
    View<VertexIterator> vertices() { return View(vertexBegin(), vertexEnd()); }

    /**
     * @brief Returns a lightweight const view object that stores the begin and
     * end iterators of the container of vertices of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (const auto* vertex : el.vertices()) {
     *     // Do something read-only with vertex...
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over vertices.
     */
    View<ConstVertexIterator> vertices() const
    {
        return View(vertexBegin(), vertexEnd());
    }

protected:
    // Component interface function
    template<typename Element>
    void importFrom(const Element& e)
    {
    }

    // PointersComponent interface functions
    template<typename Element, typename ElVType>
    void importPointersFrom(
        const Element& e,
        Vertex*        base,
        const ElVType* ebase)
    {
        if constexpr (HasVertexPointers<Element>) {
            if constexpr (N > 0) {
                // same size non-polygonal faces
                if constexpr (N == Element::VERTEX_NUMBER) {
                    importPtrsFrom(e, base, ebase);
                }
                // from polygonal to fixed size, but the polygon size == the
                // fixed face size
                else if constexpr (Element::VERTEX_NUMBER < 0) {
                    if (e.vertexNumber() == N) {
                        importPtrsFrom(e, base, ebase);
                    }
                }
                else {
                    // do not import in this case: cannot import from a face of
                    // different size
                }
            }
            else {
                // from fixed to polygonal size: need to resize first, then
                // import
                resizeVertices(e.vertexNumber());
                importPtrsFrom(e, base, ebase);
            }
        }
    }

private:
    template<typename Element, typename ElVType>
    void importPtrsFrom(const Element& e, Vertex* base, const ElVType* ebase)
    {
        if (ebase != nullptr && base != nullptr) {
            for (uint i = 0; i < e.vertexNumber(); ++i) {
                if (e.vertex(i) != nullptr) {
                    vertex(i) = base + (e.vertex(i) - ebase);
                }
            }
        }
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_VERTEX_POINTERS_H
