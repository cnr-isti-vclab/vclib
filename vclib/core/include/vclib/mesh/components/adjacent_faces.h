// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_ADJACENT_FACES_H
#define VCL_MESH_COMPONENTS_ADJACENT_FACES_H

#include "base/component_predicates.h"
#include "base/container_references_component.h"

#include <vclib/mesh/elements/base/base.h>

#include <cassert>
#include <concepts>
#include <ranges>

namespace vcl::comp {

/**
 * @brief The AdjacentFaces class is a container of Face indices. It can be used
 * by any Element (Vertex, Face, Edge) to store face adjacency information.
 *
 * It is a random access container having static or dynamic size, depending on
 * the value of N (a negative number means dynamic).
 *
 * ### Dual Index Spaces: Global Face Indices vs. Local Adjacency Positions
 *
 * It is essential to distinguish between the two indexing spaces in this
 * component:
 *
 * 1. **Global Face Index (`IndexType`)**:
 *    - The global index or ID of a face inside the mesh's `FaceContainer`
 *      (`m.face(fi)`).
 *    - Typed as `IndexType` (32 or 64 bit).
 *    - Sentinel value: `NULL_INDEX` (`vcl::NULL_INDEX<IndexType>`). Indicates
 *      that there is no adjacent face at this slot (e.g. a boundary edge of a
 *      mesh).
 *    - Returned by `adjFaceIndex(uint i)` and `adjFaceIndexMod(int i)`.
 *    - Passed to search functions like `containsAdjFace(IndexType fi)` and
 *     `indexOfAdjFace(IndexType fi)`.
 *
 * 2. **Local Adjacency Position (`uint`)**:
 *    - The 0-based position or slot `i` within this element's adjacency list
 *      (`0 <= i < adjFaceCount()`), e.g. edge 0, 1, 2 of a triangle face.
 *    - Always typed as `uint` (`uint32_t`).
 *    - Sentinel value: `vcl::UINT_NULL` (`0xFFFFFFFF`), indicating that the
 *      queried face was not found in this element's adjacency list.
 *    - Passed as the position `uint i` to `adjFace(uint i)`, `adjFaceIndex(uint
 *      i)`, and `setAdjFace(uint i, IndexType fi)`.
 *    - Returned by `indexOfAdjFace(IndexType fi)`.
 *
 * @tparam N: The size of the container, representing the number of storable
 * adjacent faces. If negative, the container is dynamic.
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
class AdjacentFaces :
        public ContainerReferencesComponent<
            CompId::ADJACENT_FACES,
            Index,
            ElemId::FACE,
            N,
            TTVC,
            OPT>
{
    using Base = ContainerReferencesComponent<
        CompId::ADJACENT_FACES,
        Index,
        ElemId::FACE,
        N,
        TTVC,
        OPT>;

public:
    using IndexType = Index;

    /**
     * @brief Static size of the container. If negative, the container is
     * dynamic.
     */
    static constexpr int ADJ_FACE_COUNT = N;

    using Base::NULL_INDEX;

    /* Constructors */

    AdjacentFaces() = default;

    /* Member functions */

    /**
     * @brief Returns the number of adjacent faces of this element.
     * @return The number of adjacent faces.
     */
    uint adjFaceCount(this const auto& self) { return Base::refCount(self); }

    /**
     * @brief Returns a proxy to the i-th adjacent face of this element.
     * @param[in] i: The position in this container.
     * @return A proxy to the adjacent face.
     */
    decltype(auto) adjFace(this auto&& self, uint i)
    {
        return Base::refElement(self, i);
    }

    /**
     * @brief Returns the global index in the FaceContainer of the i-th adjacent
     * face.
     * @param[in] i: The 0-based local position in this adjacency container (0
     * <= i < adjFaceCount()).
     * @return The global index (IndexType) of the adjacent face, or NULL_INDEX
     * if no adjacent face exists.
     */
    IndexType adjFaceIndex(this const auto& self, uint i)
    {
        return Base::refIndex(self, i);
    }

    /**
     * @brief Returns a proxy to the i-th adjacent face using modular
     * arithmetic. Supports negative indices.
     * @param[in] i: The position modulo adjFaceCount().
     * @return A proxy to the adjacent face.
     */
    decltype(auto) adjFaceMod(this auto&& self, int i)
    {
        return Base::refElementMod(self, i);
    }

    /**
     * @brief Returns the index of the i-th adjacent face using modular
     * arithmetic. Supports negative indices.
     * @param[in] i: The position modulo adjFaceCount().
     * @return The index of the adjacent face.
     */
    IndexType adjFaceIndexMod(this const auto& self, int i)
    {
        return Base::refIndexMod(self, i);
    }

    /**
     * @brief Sets the i-th adjacent face by index.
     * @param[in] i: The position in this container.
     * @param[in] fi: The face index to set.
     */
    void setAdjFace(this auto&& self, uint i, IndexType fi)
    {
        Base::setRef(self, i, fi);
    }

    /**
     * @brief Sets the i-th adjacent face from an ElementProxy.
     * @param[in] i: The position in this container.
     * @param[in] f: The adjacent face proxy.
     */
    template<typename Elem>
    void setAdjFace(this auto&& self, uint i, const Elem& f)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::setRef(self, i, f);
    }

    /**
     * @brief Sets the i-th adjacent face by index using modular arithmetic.
     * @param[in] i: The position modulo adjFaceCount().
     * @param[in] fi: The face index to set.
     */
    void setAdjFaceMod(this auto&& self, int i, IndexType fi)
    {
        Base::setRefMod(self, i, fi);
    }

    /**
     * @brief Sets the i-th adjacent face from an ElementProxy using modular
     * arithmetic.
     * @param[in] i: The position modulo adjFaceCount().
     * @param[in] f: The adjacent face proxy.
     */
    template<typename Elem>
    void setAdjFaceMod(this auto&& self, int i, const Elem& f)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::setRefMod(self, i, f);
    }

    /**
     * @brief Sets all adjacent faces from an input range.
     * @param[in] rng: The input range of face indices or proxies.
     */
    template<std::ranges::input_range Rng>
    void setAdjFaces(this auto&& self, Rng&& rng)
    {
        Base::setRefs(self, std::forward<Rng>(rng));
    }

    /**
     * @brief Sets all adjacent faces from a parameter pack.
     * @param[in] args: The face indices or proxies to set.
     */
    template<typename... Args>
    void setAdjFaces(this auto&& self, const Args&... args)
        requires (sizeof...(Args) > 1 && (sizeof...(Args) == N || N < 0))
    {
        Base::setRefs(self, args...);
    }

    /**
     * @brief Checks if this container contains the given face index.
     * @param[in] fi: The face index to search for.
     * @return true if found, false otherwise.
     */
    bool containsAdjFace(this const auto& self, IndexType fi)
    {
        return Base::containsRef(self, fi);
    }

    /**
     * @brief Checks if this container contains the given face proxy.
     * @param[in] f: The face proxy to search for.
     * @return true if found, false otherwise.
     */
    template<typename Elem>
    bool containsAdjFace(this const auto& self, const Elem& f)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        return Base::containsRef(self, f);
    }

    /**
     * @brief Finds the 0-based local position in this adjacency container of
     * the given global face index.
     * @param[in] fi: The global face index (IndexType) to search for in
     * FaceContainer.
     * @return The 0-based local position (uint) in this container, or UINT_NULL
     * if not found.
     */
    uint indexOfAdjFace(this const auto& self, IndexType fi)
    {
        return Base::indexOfRef(self, fi);
    }

    /**
     * @brief Finds the position in this container of the given face proxy.
     * @param[in] f: The face proxy to search for.
     * @return The 0-based position, or UINT_NULL if not found.
     */
    template<typename Elem>
    uint indexOfAdjFace(this const auto& self, const Elem& f)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        return Base::indexOfRef(self, f);
    }

    /* Dynamic size operations */

    /**
     * @brief Resizes the adjacent faces container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] n: The new size.
     */
    void resizeAdjFaces(this auto&& self, uint n) requires (N < 0 && !TTVC)
    {
        Base::resizeRefs(self, n);
    }

    /**
     * @brief Appends a face index to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] fi: The face index to append.
     */
    void pushAdjFace(this auto&& self, IndexType fi) requires (N < 0 && !TTVC)
    {
        Base::pushRef(self, fi);
    }

    /**
     * @brief Appends a face proxy to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] f: The face proxy to append.
     */
    template<typename Elem>
    void pushAdjFace(this auto&& self, const Elem& f) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::pushRef(self, f);
    }

    /**
     * @brief Inserts a face index at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to insert at.
     * @param[in] fi: The face index to insert.
     */
    void insertAdjFace(this auto&& self, uint i, IndexType fi)
        requires (N < 0 && !TTVC)
    {
        Base::insertRef(self, i, fi);
    }

    /**
     * @brief Inserts a face proxy at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to insert at.
     * @param[in] f: The face proxy to insert.
     */
    template<typename Elem>
    void insertAdjFace(this auto&& self, uint i, const Elem& f) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        Base::insertRef(self, i, f);
    }

    /**
     * @brief Erases the adjacent face at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] i: The position to erase.
     */
    void eraseAdjFace(this auto&& self, uint i) requires (N < 0 && !TTVC)
    {
        Base::eraseRef(self, i);
    }

    /**
     * @brief Clears all adjacent faces in the container.
     * @note Available only for dynamic, non-TTVC containers.
     */
    void clearAdjFaces(this auto&& self) requires (N < 0 && !TTVC)
    {
        Base::clearRefs(self);
    }

    /* Views / Ranges */

    /**
     * @brief Returns a lazy range yielding proxies to all adjacent faces.
     * @return A range of ElementProxy / ConstElementProxy objects.
     */
    decltype(auto) adjFaces(this auto&& self)
    {
        return Base::refElements(self);
    }

    /**
     * @brief Returns a lightweight view over the adjacent face indices.
     * @return A View object exposing begin() and end() over the indices.
     */
    auto adjFaceIndices(this auto&& self) { return Base::refIndices(self); }

protected:
    template<typename OtherElement>
    void importFrom(
        this auto&&         self,
        const OtherElement& other,
        bool                importRefs = true);
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T has the AdjacentFaces component.
 */
template<typename T>
concept HasAdjacentFaces = HasComponent<CompId::ADJACENT_FACES, T>;

/**
 * @brief Concept that checks whether a type T has an optional AdjacentFaces
 * component.
 */
template<typename T>
concept HasOptionalAdjacentFaces =
    HasAdjacentFaces<T> && HasOptionalComponent<CompId::ADJACENT_FACES, T>;

/**
 * @brief Concept that checks whether a type T has a static-size AdjacentFaces
 * component.
 */
template<typename T>
concept HasStaticAdjacentFaces =
    HasAdjacentFaces<T> &&
    (ComponentTypeT<CompId::ADJACENT_FACES, T>::ADJ_FACE_COUNT > 0);

/**
 * @brief Concept that checks whether a type T has a 3-element static
 * AdjacentFaces component.
 */
template<typename T>
concept HasTriangleAdjacentFaces =
    HasAdjacentFaces<T> &&
    (ComponentTypeT<CompId::ADJACENT_FACES, T>::ADJ_FACE_COUNT == 3);

/**
 * @brief Concept that checks whether a type T has a polygon (TTVC)
 * AdjacentFaces component.
 */
template<typename T>
concept HasPolygonAdjacentFaces =
    HasAdjacentFaces<T> &&
    (ComponentTypeT<CompId::ADJACENT_FACES, T>::ADJ_FACE_COUNT == -1) &&
    (ComponentTypeT<CompId::ADJACENT_FACES, T>::TIED_TO_VERTEX_COUNT);

/* Implementation of member functions */

template<int N, std::unsigned_integral Index, bool TTVC, bool OPT>
template<typename OtherElement>
void AdjacentFaces<N, Index, TTVC, OPT>::importFrom(
    this auto&&         self,
    const OtherElement& other,
    bool                importRefs)
{
    if (importRefs) {
        if constexpr (HasAdjacentFaces<OtherElement>) {
            if (other.template isComponentAvailable<CompId::ADJACENT_FACES>()) {
                using OtherComp =
                    ComponentTypeT<CompId::ADJACENT_FACES, OtherElement>;
                constexpr int OTHER_N = OtherComp::ADJ_FACE_COUNT;

                if constexpr (N > 0) {
                    if constexpr (N == OTHER_N) {
                        for (uint i = 0; i < N; ++i) {
                            self.setAdjFace(
                                i, Base::castIndex(other.adjFaceIndex(i)));
                        }
                    }
                    else if constexpr (OTHER_N < 0) {
                        if (other.adjFaceCount() == N) {
                            for (uint i = 0; i < N; ++i) {
                                self.setAdjFace(
                                    i, Base::castIndex(other.adjFaceIndex(i)));
                            }
                        }
                    }
                }
                else if constexpr (!TTVC) {
                    self.resizeAdjFaces(other.adjFaceCount());
                    for (uint i = 0; i < other.adjFaceCount(); ++i) {
                        self.setAdjFace(
                            i, Base::castIndex(other.adjFaceIndex(i)));
                    }
                }
                else {
                    if (self.adjFaceCount() == other.adjFaceCount()) {
                        for (uint i = 0; i < self.adjFaceCount(); ++i) {
                            self.setAdjFace(
                                i, Base::castIndex(other.adjFaceIndex(i)));
                        }
                    }
                }
            }
        }
    }
}

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_ADJACENT_FACES_H
