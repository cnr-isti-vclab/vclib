// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
#define VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H

#include "base/component_predicates.h"
#include "base/container_references_component.h"

#include <vclib/mesh/elements/base/base.h>

#include <cassert>
#include <concepts>
#include <ranges>

namespace vcl::comp {

/**
 * @brief The AdjacentVertices class is a container of Vertex indices. It is
 * typically used by Vertex elements to store the 1-ring neighborhood of
 * adjacent vertices.
 *
 * It is a random access container having static or dynamic size (typically
 * dynamic, N = -1).
 *
 * ### Dual Index Spaces: Global Vertex Indices vs. Local Adjacency Positions
 *
 * It is essential to distinguish between the two indexing spaces in this
 * component:
 *
 * 1. **Global Vertex Index (`IndexType`)**:
 *    - The global index or ID of a vertex inside the mesh's `VertexContainer`
 *      (`m.vertex(vi)`).
 *    - Typed as `IndexType` (32 or 64 bit).
 *    - Sentinel value: `NULL_INDEX` (`vcl::NULL_INDEX<IndexType>`). Indicates
 *      that there is no adjacent vertex at this slot.
 *    - Returned by `adjVertexIndex(uint i)` and `adjVertexIndexMod(int i)`.
 *    - Passed to search functions like `containsAdjVertex(IndexType vi)` and
 *      `indexOfAdjVertex(IndexType vi)`.
 *
 * 2. **Local Adjacency Position (`uint`)**:
 *    - The 0-based position or slot `i` within this element's 1-ring adjacency
 *      list (`0 <= i < adjVertexCount()`).
 *    - Always typed as `uint` (`uint32_t`).
 *    - Sentinel value: `vcl::UINT_NULL` (`0xFFFFFFFF`), indicating that the
 *      queried vertex was not found in this element's adjacency list.
 *    - Passed as the position `uint i` to `adjVertex(uint i)`,
 * `adjVertexIndex(uint i)`, and `setAdjVertex(uint i, IndexType vi)`.
 *    - Returned by `indexOfAdjVertex(IndexType vi)`.
 *
 * @tparam N: The size of the container, representing the number of storable
 * adjacent vertices. If negative (default), the container is dynamic.
 * @tparam Index: The unsigned integral type used to store indices (default
 * uint).
 * @tparam TTVC: If true, the container size will be tied to the vertex count of
 * the element.
 * @tparam OPT: If true, the component is optional.
 *
 * @ingroup components
 */
template<
    int                    N     = -1,
    std::unsigned_integral Index = uint,
    bool                   TTVC  = false,
    bool                   OPT   = false>
class AdjacentVertices :
        public ContainerReferencesComponent<
            CompId::ADJACENT_VERTICES,
            Index,
            ElemId::VERTEX,
            N,
            TTVC,
            OPT>
{
    using Base = ContainerReferencesComponent<
        CompId::ADJACENT_VERTICES,
        Index,
        ElemId::VERTEX,
        N,
        TTVC,
        OPT>;

public:
    using IndexType = Index;

    /**
     * @brief Static size of the container. If negative, the container is
     * dynamic.
     */
    static constexpr int ADJ_VERTEX_COUNT = N;

    using Base::NULL_INDEX;

    /* Constructors */

    AdjacentVertices() = default;

    /* Member functions */

    /**
     * @brief Returns the number of adjacent vertices of this element.
     * @return The number of adjacent vertices.
     */
    uint adjVertexCount(this const auto& self) { return Base::refCount(self); }

    /**
     * @brief Returns a proxy to the i-th adjacent vertex of this element.
     * @param[in] i: The position in this container.
     * @return A proxy to the adjacent vertex.
     */
    decltype(auto) adjVertex(this auto&& self, uint i)
    {
        return Base::refElement(self, i);
    }

    /**
     * @brief Returns the global index in the VertexContainer of the i-th
     * adjacent vertex.
     * @param[in] i: The 0-based local position in this adjacency container (0
     * <= i < adjVertexCount()).
     * @return The global index (IndexType) of the adjacent vertex, or
     * NULL_INDEX if no adjacent vertex exists.
     */
    IndexType adjVertexIndex(this const auto& self, uint i)
    {
        return Base::refIndex(self, i);
    }

    /**
     * @brief Returns a proxy to the i-th adjacent vertex using modular
     * arithmetic. Supports negative indices.
     * @param[in] i: The position modulo adjVertexCount().
     * @return A proxy to the adjacent vertex.
     */
    decltype(auto) adjVertexMod(this auto&& self, int i)
    {
        return Base::refElementMod(self, i);
    }

    /**
     * @brief Returns the index of the i-th adjacent vertex using modular
     * arithmetic. Supports negative indices.
     * @param[in] i: The position modulo adjVertexCount().
     * @return The index of the adjacent vertex.
     */
    IndexType adjVertexIndexMod(this const auto& self, int i)
    {
        return Base::refIndexMod(self, i);
    }

    /**
     * @brief Sets the i-th adjacent vertex by index.
     * @param[in] i: The position in this container.
     * @param[in] vi: The vertex index to set.
     */
    void setAdjVertex(this auto&& self, uint i, IndexType vi)
    {
        Base::setRef(self, i, vi);
    }

    /**
     * @brief Sets the i-th adjacent vertex from an ElementProxy.
     * @param[in] i: The position in this container.
     * @param[in] v: The adjacent vertex proxy.
     */
    template<typename Elem>
    void setAdjVertex(this auto&& self, uint i, const Elem& v)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::setRef(self, i, v);
    }

    /**
     * @brief Sets the i-th adjacent vertex by index using modular arithmetic.
     * @param[in] i: The position modulo adjVertexCount().
     * @param[in] vi: The vertex index to set.
     */
    void setAdjVertexMod(this auto&& self, int i, IndexType vi)
    {
        Base::setRefMod(self, i, vi);
    }

    /**
     * @brief Sets the i-th adjacent vertex from an ElementProxy
     * using modular arithmetic.
     * @param[in] i: The position modulo adjVertexCount().
     * @param[in] v: The adjacent vertex proxy.
     */
    template<typename Elem>
    void setAdjVertexMod(this auto&& self, int i, const Elem& v)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::setRefMod(self, i, v);
    }

    /**
     * @brief Sets all adjacent vertices from an input range.
     * @param[in] rng: The input range of vertex indices or proxies.
     */
    template<std::ranges::input_range Rng>
    void setAdjVertices(this auto&& self, Rng&& rng)
    {
        Base::setRefs(self, std::forward<Rng>(rng));
    }

    /**
     * @brief Sets all adjacent vertices from a parameter pack.
     * @param[in] args: The vertex indices or proxies to set.
     */
    template<typename... Args>
    void setAdjVertices(this auto&& self, const Args&... args)
        requires (sizeof...(Args) > 1 && (sizeof...(Args) == N || N < 0))
    {
        Base::setRefs(self, args...);
    }

    /**
     * @brief Checks if this container contains the given vertex index.
     * @param[in] vi: The vertex index to search for.
     * @return true if found, false otherwise.
     */
    bool containsAdjVertex(this const auto& self, IndexType vi)
    {
        return Base::containsRef(self, vi);
    }

    /**
     * @brief Checks if this container contains the given vertex proxy.
     * @param[in] v: The vertex proxy to search for.
     * @return true if found, false otherwise.
     */
    template<typename Elem>
    bool containsAdjVertex(this const auto& self, const Elem& v)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        return Base::containsRef(self, v);
    }

    /**
     * @brief Finds the 0-based local position in this adjacency container of
     * the given global vertex index.
     * @param[in] vi: The global vertex index (IndexType) to search for in
     * VertexContainer.
     * @return The 0-based local position (uint) in this container, or UINT_NULL
     * if not found.
     */
    uint indexOfAdjVertex(this const auto& self, IndexType vi)
    {
        return Base::indexOfRef(self, vi);
    }

    /**
     * @brief Finds the position in this container of the given vertex proxy.
     * @param[in] v: The vertex proxy to search for.
     * @return The 0-based position, or UINT_NULL if not found.
     */
    template<typename Elem>
    uint indexOfAdjVertex(this const auto& self, const Elem& v)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        return Base::indexOfRef(self, v);
    }

    /* Dynamic size operations */

    /**
     * @brief Resizes the adjacent vertices container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] n: The new size.
     */
    void resizeAdjVertices(this auto&& self, uint n) requires (N < 0 && !TTVC)
    {
        Base::resizeRefs(self, n);
    }

    /**
     * @brief Appends a vertex index to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] vi: The vertex index to append.
     */
    void pushAdjVertex(this auto&& self, IndexType vi) requires (N < 0 && !TTVC)
    {
        Base::pushRef(self, vi);
    }

    /**
     * @brief Appends a vertex proxy to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] v: The vertex proxy to append.
     */
    template<typename Elem>
    void pushAdjVertex(this auto&& self, const Elem& v) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::pushRef(self, v);
    }

    /**
     * @brief Inserts a vertex index at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to insert at.
     * @param[in] vi: The vertex index to insert.
     */
    void insertAdjVertex(this auto&& self, uint i, IndexType vi)
        requires (N < 0 && !TTVC)
    {
        Base::insertRef(self, i, vi);
    }

    /**
     * @brief Inserts a vertex proxy at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to insert at.
     * @param[in] v: The vertex proxy to insert.
     */
    template<typename Elem>
    void insertAdjVertex(this auto&& self, uint i, const Elem& v) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::insertRef(self, i, v);
    }

    /**
     * @brief Erases the adjacent vertex at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to erase.
     */
    void eraseAdjVertex(this auto&& self, uint i) requires (N < 0 && !TTVC)
    {
        Base::eraseRef(self, i);
    }

    /**
     * @brief Clears all adjacent vertices in the container.
     * @note Available only for dynamic, non-TTVC containers.
     */
    void clearAdjVertices(this auto&& self) requires (N < 0 && !TTVC)
    {
        Base::clearRefs(self);
    }

    /* Views / Ranges */

    /**
     * @brief Returns a lazy range yielding proxies to all adjacent vertices.
     * @return A range of ElementProxy / ConstElementProxy objects.
     */
    decltype(auto) adjVertices(this auto&& self)
    {
        return Base::refElements(self);
    }

    /**
     * @brief Returns a lightweight view over the adjacent vertex indices.
     * @return A View object exposing begin() and end() over the indices.
     */
    auto adjVertexIndices(this auto&& self) { return Base::refIndices(self); }

protected:
    template<typename OtherElement>
    void importFrom(
        this auto&&         self,
        const OtherElement& other,
        bool                importRefs = true);
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T has the AdjacentVertices
 * component.
 */
template<typename T>
concept HasAdjacentVertices = HasComponent<CompId::ADJACENT_VERTICES, T>;

/**
 * @brief Concept that checks whether a type T has an optional AdjacentVertices
 * component.
 */
template<typename T>
concept HasOptionalAdjacentVertices =
    HasAdjacentVertices<T> &&
    HasOptionalComponent<CompId::ADJACENT_VERTICES, T>;

/**
 * @brief Concept that checks whether a type T has a static-size
 * AdjacentVertices component.
 */
template<typename T>
concept HasStaticAdjacentVertices =
    HasAdjacentVertices<T> &&
    (ComponentTypeT<CompId::ADJACENT_VERTICES, T>::ADJ_VERTEX_COUNT > 0);

/* Implementation of member functions */

template<int N, std::unsigned_integral Index, bool TTVC, bool OPT>
template<typename OtherElement>
void AdjacentVertices<N, Index, TTVC, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool                importRefs)
{
    if (importRefs) {
        if constexpr (HasAdjacentVertices<OtherElement>) {
            if (other.template isComponentAvailable<
                    CompId::ADJACENT_VERTICES>()) {
                using OtherComp =
                    ComponentTypeT<CompId::ADJACENT_VERTICES, OtherElement>;
                constexpr int OTHER_N = OtherComp::ADJ_VERTEX_COUNT;

                if constexpr (N > 0) {
                    if constexpr (N == OTHER_N) {
                        for (uint i = 0; i < N; ++i) {
                            self.setAdjVertex(
                                i, Base::castIndex(other.adjVertexIndex(i)));
                        }
                    }
                    else if constexpr (OTHER_N < 0) {
                        if (other.adjVertexCount() == N) {
                            for (uint i = 0; i < N; ++i) {
                                self.setAdjVertex(
                                    i,
                                    Base::castIndex(other.adjVertexIndex(i)));
                            }
                        }
                    }
                }
                else if constexpr (!TTVC) {
                    self.resizeAdjVertices(other.adjVertexCount());
                    for (uint i = 0; i < other.adjVertexCount(); ++i) {
                        self.setAdjVertex(
                            i, Base::castIndex(other.adjVertexIndex(i)));
                    }
                }
                else {
                    if (self.adjVertexCount() == other.adjVertexCount()) {
                        for (uint i = 0; i < self.adjVertexCount(); ++i) {
                            self.setAdjVertex(
                                i, Base::castIndex(other.adjVertexIndex(i)));
                        }
                    }
                }
            }
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_VERTICES_H
