// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_BASE_ELEMENT_CONTAINER_PROXY_H
#define VCL_MESH_CONTAINERS_BASE_ELEMENT_CONTAINER_PROXY_H

#include "component_view.h"

#include <vclib/mesh/components.h>

#include <vclib/base.h>

#include <cassert>
#include <ranges>
#include <type_traits>

namespace vcl::mesh {

/**
 * @brief Proxy view over an ElementContainer of a Mesh.
 *
 * It provides range-based iteration over elements (yielding ElementProxy
 * instances), random access via `operator[]`, and high-performance
 * column-oriented access to individual components via ComponentView (e.g.
 * `positions()`, `normals()`, `colors()`).
 *
 * Iteration over elements or component views automatically skips elements
 * marked as deleted by default (when jumpDeleted is true).
 *
 * @tparam Container: The type of the container (mutable or const).
 */
template<typename Container>
class ElementContainerProxy :
        public std::ranges::view_interface<ElementContainerProxy<Container>>
{
    Container* mContainer   = nullptr;
    bool       mJumpDeleted = true;

public:
    using ContainerType = std::remove_cv_t<Container>;
    using IndexType     = typename ContainerType::IndexType;
    using Components    = typename ContainerType::Components;
    using ElementType   = std::conditional_t<
          std::is_const_v<Container>,
          typename ContainerType::ConstElementType,
          typename ContainerType::ElementType>;
    using Iterator = std::conditional_t<
        std::is_const_v<Container>,
        typename ContainerType::ConstIterator,
        typename ContainerType::Iterator>;

    ElementContainerProxy() = default;

    ElementContainerProxy(Container& container, bool jumpDeleted = true) :
            mContainer(&container), mJumpDeleted(jumpDeleted)
    {
    }

    template<typename>
    friend class ElementContainerProxy;

    // Implicit conversion from mutable proxy to const proxy
    template<typename OtherContainerType>
    requires std::is_same_v<Container, const OtherContainerType>
    ElementContainerProxy(
        const ElementContainerProxy<OtherContainerType>& other) :
            mContainer(other.mContainer), mJumpDeleted(other.mJumpDeleted)
    {
    }

    /* Element-level iteration and access */

    Iterator begin() const
    {
        return mContainer ? mContainer->begin(mJumpDeleted) : Iterator();
    }

    Iterator end() const
    {
        return mContainer ? mContainer->end(mJumpDeleted) : Iterator();
    }

    ElementType operator[](IndexType i) const
    {
        assert(mContainer != nullptr);
        return mContainer->element(i);
    }

    std::size_t size() const
    {
        if (!mContainer) {
            return 0;
        }
        return mJumpDeleted ? mContainer->elementCount() :
                              mContainer->elementContainerSize();
    }

    bool empty() const { return size() == 0; }

    bool isCompact() const { return mContainer && mContainer->isCompact(); }

    template<CompId COMP_ID>
    bool isComponentAvailable() const
    {
        return mContainer &&
               mContainer->template isComponentAvailable<COMP_ID>();
    }

    template<CompId COMP_ID>
    void enableOptionalComponent() const requires (!std::is_const_v<Container>)
    {
        assert(mContainer != nullptr);
        mContainer->template enableOptionalComponent<COMP_ID>();
    }

    template<CompId COMP_ID>
    void disableOptionalComponent() const requires (!std::is_const_v<Container>)
    {
        assert(mContainer != nullptr);
        mContainer->template disableOptionalComponent<COMP_ID>();
    }

    void enableAllOptionalComponents() const
        requires (!std::is_const_v<Container>)
    {
        assert(mContainer != nullptr);
        mContainer->enableAllOptionalComponents();
    }

    void disableAllOptionalComponents() const
        requires (!std::is_const_v<Container>)
    {
        assert(mContainer != nullptr);
        mContainer->disableAllOptionalComponents();
    }

    /* Component Views */

    /**
     * @brief Returns a ComponentView over the positions of the elements.
     */
    auto positions() const requires comp::HasPosition<Container>
    {
        return component<CompId::POSITION>();
    }

    /**
     * @brief Returns a ComponentView over the normals of the elements.
     */
    auto normals() const requires comp::HasNormal<Container>
    {
        return component<CompId::NORMAL>();
    }

    /**
     * @brief Returns a ComponentView over the colors of the elements.
     */
    auto colors() const requires comp::HasColor<Container>
    {
        return component<CompId::COLOR>();
    }

    /**
     * @brief Returns a ComponentView over the vertex indices of the elements.
     * Available only for elements with a static/fixed number of vertices
     * (e.g. triangles, quads), representing a contiguous Index Buffer.
     */
    auto vertexIndices() const
        requires comp::HasStaticVertexReferences<Container>
    {
        return component<CompId::VERTEX_REFERENCES>();
    }

    /**
     * @brief Returns a ComponentView over the principal curvatures of the
     * elements.
     */
    auto principalCurvatures() const
        requires comp::HasPrincipalCurvature<Container>
    {
        return component<CompId::PRINCIPAL_CURVATURE>();
    }

    /**
     * @brief Returns a ComponentView over the qualities of the elements.
     */
    auto qualities() const requires comp::HasQuality<Container>
    {
        return component<CompId::QUALITY>();
    }

    /**
     * @brief Returns a ComponentView over the texture coordinates of the
     * elements.
     */
    auto texCoords() const requires comp::HasTexCoord<Container>
    {
        return component<CompId::TEX_COORD>();
    }

    /**
     * @brief Returns a ComponentView over the material indices of the elements.
     */
    auto materialIndices() const requires comp::HasMaterialIndex<Container>
    {
        return component<CompId::MATERIAL_INDEX>();
    }

    /**
     * @brief Returns a ComponentView over the tangents of the elements.
     */
    auto tangents() const requires comp::HasTangent<Container>
    {
        return component<CompId::TANGENT>();
    }

    /**
     * @brief Generic access to a ComponentView for a specific COMP_ID.
     *
     * Available for attribute components (IsAttributeComponent) or static
     * vertex references.
     */
    template<CompId COMP_ID>
    requires comp::HasComponent<COMP_ID, Container> &&
             (comp::IsAttributeComponent<COMP_ID> ||
              (COMP_ID == CompId::VERTEX_REFERENCES &&
               comp::HasStaticVertexReferences<Container>) )
    auto component() const
    {
        assert(mContainer != nullptr);
        return ComponentView<COMP_ID, Container>(*mContainer, mJumpDeleted);
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_BASE_ELEMENT_CONTAINER_PROXY_H
