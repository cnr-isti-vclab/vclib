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

#ifndef VCL_MESH_VIEWS_ELEMENTS_H
#define VCL_MESH_VIEWS_ELEMENTS_H

#include "pipe.h"

#include <vclib/mesh/requirements.h>

namespace vcl {
namespace internal {

template<typename T>
concept CleanMeshConcept = MeshConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanFaceMeshConcept = FaceMeshConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanFaceConcept = FaceConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanEdgeMeshConcept = EdgeMeshConcept<RemoveConstRef<T>>;

template<typename T>
concept CleanDcelMeshConcept = DcelMeshConcept<RemoveConstRef<T>>;

struct VerticesViewClosure
{
	constexpr VerticesViewClosure(){}

	template <CleanMeshConcept R>
	constexpr auto operator()(R && r) const
	{
		return r.vertices();
	}

	template <CleanFaceConcept R>
	constexpr auto operator()(R && r) const
	{
		return r.vertices();
	}
};

struct FacesViewClosure
{
	constexpr FacesViewClosure(){}

	template <CleanFaceMeshConcept R>
	constexpr auto operator()(R && r) const
	{
		return r.faces();
	}
};

struct EdgesViewClosure
{
	constexpr EdgesViewClosure(){}

	template <CleanEdgeMeshConcept R>
	constexpr auto operator()(R && r) const
	{
		return r.edges();
	}
};

struct HalfEdgesViewClosure
{
	constexpr HalfEdgesViewClosure(){}

	template <CleanDcelMeshConcept R>
	constexpr auto operator()(R && r) const
	{
		return r.halfEdges();
	}
};

} // namespace vcl::internal

namespace views {

internal::VerticesViewClosure vertices;
internal::FacesViewClosure faces;
internal::EdgesViewClosure edges;
internal::HalfEdgesViewClosure halfEdges;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_H
