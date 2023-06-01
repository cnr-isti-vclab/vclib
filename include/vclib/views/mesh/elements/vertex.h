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

#ifndef VCL_VIEWS_MESH_ELEMENTS_VERTEX_H
#define VCL_VIEWS_MESH_ELEMENTS_VERTEX_H

#include <vclib/concepts/mesh.h>

namespace vcl::views {
namespace internal {

template<typename T>
concept CleanMeshConcept = MeshConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanVertexPtrsConcept = comp::HasVertexPointers<RemoveConstRef<T>>;

struct VerticesView
{
	constexpr VerticesView() = default;

	template <CleanMeshConcept R>
	friend constexpr auto operator|(R&& r, VerticesView)
	{
		return r.vertices();
	}

	template <CleanVertexPtrsConcept R>
	friend constexpr auto operator|(R&& r, VerticesView)
	{
		return r.vertices();
	}
};

} // namespace vcl::views::internal

/**
 * @brief A view that allows to iterate over the Vertex elements of an object.
 *
 * This view can be applied to objects having type that satisfies one of the following concepts:
 * - MeshConcept
 * - HasVertexPointers
 *
 * @ingroup views
 */
inline constexpr internal::VerticesView vertices;

} // namespace vcl::views



#endif // VCL_VIEWS_MESH_ELEMENTS_VERTEX_H