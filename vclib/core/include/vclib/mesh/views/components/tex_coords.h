// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_COMPONENTS_TEX_COORDS_H
#define VCL_MESH_VIEWS_COMPONENTS_TEX_COORDS_H

#include "../component.h"

#include <vclib/base.h>

#include <ranges>

namespace vcl {

template<typename T>
concept HasTexCoordMember = requires(T&& t) {
    { t.texCoord() };
};

template<typename R>
concept TexCoordRangeConcept =
    std::ranges::range<R> &&
    (requires(R&& r) {
        { std::forward<R>(r).texCoords() } -> std::ranges::range;
    } || HasTexCoordMember<std::ranges::range_reference_t<R>>);

namespace views {

namespace detail {

inline constexpr auto getTexCoord = [](auto&& p) -> decltype(auto) {
    return p.texCoord();
};

struct TexCoordsView : std::ranges::range_adaptor_closure<TexCoordsView>
{
    template<TexCoordRangeConcept R>
    constexpr auto operator()(R&& r) const
    {
        if constexpr (requires {
                          {
                              std::forward<R>(r).texCoords()
                          } -> std::ranges::range;
                      }) {
            return std::forward<R>(r).texCoords();
        }
        else {
            return std::forward<R>(r) | std::views::transform(getTexCoord);
        }
    }
};

} // namespace detail

/**
 * @brief View adaptor that gives access to the texture coordinates.
 *
 * When applied directly to an ElementContainerProxy (e.g.
 * `mesh.vertices() | views::texCoords`), it directly returns a contiguous
 * ComponentView over the SoA buffer without proxy overhead.
 *
 * When applied to a filtered or transformed range (e.g.
 * `mesh.vertices() | views::selected | views::texCoords`), it transforms each
 * element to its texture coordinate reference.
 *
 * When applied to a range of wedge elements (e.g.
 * `face.wedges() | views::texCoords` or `face | views::wedges | views::texCoords`),
 * it returns the range of its wedge texture coordinates.
 *
 * @ingroup views
 */
inline constexpr detail::TexCoordsView texCoords;

template<>
inline constexpr auto component<CompId::TEX_COORD> = texCoords;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_COMPONENTS_TEX_COORDS_H
