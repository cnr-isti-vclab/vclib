// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_NORMALS_H
#define VCL_MESH_VIEWS_COMPONENTS_NORMALS_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasNormalMember = requires(T&& t) {
    { t.normal() };
};

template<typename R>
concept NormalRangeConcept =
    std::ranges::range<R> &&
    (requires(R&& r) {
        { std::forward<R>(r).normals() } -> std::ranges::range;
    } || HasNormalMember<std::ranges::range_reference_t<R>>);

namespace views {

namespace detail {

inline constexpr auto getNormal = [](auto&& p) -> decltype(auto) {
    return p.normal();
};

struct NormalsView : std::ranges::range_adaptor_closure<NormalsView>
{
    template<NormalRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        if constexpr (requires {
                          {
                              std::forward<R>(r).normals()
                          } -> std::ranges::range;
                      }) {
            return std::forward<R>(r).normals();
        }
        else {
            return std::forward<R>(r) | std::views::transform(getNormal);
        }
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of their normals.
 *
 * When applied directly to an ElementContainerProxy (e.g. `mesh.vertices() | views::normals` or
 * `mesh.faces() | views::normals`), it directly returns a contiguous ComponentView over the SoA buffer
 * without proxy overhead.
 *
 * When applied to a filtered or transformed range (e.g. `mesh.vertices() | views::selected | views::normals`),
 * it transforms each element to its normal reference.
 *
 * @ingroup views
 */
inline constexpr detail::NormalsView normals;

template<>
inline constexpr auto component<CompId::NORMAL> = normals;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_NORMALS_H
