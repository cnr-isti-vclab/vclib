/**
 * This file is part of mgplib: https://github.com/alemuntoni/mgplib
 * This Source Code Form is subject to the terms of the GNU GPL 3.0
 */

#ifndef MGP_IO_PLY_H
#define MGP_IO_PLY_H

#include <assert.h>
#include <fstream>
#include <list>

#include <mgp/misc/tokenizer.h>

namespace mgp::ply {

typedef enum { ASCII, BINARY, UNKNOWN } Format;
typedef enum { VERTEX, FACE, EDGE, MATERIAL, OTHER } ElementType;
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
	vertex1,
	vertex2
} PropertyName;
typedef enum { CHAR, UCHAR, SHORT, USHORT, INT, UINT, FLOAT, DOUBLE } PropertyType;

struct Property
{
	PropertyName name;
	PropertyType type;
	bool         list;
	PropertyType listSizeType;
	std::string  unknownPropertyName;  // used when a property is not recognized
};
struct Element
{
	ElementType         type;
	std::list<Property> properties;
	unsigned int        numberElements;
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
T readProperty(mgp::Tokenizer::iterator& token, PropertyType type, bool isColor = false);

inline bool nextLine(std::ifstream& file, mgp::Tokenizer& tokenizer);

// color management

template<typename T>
T colorValue(int value);

template<typename T>
T colorValue(double value);

template<typename T>
T colorValue(mgp::Tokenizer::iterator& token, PropertyType type);

} // namespace internal

} // namespace mgp::ply

#include "ply.cpp"

#endif // MGP_IO_PLY_H
