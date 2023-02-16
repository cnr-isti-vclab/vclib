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

#ifndef VCL_PLY_VERTEX_H
#define VCL_PLY_VERTEX_H

#include "../file_mesh_info.h"
#include "ply_header.h"
#include <fstream>

namespace vcl::io::ply {

template<MeshConcept MeshType>
void saveVertices(std::ofstream& file, const PlyHeader& header, const MeshType& mesh);

template<MeshConcept MeshType>
void loadVertices(std::ifstream& file, const PlyHeader& header, MeshType& mesh);

} // namespace vcl

#include "ply_vertex.cpp"

#endif // VCL_PLY_VERTEX_H
