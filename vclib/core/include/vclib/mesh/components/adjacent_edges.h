// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
#define VCL_MESH_COMPONENTS_ADJACENT_EDGES_H

#include "base/component_predicates.h"
#include "base/container_references_component.h"

#include <vclib/mesh/elements/base/base.h>

#include <cassert>
#include <concepts>
#include <ranges>

namespace vcl::comp {

/**
 * @brief The AdjacentEdges class is a container of Edge indices. It can be used
 * by any Element (Vertex, Face, Edge) to store edge adjacency information.
 *
 * It is a random access container having static or dynamic size, depending on
 * the value of N (a negative number means dynamic).
 *
 * ### Dual Index Spaces: Global Edge Indices vs. Local Adjacency Positions
 *
 * It is essential to distinguish between the two indexing spaces in this
 * component:
 *
 * 1. **Global Edge Index (`IndexType`)**:
 *    - The global index or ID of an edge inside the mesh's `EdgeContainer`
 *      (`m.edge(ei)`).
 *    - Typed as `IndexType` (32 or 64 bit).
 *    - Sentinel value: `NULL_INDEX` (`vcl::NULL_INDEX<IndexType>`). Indicates
 *      that there is no adjacent edge at this slot.
 *    - Returned by `adjEdgeIndex(uint i)` and `adjEdgeIndexMod(int i)`.
 *    - Passed to search functions like `containsAdjEdge(IndexType ei)` and
 *     `indexOfAdjEdge(IndexType ei)`.
 *
 * 2. **Local Adjacency Position (`uint`)**:
 *    - The 0-based position or slot `i` within this element's adjacency list
 *      (`0 <= i < adjEdgeCount()`).
 *    - Always typed as `uint` (`uint32_t`).
 *    - Sentinel value: `vcl::UINT_NULL` (`0xFFFFFFFF`), indicating that the
 *      queried edge was not found in this element's adjacency list.
 *    - Passed as the position `uint i` to `adjEdge(uint i)`, `adjEdgeIndex(uint
 *      i)`, and `setAdjEdge(uint i, IndexType ei)`.
 *    - Returned by `indexOfAdjEdge(IndexType ei)`.
 *
 * @tparam N: The size of the container, representing the number of storable
 * adjacent edges. If negative, the container is dynamic.
 * @tparam Index: The unsigned integral type used to store indices (default
 * uint).
 * @tparam TTVC: If true, the container size will be tied to the vertex count of
 * the element (e.g. on polygon faces).
 * @tparam OPT: If true, the component is optional.
 *
 * @ingroup components
 */
template<
    int                    N     = -1,
    std::unsigned_integral Index = uint,
    bool                   TTVC  = false,
    bool                   OPT   = false>
class AdjacentEdges :
        public ContainerReferencesComponent<
            CompId::ADJACENT_EDGES,
            Index,
            ElemId::EDGE,
            N,
            TTVC,
            OPT>
{
    using Base = ContainerReferencesComponent<
        CompId::ADJACENT_EDGES,
        Index,
        ElemId::EDGE,
        N,
        TTVC,
        OPT>;

public:
    using IndexType = Index;

    /**
     * @brief Static size of the container. If negative, the container is
     * dynamic.
     */
    static constexpr int ADJ_EDGE_COUNT = N;

    using Base::NULL_INDEX;

    /* Constructors */

    AdjacentEdges() = default;

    /* Member functions */

    /**
     * @brief Returns the number of adjacent edges of this element.
     * @return The number of adjacent edges.
     */
    uint adjEdgeCount(this const auto& self) { return Base::refCount(self); }

    /**
     * @brief Returns a proxy to the i-th adjacent edge of this element.
     * @param[in] i: The position in this container.
     * @return A proxy to the adjacent edge.
     */
    decltype(auto) adjEdge(this auto&& self, uint i)
    {
        return Base::refElement(self, i);
    }

    /**
     * @brief Returns the global index in the EdgeContainer of the i-th adjacent
     * edge.
     * @param[in] i: The 0-based local position in this adjacency container (0
     * <= i < adjEdgeCount()).
     * @return The global index (IndexType) of the adjacent edge, or NULL_INDEX
     * if no adjacent edge exists.
     */
    IndexType adjEdgeIndex(this const auto& self, uint i)
    {
        return Base::refIndex(self, i);
    }

    /**
     * @brief Returns a proxy to the i-th adjacent edge using modular
     * arithmetic. Supports negative indices.
     * @param[in] i: The position modulo adjEdgeCount().
     * @return A proxy to the adjacent edge.
     */
    decltype(auto) adjEdgeMod(this auto&& self, int i)
    {
        return Base::refElementMod(self, i);
    }

    /**
     * @brief Returns the index of the i-th adjacent edge using modular
     * arithmetic. Supports negative indices.
     * @param[in] i: The position modulo adjEdgeCount().
     * @return The index of the adjacent edge.
     */
    IndexType adjEdgeIndexMod(this const auto& self, int i)
    {
        return Base::refIndexMod(self, i);
    }

    /**
     * @brief Sets the i-th adjacent edge by index.
     * @param[in] i: The position in this container.
     * @param[in] ei: The edge index to set.
     */
    void setAdjEdge(this auto&& self, uint i, IndexType ei)
    {
        Base::setRef(self, i, ei);
    }

    /**
     * @brief Sets the i-th adjacent edge from an ElementProxy.
     * @param[in] i: The position in this container.
     * @param[in] e: The adjacent edge proxy.
     */
    template<typename Elem>
    void setAdjEdge(this auto&& self, uint i, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::setRef(self, i, e);
    }

    /**
     * @brief Sets the i-th adjacent edge by index using modular arithmetic.
     * @param[in] i: The position modulo adjEdgeCount().
     * @param[in] ei: The edge index to set.
     */
    void setAdjEdgeMod(this auto&& self, int i, IndexType ei)
    {
        Base::setRefMod(self, i, ei);
    }

    /**
     * @brief Sets the i-th adjacent edge from an ElementProxy using modular
     * arithmetic.
     * @param[in] i: The position modulo adjEdgeCount().
     * @param[in] e: The adjacent edge proxy.
     */
    template<typename Elem>
    void setAdjEdgeMod(this auto&& self, int i, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::setRefMod(self, i, e);
    }

    /**
     * @brief Sets all adjacent edges from an input range.
     * @param[in] rng: The input range of edge indices or proxies.
     */
    template<std::ranges::input_range Rng>
    void setAdjEdges(this auto&& self, Rng&& rng)
    {
        Base::setRefs(self, std::forward<Rng>(rng));
    }

    /**
     * @brief Sets all adjacent edges from a parameter pack.
     * @param[in] args: The edge indices or proxies to set.
     */
    template<typename... Args>
    void setAdjEdges(this auto&& self, const Args&... args)
        requires (sizeof...(Args) > 1 && (sizeof...(Args) == N || N < 0))
    {
        Base::setRefs(self, args...);
    }

    /**
     * @brief Checks if this container contains the given edge index.
     * @param[in] ei: The edge index to search for.
     * @return true if found, false otherwise.
     */
    bool containsAdjEdge(this const auto& self, IndexType ei)
    {
        return Base::containsRef(self, ei);
    }

    /**
     * @brief Checks if this container contains the given edge proxy.
     * @param[in] e: The edge proxy to search for.
     * @return true if found, false otherwise.
     */
    template<typename Elem>
    bool containsAdjEdge(this const auto& self, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        return Base::containsRef(self, e);
    }

    /**
     * @brief Finds the 0-based local position in this adjacency container of
     * the given global edge index.
     * @param[in] ei: The global edge index (IndexType) to search for in
     * EdgeContainer.
     * @return The 0-based local position (uint) in this container, or UINT_NULL
     * if not found.
     */
    uint indexOfAdjEdge(this const auto& self, IndexType ei)
    {
        return Base::indexOfRef(self, ei);
    }

    /**
     * @brief Finds the position in this container of the given edge proxy.
     * @param[in] e: The edge proxy to search for.
     * @return The 0-based position, or UINT_NULL if not found.
     */
    template<typename Elem>
    uint indexOfAdjEdge(this const auto& self, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        return Base::indexOfRef(self, e);
    }

    /* Dynamic size operations */

    /**
     * @brief Resizes the adjacent edges container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] n: The new size.
     */
    void resizeAdjEdges(this auto&& self, uint n) requires (N < 0 && !TTVC)
    {
        Base::resizeRefs(self, n);
    }

    /**
     * @brief Appends an edge index to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] ei: The edge index to append.
     */
    void pushAdjEdge(this auto&& self, IndexType ei) requires (N < 0 && !TTVC)
    {
        Base::pushRef(self, ei);
    }

    /**
     * @brief Appends an edge proxy to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] e: The edge proxy to append.
     */
    template<typename Elem>
    void pushAdjEdge(this auto&& self, const Elem& e) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::pushRef(self, e);
    }

    /**
     * @brief Inserts an edge index at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to insert at.
     * @param[in] ei: The edge index to insert.
     */
    void insertAdjEdge(this auto&& self, uint i, IndexType ei)
        requires (N < 0 && !TTVC)
    {
        Base::insertRef(self, i, ei);
    }

    /**
     * @brief Inserts an edge proxy at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to insert at.
     * @param[in] e: The edge proxy to insert.
     */
    template<typename Elem>
    void insertAdjEdge(this auto&& self, uint i, const Elem& e) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::insertRef(self, i, e);
    }

    /**
     * @brief Erases the adjacent edge at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to erase.
     */
    void eraseAdjEdge(this auto&& self, uint i) requires (N < 0 && !TTVC)
    {
        Base::eraseRef(self, i);
    }

    /**
     * @brief Clears all adjacent edges in the container.
     * @note Available only for dynamic, non-TTVC containers.
     */
    void clearAdjEdges(this auto&& self) requires (N < 0 && !TTVC)
    {
        Base::clearRefs(self);
    }

    /* Views / Ranges */

    /**
     * @brief Returns a lazy range yielding proxies to all adjacent edges.
     * @return A range of ElementProxy / ConstElementProxy objects.
     */
    decltype(auto) adjEdges(this auto&& self)
    {
        return Base::refElements(self);
    }

    /**
     * @brief Returns a lightweight view over the adjacent edge indices.
     * @return A View object exposing begin() and end() over the indices.
     */
    auto adjEdgeIndices(this auto&& self) { return Base::refIndices(self); }

protected:
    template<typename OtherElement>
    void importFrom(
        this auto&&         self,
        const OtherElement& other,
        bool                importRefs = true);
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T has the AdjacentEdges component.
 */
template<typename T>
concept HasAdjacentEdges = HasComponent<CompId::ADJACENT_EDGES, T>;

/**
 * @brief Concept that checks whether a type T has an optional AdjacentEdges
 * component.
 */
template<typename T>
concept HasOptionalAdjacentEdges =
    HasAdjacentEdges<T> && HasOptionalComponent<CompId::ADJACENT_EDGES, T>;

/**
 * @brief Concept that checks whether a type T has a static-size AdjacentEdges
 * component.
 */
template<typename T>
concept HasStaticAdjacentEdges =
    HasAdjacentEdges<T> &&
    (ComponentTypeT<CompId::ADJACENT_EDGES, T>::ADJ_EDGE_COUNT > 0);

/* Implementation of member functions */

template<int N, std::unsigned_integral Index, bool TTVC, bool OPT>
template<typename OtherElement>
void AdjacentEdges<N, Index, TTVC, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool                importRefs)
{
    if (importRefs) {
        if constexpr (HasAdjacentEdges<OtherElement>) {
            if (other.template isComponentAvailable<CompId::ADJACENT_EDGES>()) {
                using OtherComp =
                    ComponentTypeT<CompId::ADJACENT_EDGES, OtherElement>;
                constexpr int OTHER_N = OtherComp::ADJ_EDGE_COUNT;

                if constexpr (N > 0) {
                    if constexpr (N == OTHER_N) {
                        for (uint i = 0; i < N; ++i) {
                            self.setAdjEdge(
                                i, Base::castIndex(other.adjEdgeIndex(i)));
                        }
                    }
                    else if constexpr (OTHER_N < 0) {
                        if (other.adjEdgeCount() == N) {
                            for (uint i = 0; i < N; ++i) {
                                self.setAdjEdge(
                                    i, Base::castIndex(other.adjEdgeIndex(i)));
                            }
                        }
                    }
                }
                else if constexpr (!TTVC) {
                    self.resizeAdjEdges(other.adjEdgeCount());
                    for (uint i = 0; i < other.adjEdgeCount(); ++i) {
                        self.setAdjEdge(
                            i, Base::castIndex(other.adjEdgeIndex(i)));
                    }
                }
                else {
                    if (self.adjEdgeCount() == other.adjEdgeCount()) {
                        for (uint i = 0; i < self.adjEdgeCount(); ++i) {
                            self.setAdjEdge(
                                i, Base::castIndex(other.adjEdgeIndex(i)));
                        }
                    }
                }
            }
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_EDGES_H
