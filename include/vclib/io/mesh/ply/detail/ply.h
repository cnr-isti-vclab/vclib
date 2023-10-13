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

#ifndef VCL_IO_PLY_H
#define VCL_IO_PLY_H

#include <assert.h>
#include <fstream>
#include <list>

#include <vclib/io/utils.h>
#include <vclib/misc/tokenizer.h>

namespace vcl::detail {

// put all these enumeration names inside a ply namespace, to avoid collisions

namespace ply {

typedef enum { ASCII, BINARY, UNKNOWN } Format;

typedef enum { VERTEX, FACE, EDGE, TRISTRIP, MATERIAL, OTHER } ElementType;

typedef enum { RGB, RGBA } ColorMode;

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
    quality,
    texture_u,
    texture_v,
    texnumber,
    vertex_indices,
    texcoord,
    vertex1,
    vertex2
} PropertyName;

using PropertyType = vcl::PrimitiveType;

} // namespace ply

struct PlyProperty
{
    ply::PropertyName name;
    ply::PropertyType type;
    bool              list = false;
    ply::PropertyType listSizeType;
    std::string       unknownPropertyName; // when a property is not recognized
};

struct PlyElement
{
    ply::ElementType       type;
    std::list<PlyProperty> properties;
    uint                   numberElements;
    std::string unknownElementType; // when an element is not recognized
};

} // namespace vcl::detail

#endif // VCL_IO_PLY_H
