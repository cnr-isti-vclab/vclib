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

#include <vclib/concepts/mesh/components/vertex_references.h>
#include <vclib/views/view.h>

#include "bases/reference_container_component.h"

namespace vcl::comp {

/**
 * @brief The VertexReferences class represents a component that stores a
 * container of indices or pointers to vertices that will be part of an Element
 * (e.g. Face, Edge, Tetrahedron, etc.).
 *
 * The container of Vertex references is a random access container that could
 * have static or dynamic size, depending on the value of the template argument
 * N (a negative number means dynamic).
 *
 * For example, if a Face element should represent only triangles, N could be
 * 3. In this case, the container will have static size and it can't be resized
 * in any way. If N is -1, the container will have dynamic size and it will be
 * possible to resize it (e.g. if the Face element should represent a polygon).
 *
 * The member functions of this class will be available in the instance of any
 * Element that will contain this component.
 *
 * For example, if you have a Face Element `f` that has the VertexReferences
 * component, you'll be able to access to this component member functions from
 * `f`:
 *
 * @code{.cpp}
 * uint n = f.vertexNumber();
 * auto* v = f.vertex(0);
 * uint vi = f.vertexIndex(0);
 * @endcode
 *
 * @note This component is usually the main component of an Element, and
 * therefore it cannot be optional.
 *
 * @note Several components are *Tied To Vertex Number*: in other words, they
 * are composed by a container that has the same size of the VertexReferences
 * and, when the VertexReferences container is resized, also the container of
 * these components is resized automatically.
 *
 * @tparam STORE_INDICES: If true, the component will store indices, otherwise
 * pointers to Vertex.
 * @tparam Vertex The type of the vertices.
 * @tparam N: The size of the container, that will represent the number of
 * storable vertices. If negative, the container is dynamic.
 * @tparam ParentElemType: This type is used to get access to the Element that
 * has the component (and, in case, to the Mesh that has the Element). If the
 * component doesn't need to access the Element, this type can be void. Note:
 * if the component is vertical (or optional), this type cannot be void.
 * @tparam VERT: If true, the component will be stored vertically. This argument
 * is considered only if the ElementType is not void.
 *
 * @ingroup components
 */
template<
    bool STORE_INDICES,
    typename Vertex,
    int N,
    typename ParentElemType = void,
    bool VERT               = false>
class VertexReferences :
        public ReferenceContainerComponent<
            STORE_INDICES,
            VertexReferences<STORE_INDICES, Vertex, N, ParentElemType, VERT>,
            CompId::VERTEX_REFERENCES,
            Vertex,
            N,
            ParentElemType,
            VERT,
            false,
            false>
{
    using Base = ReferenceContainerComponent<
        STORE_INDICES,
        VertexReferences<STORE_INDICES, Vertex, N, ParentElemType, VERT>,
        CompId::VERTEX_REFERENCES,
        Vertex,
        N,
        ParentElemType,
        VERT,
        false,
        false>;

public:
    /**
     * @brief Expose the type of the Vertex.
     */
    using VertexType = Vertex;

    /* Iterator Types declaration */

    using VertexIterator           = Base::Iterator;
    using ConstVertexIterator      = Base::ConstIterator;
    using ConstVertexIndexIterator = Base::ConstIndexIterator;

    static const int VERTEX_NUMBER = Base::SIZE;

    /* Constructors */

    /**
     * @brief Empty constructor.
     *
     * If the VertexReferences container size is static, initializes all the
     * pointers/indices to `nullptr`/`UINT_NULL`, otherwise the container will
     * be empty.
     */
    VertexReferences() = default;

    /* Member functions */

    /**
     * @brief Returns the number of vertices of the element.
     * @return The number of vertices of the element.
     */
    uint vertexNumber() const { return Base::size(); }

    /**
     * @brief Returns the pointer to the i-th vertex of the element.
     *
     * @param[in] i: the position of the required vertex in this container.
     * @return The pointer i-th vertex of the element.
     */
    Vertex* vertex(uint i) { return Base::element(i); }

    /**
     * @brief Returns a const pointer to the i-th vertex of the element.
     * @param[in] i: the position of the required vertex in this container;
     * the value must be between 0 and the number of vertices.
     * @return The pointer to the i-th vertex of the element.
     */
    const Vertex* vertex(uint i) const { return Base::element(i); }

    /**
     * @brief Returns the index in the vertex container of the i-th vertex of
     * the element.
     * @param[in] i: the position of the required vertex in this container.
     * @return The index of the i-th vertex of the element.
     */
    uint vertexIndex(uint i) const { return Base::elementIndex(i); }

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
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The pointer to the required vertex of the element.
     */
    Vertex* vertexMod(int i) { return Base::elementMod(i); }

    /**
     * @brief Same of vertexMod, but returns a const pointer to the vertex.
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The pointer to the required vertex of the element.
     */
    const Vertex* vertexMod(int i) const { return Base::elementMod(i); }

    /**
     * @brief Returns the index in the vertex container of the i-th vertex of
     * the element, but using as index the module between i and the number of
     * vertices. You can use this function if you need to get the "index of the
     * vertex next to position k", without check if it is less than the number
     * of vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * auto idx = e.vertexIndexMod(k+1); // the index of the vertex next to k,
     *                                   // that may also be at pos 0
     * auto lastIdx = e.vertexIndexMod(-1); // the index of the vertex in
     *                                      // position vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position of the required vertex in this container,
     * w.r.t. the position 0; value is modularized on vertexNumber().
     * @return The index of the required vertex of the element.
     */
    uint vertexIndexMod(int i) const { return Base::elementIndexMod(i); }

    /**
     * @brief Sets the i-th vertex of the element.
     * @param[in] i: the position in this container on which set the vertex; the
     * value must be between 0 and the number of vertices.
     * @param[in] v: The pointer to the vertex to set to the element.
     */
    void setVertex(uint i, Vertex* v) { Base::setElement(i, v); }

    /**
     * @brief Sets the i-th vertex of the element.
     * @param[in] i: the position in this container on which set the vertex; the
     * value must be between 0 and the number of vertices.
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertex(uint i, uint vi) { Base::setElement(i, vi); }

    /**
     * @brief Sets the vertex pointed by the iterator.
     * @param[in] it:the iterator in this container on which set the vertex; the
     * value must be between begin() and end().
     * @param[in] v: The pointer to the vertex to set to the element.
     */
    void setVertex(ConstVertexIterator it, Vertex* v)
    {
        Base::setElement(it, v);
    }

    /**
     * @brief Sets the vertex pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the vertex;
     * the value must be between begin() and end().
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertex(ConstVertexIterator it, uint vi)
    {
        Base::setElement(it, vi);
    }

    /**
     * @brief Sets the vertex pointed by the iterator.
     * @param[in] it: iterator in this container on which set the vertex; the
     * value must be between begin() and end().
     * @param[in] v: The pointer to the vertex to set to the element.
     */
    void setVertex(ConstVertexIndexIterator it, Vertex* v)
    {
        Base::setElement(it, v);
    }

    /**
     * @brief Sets the vertex pointed by the iterator.
     * @param[in] it: the iterator in this container on which set the vertex;
     * the value must be between begin() and end().
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertex(ConstVertexIndexIterator it, uint vi)
    {
        Base::setElement(it, vi);
    }

    /**
     * @brief Sets the i-th vertex of the element, but using as index the module
     * between i and the number of vertices. You can use this function if you
     * need to set the "next vertex after position k", without check if it is
     * less than the number of vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * e.setVertexMod(k+1, aVertex); // set the vertex next to k, that may also
     *                               // be at pos 0
     * e.setVertexMod(-1, aVertex); // set the vertex in position
     *                              // vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the vertex; value is modularized on vertexNumber().
     * @param[in] v: The pointer to the vertex to set to the element.
     */
    void setVertexMod(int i, Vertex* v) { Base::setElementMod(i, v); }

    /**
     * @brief Sets the i-th vertex of the element, but using as index the module
     * between i and the number of vertices. You can use this function if you
     * need to set the "next vertex after position k", without check if it is
     * less than the number of vertices. Works also for negative numbers:
     *
     * @code{.cpp}
     * k = pos; // some position of a vertex
     * e.setVertexMod(k+1, aVertInd); // set the vertex next to k, that may also
     *                               // be at pos 0
     * e.setVertexMod(-1, aVertInd); // set the vertex in position
     *                              // vertexNumber()-1
     * @endcode
     *
     * @param[in] i: the position in this container w.r.t. the position 0 on
     * which set the vertex; value is modularized on vertexNumber().
     * @param[in] vi: The index in the vertex container of the vertex to set.
     */
    void setVertexMod(int i, uint vi) { Base::setElementMod(i, vi); }

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
        Base::setElements(r);
    }

    /**
     * @brief Sets all the vertex pointers of the element.
     *
     * If the size of the container is static, the size of the input range must
     * be the same one of the container.
     *
     * @tparam Rng: The type of the range of vertex indices to set. The value
     * type of the range must be convertible to unsigned integer.
     *
     * @param[in] r: range of vertex indices to set.
     */
    template<Range Rng>
    void setVertices(Rng&& r) requires RangeOfConvertibleTo<Rng, uint>
    {
        Base::setElements(r);
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
        return Base::containsElement(v);
    }

    /**
     * @brief Returns `true` if the container of vertices contains the vertex
     * with the given index, `false` otherwise.
     *
     * @param[in] vi: the index of the vertex to search.
     * @return `true` if the container of vertices contains the vertex with the
     * given index, `false` otherwise.
     */
    bool containsVertex(uint vi) const { return Base::containsElement(vi); }

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
        return Base::indexOfElement(v);
    }

    /**
     * @brief Returns the index of the vertex with the given index in the
     * container of the element. If the vertex with the given index is not in
     * the container, returns UINT_NULL.
     *
     * @param[in] vi: the index of the vertex to search.
     * @return the index of the vertex with the given index, or UINT_NULL if it
     * is not found.
     */
    uint indexOfVertex(uint vi) const { return Base::indexOfElement(vi); }

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
        if constexpr (STORE_INDICES) {
            return indexOfEdge(
                Base::indexFromPointer(v1), Base::indexFromPointer(v2));
        }
        else {
            uint vid = indexOfVertex(v1);
            if (vid == UINT_NULL) {
                return UINT_NULL;
            }
            else if (vertexMod(vid + 1) == v2) {
                return vid;
            }
            else if (vertexMod((int) vid - 1) == v2) {
                int n = vertexNumber(); // n must be int to avoid unwanted casts
                return (((int) vid - 1) % n + n) % n;
            }
            else {
                return UINT_NULL;
            }
        }
    }

    /**
     * @brief Returns the index of the edge composed of the two vertices with
     * the given indices in the container of the element.
     *
     * The order of the two vertices is not important. The index of the edge
     * corresponds to the index of the first vertex found in the container. If
     * the edge formed by the two vertices is not in the container, returns
     * UINT_NULL.
     *
     * @param[in] vi1: the index of the first vertex that compose the edge to
     * search.
     * @param[in] vi2: the index of the second vertex that compose the edge to
     * search.
     * @return the index of the edge composed of the vertices with the given
     * indices, or UINT_NULL if it is not found.
     */
    uint indexOfEdge(uint vi1, uint vi2) const
    {
        if constexpr (STORE_INDICES) {
            uint vid = indexOfVertex(vi1);
            if (vid == UINT_NULL) {
                return UINT_NULL;
            }
            else if (vertexIndexMod(vid + 1) == vi2) {
                return vid;
            }
            else if (vertexIndexMod((int) vid - 1) == vi2) {
                int n = vertexNumber(); // n must be int to avoid unwanted casts
                return (((int) vid - 1) % n + n) % n;
            }
            else {
                return UINT_NULL;
            }
        }
        else {
            return indexOfEdge(
                Base::elemFromParent(vi1), Base::elemFromParent(vi2));
        }
    }

    /* Member functions specific for dynamic vector of refs */

    /**
     * @brief Resize the container of the vertices to the given size.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] n: The new size of the vertices container.
     */
    void resizeVertices(uint n) requires (N < 0) { Base::resize(n); }

    /**
     * @brief Pushes in the back of the container the given vertex.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] v: The pointer to the vertex to push in the back of the
     * container.
     */
    void pushVertex(Vertex* v) requires (N < 0) { Base::pushBack(v); }

    /**
     * @brief Pushes in the back of the container the given vertex.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] vi: The index to the vertex to push in the back of the
     * container.
     */
    void pushVertex(uint vi) requires (N < 0) { Base::pushBack(vi); }

    /**
     * @brief Inserts the given vertex in the container at the given position.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position in this container where to insert the vertex.
     * @param[in] v: The pointer to the vertex to insert in the container.
     */
    void insertVertex(uint i, Vertex* v) requires (N < 0)
    {
        Base::insert(i, v);
    }

    /**
     * @brief Inserts the vertex with the given index in the container at the
     * given position.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position in this container where to insert the vertex.
     * @param[in] vi: The index of the vertex to insert in the container.
     */
    void insertVertex(uint i, uint vi) requires (N < 0) { Base::insert(i, vi); }

    /**
     * @brief Removes the vertex at the given position from the container.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     * @param[in] i: The position of the vertex to remove from this container.
     */
    void eraseVertex(uint i) requires (N < 0) { Base::erase(i); }

    /**
     * @brief Clears the container of vertices, making it empty.
     * @note This function is available only if the container of the Vertices
     * has dynamic size.
     */
    void clearVertices() requires (N < 0) { Base::clear(); }

    /* Iterator Member functions */

    /**
     * @brief Returns an iterator to the first vertex in the container of
     * this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    VertexIterator vertexBegin() { return Base::elementBegin(); }

    /**
     * @brief Returns an iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    VertexIterator vertexEnd() { return Base::elementEnd(); }

    /**
     * @brief Returns a const iterator to the first vertex in the container of
     * this component.
     *
     * @return an iterator pointing to the begin of this container.
     */
    ConstVertexIterator vertexBegin() const { return Base::elementBegin(); }

    /**
     * @brief Returns a const iterator to the end of the container of this
     * component.
     *
     * @return an iterator pointing to the end of this container.
     */
    ConstVertexIterator vertexEnd() const { return Base::elementEnd(); }

    /**
     * @brief Returns an iterator to the first vertex index in the container of
     * this component.
     *
     * @return an iterator pointing to the begin of the vertex indices.
     */
    ConstVertexIndexIterator vertexIndexBegin() const
    {
        return Base::elementIndexBegin();
    }

    /**
     * @brief Returns an iterator to the end of the container of this component.
     *
     * @return an iterator pointing to the end of the vertex indices.
     */
    ConstVertexIndexIterator vertexIndexEnd() const
    {
        return Base::elementIndexEnd();
    }

    /**
     * @brief Returns a lightweight view object that stores the begin and
     * end iterators of the container of vertices of the element. The view
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
    View<VertexIterator> vertices() { return Base::elements(); }

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
    View<ConstVertexIterator> vertices() const { return Base::elements(); }

    /**
     * @brief Returns a lightweight view object that stores the begin and end
     * iterators of the container of vertex indices of the element. The view
     * object exposes the iterators trough the `begin()` and `end()` member
     * functions, and therefore the returned object can be used in range-based
     * for loops:
     *
     * @code{.cpp}
     * for (uint vid : el.vertexIndices()) {
     *     // Do something with vertex index...
     * }
     * @endcode
     *
     * @return a lightweight view object that can be used in range-based for
     * loops to iterate over vertex indices.
     */
    View<ConstVertexIndexIterator> vertexIndices() const
    {
        return Base::elementIndices();
    }

protected:
    // Component interface functions
    template<typename Element>
    void importFrom(const Element& e, bool importRefs = true)
    {
        if (importRefs) {
            if constexpr (HasVertexReferences<Element>) {
                if constexpr (N > 0) {
                    // same size non-polygonal faces
                    if constexpr (N == Element::VERTEX_NUMBER) {
                        importIndicesFrom(e);
                    }
                    // from polygonal to fixed size, but the polygon size == the
                    // fixed face size
                    else if constexpr (Element::VERTEX_NUMBER < 0) {
                        if (e.vertexNumber() == N) {
                            importIndicesFrom(e);
                        }
                    }
                    else {
                        // do not import in this case: cannot import from a face
                        // of different size
                    }
                }
                else {
                    // from fixed to polygonal size: need to resize first, then
                    // import
                    resizeVertices(e.vertexNumber());
                    importIndicesFrom(e);
                }
            }
        }
    }

    void serialize(std::ostream& os) const
    {
        // regardless of the type of the container (indices or pointers), the
        // serialization is always done using the indices
        if constexpr (N < 0) {
            vcl::serialize(os, vertexNumber());
        }
        for (uint i = 0; i < vertexNumber(); ++i) {
            vcl::serialize(os, vertexIndex(i));
        }
    }

    void deserialize(std::istream& is)
    {
        if constexpr (N < 0) {
            uint n;
            vcl::deserialize(is, n);
            resizeVertices(n);
        }
        for (uint i = 0; i < vertexNumber(); ++i) {
            uint vi;
            vcl::deserialize(is, vi);
            setVertex(i, vi);
        }
    }

private:
    template<typename Element>
    void importIndicesFrom(const Element& e)
    {
        for (uint i = 0; i < e.vertexNumber(); ++i) {
            setVertex(i, e.vertexIndex(i));
        }
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_VERTEX_POINTERS_H
