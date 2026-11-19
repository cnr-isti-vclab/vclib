// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_QUALITY_H
#define VCL_MESH_VIEWS_COMPONENTS_QUALITY_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasQualityMember = requires(T&& t) {
    { t.quality() };
};

template<typename R>
concept QualitiesRangeConcept =
    std::ranges::range<R> &&
    (requires(R&& r) {
        { std::forward<R>(r).qualities() } -> std::ranges::range;
    } || HasQualityMember<std::ranges::range_reference_t<R>>);

template<typename R>
concept QualityRangeConcept = QualitiesRangeConcept<R>;

namespace views {

namespace detail {

inline constexpr auto getQuality = [](auto&& p) -> decltype(auto) {
    return p.quality();
};

struct QualitiesView : std::ranges::range_adaptor_closure<QualitiesView>
{
    template<QualitiesRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        if constexpr (requires {
                          {
                              std::forward<R>(r).qualities()
                          } -> std::ranges::range;
                      }) {
            return std::forward<R>(r).qualities();
        }
        else {
            return std::forward<R>(r) | std::views::transform(getQuality);
        }
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of their qualities.
 *
 * When applied directly to an ElementContainerProxy (e.g. `mesh.vertices() | views::qualities` or
 * `mesh.faces() | views::qualities`), it directly returns a contiguous ComponentView over the SoA buffer
 * without proxy overhead.
 *
 * When applied to a filtered or transformed range (e.g. `mesh.vertices() | views::selected | views::qualities`),
 * it transforms each element to its quality reference.
 *
 * @ingroup views
 */
inline constexpr detail::QualitiesView qualities;
inline constexpr auto quality = qualities;

template<>
inline constexpr auto component<CompId::QUALITY> = qualities;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_QUALITY_H
