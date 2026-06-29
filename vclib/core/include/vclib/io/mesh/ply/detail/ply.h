// VCLib - Visual Computing Library
// Copyright (C) 2021-2026 Visual Computing Lab, ISTI - CNR.
//
// This Source Code Form is subject to the terms of the Mozilla Public License,
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at https://mozilla.org/MPL/2.0/.

#ifndef VCL_IO_MESH_PLY_DETAIL_PLY_H
#define VCL_IO_MESH_PLY_DETAIL_PLY_H

#include <vclib/base.h>

#include <list>

namespace vcl::detail {

// put all these enumeration names inside a ply namespace, to avoid collisions

namespace ply {

typedef enum { ASCII, BINARY_LITTLE_ENDIAN, BINARY_BIG_ENDIAN, UNKNOWN } Format;

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
    bit_flags,
    quality,
    texture_u,
    texture_v,
    material_index,
    vertex_indices,
    texcoord,
    vertex1,
    vertex2,
    // material properties (base color is red, green, blue, alpha)
    name,
    metallic,
    roughness,
    emissive_red,
    emissive_green,
    emissive_blue,
    alpha_mode,
    alpha_cutoff,
    normal_scale,
    occlusion_strength,
    double_sided,
    base_color_texture,
    metallic_roughness_texture,
    normal_texture,
    occlusion_texture,
    emissive_texture
} PropertyName;

using PropertyType = PrimitiveType;

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
    uint                   elementCount;
    std::string unknownElementType; // when an element is not recognized
};

} // namespace vcl::detail

#endif // VCL_IO_MESH_PLY_DETAIL_PLY_H
