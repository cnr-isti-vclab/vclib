// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_TANGENTS_H
#define VCL_MESH_VIEWS_COMPONENTS_TANGENTS_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasTangentMember = requires(T&& t) {
    { t.tangent() };
};

template<typename R>
concept TangentsRangeConcept =
    std::ranges::range<R> &&
    HasTangentMember<std::ranges::range_reference_t<R>>;

template<typename R>
concept TangentRangeConcept = TangentsRangeConcept<R>;

namespace views {

namespace detail {

inline constexpr auto getTangent = [](auto&& p) -> decltype(auto) {
    return p.tangent();
};

struct TangentsView : std::ranges::range_adaptor_closure<TangentsView>
{
    template<TangentsRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        return std::forward<R>(r) | std::views::transform(getTangent);
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of their tangents.
 *
 * When applied to a range of elements (e.g. `mesh.vertices() | views::tangents` or
 * `mesh.vertices() | views::selected | views::tangents`), it transforms each element to its tangent reference.
 *
 * @ingroup views
 */
inline constexpr detail::TangentsView tangents;
inline constexpr auto tangent = tangents;

template<>
inline constexpr auto component<CompId::TANGENT> = tangents;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_TANGENTS_H
