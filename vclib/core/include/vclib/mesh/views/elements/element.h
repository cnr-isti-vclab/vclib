// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENTS_ELEMENT_H
#define VCL_MESH_VIEWS_ELEMENTS_ELEMENT_H

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto index = [](auto&& p) -> uint {
    if constexpr (IsPointer<decltype(p)>) {
        if (p == nullptr) [[unlikely]]
            return UINT_NULL;
        else
            return p->index();
    }
    else
        return p.index();
};

struct IndexView
{
    constexpr IndexView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, IndexView)
    {
        return std::forward<R>(r) | std::views::transform(index);
    }
};

} // namespace detail

inline constexpr detail::IndexView indices;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_ELEMENTS_ELEMENT_H
