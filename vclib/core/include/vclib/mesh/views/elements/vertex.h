/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef VCL_MESH_VIEWS_ELEMENTS_VERTEX_H
#define VCL_MESH_VIEWS_ELEMENTS_VERTEX_H

#include <vclib/mesh/concepts/components.h>
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
