// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_MESH_VIEWS_ELEMENTS_VERTEX_H
#define VCL_MESH_VIEWS_ELEMENTS_VERTEX_H

#include <vclib/mesh/requirements/vertex_requirements.h>

namespace vcl {

/**
 * @brief The VertexRangeConcept evaluates to true if Rng is a valid Range on
 * Vertices.
 *
 * This means that Rng must be a Range of VertexConcept: the iterated type must
 * satisfy the VertexConcept.
 *
 * @ingroup vertex_concepts
 */
template<typename Rng>
concept VertexRangeConcept =
    Range<Rng> &&
    VertexConcept<typename std::ranges::iterator_t<Rng>::value_type>;

/**
 * @brief The VertexPointerRangeConcept evaluates to true if Rng is a valid
 * Range on Vertex Pointers.
 *
 * This means that Rng must be a Range of pointers to a type that satisfy the
 * VertexConcept.
 *
 * @ingroup vertex_concepts
 */
template<typename Rng>
concept VertexPointerRangeConcept =
    Range<Rng> &&
    IsPointer<typename std::ranges::iterator_t<Rng>::value_type> &&
    VertexConcept<typename std::decay_t<
        RemovePtr<typename std::ranges::iterator_t<Rng>::value_type>>>;

namespace views {
namespace detail {

struct VerticesView
{
    constexpr VerticesView() = default;

    template<MeshConcept R>
    friend constexpr auto operator|(R&& r, VerticesView)
    {
        return r.vertices();
    }

    template<comp::HasVertexReferences R>
    friend constexpr auto operator|(R&& r, VerticesView)
    {
        return r.vertices();
    }
};

} // namespace detail

/**
 * @brief A view that allows to iterate over the Vertex elements of an object.
 *
 * This view can be applied to objects having type that satisfies one of the
 * following concepts:
 * - MeshConcept
 * - HasVertexReferences
 *
 * @ingroup views
 */
inline constexpr detail::VerticesView vertices;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_VERTEX_H
