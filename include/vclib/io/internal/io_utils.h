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

#ifndef VCL_IO_UTILS_H
#define VCL_IO_UTILS_H

#include <fstream>

#include "../file_mesh_info.h"

#include <vclib/misc/tokenizer.h>

namespace vcl::io::internal {

typedef enum { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, NONE } PropertyType;

std::ofstream saveFileStream(const std::string& filename, const std::string& ext);

std::ifstream loadFileStream(const std::string& filename);

template<MeshConcept MeshType>
void enableOptionalComponents(FileMeshInfo& info, MeshType& m);

vcl::Tokenizer nextNonEmptyTokenizedLine(std::ifstream& file, char separator = ' ');
vcl::Tokenizer nextNonEmptyTokenizedLineNoThrow(std::ifstream& file, char separator = ' ');

} // namespace vcl::io::internal

#include "io_utils.cpp"

#endif // VCL_IO_UTILS_H
