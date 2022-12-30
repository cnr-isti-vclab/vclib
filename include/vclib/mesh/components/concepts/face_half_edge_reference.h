/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_FACE_HALF_EDGE_REFERENCE_H
#define VCL_MESH_COMPONENTS_CONCEPTS_FACE_HALF_EDGE_REFERENCE_H

#include <vclib/misc/types.h>

namespace vcl::comp {

/**
 * @brief HasFaceHalfEdgeReference concept
 *
 * This concept is satisfied only if a class has the following member functions:
 * - outerHalfEdge()
 * - numberHoles(), which returns an uint
 * - innerHalfEdge(uint)
 */
template<typename T>
concept HasFaceHalfEdgeReference = requires(T o)
{
	typename T::HalfEdgeType;
	typename T::VertexType;
	o.outerHalfEdge();
	{ o.numberHoles() } -> std::same_as<uint>;
	o.innerHalfEdge(uint());
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_FACE_HALF_EDGE_REFERENCE_H
