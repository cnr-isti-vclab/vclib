// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENTS_WEDGE_H
#define VCL_MESH_VIEWS_ELEMENTS_WEDGE_H

#include "../element.h"

#include <vclib/mesh/components/wedges/wedge_proxy.h>

#include <ranges>

namespace vcl {

/**
 * @brief Concept that checks whether Rng is a valid range of wedges.
 *
 * @ingroup element_concepts
 */
template<typename Rng>
concept WedgeRangeConcept =
    std::ranges::range<Rng> &&
    WedgeConcept<typename std::ranges::iterator_t<Rng>::value_type>;

namespace views {

namespace detail {

struct WedgesView : std::ranges::range_adaptor_closure<WedgesView>
{
    constexpr WedgesView() = default;

    template<typename R>
    requires requires (R&& r) { { r.wedges() } -> std::ranges::range; }
    constexpr auto operator()(R&& r) const
    {
        return r.wedges();
    }

    template<typename R>
    requires (!std::ranges::range<R>) &&
             requires (R&& r) { { r.wedges() } -> std::ranges::range; }
    friend constexpr auto operator|(R&& r, WedgesView)
    {
        return r.wedges();
    }
};

} // namespace detail

/**
 * @brief A view adaptor that allows to iterate over the Wedge elements of an object.
 *
 * Can be applied to:
 * - Face / Element with Wedges: `face | views::wedges`
 *
 * Supports both pipe syntax (`face | views::wedges`) and function call syntax
 * (`views::wedges(face)`).
 *
 * @ingroup views
 */
inline constexpr detail::WedgesView wedges;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_WEDGE_H

