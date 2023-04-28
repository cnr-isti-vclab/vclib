/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2023                                                    *
 * Alessandro Muntoni                                                        *
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

#ifndef VCL_VIEWS_MESH_COMPONENTS_ADJ_VERTICES_H
#define VCL_VIEWS_MESH_COMPONENTS_ADJ_VERTICES_H

#include <vclib/concepts/mesh.h>
#include <vclib/types.h>

namespace vcl::views {

namespace internal {

template<typename T>
concept CleanAdjVerticesConcept = comp::HasAdjacentVertices<RemoveConstRef<T>>;

struct AdjVerticesView
{
	constexpr AdjVerticesView() = default;

	template <CleanAdjVerticesConcept R>
	friend constexpr auto operator|(R&& r, AdjVerticesView)
	{
		if constexpr(IsPointer<R>)
			return r->adjVertices();
		else
			return r.adjVertices();
	}
};

} // namespace vcl::views::internal

/**
 * @brief The adjVertices view allows to obtain a view that access to the adjacent vertices of
 * the object that has been piped. Every object having type that satisfies the HasAdjacentVertices
 * concept can be applied to this view.
 *
 * Resulting adjacent faces will be pointers to Vertices, that may be `nullptr`.
 * If you are interested only on the not-null pointers, you can use the `notNull` view:
 *
 * @code{.cpp}
 * for (auto* av: v | views::adjVertices | views::notNull) { ... }
 * @endcode
 *
 * @ingroup views
 */
inline constexpr internal::AdjVerticesView adjVertices;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_COMPONENTS_ADJ_VERTICES_H
