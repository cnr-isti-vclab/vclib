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

#include "parent_mesh_pointer.h"

namespace vcl::comp {

template<typename MeshType>
ParentMeshPointer<MeshType>::ParentMeshPointer()
{
}

template<typename MeshType>
constexpr MeshType* ParentMeshPointer<MeshType>::parentMesh()
{
	return parent;
}

template<typename MeshType>
constexpr const MeshType* ParentMeshPointer<MeshType>::parentMesh() const
{
	return parent;
}

template<typename MeshType>
void ParentMeshPointer<MeshType>::setParentMesh(void* parentMesh)
{
	parent = static_cast<MeshType*>(parentMesh);
}

} // namespace vcl::comp
