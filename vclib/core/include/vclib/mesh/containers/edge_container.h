// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_EDGE_CONTAINER_H
#define VCL_MESH_CONTAINERS_EDGE_CONTAINER_H

#include <vclib/mesh/components/base/component_predicates.h>
#include <vclib/mesh/containers/base/element_container.h>
#include <vclib/mesh/containers/base/macros.h>
#include <vclib/mesh/elements/edge.h>
#include <vclib/mesh/elements/edge_components.h>

#include <vclib/base.h>

namespace vcl::mesh {

/**
 * @brief EdgeContainer stores the per-edge data in a Structure of Arrays (SoA),
 * and it is meant to be used as a template parameter of the Mesh class.
 *
 * It accepts the Edge component list (satisfying HasRequiredEdgeComponents),
 * instantiates the underlying storage and exposes the Edge element view.
 *
 * @tparam CompList: The edge components type (e.g. EdgeComponents<BitFlags,
 * EdgeVertexReferences<uint>>).
 * @tparam Index: The index type (default uint).
 * @tparam ParentMesh: The parent mesh type. On Mesh instantiation, this is
 * automatically set to the Mesh type. Default is void.
 *
 * @ingroup containers
 */
template<
    HasRequiredEdgeComponents CompList,
    std::unsigned_integral    Index = uint,
    typename ParentMesh             = void>
class EdgeContainer :
    public ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Edge>
{
    using Base = ElementContainer<
        typename comp::detail::ComponentListHelper<CompList>::type,
        ParentMesh,
        Index,
        Edge>;

public:
    using Edge           = typename Base::ElementType;
    using ConstEdge      = typename Base::ConstElementType;
    using EdgeComponents = typename Base::Components;
    using Components     = EdgeComponents;
    using Base::NULL_INDEX;

    // Rebind idiom to allow changing the ParentMesh type while keeping the same
    // component list and index type.
    template<typename NewParent>
    using Rebind = EdgeContainer<CompList, Index, NewParent>;

    /**
     * @brief Empty constructor that creates an empty container of Edges.
     */
    EdgeContainer() = default;

    friend void swap(EdgeContainer& a, EdgeContainer& b) noexcept
    {
        a.swap(b);
    }

    /**
     * @brief Returns the Edge at the i-th position in the Edge Container of
     * the Mesh, which will be the edge having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * edgeContainerSize(), this function will return a valid Edge
     * (note that the Edge may have been flagged as deleted).
     *
     * @param[in] i: the index of the edge that will be returned.
     *
     * @return The i-th Edge of the Edge Container.
     */
    Edge edge(Index i) { return Base::element(i); }

    /**
     * @brief Returns the const Edge at the i-th position in the Edge
     * Container of the Mesh, which will be the edge having index = i.
     *
     * This function does not perform any sanity check: if i is less than
     * edgeContainerSize(), this function will return a valid ConstEdge
     * (note that the Edge may have been flagged as deleted).
     *
     * @param[in] i: the index of the edge that will be returned.
     *
     * @return The i-th const Edge of the Edge Container.
     */
    ConstEdge edge(Index i) const { return Base::element(i); }

    /**
     * @brief Returns the number of **non-deleted** edges contained in the
     * Edge container of the Mesh.
     *
     * If edgeCount() != edgeContainerSize(), it means that there are some
     * edges that are flagged as deleted.
     *
     * @return The number of non-deleted edges of the Mesh.
     */
    std::size_t edgeCount() const { return Base::elementCount(); }

    /**
     * @brief Returns the number of edges (also deleted) contained in the
     * Edge container of the Mesh.
     *
     * If edgeCount() != edgeContainerSize(), it means that there are some
     * edges that are flagged as deleted.
     *
     * @return The number of all the edges contained in the Mesh.
     */
    std::size_t edgeContainerSize() const
    {
        return Base::elementContainerSize();
    }

    /**
     * @brief Returns the number of deleted edges in the Edge container,
     * that is edgeContainerSize() - edgeCount().
     *
     * @return The number of deleted edges in the container.
     */
    std::size_t deletedEdgeCount() const { return Base::deletedElementCount(); }

    /**
     * @brief Adds a new edge into the Edge container, returning the index of
     * the added edge.
     *
     * @return The index of the new edge.
     */
    Index addEdge() { return Base::addElement(); }

    /**
     * @brief Adds a new edge with the given vertices or vertex indices into the
     * Edge container, returning the index of the added edge.
     *
     * @tparam Args: The types of the vertex arguments (references or indices).
     * @param[in] args: The vertices or vertex indices of the new edge.
     * @return The index of the new edge.
     */
    template<typename... Args>
    Index addEdge(const Args&... args) requires (sizeof...(Args) == 2)
    {
        Index eid = addEdge();
        edge(eid).setVertices(args...);
        return eid;
    }

    /**
     * @brief Adds a new edge with the vertices or vertex indices contained in
     * the given range, returning the index of the added edge.
     *
     * @tparam Rng: An input range type containing vertices or vertex indices.
     * @param[in] rng: The range of vertices or vertex indices of the new edge.
     * @return The index of the new edge.
     */
    template<std::ranges::input_range Rng>
    Index addEdge(Rng&& rng)
    {
        Index eid = addEdge();
        edge(eid).setVertices(std::forward<Rng>(rng));
        return eid;
    }

    /**
     * @brief Adds an arbitrary number of n edges, returning the index of the
     * first added edge.
     *
     * @param[in] n: the number of edges to add to the mesh.
     * @return The index of the first added edge.
     */
    Index addEdges(std::size_t n) { return Base::addElements(n); }

    /**
     * @brief Adds an arbitrary number of edges with the vertices or vertex
     * indices contained in the given range of ranges, returning the index of
     * the first added edge.
     *
     * @tparam Rng: A range of ranges containing vertices or vertex indices.
     * @param[in] rng: A range of ranges containing the vertices or vertex
     * indices for each edge.
     * @return The index of the first added edge.
     */
    template<std::ranges::input_range Rng>
    requires std::ranges::input_range<std::ranges::range_value_t<Rng>>
    Index addEdges(Rng&& rng)
    {
        Index baseId = edgeContainerSize();
        if constexpr (std::ranges::sized_range<Rng>) {
            reserveEdges(baseId + std::ranges::size(rng));
        }
        for (auto&& e : rng) {
            addEdge(e);
        }
        return baseId;
    }

    /**
     * @brief Clears the Edge container of the Mesh, deleting all the
     * edges.
     *
     * The contained edge components are actually removed from the container,
     * not only marked as deleted. Therefore, the container will have size 0
     * (`edgeContainerSize() == 0`) after the call of this function.
     *
     * @warning Any index or reference to edges in the Mesh will be left
     * unchanged, and therefore will point to invalid edges.
     */
    void clearEdges() { Base::clearElements(); }

    /**
     * @brief Resizes the Edge container to contain `n` edges.
     *
     * If the new size is greater than the old one, new edges are added to
     * the container. If the new size is smaller than the old one, the container
     * is truncated to `n` edges (edges beyond `n` are removed).
     *
     * @warning Any index to removed edges in the Mesh will be left
     * unchanged, and therefore will point to invalid edges.
     *
     * @param[in] n: the new size of the Edge container.
     */
    void resizeEdges(std::size_t n) { Base::resizeElements(n); }

    /**
     * @brief Reserves a number of edges in the container of Edges. This is
     * useful when you know (or you have an idea) of how many edges will be
     * added to a new or existing mesh. Calling this function before any
     * addEdge() call will avoid unnecessary reallocations of the container,
     * saving execution time.
     *
     * The philosophy of this function is similar to the one of the
     * [reserve()](https://en.cppreference.com/w/cpp/container/vector/reserve)
     * function of the [std::vector
     * class](https://en.cppreference.com/w/cpp/container/vector).
     *
     * @param[in] n: the new capacity of the edge container.
     */
    void reserveEdges(std::size_t n) { Base::reserveElements(n); }

    /**
     * @brief Compacts the Edge Container, removing all the edges marked as
     * deleted. Edge references will automatically change accordingly in all
     * the containers of the Mesh.
     *
     * @return A vector that maps each old edge index to its new index in the
     * compacted container (or NULL_INDEX if it was deleted).
     */
    std::vector<Index> compactEdges() { return Base::compactElements(); }

    /**
     * @brief Marks as deleted the edge with the given id.
     *
     * This member function does not perform any reallocation of the edges:
     * the deleted edges will stay in the Edge Container, but will be
     * marked as deleted.
     *
     * Deleted edges are automatically jumped by the iterators provided by
     * the Edge Container.
     *
     * @param[in] i: the index of the edge that will be marked as deleted.
     */
    void deleteEdge(Index i) { Base::deleteElement(i); }

    /**
     * @brief Marks as deleted the given edge.
     *
     * This member function does not perform any reallocation of the edges:
     * the deleted edges will stay in the Edge Container, but will be
     * marked as deleted.
     *
     * Deleted edges are automatically jumped by the iterators provided by
     * the Edge Container.
     *
     * @param[in] e: the edge that will be marked as deleted.
     */
    void deleteEdge(const Edge& e) { Base::deleteElement(e.index()); }

    /**
     * @brief This is a utility member function that returns the index of an
     * element if the container were compact, that is the number of
     * non-deleted elements before the edge with the given index.
     *
     * Complexity: O(n), with n the number of edges in the container.
     *
     * This function does not perform any sanity check on the given index.
     *
     * @param[in] i: the index of an edge of the container.
     * @return The index that the edge with index i would have if this
     * container were compact.
     */
    Index edgeIndexIfCompact(Index i) const
    {
        return Base::elementIndexIfCompact(i);
    }

    /**
     * @brief Returns a vector that tells, for each actual edge index, the new
     * index that the edge would have in a compacted container. For each
     * deleted edge index, the value of the vector will be NULL_INDEX.
     *
     * This is useful if you need to know the indices of the edges that they
     * would have in a compact container, without considering the deleted ones.
     *
     * @return A vector containing, for each edge index, its index if the
     * container were compact.
     */
    std::vector<Index> edgeCompactIndices() const
    {
        return Base::elementCompactIndices();
    }

    /**
     * @brief Updates references to the edges of this mesh according to the
     * mapping stored in the newIndices vector.
     *
     * This function is useful when edges are deleted, compacted, or
     * permuted, and you want to update all references stored across the
     * mesh containers to point to their new indices.
     *
     * @note This function does not change the position of the edges in this
     * container. It updates edge references stored in this and other
     * containers of the mesh.
     *
     * @param[in] newIndices: A vector mapping old edge indices to new ones.
     * If an edge was deleted or should become unreferenced, its mapping entry
     * must be NULL_INDEX.
     */
    void updateEdgeReferences(const std::vector<Index>& newIndices)
    {
        Base::updateElementReferences(newIndices);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate over
     * the edges of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * edges using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (Edge e : m.edges()) {
     *     // do something with this edge
     * }
     * @endcode
     *
     * The iterator used to iterate over edges is automatically initialized
     * to jump deleted edges of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted edges.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto edges(bool jumpDeleted = true)
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns an ElementContainerProxy object that allows to iterate over
     * the edges of the container, providing begin() and end() member
     * functions.
     *
     * This member function is very useful when you want to iterate over the
     * edges using the C++ range-based for loop syntax:
     *
     * @code{.cpp}
     * for (ConstEdge e : m.edges()) {
     *     // do something with this edge
     * }
     * @endcode
     *
     * The iterator used to iterate over edges is automatically initialized
     * to jump deleted edges of the container. You can change this option by
     * calling this function with jumpDeleted = false.
     *
     * @param[in] jumpDeleted (default: true): boolean that tells if the
     * iterator should jump deleted edges.
     * @return An object having begin() and end() functions, allowing to
     * iterate over the container.
     */
    auto edges(bool jumpDeleted = true) const
    {
        return Base::elements(jumpDeleted);
    }

    /**
     * @brief Returns a view object that allows to iterate over the edges of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the edges() function, this member function does not
     * automatically jump deleted edges, but it iterates over the
     * edges in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first edge to be included in the
     * range. It must be less than or equal to edgeContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last edge to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * edgeContainerSize(), it will be set to edgeContainerSize().
     * @return A View object allowing to iterate over the given range of the container.
     */
    auto edges(Index begin, Index end = NULL_INDEX)
    {
        return Base::elements(begin, end);
    }

    /**
     * @brief Returns a view object that allows to iterate over the edges of
     * the container in the range [begin, end), providing begin() and end()
     * member functions.
     *
     * @note Unlike the edges() function, this member function does not
     * automatically jump deleted edges, but it iterates over the
     * edges in the given range, regardless of whether they are deleted or
     * not.
     *
     * @param[in] begin: the index of the first edge to be included in the
     * range. It must be less than or equal to edgeContainerSize() and less than
     * or equal to the end index.
     * @param[in] end: the index of the last edge to be included in the
     * range (exclusive). If NULL_INDEX (default) or greater than
     * edgeContainerSize(), it will be set to edgeContainerSize().
     * @return A View object allowing to iterate over the given range of the container.
     */
    auto edges(Index begin, Index end = NULL_INDEX) const
    {
        return Base::elements(begin, end);
    }

    // --- Components ---

    /**
     * @brief Checks if the edge component with the given COMP_ID is
     * available (present at compile time and enabled at run time).
     *
     * Non-optional components are always available (true).
     * Optional components return true if currently enabled, false otherwise.
     *
     * @tparam COMP_ID: The ID of the component to check.
     * @return `true` if currently available, `false` otherwise.
     */
    template<CompId COMP_ID>
    bool isEdgeComponentAvailable() const
    {
        return Base::template isComponentAvailable<COMP_ID>();
    }

    /**
     * @brief Enables the optional edge component with the given COMP_ID.
     *
     * @tparam COMP_ID: The ID of the component to enable.
     */
    template<CompId COMP_ID>
    void enableEdgeComponent()
        requires comp::HasOptionalComponent<COMP_ID, EdgeComponents>
    {
        Base::template enableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Disables the optional edge component with the given COMP_ID.
     * Note: Non-optional components cannot be disabled.
     *
     * @tparam COMP_ID: The ID of the component to disable.
     */
    template<CompId COMP_ID>
    void disableEdgeComponent()
        requires comp::HasOptionalComponent<COMP_ID, EdgeComponents>
    {
        Base::template disableOptionalComponent<COMP_ID>();
    }

    /**
     * @brief Enables all optional edge components in the container.
     */
    void enableAllEdgeOptionalComponents()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all optional edge components in the container.
     */
    void disableAllEdgeOptionalComponents()
    {
        Base::disableAllOptionalComponents();
    }

    /**
     * @brief Enables all optional edge components in the container.
     * Alias for enableAllEdgeOptionalComponents().
     */
    void enableAllEdgeComponents()
    {
        Base::enableAllOptionalComponents();
    }

    /**
     * @brief Disables all optional edge components in the container.
     * Alias for disableAllEdgeOptionalComponents().
     */
    void disableAllEdgeComponents()
    {
        Base::disableAllOptionalComponents();
    }

    VCL_COMMON_CONTAINER_COMPONENT_METHODS(Edge)
    VCL_CONTAINER_COMPONENT_METHODS(Edge, AdjacentEdges, ADJACENT_EDGES)
    VCL_CONTAINER_COMPONENT_METHODS(Edge, AdjacentFaces, ADJACENT_FACES)

    // --- Custom Components ---

    /**
     * @brief Checks if Edges have a custom component with the given name.
     *
     * This function does not take into account the type of the custom
     * component.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component to check.
     * @return `true` if the Edge Element has a custom component with the given
     * name.
     */
    bool hasEdgeCustomComponent(const std::string& name) const
        requires edge::HasCustomComponents<EdgeComponents>
    {
        return Base::hasElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * of any type associated to the Edge Element.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @return A vector of strings representing all the names of the custom
     * components.
     */
    std::vector<std::string> edgeCustomComponentNames() const
        requires edge::HasCustomComponents<EdgeComponents>
    {
        return Base::elemCustomComponentNames();
    }

    /**
     * @brief Checks if the custom component of the Edge Element having the
     * given name has the same type of the given template argument type of this
     * function.
     *
     * @note This function is available only if the Edge Element has the
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
    bool isEdgeCustomComponentOfType(const std::string& name) const
        requires edge::HasCustomComponents<EdgeComponents>
    {
        return Base::template isElemCustomComponentOfType<K>(name);
    }

    /**
     * @brief Returns the std::type_index of the custom component of the Edge
     * Element having the given input name.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component to get the
     * std::type_index from.
     * @throws std::out_of_range if no custom component of the given name was
     * found.
     * @return The std::type_index of the custom component having the given
     * input name.
     */
    std::type_index edgeCustomComponentType(const std::string& name) const
        requires edge::HasCustomComponents<EdgeComponents>
    {
        return Base::elemCustomComponentType(name);
    }

    /**
     * @brief Returns a vector containing all the names of the custom components
     * associated to the Edge Element having the same type of the given
     * template argument type of this function.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component names.
     * @return A vector of strings representing the names of the custom
     * components of a given type.
     */
    template<typename K>
    std::vector<std::string> edgeCustomComponentNamesOfType() const
        requires edge::HasCustomComponents<EdgeComponents>
    {
        return Base::template elemCustomComponentNamesOfType<K>();
    }

    /**
     * @brief Adds a custom component of type K to the Edge, having the given
     * name.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component added to the Edge.
     * @param[in] name: The name of the custom component added to the Edge.
     */
    template<typename K>
    void addEdgeCustomComponent(const std::string& name)
        requires edge::HasCustomComponents<EdgeComponents>
    {
        Base::template addElemCustomComponent<K>(name);
    }

    /**
     * @brief Deletes the custom component of the given name from the Edge
     * Element.
     *
     * The function does nothing if the custom component does not exist.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @param[in] name: The name of the custom component that will be removed
     * from the Edge.
     */
    void deleteEdgeCustomComponent(const std::string& name)
        requires edge::HasCustomComponents<EdgeComponents>
    {
        Base::deleteElemCustomComponent(name);
    }

    /**
     * @brief Returns a vector handle to the custom component having the type K
     * and the given name.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the handle.
     * @param[in] name: Name of the custom component for which to return the handle.
     * @return A vector handle that allows access to the custom component.
     */
    template<typename K>
    CustomComponentVectorHandle<K> edgeCustomComponentVectorHandle(
        const std::string& name)
        requires edge::HasCustomComponents<EdgeComponents>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Returns a const vector handle to the custom component having type
     * K and the given name.
     *
     * @note This function is available only if the Edge Element has the
     * CustomComponents Component.
     *
     * @tparam K: The type of the custom component for which to return the handle.
     * @param[in] name: Name of the custom component for which to return the handle.
     * @return A const vector handle that allows access to the custom
     * component.
     */
    template<typename K>
    ConstCustomComponentVectorHandle<K> edgeCustomComponentVectorHandle(
        const std::string& name) const
        requires edge::HasCustomComponents<EdgeComponents>
    {
        return Base::template customComponentVectorHandle<K>(name);
    }

    /**
     * @brief Serializes all per-edge custom components of type K stored in
     * this container to the given output stream.
     *
     * @tparam K: The type of the custom components to serialize.
     * @param os: The output stream to write to.
     */
    template<typename K>
    void serializeEdgeCustomComponentsOfType(std::ostream& os) const
        requires edge::HasCustomComponents<EdgeComponents>
    {
        Base::template serializeElemCustomComponentsOfType<K>(os);
    }

    /**
     * @brief Deserializes all per-edge custom components of type K from the
     * given input stream and stores them into this container.
     *
     * @tparam K: The type of the custom components to deserialize.
     * @param is: The input stream to read from.
     */
    template<typename K>
    void deserializeEdgeCustomComponentsOfType(std::istream& is)
        requires edge::HasCustomComponents<EdgeComponents>
    {
        Base::template deserializeElemCustomComponentsOfType<K>(is);
    }

protected:
    /**
     * @brief Imports all edges from the corresponding container of the
     * other mesh into this container.
     *
     * @param[in] m: The other mesh to import from.
     */
    template<typename OtherMesh>
    void importFrom(const OtherMesh& m)
    {
        Base::importFrom(m);
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_EDGE_CONTAINER_H
