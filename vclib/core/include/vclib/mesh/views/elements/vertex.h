// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENTS_VERTEX_H
#define VCL_MESH_VIEWS_ELEMENTS_VERTEX_H

#include "../element.h"

#include <vclib/mesh/elements/vertex.h>

#include <ranges>

namespace vcl {

/**
 * @brief Concept that checks whether Rng is a valid range of vertices.
 *
 * @ingroup element_concepts
 */
template<typename Rng>
concept VertexRangeConcept =
    std::ranges::range<Rng> &&
    VertexConcept<typename std::ranges::iterator_t<Rng>::value_type>;

namespace views {

namespace detail {

struct VerticesView : std::ranges::range_adaptor_closure<VerticesView>
{
    constexpr VerticesView() = default;

    template<typename R>
    requires requires (R&& r) {
        { r.vertices() } -> std::ranges::range;
    }
    constexpr auto operator()(R&& r) const
    {
        return r.vertices();
    }

    template<typename R>
    requires (!std::ranges::range<R>) && requires (R&& r) {
        { r.vertices() } -> std::ranges::range;
    }
    friend constexpr auto operator|(R&& r, VerticesView)
    {
        return r.vertices();
    }
};

} // namespace detail

/**
 * @brief A view adaptor that allows to iterate over the Vertex elements of an
 * object.
 *
 * Can be applied to:
 * - Meshes: `mesh | views::vertices`
 * - Face / Element with VertexReferences: `face | views::vertices`
 *
 * Supports both pipe syntax (`obj | views::vertices`) and function call syntax
 * (`views::vertices(obj)`).
 *
 * @ingroup views
 */
inline constexpr detail::VerticesView vertices;

template<>
inline constexpr auto elements<ElemId::VERTEX> = vertices;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_VERTEX_H
