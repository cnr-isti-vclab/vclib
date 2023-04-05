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
 * GNU General Public License (http://www.gnu.ostd::ranges/licenses/gpl.txt)          *
 * for more details.                                                         *
 ****************************************************************************/

#ifndef VCL_MESH_VIEWS_ELEMENTS_H
#define VCL_MESH_VIEWS_ELEMENTS_H

#include "pipe.h"

#include <vclib/mesh/requirements.h>

namespace vcl {
namespace internal {

struct VerticesViewClosure
{
	constexpr VerticesViewClosure(){}

	template <typename R>
	constexpr auto operator()(R && r) const
	{
		return r.vertices();
	}
};

struct FacesViewClosure
{
	constexpr FacesViewClosure(){}

	template <typename R>
	constexpr auto operator()(R && r) const
	{
		return r.faces();
	}
};

} // namespace vcl::internal

namespace views {

internal::VerticesViewClosure vertices;
internal::FacesViewClosure faces;

} // namespace vcl::views

} // namespace vcl

#endif // VCL_MESH_VIEWS_ELEMENTS_H
