// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_BASE_COMPONENT_VIEW_H
#define VCL_MESH_CONTAINERS_BASE_COMPONENT_VIEW_H

#include <vclib/base.h>
#include <vclib/mesh/components/base/base.h>
#include <vclib/mesh/iterators/component_iterator.h>

#include <cassert>
#include <ranges>
#include <type_traits>

namespace vcl::mesh {

/**
 * @brief A view over a single component column in an ElementContainer.
 *
 * It provides random access via `operator[]`, raw buffer access via `data()`,
 * and bidirectional iteration via `begin()` and `end()` that respects the
 * container's deleted flags (jumping deleted elements if jumpDeleted is true).
 *
 * It does not allow mutating the container size or capacity, protecting the
 * Structure of Arrays (SoA) synchronization invariants.
 *
 * @tparam COMP_ID: The component ID.
 * @tparam Container: The type of the container (mutable or const).
 */
template<CompId COMP_ID, typename Container>
class ComponentView :
        public std::ranges::view_interface<ComponentView<COMP_ID, Container>>
{
private:
    Container* mContainer   = nullptr;
    bool       mJumpDeleted = true;

public:
    using ContainerType = std::remove_cv_t<Container>;
    using IndexType     = ContainerType::IndexType;
    using CompType      = comp::ComponentTypeT<COMP_ID, Container>;
    using RawDataType   = CompType::DataType;
    using DataType      = std::conditional_t<
             std::is_const_v<Container>,
             const RawDataType,
             RawDataType>;

    using value_type      = RawDataType;
    using reference       = DataType&;
    using const_reference = const RawDataType&;
    using pointer         = DataType*;
    using const_pointer   = const RawDataType*;

    using Iterator      = ComponentIterator<COMP_ID, Container>;
    using ConstIterator = ComponentIterator<COMP_ID, const Container>;

public:
    ComponentView() = default;

    ComponentView(Container& container, bool jumpDeleted = true) :
            mContainer(&container), mJumpDeleted(jumpDeleted)
    {
    }

    template<CompId, typename>
    friend class ComponentView;

    // Implicit conversion from mutable ComponentView to const ComponentView
    template<typename OtherContainerType>
    requires std::is_same_v<Container, const OtherContainerType>
    ComponentView(const ComponentView<COMP_ID, OtherContainerType>& other) :
            mContainer(other.mContainer), mJumpDeleted(other.mJumpDeleted)
    {
    }

    /**
     * @brief Checks if the component is available in the container.
     */
    bool isAvailable() const
    {
        return mContainer &&
               mContainer->template isComponentAvailable<COMP_ID>();
    }

    Iterator begin() const
    {
        if (!mContainer || !isAvailable()) {
            return Iterator();
        }
        return Iterator(*mContainer, 0, needJump());
    }

    Iterator end() const
    {
        if (!mContainer || !isAvailable()) {
            return Iterator();
        }
        return Iterator(
            *mContainer, mContainer->elementContainerSize(), needJump());
    }

    reference operator[](IndexType i) const
    {
        assert(mContainer != nullptr);
        assert(isAvailable() && "Component is not available!");
        assert(i < mContainer->elementContainerSize());
        return mContainer->template vector<COMP_ID>()[i];
    }

    pointer data() const
    {
        if (!mContainer || !isAvailable()) {
            return nullptr;
        }
        return mContainer->template vector<COMP_ID>().data();
    }

    std::size_t size() const
    {
        if (!mContainer || !isAvailable()) {
            return 0;
        }
        return mJumpDeleted ? mContainer->elementCount() :
                              mContainer->elementContainerSize();
    }

    bool empty() const { return size() == 0; }

    bool isCompact() const { return mContainer && mContainer->isCompact(); }

private:
    // begin() and end() must agree on this flag
    bool needJump() const
    {
        return mJumpDeleted &&
               (mContainer->size() != mContainer->elementCount());
    }
};

} // namespace vcl::mesh

#endif // VCL_MESH_CONTAINERS_BASE_COMPONENT_VIEW_H
