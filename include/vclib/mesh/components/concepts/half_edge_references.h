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

#ifndef VCL_MESH_COMPONENTS_CONCEPTS_HALF_EDGE_REFERENCES_H
#define VCL_MESH_COMPONENTS_CONCEPTS_HALF_EDGE_REFERENCES_H

#include "component.h"

namespace vcl::comp {

/**
 * @brief HasHalfEdgeReferences concept
 *
 * This concept is satisfied only if a class has the following member functions:
 * - next()
 * - prev()
 * - twin()
 */
template<typename T>
concept HasHalfEdgeReferences =
	requires(T o)
{
	typename T::HalfEdgeType;
	typename T::VertexType;
	typename T::FaceType;
	o.next();
	o.prev();
	o.twin();
};

} // namespace vcl::comp

#endif // VCL_MESH_COMPONENTS_CONCEPTS_HALF_EDGE_REFERENCES_H
