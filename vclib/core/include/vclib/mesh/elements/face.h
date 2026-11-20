// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_ELEMENTS_FACE_H
#define VCL_MESH_ELEMENTS_FACE_H

#include "face_components.h"

#include <vclib/mesh/components/bit_flags.h>
#include <vclib/mesh/components/vertex_references.h>
#include <vclib/mesh/elements/base/base.h>
#include <vclib/mesh/elements/base/element_proxy.h>

#include <ranges>

namespace vcl {

/**
 * @brief The Face class represents the runtime proxy (view) of a
 * Face element in a mesh container.
 *
 * It inherits from ElementProxy and provides object-oriented access to
 * the per-face components stored in the container's Structure of Arrays.
 *
 * @tparam ContainerType: The type of the container (mutable or const).
 *
 * @ingroup elements
 */
template<typename ContainerType>
class Face : public ElementProxy<ElemId::FACE, ContainerType>
{
    using Base = ElementProxy<ElemId::FACE, ContainerType>;

    template<typename Self>
    using ResolvedFace = std::conditional_t<
        std::is_const_v<std::remove_reference_t<Self>> ||
            std::is_const_v<ContainerType>,
        Face<const std::remove_const_t<ContainerType>>,
        Face<ContainerType>>;

public:
    using Base::NULL_INDEX;
    using typename Base::Components;
    using typename Base::IndexType;
    using typename Base::ParentMeshType;

public:
    static constexpr int VERTEX_COUNT = comp::
        ComponentTypeT<CompId::VERTEX_REFERENCES, Components>::VERTEX_COUNT;

    static_assert(
        comp::HasVertexReferences<Components>,
        "A Face must have the VertexReferences component (e.g. "
        "TriangleVertexReferences).");
    static_assert(
        comp::HasBitFlags<Components>,
        "A Face must have the BitFlags component "
        "(vcl::comp::BitFlags).");

    Face() = default;

    // Implicit conversion constructor from mutable Face to const Face
    template<typename OtherContainerType>
    requires std::is_same_v<ContainerType, const OtherContainerType>
    Face(const Face<OtherContainerType>& other) : Base(other)
    {
    }

    /**
     * @brief Swaps this Face proxy handle with another one.
     */
    void swap(Face& other) noexcept { Base::swap(other); }

    /**
     * @brief Swaps two Face proxy handles.
     */
    friend void swap(Face& a, Face& b) noexcept { a.swap(b); }

    template<typename OtherElement>
    void importFrom(const OtherElement& other, bool importRefs = true)
        requires (!std::is_const_v<ContainerType>)
    {
        if (importRefs) {
            if constexpr (
                comp::HasPolygonVertexReferences<Components> &&
                comp::HasVertexReferences<OtherElement>) {
                this->resizeVertices(other.vertexCount());
            }
        }
        Base::importFrom(other, importRefs);
    }

    /**
     * @brief Returns a proxy to the i-th wedge (corner) of the face.
     * @param[in] i: The 0-based local corner index (0 <= i < vertexCount()).
     * @return A WedgeProxy for the i-th corner.
     */
    decltype(auto) wedge(this auto&& self, uint i)
    {
        assert(i < self.vertexCount());
        using TargetFace = ResolvedFace<decltype(self)>;
        return comp::WedgeProxy<TargetFace>(self, i);
    }

    /**
     * @brief Returns a proxy to the modularized i-th wedge (corner) of the
     * face.
     * @param[in] i: The modularized corner index (modularized on
     * vertexCount()).
     * @return A WedgeProxy for the modularized corner.
     */
    decltype(auto) wedgeMod(this auto&& self, int i)
    {
        const int n = self.vertexCount();
        assert(n > 0);
        return self.wedge(((i % n) + n) % n);
    }

    /**
     * @brief Returns the container proxy for all wedges of the face.
     * Allows iterating over wedges (for auto w : f.wedges()) or direct
     * column container access (for auto& n : f.wedges().normals()).
     * @return A WedgeContainerProxy for this face.
     */
    decltype(auto) wedges(this auto&& self)
    {
        using TargetFace = ResolvedFace<decltype(self)>;
        return comp::WedgeContainerProxy<TargetFace>(self);
    }

protected:
    Face(ContainerType& container, IndexType index) : Base(container, index) {}

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
    friend class Face;
};

/**
 * @brief The FaceComponents struct defines a type-wrapper for the components
 * of a Face, checking at compile-time that the required components (BitFlags,
 * VertexReferences) are present.
 *
 * @tparam Comps: The components that compose the Face.
 *
 * @ingroup elements
 */
template<typename... Comps>
struct FaceComponents : vcl::TypeWrapper<Comps...>
{
    static_assert(
        comp::HasVertexReferences<vcl::TypeWrapper<Comps...>>,
        "Face components must include VertexReferences (e.g. "
        "TriangleVertexReferences).");
    static_assert(
        comp::HasBitFlags<vcl::TypeWrapper<Comps...>>,
        "Face components must include BitFlags (vcl::comp::BitFlags).");
};

template<typename... Comps>
struct FaceComponents<vcl::TypeWrapper<Comps...>> : FaceComponents<Comps...>
{
};

/* Concepts */

/**
 * @brief Concept that checks whether a type T is a Face element.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept FaceConcept =
    ElementConcept<T> && (std::remove_cvref_t<T>::ELEMENT_ID == ElemId::FACE) &&
    comp::HasBitFlags<typename std::remove_cvref_t<T>::Components> &&
    comp::HasVertexReferences<typename std::remove_cvref_t<T>::Components>;

/**
 * @brief Concept that checks whether a type T is a valid component list for a
 * Face.
 *
 * @ingroup element_concepts
 */
template<typename T>
concept HasRequiredFaceComponents =
    comp::HasBitFlags<T> && comp::HasVertexReferences<T>;

} // namespace vcl

#endif // VCL_MESH_ELEMENTS_FACE_H
