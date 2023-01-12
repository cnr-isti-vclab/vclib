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

#ifndef VCL_IO_PLY_H
#define VCL_IO_PLY_H

#include <assert.h>
#include <fstream>
#include <list>

#include <vclib/misc/tokenizer.h>

#include "../internal/io_utils.h"

namespace vcl::io::ply {

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
	texture_u,
	texture_v,
	texnumber,
	vertex_indices,
	texcoord,
	vertex1,
	vertex2
} PropertyName;

using PropertyType = vcl::io::internal::PropertyType;

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
	uint                numberElements;
	std::string         unknownElementType; // used when an element is not recognized
};

} // namespace vcl::ply


#endif // VCL_IO_PLY_H
