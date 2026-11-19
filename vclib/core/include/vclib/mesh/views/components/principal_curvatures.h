// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H
#define VCL_MESH_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasPrincipalCurvatureMember = requires(T&& t) {
    { t.principalCurvature() };
};

template<typename R>
concept PrincipalCurvaturesRangeConcept =
    std::ranges::range<R> &&
    (requires(R&& r) {
        { std::forward<R>(r).principalCurvatures() } -> std::ranges::range;
    } || HasPrincipalCurvatureMember<std::ranges::range_reference_t<R>>);

namespace views {

namespace detail {

inline constexpr auto getPrincipalCurvature = [](auto&& p) -> decltype(auto) {
    return p.principalCurvature();
};

struct PrincipalCurvaturesView :
        std::ranges::range_adaptor_closure<PrincipalCurvaturesView>
{
    template<PrincipalCurvaturesRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        if constexpr (requires {
                          {
                              std::forward<R>(r).principalCurvatures()
                          } -> std::ranges::range;
                      }) {
            return std::forward<R>(r).principalCurvatures();
        }
        else {
            return std::forward<R>(r) |
                   std::views::transform(getPrincipalCurvature);
        }
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of their principal curvatures.
 *
 * When applied directly to an ElementContainerProxy (e.g. `mesh.vertices() | views::principalCurvatures`),
 * it directly returns a contiguous ComponentView over the SoA buffer
 * without proxy overhead.
 *
 * When applied to a filtered or transformed range (e.g. `mesh.vertices() | views::selected | views::principalCurvatures`),
 * it transforms each element to its principal curvature reference.
 *
 * @ingroup views
 */
inline constexpr detail::PrincipalCurvaturesView principalCurvatures;

template<>
inline constexpr auto component<CompId::PRINCIPAL_CURVATURE> =
    principalCurvatures;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_PRINCIPAL_CURVATURES_H
