// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENTS_EDGE_H
#define VCL_MESH_VIEWS_ELEMENTS_EDGE_H

#include "../element.h"

#include <vclib/mesh/elements/edge.h>

#include <ranges>

namespace vcl {

/**
 * @brief Concept that checks whether Rng is a valid range of edges.
 *
 * @ingroup element_concepts
 */
template<typename Rng>
concept EdgeRangeConcept =
    std::ranges::range<Rng> &&
    EdgeConcept<typename std::ranges::iterator_t<Rng>::value_type>;

namespace views {

namespace detail {

struct EdgesView : std::ranges::range_adaptor_closure<EdgesView>
{
    constexpr EdgesView() = default;

    template<typename R>
    requires requires (R&& r) {
        { r.edges() } -> std::ranges::range;
    }
    constexpr auto operator()(R&& r) const
    {
        return r.edges();
    }

    template<typename R>
    requires (!std::ranges::range<R>) && requires (R&& r) {
        { r.edges() } -> std::ranges::range;
    }
    friend constexpr auto operator|(R&& r, EdgesView)
    {
        return r.edges();
    }
};

} // namespace detail

/**
 * @brief A view adaptor that allows to iterate over the Edge elements of an
 * object.
 *
 * Can be applied to Meshes: `mesh | views::edges`
 *
 * Supports both pipe syntax (`mesh | views::edges`) and function call syntax
 * (`views::edges(mesh)`).
 *
 * @ingroup views
 */
inline constexpr detail::EdgesView edges;

template<>
inline constexpr auto elements<ElemId::EDGE> = edges;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_EDGE_H
