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

#ifndef VCL_IO_STL_SAVE_H
#define VCL_IO_STL_SAVE_H

#include <vclib/io/file_mesh_info.h>

namespace vcl::io {

template<MeshConcept MeshType>
void saveStl(
	const MeshType&    m,
	const std::string& filename,
	bool               magicsMode = false,
	bool               binary     = true);

template<MeshConcept MeshType>
void saveStl(
	const MeshType&     m,
	const std::string&  filename,
	const FileMeshInfo& info,
	bool                magicsMode = false,
	bool                binary     = true);

} // namespace vcl::io

#include "save.cpp"

#endif // VCL_IO_STL_SAVE_H
