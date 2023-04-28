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

#ifndef VCL_VIEWS_MESH_ELEMENTS_H
#define VCL_VIEWS_MESH_ELEMENTS_H

#include <vclib/concepts/mesh.h>

namespace vcl::views {
namespace internal {

template<typename T>
concept CleanMeshConcept = MeshConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanFaceMeshConcept = FaceMeshConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanVertexPtrsConcept = comp::HasVertexPointers<RemoveConstRef<T>>;

template<typename T>
concept CleanEdgeMeshConcept = EdgeMeshConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanDcelMeshConcept = DcelMeshConcept<RemoveConstRef<T>>;

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

struct FacesView
{
	constexpr FacesView() = default;

	template <CleanFaceMeshConcept R>
	friend constexpr auto operator|(R&& r, FacesView)
	{
		return r.faces();
	}
};

struct EdgesView
{
	constexpr EdgesView() = default;

	template <CleanEdgeMeshConcept R>
	friend constexpr auto operator|(R&& r, EdgesView)
	{
		return r.edges();
	}
};

struct HalfEdgesView
{
	constexpr HalfEdgesView() = default;

	template <CleanDcelMeshConcept R>
	friend constexpr auto operator|(R&& r, HalfEdgesView)
	{
		return r.halfEdges();
	}
};

} // namespace vcl::views::internal

inline constexpr internal::VerticesView vertices;
inline constexpr internal::FacesView faces;
inline constexpr internal::EdgesView edges;
inline constexpr internal::HalfEdgesView halfEdges;

} // namespace vcl::views

#endif // VCL_VIEWS_MESH_ELEMENTS_H
