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

#ifndef VCL_MESH_VIEWS_ELEMENTS_EDGE_H
#define VCL_MESH_VIEWS_ELEMENTS_EDGE_H

#include <vclib/mesh/requirements/edge_requirements.h>

namespace vcl {

/**
 * @brief The EdgeRangeConcept evaluates to true if Rng is a valid Range
 * on Edges.
 *
 * This means that Rng must be a Range of EdgeConcept: the iterated type
 * must satisfy the EdgeConcept.
 *
 * @ingroup edge_concepts
 */
template<typename Rng>
concept EdgeRangeConcept =
    Range<Rng> &&
    EdgeConcept<typename std::ranges::iterator_t<Rng>::value_type>;

/**
 * @brief The EdgePointerRangeConcept evaluates to true if Rng is a valid
 * Range on Edge Pointers.
 *
 * This means that Rng must be a Range of pointers to a type that satisfy the
 * EdgeConcept.
 *
 * @ingroup edge_concepts
 */
template<typename Rng>
concept EdgePointerRangeConcept =
    Range<Rng> &&
    IsPointer<typename std::ranges::iterator_t<Rng>::value_type> &&
    EdgeConcept<typename std::decay_t<
        RemovePtr<typename std::ranges::iterator_t<Rng>::value_type>>>;

namespace views {
namespace detail {

struct EdgesView
{
    constexpr EdgesView() = default;

    template<EdgeMeshConcept R>
    friend constexpr auto operator|(R&& r, EdgesView)
    {
        return r.edges();
    }
};

} // namespace detail

/**
 * @brief A view that allows to iterate overt the Edge elements of an object.
 *
 * This view can be applied to objects having type that satisfies the
 * EdgeMeshConcept.
 *
 * @ingroup views
 */
inline constexpr detail::EdgesView edges;

} // namespace views
} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_EDGE_H
