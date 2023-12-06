/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
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

#ifndef VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
#define VCL_MESH_CONTAINER_VERTEX_CONTAINER_H

#include <typeindex>
#include <vclib/mesh/elements/vertex.h>
#include <vclib/mesh/elements/vertex_components.h>

#include "element_container.h"

namespace vcl::mesh {

/**
 * @brief The Vertex Container class, will be used when the template argument
 * given to the Mesh is a Vertex.
 *
 * This class adds a container (vector) of vertices to the Mesh, making
 * available the accessors members to the vertices, the vertex number,
 * iterators... This class will also take care to add enablers/disablers of the
 * eventual optional components of the vertex.
 *
 * This container can be templated on a type that satisfies the VertexConcept
 * concept.
 *
 * @ingroup containers
 */
template<vcl::VertexConcept T>
class VertexContainer : public ElementContainer<T>
{
    template<VertexConcept U>
    friend class VertexContainer;

    using VertexContainerType = VertexContainer<T>;
    using Base                = ElementContainer<T>;

public:
    using Vertex              = T;
    using VertexType          = T;
    using VertexIterator      = Base::ElementIterator;
    using ConstVertexIterator = Base::ConstElementIterator;

    /**
     * @brief Empty constructor that creates an empty container of Vertices.
     */
    VertexContainer() = default;

    /**
     * @brief Returns a const reference of the vertex at the i-th position in
     * the Vertex Container of the Mesh, which will be the vertex having index =
     * i.
     *
     * This function does not perform any sanity check: if i is less than
     * vertexContainerSize(), this function will return a valid Vertex reference
     * (note that the Vertex may have been flagged as deleted).
     *
     * @param[in] i: the index of the vertex that will be returned.
     */
    const VertexType& vertex(uint i) const { return Base::element(i); }

    /**
     * @brief Returns a reference of the vertex at the i-th position in the
     * Vertex Container of the Mesh, which will be the vertex having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * vertexContainerSize(), this function will return a valid Vertex reference
     * (note that the Vertex may have been flagged as deleted).
     *
     * @param[in] i: the index of the vertex that will be returned.
     */
    VertexType& vertex(uint i) { return Base::element(i); }

    /**
     * @brief Returns the number of **non-deleted** vertices contained in the
     * Vertex container of the Mesh.
     *
     * If vertexNumber() != vertexContainerSize(), it means that there are some
     * vertices that are flagged as deleted.
     *
     * @return The number of non-deleted vertices of the Mesh.
     */
    uint vertexNumber() const { return Base::elementNumber(); }

    /**
     * @brief Returns the number of vertices (also deleted) contained in the
     * Vertex container of the Mesh.
     *
     * If vertexNumber() != vertexContainerSize(), it means that there are some
     * vertices that are flagged as deleted.
     *
     * @return The number of all the vertices contained in the Mesh.
     */
    uint vertexContainerSize() const { return Base::elementContainerSize(); }

    /**
     * @brief Returns the number of deleted vertices in the Vertex container,
     * that is vertexContainerSize() - vertexNumber().
     *
     * @return The number of deleted vertices in the container.
     */
    uint deletedVertexNumber() const { return Base::deletedElementNumber(); }

    /**
     * @brief Add a new vertex into the vertex container, returning the index of
     * the added vertex.
     *
     * If the call of this function will cause a reallocation of the Vertex
     * container, the function will automatically take care of updating all the
     * Vertex pointers contained in the Mesh.
     *
     * @return the index of the new vertex.
     */
    uint addVertex() { return Base::addElement(); }

    /**
     * @brief Add a new vertex with the given coordinate into the vertex
     * container, returning the id of the added vertex.
     *
     * If the call of this function will cause a reallocation of the Vertex
     * container, the function will automatically take care of updating all the
     * Vertex pointers contained in the Mesh.
     *
     * @param p: coordinate of the new vertex.
     * @return the id of the new vertex.
     */
    uint addVertex(const typename T::CoordType& p)
    {
        uint vid            = addVertex();
        vertex(vid).coord() = p; // set the coordinate to the vertex
        return vid;
    }

    /**
     * @brief Add an arbitrary number of n vertices, returning the id of the
     * first added vertex.
     *
     * This means that, if you want to add 5 vertices and this member function
     * returns 4, the added vertices will have id from 4 to id 8 included.
     *
     * If the call of this function will cause a reallocation of the Vertex
     * container, the function will automatically take care of updating all the
     * Vertex pointers contained in the Mesh.
     *
     * @param n: the number of vertices to add to the mesh.
     * @return the id of the first added vertex.
     */
    uint addVertices(uint n) { return Base::addElements(n); }

    /**
     * @brief Add an arbitrary number of vertices with the given coordinates,
     * returning the id of the first added vertex.
     *
     * You can call this member function like:
     *
     * @code{.cpp}
     * CoordType p0, p1, p2, p3;
     * // init coords...
     * m.addVertices(p0, p1, p2, p3);
     * @endcode
     *
     * The number of accepted Coordtype arguments is variable.
     *
     * If the call of this function will cause a reallocation of the Vertex
     * container, the function will automatically take care of updating all the
     * Vertex pointers contained in the Mesh.
     *
     * @param p: first vertex coordinate
     * @param v: list of other vertex coordinates
     * @return the id of the first added vertex.
     */
    template<typename... VC>
    uint addVertices(const typename T::CoordType& p, const VC&... v)
    {
        uint vid = vertexContainerSize();
        // reserve the new number of vertices
        reserveVertices(vid + sizeof...(VC) + 1);
        addVertex(p);
        // pack expansion: will be translated at compile time as an addVertex()
        // call for each argument of the addVertices member function
        (addVertex(v), ...);
        return vid;
    }

    /**
     * @brief Clears the Vertex container of the Mesh, deleting all the
     * vertices.
     *
     * The contained vertices are actually removed from the container, not only
     * marked as deleted. Therefore, the container will have size 0
     * (`vertexContainerSize() == 0`) after the call of this function.
     *
     * @note This function does not cause a reallocation of the Vertex
     * container.
     *
     * @warning Any pointer to vertices in the Mesh will be left unchanged, and
     * therefore will point to invalid vertices. This means that, if you have a
     * pointer to a vertex and you call this function, you will have a dangling
     * pointer.
     */
    void clearVertices() { Base::clearElements(); }

    /**
     * @brief Resizes the Vertex container to contain `n` vertices.
     *
     * If the new size is greater than the old one, new vertices are added to
     * the container, and a reallocation may happen. If the new size is smaller
     * than the old one, the container will keep its first non-deleted `n`
     * vertices, and the remaining vertices are marked as deleted.
     *
     * If the call of this function will cause a reallocation of the Vertex
     * container, the function will automatically take care of updating all the
     * Vertex pointers contained in the Mesh.
     *
     * @warning The given size `n` is relative to the number of non-deleted
     * vertices, not to the size of the vertex container. For example, if you
     * have a mesh with 10 vertices and vertexContainerSize() == 20, calling
     * resizeVertices(5) will not cause a reallocation of the container, but
     * will mark as deleted the least 5 non-deleted vertices of the container.
     * In the same scenario, calling resizeVertices(15) will result in a vertex
     * container having 15 vertices and vertexContainerSize() == 25. The latest
     * 5 vertices will be the newly added.
     *
     * @warning Any pointer to deleted vertices in the Mesh will be left
     * unchanged, and therefore will point to invalid vertices. This means that
     * if you call this member function with a lower number of vertices, you'll
     * need to manually manage the pointers to the deleted vertices.
     *
     * @param[in] n: the new size of the Vertex container.
     */
    void resizeVertices(uint n) { Base::resizeElements(n); }

    /**
     * @brief Reserve a number of vertices in the container of Vertices. This is
     * useful when you know (or you have an idea) of how much vertices are going
     * to add into a newly or existing mesh. Calling this function before any
     * add_vertex() call will avoid unuseful reallocations of the container,
     * saving execution time.
     *
     * The filosofy of this function is similar to the one of the
     * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
     * function of the [std::vector
     * class](https://en.cppreference.com/w/cpp/container/vector).
     *
     * If the call of this function will cause a reallocation of the Vertex
     * container, the function will automatically take care of updating all the
     * Vertex pointers contained in the Mesh.
     *
     * @param n: the new capacity of the vertex container.
     */
    void reserveVertices(uint n) { Base::reserveElements(n); }

    /**
     * @brief Compacts the Vertex Container, removing all the vertices marked as
     * deleted. Vertices indices will change accordingly. The function will
     * automatically take care of updating all the Vertex pointers contained in
     * the Mesh.
     */
    void compactVertices() { Base::compactElements(); }

    /**
     * @brief Marks as deleted the vertex with the given id.
     *
     * This member function does not perform any reallocation of the vertices:
     * the deleted vertices will stay in the Vertex Container, but will be
     * marked as deleted.
     *
     * Deleted vertices are automatically jumped by the iterators provided by
     * the Vertex Container.
     *
     * @param[in] i: the id of the vertex that will be marked as deleted.
     */
    void deleteVertex(uint i) { Base::deleteElement(i); }

    /**
     * @brief Marks as deleted the given vertex, before asserting that the
     * vertex belongs to this container.
     *
     * This member function does not perform any reallocation of the vertices:
     * the deleted vertices will stay in the Vertex Container, but will be
     * marked as deleted.
     *
     * Deleted vertices are automatically jumped by the iterators provided by
     * the Vertex Container.
     *
     * @param[in] v: the pointer of the vertex that will be marked as deleted.
     */
    void deleteVertex(const VertexType* v) { Base::deleteElement(v); }

    /**
     * @brief This is an utility member function that returns the index of an
     * element if the container would be compact, that is the number of
     * non-deleted elements before the vertex with the given index.
     *
     * Complexity: O(n), with n the number of vertices in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of a vertex of the container.
     * @return The index that the vertex with index i would have if this
     * container would be compact.
     */
    uint vertexIndexIfCompact(uint i) const
    {
        return Base::elementIndexIfCompact(i);
    }

    /**
     * @brief Returns a vector that tells, for each actual vertex index, the new
     * index that the vertex would have in a compacted container. For each
     * deleted vertex index, the value of the vector will be UINT_NULL.
     *
     * This is useful if you need to know the indices of the vertices that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each vertex index, its index if the
     * container would be compact.
     */
    std::vector<uint> vertexCompactIndices() const
    {
        return Base::elementCompactIndices();
    }

    /**
     * @brief Updates all the indices and pointers of the vertices of this
     * container that are stored in any container of the mesh, according to the
     * mapping stored in the newIndices vector, that tells for each old vertex
     * index, the new vertex index.
     *
     * This function is useful when some vertices, and you want to update the
     * indices/pointers stored in all the containers of the mesh accordingly.
     *
     * E.g. Supposing you deleted a set of vertices, you can give to this
     * function the vector telling, for each of the old vertex indices, the new
     * vertex index (or UINT_NULL if you want to leave it unreferenced). This
     * function will update all the pointers stored in the mesh containers
     * accordingly (if they store adjacencies to the vertices).
     *
     * @note This function *does not change the position of the vertices in this
     * container*. It just updates the indices/pointers of the vertices stored
     * in this or other containers.
     *
     * @param[in] newIndices: a vector that tells, for each old vertex index,
     * the new vertex index. If the old vertex must be left as unreferenced
     * (setting `nullptr` to the pointers), the value of the vector must be
     * UINT_NULL.
     */
    void updateVertexIndices(const std::vector<uint>& newIndices)
    {
        Base::updateElementIndices(newIndices);
    }

    /**
     * @brief Returns an iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted vertices of the
     * container. You can change this option by calling this function with
     * jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted vertices.
     * @return An iterator the the first vertex of the container.
     */
    VertexIterator vertexBegin(bool jumpDeleted = true)
    {
        return Base::elementBegin(jumpDeleted);
    }

    /**
     * @brief Returns an iterator to the end of the container.
     * @return An iterator to the end of the container.
     */
    VertexIterator vertexEnd() { return Base::elementEnd(); }

    /**
     * @brief Returns a const iterator to the beginning of the container.
     *
     * The iterator is automatically initialized to jump deleted vertices of the
     * container. You can change this option by calling this function with
     * jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted vertices.
     * @return A const iterator the the first vertex of the container.
     */
    ConstVertexIterator vertexBegin(bool jumpDeleted = true) const
    {
        return Base::elementBegin(jumpDeleted);
    }

    /**
     * @brief Returns a const iterator to the end of the container.
     * @return A const iterator to the end of the container.
     */
    ConstVertexIterator vertexEnd() const { return Base::elementEnd(); }

    /**
     * @brief Returns a small utility object that allows to iterate over the
     * vertices of the containers, providing two member functions begin() and
     * end().
     *
     * This member function is very useful when you want to iterate over the
     * vertices using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (Vertex& v : m.vertices()){
     *     // do something with this vertex
     * }
     * @endcode
     *
     * The iterator used to iterate over vertices is automatically initialized
     * to jump deleted vertices of the container. You can change this option by
     * calling this function with jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted vertices.
     * @return An object having begin() and end() function, allowing to iterate
     * over the container.
     */
    auto vertices(bool jumpDeleted = true)
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns a small utility object that allows to iterate over the
     * vertices of the containers, providing two member functions begin() and
     * end().
     *
     * This member function is very useful when you want to iterate over the
     * vertices using the C++ foreach syntax:
     *
     * @code{.cpp}
     * for (const Vertex& v : m.vertices()){
     *     // do something with this vertex
     * }
     * @endcode
     *
     * The iterator used to iterate over vertices is automatically initialized
     * to jump deleted vertices of the container. You can change this option by
     * calling this function with jumpDeleted=false.
     *
     * @param[in] jumpDeleted (def: true): boolean that tells if the iterator
     * should jump deleted vertices.
     * @return An object having begin() and end() function, allowing to iterate
     * over the container.
     */
    auto vertices(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Enables all the optional components associated to the Vertex type
     * contained in the VertexContainer.
     */
    void enableAllPerVertexOptionalComponents()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all the optional components associated to the Vertex type
     * contained in the VertexContainer.
     */
    void disableAllPerVertexOptionalComponents()
    {
        Base::disableAllOptionalComponents();
    }

    // Adjacent Edges

    /**
     * @brief Checks if the vertex Optional Adjacent Edges component is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentEdges Component.
     *
     * @return true if the Optional Adjacent Edges is enabled, false otherwise.
     */
    bool isPerVertexAdjacentEdgesEnabled() const
        requires vert::HasOptionalAdjacentEdges<T>
    {
        return Base::template isOptionalComponentEnabled<
            typename T::AdjacentEdges>();
    }

    /**
     * @brief Enables the Optional Adjacent Edges of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentEdges Component.
     */
    void enablePerVertexAdjacentEdges()
        requires vert::HasOptionalAdjacentEdges<T>
    {
        Base::template enableOptionalComponent<typename T::AdjacentEdges>();
    }

    /**
     * @brief Disables the Optional Adjacent Edges of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentEdges Component.
     */
    void disablePerVertexAdjacentEdges()
        requires vert::HasOptionalAdjacentEdges<T>
    {
        Base::template disableOptionalComponent<typename T::AdjacentEdges>();
    }

    // Adjacent Faces

    /**
     * @brief Checks if the vertex Optional Adjacent Faces component is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentFaces Component.
     *
     * @return true if the Optional Adjacent Faces is enabled, false otherwise.
     */
    bool isPerVertexAdjacentFacesEnabled() const
        requires vert::HasOptionalAdjacentFaces<T>
    {
        return Base::template isOptionalComponentEnabled<
            typename T::AdjacentFaces>();
    }

    /**
     * @brief Enables the Optional Adjacent Faces of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentFaces Component.
     */
    void enablePerVertexAdjacentFaces()
        requires vert::HasOptionalAdjacentFaces<T>
    {
        Base::template enableOptionalComponent<typename T::AdjacentFaces>();
    }

    /**
     * @brief Disables the Optional Adjacent Faces of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentFaces Component.
     */
    void disablePerVertexAdjacentFaces()
        requires vert::HasOptionalAdjacentFaces<T>
    {
        Base::template disableOptionalComponent<typename T::AdjacentFaces>();
    }

    // Adjacent Vertices

    /**
     * @brief Checks if the vertex Optional Adjacent Vertices component is
     * enabled.
     *
     * @note This function is available **only if the Vertex Element has the
     * OptionalAdjacentVertices Component.
     *
     * @return true if the Optional Adjacent Vertices is enabled, false
     * otherwise.
     */
    bool isPerVertexAdjacentVerticesEnabled() const
        requires vert::HasOptionalAdjacentVertices<T>
    {
        return Base::template isOptionalComponentEnabled<
            typename T::AdjacentVertices>();
    }

    /**
     * @brief Enables the Optional Adjacent Vertices of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentVertices Component.
     */
    void enablePerVertexAdjacentVertices()
        requires vert::HasOptionalAdjacentVertices<T>
    {
        Base::template enableOptionalComponent<typename T::AdjacentVertices>();
    }

    /**
     * @brief Disables the Optional Adjacent Vertices of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalAdjacentVertices Component.
     */
    void disablePerVertexAdjacentVertices()
        requires vert::HasOptionalAdjacentVertices<T>
    {
        Base::template disableOptionalComponent<typename T::AdjacentVertices>();
    }

    // Color

    /**
     * @brief Checks if the vertex Optional Color is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalColor Component.
     *
     * @return true if the Optional Color is enabled, false otherwise.
     */
    bool isPerVertexColorEnabled() const requires vert::HasOptionalColor<T>
    {
        return Base::template isOptionalComponentEnabled<typename T::Color>();
    }

    /**
     * @brief Enables the Optional Color of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalColor Component.
     */
    void enablePerVertexColor() requires vert::HasOptionalColor<T>
    {
        return Base::template enableOptionalComponent<typename T::Color>();
    }

    /**
     * @brief Disables the Optional Color of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalColor Component.
     */
    void disablePerVertexColor() requires vert::HasOptionalColor<T>
    {
        Base::template disableOptionalComponent<typename T::Color>();
    }

    // Mark

    /**
     * @brief Checks if the vertex Optional Mark is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalMark Component.
     *
     * @return true if the Optional Mark is enabled, false otherwise.
     */
    bool isPerVertexMarkEnabled() const requires vert::HasOptionalMark<T>
    {
        return Base::template isOptionalComponentEnabled<typename T::Mark>();
    }

    /**
     * @brief Enables the Optional Mark of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalMark Component.
     */
    void enablePerVertexMark() requires vert::HasOptionalMark<T>
    {
        Base::template enableOptionalComponent<typename T::Mark>();
    }

    /**
     * @brief Container::disableVertexMark disables the Optional Mark of the
     * vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalMark Component.
     */
    void disablePerVertexMark() requires vert::HasOptionalMark<T>
    {
        Base::template disableOptionalComponent<typename T::Mark>();
    }

    // Normal

    /**
     * @brief Checks if the vertex Optional Normal is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalNormal Component.
     *
     * @return true if the Optional Normal is enabled, false otherwise.
     */
    bool isPerVertexNormalEnabled() const requires vert::HasOptionalNormal<T>
    {
        return Base::template isOptionalComponentEnabled<typename T::Normal>();
    }

    /**
     * @brief Enables the Optional Normal of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalNormal Component.
     */
    void enablePerVertexNormal() requires vert::HasOptionalNormal<T>
    {
        Base::template enableOptionalComponent<typename T::Normal>();
    }

    /**
     * @brief Checks if the vertex Optional PrincipalCurvature is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalPrincipalCurvature Component.
     *
     * @return true if the Optional PrincipalCurvature is enabled, false
     * otherwise.
     */
    void disablePerVertexNormal() requires vert::HasOptionalNormal<T>
    {
        Base::template disableOptionalComponent<typename T::Normal>();
    }

    // PrincipalCurvature

    /**
     * @brief Checks if the vertex Optional PrincipalCurvature is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalPrincipalCurvature Component.
     *
     * @return true if the Optional PrincipalCurvature is enabled, false
     * otherwise.
     */
    bool isPerVertexPrincipalCurvatureEnabled() const
        requires vert::HasOptionalPrincipalCurvature<T>
    {
        return Base::template isOptionalComponentEnabled<
            typename T::PrincipalCurvature>();
    }

    /**
     * @brief Enables the Optional PrincipalCurvature of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalPrincipalCurvature Component.
     */
    void enablePerVertexPrincipalCurvature()
        requires vert::HasOptionalPrincipalCurvature<T>
    {
        Base::template enableOptionalComponent<
            typename T::PrincipalCurvature>();
    }

    /**
     * @brief Disables the Optional PrincipalCurvature of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalPrincipalCurvature Component.
     */
    void disablePerVertexPrincipalCurvature()
        requires vert::HasOptionalPrincipalCurvature<T>
    {
        Base::template disableOptionalComponent<
            typename T::PrincipalCurvature>();
    }

    // Quality

    /**
     * @brief Checks if the vertex Optional Quality is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalQuality Component.
     *
     * @return true if the Optional Quality is enabled, false otherwise.
     */
    bool isPerVertexQualityEnabled() const requires vert::HasOptionalQuality<T>
    {
        return Base::template isOptionalComponentEnabled<typename T::Quality>();
    }

    /**
     * @brief Enables the Optional Quality of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalQuality Component.
     */
    void enablePerVertexQuality() requires vert::HasOptionalQuality<T>
    {
        Base::template enableOptionalComponent<typename T::Quality>();
    }

    /**
     * @brief Disables the Optional Quality of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalQuality Component.
     */
    void disablePerVertexQuality() requires vert::HasOptionalQuality<T>
    {
        Base::template disableOptionalComponente<typename T::Quality>();
    }

    // TexCoord

    /**
     * @brief Checks if the vertex Optional TexCoord is enabled.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalTexCoord Component.
     *
     * @return true if the Optional TexCoord is enabled, false otherwise.
     */
    bool isPerVertexTexCoordEnabled() const
        requires vert::HasOptionalTexCoord<T>
    {
        return Base::template isOptionalComponentEnabled<
            typename T::TexCoord>();
    }

    /**
     * @brief Enables the Optional TexCoord of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalTexCoord Component.
     */
    void enablePerVertexTexCoord() requires vert::HasOptionalTexCoord<T>
    {
        Base::template enableOptionalComponent<typename T::TexCoord>();
    }

    /**
     * @brief Disables the Optional TexCoord of the vertex.
     *
     * @note This function is available only if the Vertex Element has the
     * OptionalTexCoord Component.
     */
    void disablePerVertexTexCoord() requires vert::HasOptionalTexCoord<T>
    {
        Base::template disableOptionalComponent<typename T::TexCoord>();
    }

    // Custom Components

    /**
     * @brief Checks if vertices have a custom component with the given name.
     *
     * This function does not take into account the type of the custom
     * component.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @return `true` if the Vertex Element has a custom component with the
     * given name.
     */
    bool hasPerVertexCustomComponent(const std::string& name) const
        requires vert::HasCustomComponents<T>
    {
        return Base::hasElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * of any type associated to the Vertex Element.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @return A vector of strings representing all the names of the custom
     * components.
     */
    std::vector<std::string> perVertexCustomComponentNames() const
        requires vert::HasCustomComponents<T>
    {
        return Base::elemCustomComponentNames();
    }

    /**
     * @brief Checks if the custom component of the Vertex Element having the
     * given name has the same type of the given template argument type of this
     * function.
     *
     * For example, the following code checks if the component called `cc` is of
     * type `double`:
     * @code{.cpp}
     * if (m.isPerVertexCustomComponentOfType<double>("cc")) {
     *   ...
     * }
     * @endcode
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @tparam K: the type of the custom component to check.
     * @param[in] name: the name of the custom component to check.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return `true` if the custom component is of the same type of the
     * template argument.
     */
    template<typename K>
    bool isPerVertexCustomComponentOfType(const std::string& name) const
        requires vert::HasCustomComponents<T>
    {
        return Base::template isElemCustomComponentOfType<K>(name);
    }

    /**
     * @brief Returns the std::type_index of the custom component of the Vertex
     * Element having the given input name.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @param[in] name: the name of the custom component to get the
     * std::type_index from.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     *
     * @return The std::type_index of the custom component having the given
     * input name.
     */
    std::type_index perVertexCustomComponentType(const std::string& name) const
        requires vert::HasCustomComponents<T>
    {
        return Base::elemComponentType(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * associated to the Vertex Element having the same type of the given
     * template argument type of this function.
     *
     * For example, the following code gets a vector containing all the custom
     * components of type `double`:
     * @code{.cpp}
     * std::vector<std::string> cdouble =
     * m.perVertexCustomComponentNamesOfType<double>();
     * @endcode
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @tparam K: the type of the custom component names.
     * @return A vector of strings representing the names of the custom
     * components of a given type.
     */
    template<typename K>
    std::vector<std::string> perVertexCustomComponentNamesOfType() const
        requires vert::HasCustomComponents<T>
    {
        return Base::template elemCustomComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a custom component of type K to the Vertex, having the given
     * name.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @tparam K: the type of the custom component added to the Vertex.
     * @param[in] name: the name of the custom component added to the Vertex.
     */
    template<typename K>
    void addPerVertexCustomComponent(const std::string& name)
        requires vert::HasCustomComponents<T>
    {
        Base::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component of the given name from the Vertex
     * Element.
     *
     * The function does nothing if the custom component does not exists.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @param[in] name: the name of the custom component that will be removed
     * from the Vertex.
     */
    void deletePerVertexCustomComponent(const std::string& name)
        requires vert::HasCustomComponents<T>
    {
        Base::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to the custom component having the type K
     * and the given name.
     *
     * The handle can be used like a normal std::vector, but does not have
     * access to the modifiers member functions (resize, push_back...). The
     * handle contains **references** to the custom component, therefore you can
     * modify the custom component by modifying the element of the handle vector
     * normally. Since the handle stores references, there are no copies
     * performed when calling this function.
     *
     * For example, assuming that the mesh has a vertex custom component named
     * "cc" of type int:
     *
     * @code{.cpp}
     * auto handle = m.perVertexCustomComponentVectorHandle<int>("cc");
     * for (Vertex& v : m.vertices() {
     *    handle[m.index(v)] = 5; // v.customComponent<int>("cc") == 5
     *    assert(v.customComponent<int>("cc") == 5);
     * }
     * @endcode
     *
     * Using handles allows to access more efficiently to custom components
     * rather accessing from an element object. However, note that references
     * are binded to the container of the mesh.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @note Since the handle contains references, any operation that changes
     * the size of the container could be destructive and invalidate the
     * references contained in the handle.
     *
     * @tparam K: the type of the custom component on which return the handle.
     *
     * @param[in] name: name of the custom component on which return the handle.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     *
     * @return a vector handle that allows to access to the custom component.
     */
    template<typename K>
    CustomComponentVectorHandle<K> perVertexCustomComponentVectorHandle(
        const std::string& name) requires vert::HasCustomComponents<T>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to the custom component having type
     * K and the given name.
     *
     * The handle can be used like a normal std::vector, but does not have
     * access to the modifiers member functions (resize, push_back...). The
     * handle contains **const references** to the custom component, therefore
     * you can access to the custom component by accessing the element of the
     * handle vector normally. Since the handle stores references, there are no
     * copies performed when calling this function.
     *
     * For example, assuming that the mesh has a vertex custom component named
     * "cc" of type int:
     *
     * @code{.cpp}
     * // access to the const handle by making const the template parameter:
     * auto handle = m.perVertexCustomComponentVectorHandle<const int>("cc");
     * int sum = 0;
     * for (const Vertex& v : m.vertices() {
     *    sum += handle[m.index(v)];
     *    // handle[m.index(v)] = 5; // not allowed, because the handle is const
     * }
     * @endcode
     *
     * Using handles allows to access more efficiently to custom components
     * rather accessing from an element object. However, note that references
     * are binded to the container of the mesh.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @note Since the handle contains references, any operation that changes
     * the size of the container could be destructive and invalidate the
     * references contained in the handle.
     *
     * @tparam K: the type of the custom component on which return the handle.
     *
     * @param[in] name: name of the custom component on which return the handle.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     *
     * @return a const vector handle that allows to access to the custom
     * component.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> perVertexCustomComponentVectorHandle(
        const std::string& name) const requires vert::HasCustomComponents<T>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINER_VERTEX_CONTAINER_H
