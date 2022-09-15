/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
 * Alessandro Muntoni                                                        *
 * VCLab - ISTI - Italian National Research Council                          *
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

#ifndef VCL_IO_PLY_EDGE_H
#define VCL_IO_PLY_EDGE_H

#include <fstream>
#include <vclib/misc/tokenizer.h>

#include "ply_header.h"

namespace vcl::io::ply {

template<typename MeshType>
void saveEdges(std::ofstream& file, const PlyHeader& header, const MeshType& mesh);


} // namespace vcl::ply

#include "ply_edge.cpp"

#endif // VCL_IO_PLY_EDGE_H
