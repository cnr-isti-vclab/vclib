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

#ifndef VCL_VIEWS_MESH_ELEMENTS_EDGE_H
#define VCL_VIEWS_MESH_ELEMENTS_EDGE_H

#include <vclib/concepts/mesh.h>

namespace vcl::views {
namespace internal {

template<typename T>
concept CleanEdgeMeshConcept = EdgeMeshConcept<std::remove_cvref_t<T>>;

struct EdgesView
{
	constexpr EdgesView() = default;

	template <CleanEdgeMeshConcept R>
	friend constexpr auto operator|(R&& r, EdgesView)
	{
		return r.edges();
	}
};

} // namespace vcl::views::internal

/**
 * @brief A view that allows to iterate overt the Edge elements of an object.
 *
 * This view can be applied to objects having type that satisfies the EdgeMeshConcept
 *
 * @ingroup views
 */
inline constexpr internal::EdgesView edges;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_ELEMENTS_EDGE_H
