// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_GAUSSIAN_SPLATS_H
#define VCL_MESH_VIEWS_COMPONENTS_GAUSSIAN_SPLATS_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasGaussianSplatMember = requires(T&& t) {
    { t.gaussianSplat() };
};

template<typename R>
concept GaussianSplatsRangeConcept =
    std::ranges::range<R> && (requires(R&& r) {
        { std::forward<R>(r).gaussianSplats() } -> std::ranges::range;
    } || HasGaussianSplatMember<std::ranges::range_reference_t<R>>);

template<typename R>
concept GaussianSplatRangeConcept = GaussianSplatsRangeConcept<R>;

namespace views {

namespace detail {

inline constexpr auto getGaussianSplat = [](auto&& p) -> decltype(auto) {
    return p.gaussianSplat();
};

struct GaussianSplatsView :
        std::ranges::range_adaptor_closure<GaussianSplatsView>
{
    template<GaussianSplatsRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        if constexpr (requires {
                          {
                              std::forward<R>(r).gaussianSplats()
                          } -> std::ranges::range;
                      }) {
            return std::forward<R>(r).gaussianSplats();
        }
        else {
            return std::forward<R>(r) |
                   std::views::transform(getGaussianSplat);
        }
    }
};

} // namespace detail

/**
 * @brief View adaptor that transforms a range of elements into a range of their
 * gaussian splats.
 *
 * When applied directly to an ElementContainerProxy (e.g. `mesh.vertices() |
 * views::gaussianSplats`), it directly returns a contiguous ComponentView over
 * the SoA buffer without proxy overhead.
 *
 * When applied to a filtered or transformed range (e.g. `mesh.vertices() |
 * views::selected | views::gaussianSplats`), it transforms each element to its
 * gaussian splat reference.
 *
 * @ingroup views
 */
inline constexpr detail::GaussianSplatsView gaussianSplats;
inline constexpr auto                      gaussianSplat = gaussianSplats;

template<>
inline constexpr auto component<CompId::GAUSSIAN_SPLAT> = gaussianSplats;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_GAUSSIAN_SPLATS_H
