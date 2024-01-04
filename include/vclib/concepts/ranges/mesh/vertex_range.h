/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2024                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 3 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCLIB_CONCEPTS_RANGES_MESH_VERTEX_RANGE_H
#define VCLIB_CONCEPTS_RANGES_MESH_VERTEX_RANGE_H

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
    VertexConcept<typename std::decay_t<std::remove_pointer_t<
        typename std::ranges::iterator_t<Rng>::value_type>>>;

} // namespace vcl

#endif // VCLIB_CONCEPTS_RANGES_MESH_VERTEX_RANGE_H
