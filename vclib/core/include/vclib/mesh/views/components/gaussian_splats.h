// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_GAUSSIAN_SPLATS_H
#define VCL_MESH_VIEWS_COMPONENTS_GAUSSIAN_SPLATS_H

#include <vclib/mesh/components/gaussian_splat.h>
#include <vclib/mesh/views/component.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto guassianSplat = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->gaussianSplat();
    else
        return p.gaussianSplat();
};

struct GaussianSplatsView
{
    constexpr GaussianSplatsView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, GaussianSplatsView)
    {
        return std::forward<R>(r) | std::views::transform(guassianSplat);
    }
};

} // namespace detail

inline constexpr detail::GaussianSplatsView gaussianSplats;

template<>
inline constexpr auto component<CompId::GAUSSIAN_SPLAT> = gaussianSplats;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_GAUSSIAN_SPLATS_H
