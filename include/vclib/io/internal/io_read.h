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

#ifndef VCL_IO_INTERNAL_IO_READ_H
#define VCL_IO_INTERNAL_IO_READ_H

#include "io_utils.h"

namespace vcl::io::internal {

// read/bin

template<typename T>
T readChar(std::ifstream& file);

template<typename T>
T readUChar(std::ifstream& file);

template<typename T>
T readShort(std::ifstream& file);

template<typename T>
T readUShort(std::ifstream& file);

template<typename T>
T readInt(std::ifstream& file);

template<typename T>
T readUInt(std::ifstream& file);

template<typename T>
T readFloat(std::ifstream& file, bool isColor = false);

template<typename T>
T readDouble(std::ifstream& file, bool isColor = false);

template<typename T>
T readProperty(std::ifstream& file, PropertyType type, bool isColor = false);

// read/txt

template<typename T>
T readChar(vcl::Tokenizer::iterator& token);

template<typename T>
T readUChar(vcl::Tokenizer::iterator& token);

template<typename T>
T readShort(vcl::Tokenizer::iterator& token);

template<typename T>
T readUShort(vcl::Tokenizer::iterator& token);

template<typename T>
T readInt(vcl::Tokenizer::iterator& token);

template<typename T>
T readUInt(vcl::Tokenizer::iterator& token);

template<typename T>
T readFloat(vcl::Tokenizer::iterator& token, bool isColor = false);

template<typename T>
T readDouble(vcl::Tokenizer::iterator& token, bool isColor = false);

template<typename T>
T readProperty(vcl::Tokenizer::iterator& token, PropertyType type, bool isColor = false);

} // vcl::io::internal

#include "io_read.cpp"

#endif // VCL_IO_INTERNAL_IO_READ_H
