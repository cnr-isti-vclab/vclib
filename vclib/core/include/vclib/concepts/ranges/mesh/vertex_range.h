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

#ifndef VCL_CONCEPTS_RANGES_MESH_VERTEX_RANGE_H
#define VCL_CONCEPTS_RANGES_MESH_VERTEX_RANGE_H

#include <vclib/concepts/mesh/elements/vertex.h>
#include <vclib/concepts/pointers.h>
#include <vclib/concepts/ranges/range.h>

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

} // namespace vcl

#endif // VCL_CONCEPTS_RANGES_MESH_VERTEX_RANGE_H
