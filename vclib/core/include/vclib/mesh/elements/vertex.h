// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_VERTEX_H
#define VCL_MESH_ELEMENTS_VERTEX_H

#include "vertex_components.h"

#include <vclib/mesh/components/bit_flags.h>
#include <vclib/mesh/components/position.h>
#include <vclib/mesh/elements/base/base.h>
#include <vclib/mesh/elements/base/element_proxy.h>

namespace vcl {

/**
 * @brief The Vertex class represents the runtime proxy (view) of a
 * Vertex element in a mesh container.
 *
 * It inherits from ElementProxy and provides object-oriented access to
 * the per-vertex components stored in the container's Structure of Arrays.
 *
 * @tparam ContainerType: The type of the container (mutable or const).
 *
 * @ingroup elements
 */
template<typename ContainerType>
class Vertex : public ElementProxy<ElemId::VERTEX, ContainerType>
{
    using Base = ElementProxy<ElemId::VERTEX, ContainerType>;

public:
    using Base::NULL_INDEX;
    using typename Base::Components;
    using typename Base::IndexType;
    using typename Base::ParentMeshType;

    static_assert(
        comp::HasPosition<Components>,
        "A Vertex must have the Position component "
        "(vcl::comp::Position).");
    static_assert(
        comp::HasBitFlags<Components>,
        "A Vertex must have the BitFlags component "
        "(vcl::comp::BitFlags).");

    Vertex() = default;

    // Implicit conversion constructor from mutable Vertex to const Vertex
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    Vertex(const Vertex<OtherContainerType>& other) : Base(other)
    {
    }

    /**
     * @brief Swaps this Vertex proxy handle with another one.
     */
    void swap(Vertex& other) noexcept { Base::swap(other); }

    /**
     * @brief Swaps two Vertex proxy handles.
     */
    friend void swap(Vertex& a, Vertex& b) noexcept { a.swap(b); }

protected:
    Vertex(ContainerType& container, IndexType index) : Base(container, index)
    {
    }

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
    friend class Vertex;
};

/**
 * @brief The VertexComponents struct defines a type-wrapper for the components
 * of a Vertex, checking at compile-time that the required components (BitFlags,
 * Position) are present.
 *
 * @tparam Comps: The components that compose the Vertex.
 *
 * @ingroup elements
 */
template<typename... Comps>
struct VertexComponents : vcl::TypeWrapper<Comps...>
{
    static_assert(
        comp::HasPosition<vcl::TypeWrapper<Comps...>>,
        "Vertex components must include Position "
        "(vcl::comp::Position).");
    static_assert(
        comp::HasBitFlags<vcl::TypeWrapper<Comps...>>,
        "Vertex components must include BitFlags "
        "(vcl::comp::BitFlags).");
};

template<typename... Comps>
struct VertexComponents<vcl::TypeWrapper<Comps...>> : VertexComponents<Comps...>
{
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T is a Vertex element.
 *
 * The concept is satisfied when T satisfies ElementConcept, has ELEMENT_ID ==
 * ElemId::VERTEX, and has BitFlags and Position components.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept VertexConcept =
    ElementConcept<T> &&
    (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::VERTEX) &&
    comp::HasBitFlags<typename std::remove_cvref_t<T>::Components> &&
    comp::HasPosition<typename std::remove_cvref_t<T>::Components>;

/**
 * @brief Concept that checks whether a type T is a valid component list for a
 * Vertex.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept HasRequiredVertexComponents =
    comp::HasBitFlags<T> && comp::HasPosition<T>;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_VERTEX_H
