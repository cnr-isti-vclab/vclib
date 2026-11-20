// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_EDGE_H
#define VCL_MESH_ELEMENTS_EDGE_H

#include "edge_components.h"

#include <vclib/mesh/components/bit_flags.h>
#include <vclib/mesh/components/vertex_references.h>
#include <vclib/mesh/elements/base/base.h>
#include <vclib/mesh/elements/base/element_proxy.h>

#include <ranges>

namespace vcl {

/**
 * @brief The Edge class represents the runtime proxy (view) of an
 * Edge element in a mesh container.
 *
 * It inherits from ElementProxy and provides object-oriented access to
 * the per-edge components stored in the container's Structure of Arrays.
 *
 * @tparam ContainerType: The type of the container (mutable or const).
 *
 * @ingroup elements
 */
template<typename ContainerType>
class Edge : public ElementProxy<ElemId::EDGE, ContainerType>
{
    using Base = ElementProxy<ElemId::EDGE, ContainerType>;

public:
    using Base::NULL_INDEX;
    using typename Base::Components;
    using typename Base::IndexType;
    using typename Base::ParentMeshType;

    static constexpr int VERTEX_COUNT = comp::
        ComponentTypeT<CompId::VERTEX_REFERENCES, Components>::VERTEX_COUNT;

    static_assert(
        comp::HasVertexReferences<Components>,
        "An Edge must have the VertexReferences component (e.g. "
        "EdgeVertexReferences).");
    static_assert(VERTEX_COUNT == 2, "An Edge must have exactly 2 vertices.");
    static_assert(
        comp::HasBitFlags<Components>,
        "An Edge must have the BitFlags component "
        "(vcl::comp::BitFlags).");

    Edge() = default;

    // Implicit conversion constructor from mutable Edge to const Edge
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    Edge(const Edge<OtherContainerType>& other) : Base(other)
    {
    }

    /**
     * @brief Swaps this Edge proxy handle with another one.
     */
    void swap(Edge& other) noexcept { Base::swap(other); }

    /**
     * @brief Swaps two Edge proxy handles.
     */
    friend void swap(Edge& a, Edge& b) noexcept { a.swap(b); }

protected:
    Edge(ContainerType& container, IndexType index) : Base(container, index) {}

private:
    /// friend classes declarations

    template<
        typename,
        typename,
        std::unsigned_integral,
        template<typename...> class>
    friend class mesh::ElementContainer;

    template<typename>
    friend class mesh::ElementIterator;

    template<typename>
    friend class Edge;
};

/**
 * @brief The EdgeComponents struct defines a type-wrapper for the components
 * of an Edge, checking at compile-time that the required components (BitFlags,
 * VertexReferences with VERTEX_COUNT == 2) are present.
 *
 * @tparam Comps: The components that compose the Edge.
 *
 * @ingroup elements
 */
template<typename... Comps>
struct EdgeComponents : vcl::TypeWrapper<Comps...>
{
    static_assert(
        comp::HasVertexReferences<vcl::TypeWrapper<Comps...>>,
        "Edge components must include VertexReferences (e.g. "
        "EdgeVertexReferences).");
    static_assert(
        comp::ComponentTypeT<
            CompId::VERTEX_REFERENCES,
            vcl::TypeWrapper<Comps...>>::VERTEX_COUNT == 2,
        "An Edge must have exactly 2 vertices.");
    static_assert(
        comp::HasBitFlags<vcl::TypeWrapper<Comps...>>,
        "Edge components must include BitFlags (vcl::comp::BitFlags).");
};

template<typename... Comps>
struct EdgeComponents<vcl::TypeWrapper<Comps...>> : EdgeComponents<Comps...>
{
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T is an Edge element.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept EdgeConcept =
    ElementConcept<T> && (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::EDGE) &&
    comp::HasBitFlags<typename std::remove_cvref_t<T>::Components> &&
    comp::HasVertexReferences<typename std::remove_cvref_t<T>::Components> &&
    (std::remove_cvref_t<T>::VERTEX_COUNT == 2);

/**
 * @brief Concept that checks whether a type T is a valid component list for an
 * Edge.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept HasRequiredEdgeComponents =
    comp::HasBitFlags<T> && comp::HasVertexReferences<T> &&
    (comp::ComponentTypeT<CompId::VERTEX_REFERENCES, T>::VERTEX_COUNT == 2);

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_EDGE_H
