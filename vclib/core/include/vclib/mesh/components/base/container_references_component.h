// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H
#define VCL_MESH_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H

#include "container_component.h"
#include "ref_elements_view.h"
#include "reference_vector.h"

#include <vclib/mesh/elements/base/base.h>

#include <cassert>
#include <concepts>
#include <limits>
#include <ranges>
#include <type_traits>
#include <vector>

namespace vcl::comp {

/**
 * @brief Base class for all components that store indices representing
 * topological references to other elements in the Mesh (e.g. vertex references,
 * adjacent faces, adjacent edges, adjacent vertices).
 *
 * It extends ContainerComponent by defining the referencesElement method and
 * providing a uniform, cache-friendly implementation of updateReferences for
 * mesh compaction.
 *
 * ### Dual Index Spaces: Global Element Indices vs. Local Container Positions
 *
 * Components inheriting from ContainerReferencesComponent manage topological
 * relationships by operating in two distinct index spaces:
 *
 * 1. **Global Element Index (`IndexType`)**:
 *    - Represents the ID/location of a referenced element within the mesh's
 *      corresponding `ElementContainer` (`REF_ELEM_ID`, e.g. `VertexContainer`
 *      or `FaceContainer`).
 *    - Configurable as 32-bit (`uint32_t`) or 64-bit (`uint64_t`).
 *    - The sentinel representing a null, missing, or unassigned reference (e.g.
 *      a boundary edge with no adjacent face) is `NULL_INDEX`
 *      (`vcl::NULL_INDEX<IndexType>`).
 *    - Returned by `refIndex(self, uint i)` and accepted by `setRef(self, uint
 *      i, IndexType idx)` and `indexOfRef(self, IndexType idx)`.
 *
 * 2. **Local Container Position (`uint`)**:
 *    - Represents the 0-based offset/slot `i` within this element's local
 *      reference container (`0 <= i < refCount(self)`), e.g. corner 0, 1, 2 of
 *      a triangle face.
 *    - Always typed as `uint` (`uint32_t`) because local topological
 *      cardinalities (e.g. face degree, vertex valence) are small and do not
 *      scale with total mesh size.
 *    - The sentinel representing an invalid position or an element not found in
 *      this container is `vcl::UINT_NULL` (`0xFFFFFFFF`).
 *    - Passed to `refIndex(self, uint i)` and returned by `indexOfRef(self,
 * IndexType idx)`.
 *
 * @tparam COMP_ID: The unique identifier of the component.
 * @tparam Index: The unsigned integral type used to store indices (default
 * uint).
 * @tparam REF_ELEM_ID: The ElemId of the referenced elements (e.g.
 * ElemId::VERTEX).
 * @tparam N: The size of the reference container: static (N >= 0) or dynamic (N
 * < 0).
 * @tparam TTVC: True if the component container size is tied to the vertex
 * count.
 */
template<
    CompId                 COMP_ID,
    std::unsigned_integral Index,
    ElemId                 REF_ELEM_ID,
    int                    N    = -1,
    bool                   TTVC = false,
    bool                   OPT  = false>
class ContainerReferencesComponent :
        public ContainerComponent<
            COMP_ID,
            Index,
            N,
            TTVC,
            OPT,
            ReferenceVector<Index, N>>
{
    using Base = ContainerComponent<
        COMP_ID,
        Index,
        N,
        TTVC,
        OPT,
        ReferenceVector<Index, N>>;

public:
    using IndexType = Index;

    /**
     * @brief The null/invalid sentinel value for this component's index type.
     */
    static constexpr IndexType NULL_INDEX = vcl::NULL_INDEX<IndexType>;

    template<ElemId TARGET_ELEM_ID>
    static constexpr bool referencesElement()
    {
        return TARGET_ELEM_ID == REF_ELEM_ID;
    }

    /**
     * @brief Returns the number of referenced elements stored in this
     * component.
     * @param[in] self: The component / element proxy object.
     * @return The number of referenced elements.
     */
    static uint refCount(const auto& self)
    {
        return Base::container(self).size();
    }

    /**
     * @brief Returns the global element index of the i-th referenced element.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The 0-based local position in this reference container (0
     * <= i < refCount()).
     * @return The global index (IndexType) in the target ElementContainer, or
     * NULL_INDEX if unassigned.
     */
    static IndexType refIndex(const auto& self, uint i)
    {
        assert(i < refCount(self));
        return Base::container(self)[i];
    }

protected:
    /**
     * @brief Updates the stored reference indices according to the newIndices
     * mapping after compaction, permutation, or renumbering of the referenced
     * element container.
     *
     * @tparam TARGET_ELEM_ID: The ElemId of the referenced elements.
     * @param[in] newIndices: A vector mapping old element indices to new ones.
     */
    template<ElemId TARGET_ELEM_ID>
    void updateReferences(
        this auto&&                   self,
        const std::vector<IndexType>& newIndices)
    {
        if constexpr (TARGET_ELEM_ID == REF_ELEM_ID) {
            for (auto& ref : Base::container(self)) {
                if (ref != NULL_INDEX) {
                    assert(ref < newIndices.size());
                    ref = newIndices[ref];
                }
            }
        }
    }

    /**
     * @brief Shifts the stored reference indices by offset if they match
     * TARGET_ELEM_ID. Used during append operations.
     *
     * @tparam TARGET_ELEM_ID: The element ID of the referenced elements.
     * @param[in] offset: The offset value to add to the references.
     */
    template<ElemId TARGET_ELEM_ID>
    void shiftReferences(this auto&& self, IndexType offset)
    {
        if constexpr (TARGET_ELEM_ID == REF_ELEM_ID) {
            if (offset != 0) {
                for (auto& ref : Base::container(self)) {
                    if (ref != NULL_INDEX) {
                        ref += offset;
                    }
                }
            }
        }
    }

    // --- Generic Static Reference / Adjacency Helpers ---

    /**
     * @brief Casts a reference index from another unsigned integral index type
     * to IndexType, preserving the NULL_INDEX sentinel value across different
     * index widths.
     */
    template<std::unsigned_integral OtherIndex>
    static constexpr IndexType castIndex(OtherIndex idx) noexcept
    {
        if constexpr (std::same_as<IndexType, OtherIndex>) {
            return idx;
        }
        else {
            return idx == vcl::NULL_INDEX<OtherIndex> ?
                       NULL_INDEX :
                       static_cast<IndexType>(idx);
        }
    }

    /**
     * @brief Returns a proxy to the i-th referenced element in the parent mesh.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container.
     * @return A proxy to the referenced element.
     */
    static decltype(auto) refElement(auto&& self, uint i)
    {
        assert(i < refCount(self));
        return self.parentContainer()
            .parentMesh()
            .template element<REF_ELEM_ID>(Base::container(self)[i]);
    }

    /**
     * @brief Returns the index of the i-th referenced element using modular
     * arithmetic. Supports negative indices.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @return The index in the target element container.
     */
    static IndexType refIndexMod(const auto& self, int i)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        return Base::container(self)[((i % n) + n) % n];
    }

    /**
     * @brief Returns a proxy to the i-th referenced element using modular
     * arithmetic. Supports negative indices.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @return A proxy to the referenced element.
     */
    static decltype(auto) refElementMod(auto&& self, int i)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        return refElement(self, ((i % n) + n) % n);
    }

    /**
     * @brief Sets the reference at position i to the given element index.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container.
     * @param[in] idx: The index of the referenced element.
     */
    static void setRef(auto&& self, uint i, IndexType idx)
    {
        assert(i < refCount(self));
        Base::container(self)[i] = idx;
    }

    /**
     * @brief Sets the reference at position i to the given element proxy.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container.
     * @param[in] e: The referenced element proxy.
     */
    template<typename Elem>
    static void setRef(auto&& self, uint i, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        assert(i < refCount(self));
        if constexpr (requires { e.index(); }) {
            Base::container(self)[i] = e.index();
        }
        else {
            Base::container(self)[i] = static_cast<IndexType>(e);
        }
    }

    /**
     * @brief Sets the reference at position i using modular arithmetic.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @param[in] idx: The index of the referenced element.
     */
    static void setRefMod(auto&& self, int i, IndexType idx)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        setRef(self, ((i % n) + n) % n, idx);
    }

    /**
     * @brief Sets the reference at position i using modular arithmetic.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position in this container modulo refCount().
     * @param[in] e: The referenced element proxy.
     */
    template<typename Elem>
    static void setRefMod(auto&& self, int i, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        int n = static_cast<int>(refCount(self));
        assert(n > 0);
        setRef(self, ((i % n) + n) % n, e);
    }

    /**
     * @brief Sets all stored references from an input range of indices or
     * proxies.
     * @param[in] self: The component / element proxy object.
     * @param[in] rng: The input range.
     */
    template<std::ranges::input_range Rng>
    static void setRefs(auto&& self, Rng&& rng)
    {
        if constexpr (N >= 0) {
            if constexpr (std::ranges::sized_range<Rng>) {
                assert(std::ranges::size(rng) == static_cast<std::size_t>(N));
            }
            uint i = 0;
            for (auto&& item : rng) {
                assert(i < static_cast<uint>(N));
                setRef(self, i++, item);
            }
            assert(i == static_cast<uint>(N));
        }
        else if constexpr (!TTVC) {
            if constexpr (std::ranges::sized_range<Rng>) {
                resizeRefs(self, std::ranges::size(rng));
                uint i = 0;
                for (auto&& item : rng) {
                    setRef(self, i++, item);
                }
            }
            else if constexpr (std::ranges::forward_range<Rng>) {
                resizeRefs(self, std::ranges::distance(rng));
                uint i = 0;
                for (auto&& item : rng) {
                    setRef(self, i++, item);
                }
            }
            else {
                clearRefs(self);
                for (auto&& item : rng) {
                    pushRef(self, item);
                }
            }
        }
        else {
            uint i = 0;
            for (auto&& item : rng) {
                assert(i < refCount(self));
                setRef(self, i++, item);
            }
            assert(i == refCount(self));
        }
    }

    /**
     * @brief Sets all stored references from a parameter pack of indices or
     * proxies.
     * @param[in] self: The component / element proxy object.
     * @param[in] args: The references to set.
     */
    template<typename... Args>
    static void setRefs(auto&& self, const Args&... args)
        requires (sizeof...(Args) > 1 && (sizeof...(Args) == N || N < 0))
    {
        if constexpr (N < 0 && !TTVC) {
            resizeRefs(self, sizeof...(Args));
        }
        uint i = 0;
        (setRef(self, i++, args), ...);
    }

    /**
     * @brief Checks if this container contains the given referenced element
     * index.
     * @param[in] self: The component / element proxy object.
     * @param[in] idx: The index to check.
     * @return true if found, false otherwise.
     */
    static bool containsRef(const auto& self, IndexType idx)
    {
        return Base::container(self).contains(idx);
    }

    /**
     * @brief Checks if this container contains the given referenced element
     * proxy.
     * @param[in] self: The component / element proxy object.
     * @param[in] e: The element proxy to check.
     * @return true if found, false otherwise.
     */
    template<typename Elem>
    static bool containsRef(const auto& self, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            return containsRef(self, e.index());
        }
        else {
            return containsRef(self, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Finds the local position in this reference container of the given
     * global element index.
     * @param[in] self: The component / element proxy object.
     * @param[in] idx: The global element index (IndexType) to search for in the
     * target ElementContainer.
     * @return The 0-based local position (uint) in this container, or UINT_NULL
     * if not found.
     */
    static uint indexOfRef(const auto& self, IndexType idx)
    {
        return Base::container(self).indexOf(idx);
    }

    /**
     * @brief Finds the position in this reference container of the given
     * element proxy.
     * @param[in] self: The component / element proxy object.
     * @param[in] e: The element proxy to search for.
     * @return The 0-based position in this container, or UINT_NULL if not
     * found.
     */
    template<typename Elem>
    static uint indexOfRef(const auto& self, const Elem& e)
        requires (!std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            return indexOfRef(self, e.index());
        }
        else {
            return indexOfRef(self, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Resizes the references container to size n.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] n: The new size.
     */
    static void resizeRefs(auto&& self, uint n) requires (N < 0 && !TTVC)
    {
        Base::container(self).resize(n);
    }

    /**
     * @brief Appends a referenced element index to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] idx: The index to append.
     */
    static void pushRef(auto&& self, IndexType idx) requires (N < 0 && !TTVC)
    {
        Base::container(self).pushBack(idx);
    }

    /**
     * @brief Appends a referenced element proxy to the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] e: The element proxy to append.
     */
    template<typename Elem>
    static void pushRef(auto&& self, const Elem& e) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            pushRef(self, e.index());
        }
        else {
            pushRef(self, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Inserts a referenced element index at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position to insert at.
     * @param[in] idx: The index to insert.
     */
    static void insertRef(auto&& self, uint i, IndexType idx)
        requires (N < 0 && !TTVC)
    {
        Base::container(self).insert(i, idx);
    }

    /**
     * @brief Inserts a referenced element proxy at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position to insert at.
     * @param[in] e: The element proxy to insert.
     */
    template<typename Elem>
    static void insertRef(auto&& self, uint i, const Elem& e) requires (
        N < 0 && !TTVC && !std::unsigned_integral<std::remove_cvref_t<Elem>>)
    {
        if constexpr (requires { e.index(); }) {
            insertRef(self, i, e.index());
        }
        else {
            insertRef(self, i, static_cast<IndexType>(e));
        }
    }

    /**
     * @brief Erases the reference at position i.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     * @param[in] i: The position to erase.
     */
    static void eraseRef(auto&& self, uint i) requires (N < 0 && !TTVC)
    {
        Base::container(self).erase(i);
    }

    /**
     * @brief Clears all references in the container.
     * @note Available only for dynamic, non-TTVC containers.
     * @param[in] self: The component / element proxy object.
     */
    static void clearRefs(auto&& self) requires (N < 0 && !TTVC)
    {
        Base::container(self).clear();
    }

    /**
     * @brief Returns a lightweight view over the indices stored in this
     * container.
     * @param[in] self: The component / element proxy object.
     * @return A View object exposing begin() and end() over the indices.
     */
    static auto refIndices(auto&& self)
    {
        return vcl::View(
            Base::container(self).begin(), Base::container(self).end());
    }

    /**
     * @brief Returns a lazy range of element proxies for all referenced
     * elements.
     * @param[in] self: The component / element proxy object.
     * @return A transformed range yielding ElementProxy / ConstElementProxy
     * objects.
     */
    static decltype(auto) refElements(auto&& self)
    {
        auto& m     = self.parentContainer().parentMesh();
        using MeshT = std::remove_reference_t<decltype(m)>;
        using ContT = std::remove_reference_t<decltype(Base::container(self))>;
        return RefElementsView<MeshT, ContT, REF_ELEM_ID>(
            m, Base::container(self));
    }
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_BASE_CONTAINER_REFERENCES_COMPONENT_H
