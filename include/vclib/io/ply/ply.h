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

#ifndef VCL_IO_PLY_H
#define VCL_IO_PLY_H

#include <assert.h>
#include <fstream>
#include <list>

#include <vclib/misc/tokenizer.h>

namespace vcl::ply {

typedef enum { ASCII, BINARY, UNKNOWN } Format;
typedef enum { VERTEX, FACE, EDGE, TRISTRIP, MATERIAL, OTHER } ElementType;
typedef enum { RGB, RGBA} ColorMode ;
typedef enum {
	unknown = -1,
	x,
	y,
	z,
	nx,
	ny,
	nz,
	red,
	green,
	blue,
	alpha,
	scalar,
	vertex_indices,
	texcoord,
	texnumber,
	vertex1,
	vertex2
} PropertyName;
typedef enum { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE, NONE } PropertyType;

struct Property
{
	PropertyName name;
	PropertyType type;
	bool         list = false;
	PropertyType listSizeType;
	std::string  unknownPropertyName;  // used when a property is not recognized
};
struct Element
{
	ElementType         type;
	std::list<Property> properties;
	uint        numberElements;
	std::string         unknownElementType; // used when an element is not recognized
};

namespace internal {

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
T readProperty(vcl::Tokenizer::iterator& token, PropertyType type, bool isColor = false);

inline bool nextLine(std::ifstream& file, vcl::Tokenizer& tokenizer);

// color management

template<typename T>
T colorValue(int value);

template<typename T>
T colorValue(double value);

template<typename T>
T colorValue(vcl::Tokenizer::iterator& token, PropertyType type);

} // namespace internal

} // namespace vcl::ply

#include "ply.cpp"

#endif // VCL_IO_PLY_H
