// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_VERTEX_CONTAINER_H
#define VCL_MESH_CONTAINERS_VERTEX_CONTAINER_H

#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/containers/base/element_container.h>
#include <vclib/mesh/containers/base/macros.h>
#include <vclib/mesh/elements/vertex.h>
#include <vclib/mesh/elements/vertex_components.h>

#include <vclib/base.h>

namespace vcl::mesh {

/**
 * @brief VertexContainer stores the per-vertex data in a Structure of Arrays
 * (SoA), and it is meant to be used as a template parameter of the Mesh class.
 *
 * It accepts the Vertex component list (satisfying
 * HasRequiredVertexComponents), instantiates the underlying storage and exposes
 * the Vertex element view.
 *
 * @tparam CompList: The vertex components type (e.g. VertexComponents<BitFlags,
 * Position3f, Normal3f>).
 * @tparam Index: The index type (default uint).
 * @tparam ParentMesh: The parent mesh type. On Mesh instantiation, this is
 * automatically set to the Mesh type. Default is void.
 *
 * @ingroup containers
 */
template<
    HasRequiredVertexComponents CompList,
    std::unsigned_integral Index = uint,
    typename ParentMesh          = void>
class VertexContainer :
    public ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Vertex>
{
    using Base = ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Vertex>;

public:
    using Vertex           = typename Base::ElementType;
    using ConstVertex      = typename Base::ConstElementType;
    using VertexComponents = typename Base::Components;
    using PositionType     = typename Vertex::PositionType;
    using Base::NULL_INDEX;

    // Rebind idiom to allow changing the ParentMesh type while keeping the same
    // component list and index type.
    template<typename NewParent>
    using Rebind = VertexContainer<CompList, Index, NewParent>;

    /**
     * @brief Empty constructor that creates an empty container of Vertices.
     */
    VertexContainer() = default;

    friend void swap(VertexContainer& a, VertexContainer& b) noexcept
    {
        a.swap(b);
    }

    /**
     * @brief Returns the Vertex at the i-th position in the Vertex Container of
     * the Mesh, which will be the vertex having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * vertexContainerSize(), this function will return a valid Vertex
     * (note that the Vertex may have been flagged as deleted).
     *
     * @param[in] i: the index of the vertex that will be returned.
     *
     * @return The i-th Vertex of the Vertex Container.
     */
    Vertex vertex(Index i) { return Base::element(i); }

    /**
     * @brief Returns the const Vertex at the i-th position in the Vertex
     * Container of the Mesh, which will be the vertex having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * vertexContainerSize(), this function will return a valid ConstVertex
     * (note that the Vertex may have been flagged as deleted).
     *
     * @param[in] i: the index of the vertex that will be returned.
     *
     * @return The i-th const Vertex of the Vertex Container.
     */
    ConstVertex vertex(Index i) const { return Base::element(i); }

    /**
     * @brief Returns the number of **non-deleted** vertices contained in the
     * Vertex container of the Mesh.
     *
     * If vertexCount() != vertexContainerSize(), it means that there are some
     * vertices that are flagged as deleted.
     *
     * @return The number of non-deleted vertices of the Mesh.
     */
    std::size_t vertexCount() const { return Base::elementCount(); }

    /**
     * @brief Returns the number of vertices (also deleted) contained in the
     * Vertex container of the Mesh.
     *
     * If vertexCount() != vertexContainerSize(), it means that there are some
     * vertices that are flagged as deleted.
     *
     * @return The number of all the vertices contained in the Mesh.
     */
    std::size_t vertexContainerSize() const { return Base::elementContainerSize(); }

    /**
     * @brief Returns the number of deleted vertices in the Vertex container,
     * that is vertexContainerSize() - vertexCount().
     *
     * @return The number of deleted vertices in the container.
     */
    std::size_t deletedVertexCount() const { return Base::deletedElementCount(); }

    /**
     * @brief Adds a new vertex into the Vertex container, returning the index of
     * the added vertex.
     *
     * @return The index of the new vertex.
     */
    Index addVertex() { return Base::addElement(); }

    /**
     * @brief Adds a new vertex with the given position into the vertex
     * container, returning the index of the added vertex.
     *
     * @param[in] p: position of the new vertex.
     * @return The index of the new vertex.
     */
    Index addVertex(const PositionType& p)
    {
        Index vid              = addVertex();
        vertex(vid).position() = p;
        return vid;
    }

    /**
     * @brief Adds an arbitrary number of n vertices, returning the index of the
     * first added vertex.
     *
     * @param[in] n: the number of vertices to add to the mesh.
     * @return The index of the first added vertex.
     */
    Index addVertices(std::size_t n) { return Base::addElements(n); }

    /**
     * @brief Adds an arbitrary number of vertices with the given positions,
     * returning the index of the first added vertex.
     *
     * You can call this member function like:
     *
     * @code{.cpp}
     * PositionType p0, p1, p2, p3;
     * // init positions...
     * m.addVertices(p0, p1, p2, p3);
     * @endcode
     *
     * The number of accepted PositionType arguments is variable.
     *
     * @param[in] p: first vertex position.
     * @param[in] v: list of other vertex positions.
     * @return The index of the first added vertex.
     */
    template<typename... VC>
    Index addVertices(const PositionType& p, const VC&... v)
        requires (std::is_constructible_v<PositionType, VC> && ...)
    {
        Index vid = vertexContainerSize();
        reserveVertices(vid + sizeof...(VC) + 1);
        addVertex(p);
        (addVertex(v), ...);
        return vid;
    }

    /**
     * @brief Adds an arbitrary number of vertices with the positions contained
     * in the given range, returning the index of the first added vertex.
     *
     * @param[in] range: the range of positions of the vertices to add.
     * @return The index of the first added vertex.
     */
    template<InputRange<PositionType> R>
    Index addVertices(R&& range)
    {
        Index vid = vertexContainerSize();
        if constexpr (std::ranges::sized_range<R>) {
            reserveVertices(vid + std::ranges::size(range));
        }
        for (const auto& p : range) {
            addVertex(p);
        }
        return vid;
    }

    /**
     * @brief Clears the Vertex container of the Mesh, deleting all the
     * vertices.
     *
     * The contained vertex components are actually removed from the container,
     * not only marked as deleted. Therefore, the container will have size 0
     * (`vertexContainerSize() == 0`) after the call of this function.
     *
     * @warning Any index or reference to vertices in the Mesh will be left
     * unchanged, and therefore will point to invalid vertices.
     */
    void clearVertices() { Base::clearElements(); }

    /**
     * @brief Resizes the Vertex container to contain `n` vertices.
     *
     * If the new size is greater than the old one, new vertices are added to
     * the container. If the new size is smaller than the old one, the container
     * is truncated to `n` vertices (vertices beyond `n` are removed).
     *
     * @warning Any index to removed vertices in the Mesh will be left
     * unchanged, and therefore will point to invalid vertices.
     *
     * @param[in] n: the new size of the Vertex container.
     */
    void resizeVertices(std::size_t n) { Base::resizeElements(n); }

    /**
     * @brief Reserves a number of vertices in the container of Vertices. This is
     * useful when you know (or you have an idea) of how many vertices will be
     * added to a new or existing mesh. Calling this function before any
     * addVertex() call will avoid unnecessary reallocations of the container,
     * saving execution time.
     *
     * The philosophy of this function is similar to the one of the
     * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
     * function of the [std::vector
     * class](https://en.cppreference.com/w/cpp/container/vector).
     *
     * @param[in] n: the new capacity of the vertex container.
     */
    void reserveVertices(std::size_t n) { Base::reserveElements(n); }

    /**
     * @brief Compacts the Vertex Container, removing all the vertices marked as
     * deleted. Vertex references will automatically change accordingly in all
     * the containers of the Mesh.
     *
     * @return A vector that maps each old vertex index to its new index in the
     * compacted container (or NULL_INDEX if it was deleted).
     */
    std::vector<Index> compactVertices() { return Base::compactElements(); }

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
     * @param[in] i: the index of the vertex that will be marked as deleted.
     */
    void deleteVertex(Index i) { Base::deleteElement(i); }

    /**
     * @brief Marks as deleted the given vertex.
     *
     * This member function does not perform any reallocation of the vertices:
     * the deleted vertices will stay in the Vertex Container, but will be
     * marked as deleted.
     *
     * Deleted vertices are automatically jumped by the iterators provided by
     * the Vertex Container.
     *
     * @param[in] v: the vertex that will be marked as deleted.
     */
    void deleteVertex(const Vertex& v) { Base::deleteElement(v.index()); }

    /**
     * @brief This is a utility member function that returns the index of an
     * element if the container were compact, that is the number of
     * non-deleted elements before the vertex with the given index.
     *
     * Complexity: O(n), with n the number of vertices in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of a vertex of the container.
     * @return The index that the vertex with index i would have if this
     * container were compact.
     */
    Index vertexIndexIfCompact(Index i) const
    {
        return Base::elementIndexIfCompact(i);
    }

    /**
     * @brief Returns a vector that tells, for each actual vertex index, the new
     * index that the vertex would have in a compacted container. For each
     * deleted vertex index, the value of the vector will be NULL_INDEX.
     *
     * This is useful if you need to know the indices of the vertices that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each vertex index, its index if the
     * container were compact.
     */
    std::vector<Index> vertexCompactIndices() const
    {
        return Base::elementCompactIndices();
    }

    /**
     * @brief Updates references to the vertices of this mesh according to the
     * mapping stored in the newIndices vector.
     *
     * This function is useful when vertices are deleted, compacted, or
     * permuted, and you want to update all references stored across the
     * mesh containers to point to their new indices.
     *
     * @note This function does not change the position of the vertices in this
     * container. It updates vertex references stored in this and other
     * containers of the mesh.
     *
     * @param[in] newIndices: A vector mapping old vertex indices to new ones.
     * If a vertex was deleted or should become unreferenced, its mapping entry
     * must be NULL_INDEX.
     */
    void updateVertexReferences(const std::vector<Index>& newIndices)
    {
        Base::updateElementReferences(newIndices);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate over
     * the vertices of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * vertices using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (Vertex v : m.vertices()) {
     *     // do something with this vertex
     * }
     * @endcode
     *
     * The iterator used to iterate over vertices is automatically initialized
     * to jump deleted vertices of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted vertices.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto vertices(bool jumpDeleted = true)
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate over
     * the vertices of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * vertices using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (ConstVertex v : m.vertices()) {
     *     // do something with this vertex
     * }
     * @endcode
     *
     * The iterator used to iterate over vertices is automatically initialized
     * to jump deleted vertices of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted vertices.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto vertices(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the vertices of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the vertices() function, this member function does not
     * automatically jump deleted vertices, but it iterates over the
     * vertices in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first vertex to be included in the
     * range. It must be less than or equal to vertexContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last vertex to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * vertexContainerSize(), it will be set to vertexContainerSize().
     * @return A View object allowing to iterate over the given range of the container.
     */
    auto vertices(Index begin, Index end = NULL_INDEX)
    {
        return Base::elements(begin, end);
    }

    /**
     * @brief Returns a view object that allows to iterate over the vertices of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the vertices() function, this member function does not
     * automatically jump deleted vertices, but it iterates over the
     * vertices in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first vertex to be included in the
     * range. It must be less than or equal to vertexContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last vertex to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * vertexContainerSize(), it will be set to vertexContainerSize().
     * @return A View object allowing to iterate over the given range of the container.
     */
    auto vertices(Index begin, Index end = NULL_INDEX) const
    {
        return Base::elements(begin, end);
    }

    // --- Components ---

    /**
     * @brief Checks if the vertex component with the given COMP_ID is
     * available (present at compile time and enabled at run time).
     *
     * Non-optional components are always available (true).
     * Optional components return true if currently enabled, false otherwise.
     *
     * @tparam COMP_ID: The ID of the component to check.
     * @return `true` if currently available, `false` otherwise.
     */
    template<CompId COMP_ID>
    bool isVertexComponentAvailable() const
    {
        return Base::template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Enables the optional vertex component with the given COMP_ID.
     *
     * @tparam COMP_ID: The ID of the component to enable.
     */
    template<CompId COMP_ID>
    void enableVertexComponent()
        requires comp::HasOptionalComponent<COMP_ID, VertexComponents>
    {
        Base::template enableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Disables the optional vertex component with the given COMP_ID.
     * Note: Non-optional components cannot be disabled.
     *
     * @tparam COMP_ID: The ID of the component to disable.
     */
    template<CompId COMP_ID>
    void disableVertexComponent()
        requires comp::HasOptionalComponent<COMP_ID, VertexComponents>
    {
        Base::template disableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Enables all optional vertex components in the container.
     */
    void enableAllVertexOptionalComponents()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all optional vertex components in the container.
     */
    void disableAllVertexOptionalComponents()
    {
        Base::disableAllOptionalComponents();
    }

    /**
     * @brief Enables all optional vertex components in the container.
     * Alias for enableAllVertexOptionalComponents().
     */
    void enableAllVertexComponents()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all optional vertex components in the container.
     * Alias for disableAllVertexOptionalComponents().
     */
    void disableAllVertexComponents()
    {
        Base::disableAllOptionalComponents();
    }

    VCL_COMMON_CONTAINER_COMPONENT_METHODS(Vertex)
    VCL_CONTAINER_COMPONENT_METHODS(Vertex, AdjacentEdges, ADJACENT_EDGES)
    VCL_CONTAINER_COMPONENT_METHODS(Vertex, AdjacentFaces, ADJACENT_FACES)
    VCL_CONTAINER_COMPONENT_METHODS(Vertex, AdjacentVertices, ADJACENT_VERTICES)
    VCL_CONTAINER_COMPONENT_METHODS(Vertex, MaterialIndex, MATERIAL_INDEX)
    VCL_CONTAINER_COMPONENT_METHODS(
        Vertex,
        PrincipalCurvature,
        PRINCIPAL_CURVATURE)
    VCL_CONTAINER_COMPONENT_METHODS(Vertex, Tangent, TANGENT)
    VCL_CONTAINER_COMPONENT_METHODS(Vertex, TexCoord, TEX_COORD)

    // --- Custom Components ---

    /**
     * @brief Checks if Vertices have a custom component with the given name.
     *
     * This function does not take into account the type of the custom
     * component.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component to check.
     * @return `true` if the Vertex Element has a custom component with the
     * given name.
     */
    bool hasVertexCustomComponent(const std::string& name) const
        requires vert::HasCustomComponents<VertexComponents>
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
    std::vector<std::string> vertexCustomComponentNames() const
        requires vert::HasCustomComponents<VertexComponents>
    {
        return Base::elemCustomComponentNames();
    }

    /**
     * @brief Checks if the custom component of the Vertex Element having the
     * given name has the same type of the given template argument type of this
     * function.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component to check.
     * @param[in] name: The name of the custom component to check.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return `true` if the custom component is of the same type of the
     * template argument.
     */
    template<typename K>
    bool isVertexCustomComponentOfType(const std::string& name) const
        requires vert::HasCustomComponents<VertexComponents>
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
     * @param[in] name: The name of the custom component to get the
     * std::type_index from.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return The std::type_index of the custom component having the given
     * input name.
     */
    std::type_index vertexCustomComponentType(const std::string& name) const
        requires vert::HasCustomComponents<VertexComponents>
    {
        return Base::elemCustomComponentType(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * associated to the Vertex Element having the same type of the given
     * template argument type of this function.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component names.
     * @return A vector of strings representing the names of the custom
     * components of a given type.
     */
    template<typename K>
    std::vector<std::string> vertexCustomComponentNamesOfType() const
        requires vert::HasCustomComponents<VertexComponents>
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
     * @tparam K: The type of the custom component added to the Vertex.
     * @param[in] name: The name of the custom component added to the Vertex.
     */
    template<typename K>
    void addVertexCustomComponent(const std::string& name)
        requires vert::HasCustomComponents<VertexComponents>
    {
        Base::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component of the given name from the Vertex
     * Element.
     *
     * The function does nothing if the custom component does not exist.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component that will be removed
     * from the Vertex.
     */
    void deleteVertexCustomComponent(const std::string& name)
        requires vert::HasCustomComponents<VertexComponents>
    {
        Base::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to the custom component having the type K
     * and the given name.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the handle.
     * @param[in] name: Name of the custom component for which to return the handle.
     * @return A vector handle that allows access to the custom component.
     */
    template<typename K>
    CustomComponentVectorHandle<K> vertexCustomComponentVectorHandle(
        const std::string& name)
        requires vert::HasCustomComponents<VertexComponents>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to the custom component having type
     * K and the given name.
     *
     * @note This function is available only if the Vertex Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the handle.
     * @param[in] name: Name of the custom component for which to return the handle.
     * @return A const vector handle that allows access to the custom
     * component.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> vertexCustomComponentVectorHandle(
        const std::string& name) const
        requires vert::HasCustomComponents<VertexComponents>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Serializes all per-vertex custom components of type K stored in
     * this container to the given output stream.
     *
     * @tparam K: The type of the custom components to serialize.
     * @param os: The output stream to write to.
     */
    template<typename K>
    void serializeVertexCustomComponentsOfType(std::ostream& os) const
        requires vert::HasCustomComponents<VertexComponents>
    {
        Base::template serializeElemCustomComponentsOfType<K>(os);
    }

    /**
     * @brief Deserializes all per-vertex custom components of type K from the
     * given input stream and stores them into this container.
     *
     * @tparam K: The type of the custom components to deserialize.
     * @param is: The input stream to read from.
     */
    template<typename K>
    void deserializeVertexCustomComponentsOfType(std::istream& is)
        requires vert::HasCustomComponents<VertexComponents>
    {
        Base::template deserializeElemCustomComponentsOfType<K>(is);
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_VERTEX_CONTAINER_H
