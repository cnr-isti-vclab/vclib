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

#include "generic_drawable_mesh.h"

namespace vcl {

inline GenericDrawableMesh::GenericDrawableMesh()
{
}

template<MeshConcept MeshType>
GenericDrawableMesh::GenericDrawableMesh(const MeshType &m) :
		mrs(m)
{
}

inline const MeshRenderSettings& GenericDrawableMesh::renderSettings() const
{
	return mrs;
}

inline void GenericDrawableMesh::setRenderSettings(const MeshRenderSettings& rs)
{
	mrs = rs;
}

} // namespace vcl
