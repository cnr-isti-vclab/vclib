// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_CONTAINERS_BASE_CUSTOM_COMPONENT_VECTOR_HANDLE_H
#define VCL_MESH_CONTAINERS_BASE_CUSTOM_COMPONENT_VECTOR_HANDLE_H

#include <vclib/base.h>

#include <any>
#include <functional>
#include <ranges>
#include <vector>

namespace vcl {

/**
 * @brief Allows to access directly to a custom component.
 *
 * The class allows to access a custom component stored in a Contaner of
 * Elements without having to use the Container itself and avoiding copies, and
 * it can be used as a normal std::vector. The class stores references to the
 * custom components, therefore it allows to modify them.
 *
 * It is meant to be created by a Container, that constructs it from the vector
 * of custom components and then returns it to the user.
 *
 * @note A CustomComponentVectorHandle object is meant to be used to access the
 * custom components. It does not make sense to modify the size of the container
 * or to add or remove elements. Member functions that modify the size of the
 * container are not implemented.
 *
 * @note If the Element Container is modified after the creation of a
 * CustomComponentVectorHandle, the CustomComponentVectorHandle is not updated
 * and still contains the old references to the custom components (that may be
 * invalidated).
 *
 * @tparam T: The type of the custom component.
 */
template<typename T>
class CustomComponentVectorHandle
{
    std::vector<std::reference_wrapper<T>> mVec;

    struct Unwrap
    {
        constexpr T& operator()(std::reference_wrapper<T> r) const noexcept
        {
            return r.get();
        }
    };

    struct ConstUnwrap
    {
        constexpr const T& operator()(
            std::reference_wrapper<T> r) const noexcept
        {
            return r.get();
        }
    };

public:
    using ViewType =
        decltype(std::declval<std::vector<std::reference_wrapper<T>>&>() | std::views::transform(Unwrap {}));
    using ConstViewType =
        decltype(std::declval<const std::vector<std::reference_wrapper<T>>&>() | std::views::transform(ConstUnwrap {}));

    using Iterator      = std::ranges::iterator_t<ViewType>;
    using ConstIterator = std::ranges::iterator_t<ConstViewType>;

    CustomComponentVectorHandle() = default;

    CustomComponentVectorHandle(std::vector<std::any>& cc)
    {
        mVec.reserve(cc.size());
        for (std::size_t i = 0; i < cc.size(); ++i) {
            mVec.push_back(std::any_cast<T&>(cc[i]));
        }
    }

    CustomComponentVectorHandle(const std::vector<std::any>& cc)
        requires std::is_const_v<T>
    {
        mVec.reserve(cc.size());
        for (std::size_t i = 0; i < cc.size(); ++i) {
            mVec.push_back(std::any_cast<const std::remove_const_t<T>&>(cc[i]));
        }
    }

    template<typename OtherT>
    requires std::is_same_v<const OtherT, T>
    CustomComponentVectorHandle(
        const CustomComponentVectorHandle<OtherT>& other)
    {
        mVec.reserve(other.size());
        for (std::size_t i = 0; i < other.size(); ++i) {
            mVec.push_back(other[i]);
        }
    }

    bool empty() const { return mVec.empty(); }

    T& at(std::size_t i) { return mVec.at(i).get(); }

    const T& at(std::size_t i) const { return mVec.at(i).get(); }

    T& front() { return mVec.front().get(); }

    const T& front() const { return mVec.front().get(); }

    T& back() { return mVec.back().get(); }

    const T& back() const { return mVec.back().get(); }

    std::size_t size() const { return mVec.size(); }

    T& operator[](std::size_t i) { return mVec[i].get(); }

    const T& operator[](std::size_t i) const { return mVec[i].get(); }

    Iterator begin()
    {
        return (mVec | std::views::transform(Unwrap {})).begin();
    }

    Iterator end() { return (mVec | std::views::transform(Unwrap {})).end(); }

    ConstIterator begin() const
    {
        return (mVec | std::views::transform(ConstUnwrap {})).begin();
    }

    ConstIterator end() const
    {
        return (mVec | std::views::transform(ConstUnwrap {})).end();
    }

    ConstIterator cbegin() const { return begin(); }

    ConstIterator cend() const { return end(); }
};

template<typename T>
using ConstCustomComponentVectorHandle = CustomComponentVectorHandle<const T>;

} // namespace vcl

#endif // VCL_MESH_CONTAINERS_BASE_CUSTOM_COMPONENT_VECTOR_HANDLE_H
