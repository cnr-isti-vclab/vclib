/*****************************************************************************
 * VCLib                                                             o o     *
 * Visual and Computer Graphics Library                            o     o   *
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

#ifndef VCL_IO_LOAD_PLY_H
#define VCL_IO_LOAD_PLY_H

#include <vclib/exception/io_exception.h>
#include "ply/ply.h"
#include "ply/ply_vertex.h"
#include "ply/ply_face.h"

// todo: remove this and make template
#include <vclib/mesh/mesh.h>

namespace vcl::io {

template <typename MeshType>
MeshType loadPly(const std::string& filename, bool enableOptionalComponents = true);

template <typename MeshType>
MeshType loadPly(const std::string& filename, FileMeshInfo& loadedInfo, bool enableOptionalComponents = true);

template <typename MeshType>
void loadPly(MeshType& m, const std::string& filename, bool enableOptionalComponents = true);

template <typename MeshType>
void loadPly(MeshType& m, const std::string& filename, FileMeshInfo& loadedInfo, bool enableOptionalComponents = true);

}

#include "load_ply.cpp"

#endif // VCL_IO_LOAD_PLY_H
