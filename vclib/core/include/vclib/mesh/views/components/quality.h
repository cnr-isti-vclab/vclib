// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_QUALITY_H
#define VCL_MESH_VIEWS_COMPONENTS_QUALITY_H

#include <vclib/mesh/components/quality.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto quality = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->quality();
    else
        return p.quality();
};

struct QualityView
{
    constexpr QualityView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, QualityView)
    {
        return std::forward<R>(r) | std::views::transform(quality);
    }
};

} // namespace detail

inline constexpr detail::QualityView quality;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_QUALITY_H
