// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H
#define VCL_MESH_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H

#include <vclib/mesh/components/principal_curvature.h>

#include <vclib/base.h>

#include <ranges>

namespace vcl::views {

namespace detail {

inline constexpr auto principalCurvature = [](auto&& p) -> decltype(auto) {
    if constexpr (IsPointer<decltype(p)>)
        return p->principalCurvature();
    else
        return p.principalCurvature();
};

struct PrincipalCurvaturesView
{
    constexpr PrincipalCurvaturesView() = default;

    template<std::ranges::range R>
    friend constexpr auto operator|(R&& r, PrincipalCurvaturesView)
    {
        return std::forward<R>(r) | std::views::transform(principalCurvature);
    }
};

} // namespace detail

inline constexpr detail::PrincipalCurvaturesView principalCurvatures;

} // namespace vcl::views

#endif // VCL_MESH_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H
