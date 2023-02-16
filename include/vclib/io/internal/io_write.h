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

#ifndef VCL_IO_WRITE_H
#define VCL_IO_WRITE_H

#include "io_utils.h"

namespace vcl::io::internal {

// write/bin

template<typename T>
void writeChar(std::ofstream& file, T p, bool bin = true, bool isColor = false);

template<typename T>
void writeUChar(std::ofstream& file, T p, bool bin = true, bool isColor = false);

template<typename T>
void writeShort(std::ofstream& file, T p, bool bin = true, bool isColor = false);

template<typename T>
void writeUShort(std::ofstream& file, T p, bool bin = true, bool isColor = false);

template<typename T>
void writeInt(std::ofstream& file, T p, bool bin = true, bool isColor = false);

template<typename T>
void writeUInt(std::ofstream& file, T p, bool bin = true, bool isColor = false);

template<typename T>
void writeFloat(std::ofstream& file, const T& p, bool bin = true, bool isColor = false);

template<typename T>
void writeDouble(std::ofstream& file, const T& p, bool bin = true, bool isColor = false);

template<typename T>
void writeProperty(
	std::ofstream& file,
	const T&       p,
	PropertyType   type,
	bool           bin     = true,
	bool           isColor = false);

} // namespace vcl::io::internal

#include "io_write.cpp"

#endif // VCL_IO_WRITE_H
