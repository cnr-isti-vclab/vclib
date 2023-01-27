/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2022                                                    *
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

#include "io_write.h"

namespace vcl::io::internal {

template<typename T>
void writeChar(std::ofstream& file, T p, bool bin, bool isColor)
{
	if (isColor && !std::is_integral<T>::value)
		p *= 255;
	char tmp = p;
	if (bin)
		file.write((const char*) &tmp, 1);
	else
		file << (int) p << " "; // cast necessary to not print the ascii char
}

template<typename T>
void writeUChar(std::ofstream& file, T p, bool bin, bool isColor)
{
	if (isColor && !std::is_integral<T>::value)
		p *= 255;
	unsigned char tmp = p;
	if (bin)
		file.write((const char*) &tmp, 1);
	else
		file << (uint) p << " "; // cast necessary to not print the ascii char
}

template<typename T>
void writeShort(std::ofstream& file, T p, bool bin, bool isColor)
{
	if (isColor && !std::is_integral<T>::value)
		p *= 255;
	short tmp = p;
	if (bin)
		file.write((const char*) &tmp, 2);
	else
		file << tmp << " ";
}

template<typename T>
void writeUShort(std::ofstream& file, T p, bool bin, bool isColor)
{
	if (isColor && !std::is_integral<T>::value)
		p *= 255;
	unsigned short tmp = p;
	if (bin)
		file.write((const char*) &tmp, 2);
	else
		file << tmp << " ";
}

template<typename T>
void writeInt(std::ofstream& file, T p, bool bin, bool isColor)
{
	if (isColor && !std::is_integral<T>::value)
		p *= 255;
	int tmp = p;
	if (bin)
		file.write((const char*) &tmp, 4);
	else
		file << tmp << " ";
}

template<typename T>
void writeUInt(std::ofstream& file, T p, bool bin, bool isColor)
{
	if (isColor && !std::is_integral<T>::value)
		p *= 255;
	uint tmp = p;
	if (bin)
		file.write((const char*) &tmp, 4);
	else
		file << tmp << " ";
}

template<typename T>
void writeFloat(std::ofstream& file, const T& p, bool bin, bool isColor)
{
	float tmp = p;
	if (isColor && std::is_integral<T>::value)
		tmp /= 255;
	if (bin)
		file.write((const char*) &tmp, 4);
	else
		file << tmp << " ";
}

template<typename T>
void writeDouble(std::ofstream& file, const T& p, bool bin, bool isColor)
{
	double tmp = p;
	if (isColor && std::is_integral<T>::value)
		tmp /= 255;
	if (bin)
		file.write((const char*) &tmp, 8);
	else
		file << tmp << " ";
}

template<typename T>
void writeProperty(std::ofstream& file, const T& p, PropertyType type, bool bin, bool isColor)
{
	switch (type) {
	case CHAR: writeChar(file, p, bin, isColor); break;
	case UCHAR: writeUChar(file, p, bin, isColor); break;
	case SHORT: writeShort(file, p, bin, isColor); break;
	case USHORT: writeUShort(file, p, bin, isColor); break;
	case INT: writeInt(file, p, bin, isColor); break;
	case UINT: writeUInt(file, p, bin, isColor); break;
	case FLOAT: writeFloat(file, p, bin, isColor); break;
	case DOUBLE: writeDouble(file, p, bin, isColor); break;
	default: assert(0);
	}
}

} // namespace vcl::io::internal
