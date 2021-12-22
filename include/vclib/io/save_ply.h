/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual Computing Library                                        o     o   *
 *                                                                 _  O  _   *
 * Copyright(C) 2021-2022                                           \/)\/    *
 * Visual Computing Lab                                            /\/|      *
 * ISTI - Italian National Research Council                           |      *
 *                                                                    \      *
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

#ifndef VCL_IO_SAVE_PLY_H
#define VCL_IO_SAVE_PLY_H

#include <vclib/exception/io_exception.h>

#include "ply/ply.h"
#include "ply/ply_extra.h"
#include "ply/ply_face.h"
#include "ply/ply_vertex.h"

namespace vcl::io {

template<typename MeshType>
void savePly(const MeshType& m, const std::string& filename, bool binary = true);

template<typename MeshType>
void savePly(
	const MeshType&     m,
	const std::string&  filename,
	const FileMeshInfo& info,
	bool                binary = true);

} // namespace vcl::io

#include "save_ply.cpp"

#endif // VCL_IO_SAVE_PLY_H
